// Modified version of the DTAB7RawToDigi unpacker by Oscar Gonzalez to improve
// readability and maintenance.
// Started on (2019_01_22) as OglezDTAB7RawToDigi

//-------------------------------------------------
//
//   Class: DTAB7RawToDigi
//
//   L1 DT AB7 Raw-to-Digi
//
//
//
//   Author :
//   C. Heidemann - RWTH Aachen
//   J. Troconiz  - UAM
//   y el eterno retorno
//
//   Modification
//    M.C Fouz - CIEMAT. 14 June 2017
//    Interchanging Phi1 & Phi2 assigned wrongly
//    Forcing the chamber to be in Wheel-1
//        Since is the good type for the demonstrator
//
//--------------------------------------------------

#include "../interface/OglezDTAB7RawToDigi.h"

// CMS Classes

#include "../interface/OglezTransformJMSystem.h"
OglezTransformJMSystem* OglezTransformJMSystem::_instance = nullptr;


#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DTObjects/interface/DTReadOutMapping.h"
#include "CondFormats/DataRecord/interface/DTReadOutMappingRcd.h"

// DT-related classes

#include "DataFormats/MuonDetId/interface/DTWireId.h"
#include "EventFilter/DTRawToDigi/interface/DTROChainCoding.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "TMath.h"

#include <iostream>
#include <fstream>

//-----------------------------------------------------------------------
OglezDTAB7RawToDigi::OglezDTAB7RawToDigi(const edm::ParameterSet& pset)
// Constructor of the class
{
  // Collections to be produced for the EventRecord:
  produces<DTDigiCollection>();
  //  produces<L1MuDTChambPhContainer>();
  produces<L1Phase2MuDTPhContainer>();

  // Reading parameters

  DTAB7InputTag_ = pset.getParameter<edm::InputTag>("DTAB7_FED_Source");

  debug_ = pset.getUntrackedParameter<bool>("debug", false);

  doHexDumping_ = pset.getUntrackedParameter<bool>("doHexDumping", false);

  rawTPVars_ = pset.getUntrackedParameter<bool>("rawTPVars", false);

  correctTPTimeToL1A_ = pset.getUntrackedParameter<bool>("correctTPTimeToL1A",true);

  feds_ = pset.getUntrackedParameter<std::vector<int> >("feds", std::vector<int>());

  rawToken_ = consumes<FEDRawDataCollection>(DTAB7InputTag_);

  // Choosing the channel mapping to use:
  auto val = pset.getUntrackedParameter<std::string>("channelMapping","june2019");

  if (val==std::string("april2019")) {
    _channelMapping = &OglezDTAB7RawToDigi::sx5ChannelMapping_April2019;
    if ( debug_ ) std::cout<<"OGDT-INFO: Using the 'april2019' channel mapping";
  }

  else if (val==std::string("june2019")) {
    _channelMapping = &OglezDTAB7RawToDigi::sx5ChannelMapping_June2019;
    if ( debug_ ) std::cout<<"OGDT-INFO: Using the 'june2019' channel mapping";
  }

  else if (val==std::string("july2019")) {
    _channelMapping = &OglezDTAB7RawToDigi::sx5ChannelMapping_July2019;
    if ( debug_ ) std::cout<<"OGDT-INFO: Using the 'july2019' channel mapping";
  }

  else {  // Using the dummy selection in any other case
    _channelMapping = &OglezDTAB7RawToDigi::sx5ChannelMapping_dummy;
    if ( debug_ ) std::cout<<"OGDT-INFO: Using the dummy (blind)";
  }
  if ( debug_ ) std::cout<<" mapping of the channels to the wires in the unpacking of the DIGIS"<<std::endl;

  // Transformation to the center of the camera from JM system.

  OglezTransformJMSystem::instance()->xShiftConfigure(pset.getParameter<std::string>("xShiftFilename"));

  std::ifstream shiftfile(pset.getParameter<std::string>("xShiftFilename"));
  int rawId;
  float sh;
  while (shiftfile.good()) {
    shiftfile >> rawId >> sh;
    xShifts_[rawId] = sh;
  }
  shiftfile.close();

  shiftfile.open(pset.getParameter<std::string>("zShiftFilename"));
  while (shiftfile.good()) {
    shiftfile >> rawId >> sh;
    zShifts_[rawId] = sh;
  }
  shiftfile.close();

  if (zShifts_.size()==0 || xShifts_.size()==0) {
    std::cerr<<"OGDT-ERROR: Shifts for the reference system transformations not found! "
             <<zShifts_.size()<<" "<<xShifts_.size()<<std::endl;
  }
}

//-----------------------------------------------------------------------
OglezDTAB7RawToDigi::~OglezDTAB7RawToDigi(){}
// Destructor of the class.


//-----------------------------------------------------------------------
void OglezDTAB7RawToDigi::beginRun(edm::Run const& iRun, const edm::EventSetup& iEventSetup)
// Run at the beginning of the task to initialize parameters.
{
  // We want to read the geometry of the DT chambers

  iEventSetup.get<MuonGeometryRecord>().get(dtGeo_);//1103
//  ESHandle< DTConfigManager > dtConfig;
//  iEventSetup.get< DTConfigManagerRcd >().get( dtConfig );

}

//-----------------------------------------------------------------------
void OglezDTAB7RawToDigi::produce(edm::Event& e, const edm::EventSetup& c)
// Main routine to process in every event.
{
  digis_=new DTDigiCollection();
  primitives_.clear();

  if (!fillRawData(e, c)) return;

  auto AB7DTDigi_product = std::make_unique<DTDigiCollection>(*digis_);


  // Original structure of the trigger primitives
  //L1MuDTChambPhContainer primContainer;
  //primContainer.setContainer(primitives_);
  //auto AB7DTPrim_product = std::make_unique<L1MuDTChambPhContainer>(primContainer);

  // Phase2 structure of the trigger primitives (by Federica Primavera)
  L1Phase2MuDTPhContainer primContainer;
  primContainer.setContainer(primitives_);
  auto AB7DTPrim_product = std::make_unique<L1Phase2MuDTPhContainer>(primContainer);

//  if (primitives_.size()==0) {
//    std::cout<<"OGDT-INFO: No TP in the event (?) "<<primitives_.size()<<std::endl;
//  }

//  e.put(std::move(AB7DTDigi_product), "DTAB7Digis");
//  e.put(std::move(AB7DTPrim_product), "DTAB7Primitives");
  e.put(std::move(AB7DTDigi_product));
  e.put(std::move(AB7DTPrim_product));

  delete digis_;
}

//-----------------------------------------------------------------------
bool OglezDTAB7RawToDigi::fillRawData(edm::Event& e,
                                      const edm::EventSetup& c) {

  edm::Handle<FEDRawDataCollection> data;
  e.getByToken(rawToken_, data);

  edm::ESHandle<DTReadOutMapping> mapping;
  c.get<DTReadOutMappingRcd>().get( mapping );

  for (auto &xval : feds_) {
    if (doHexDumping_)
      std::cout << "OGDT-INFO: HEX Dump ++++++ Information about FED: "<<xval<<" (in event: "
                <<e.run()<<"-"<<e.getLuminosityBlock().luminosityBlock()
                <<"-"<<e.eventAuxiliary().event()<<")"<<std::endl;

    process(xval, data, mapping);
  }

  return true;
}

//-----------------------------------------------------------------------
void OglezDTAB7RawToDigi::process(int DTAB7FED,
                                  edm::Handle<FEDRawDataCollection> data,
                                  edm::ESHandle<DTReadOutMapping> mapping) {

  //std::cout<<"--------------------------------------------------------"<<std::endl;

  // Reading the data for the requested FED:

  FEDRawData dturosdata = data->FEDData(DTAB7FED);
  if ( dturosdata.size() == 0 ) {
    if ( debug_ ) edm::LogWarning("oglez_dtab7_unpacker") << "No data for the requested FED "<<DTAB7FED <<std::endl;
    return;
  }

  fedLinePointer_=dturosdata.data();
  long dataWord=0;
  lineCounter_=0;

  hitOrder_.clear();

  newCRC_ = 0xFFFF;

  // Reading the headers:

  readLine(&dataWord);  // Reading the word
  calcCRC(dataWord);

  // Bits 60-63 should be 0x05
  if ( ((dataWord>>60)&0x0F) != 0x5) {
    if ( debug_ ) edm::LogWarning("oglez_dtab7_unpacker")
                    << "Not a DTAB7 header code: "
                    << std::hex << dataWord << std::dec;
    return;
  }

  // Bits 20-31 is the bunch crossing as given by the FED
  bxCounter_=((dataWord>>20)&0xFFF);

  // Bits 8-19 should be the FED number (source ID, it is called)
  if ( ((dataWord>>8)&0x0FFF) != DTAB7FED) {
    if ( debug_ ) edm::LogWarning("oglez_dtab7_unpacker")
                    << "Data does not correspond to the FED "
                    << DTAB7FED<<" "
                    << std::hex << dataWord << std::dec;
    return;
  }

  if (0 || doHexDumping_) std::cout << "OGDT-INFO: Dump + Header/Block, BX: "<<bxCounter_<<std::endl;

  readLine(&dataWord);   // Second word of the header
  calcCRC(dataWord);

  int nslots = (dataWord>>52) & 0xF; // bits 52-55 Number of AMC/slots

//  std::cout<<"OGDT-INFO: Reading fed "<<DTAB7FED<<" with slots: "<<nslots<<std::endl;
//  std::cout<<"+++++++++++++++++++++++++++++++++++++++++++++ Starting AMC "<<nslots<<std::endl;

  if (doHexDumping_) std::cout << "OGDT-INFO: Dump + Header/muFOV word, nAMCs: "<<nslots<<" orbit number: "<<((dataWord>>4)&0xFFFFFFFF)<<std::endl;

  // Reading the information for the slots: one word per AMC
  std::map<int,int> slot_size;
  for (int j=0;j<nslots;++j) {
    readLine(&dataWord);   // AMC word for the slot
    calcCRC(dataWord);

    int slot=(dataWord>>16)&0xF; // Bits 16-19:
    if (slot<1 || slot>12) {
      if ( debug_ ) edm::LogWarning("oglez_dtab7_unpacker")
                      << "AMCnumber "
                      << slot << " out of range (1-12)";
      return;
    }
    slot_size[slot] = (dataWord>>32)&0xFFFFFF; // bits 32-55: n words for the AMC
    if (0 || doHexDumping_) std::cout << "OGDT-INFO: Dump + AMC: "<<slot<<" size: "<<slot_size[slot]<<std::endl;
  }

  // Reading all the payloads for the AB7... each AMC
  for (int j=0;j<nslots;++j) {
    readLine(&dataWord);  // First word header AMC
    calcCRC(dataWord);

    int slot = (dataWord>>56)&0xF;      // bit 56-59: slot number
    if (slot<1 || slot>12) {
      if ( debug_ ) edm::LogWarning("oglez_dtab7_unpacker")
                      << "AMCnumber "
                      << slot << " for the information is out of range (1-12)";
      return;
    }

    // Collision information (from Carsten's code in the original(?))

    //int dataLenght = (dataWord & 0xFFFFF);         // positions 0 -> 19
    int bx = (dataWord >> 20 ) & 0xFFF;    // positions 20 -> 31
    int event      = (dataWord >> 32 ) & 0xFFFFFF; // positions 32 -> 55
//   int AMC_ID     = (dataWord >> 56 ) & 0xF;      // positions 56 -> 59
//   int control    = (dataWord >> 60 ) & 0xF;      // positions 59 -> 63
//   int wheel      = AB7Wheel;

   // CHECK!
   if (bx!=bxCounter_) {
     edm::LogWarning("oglez_dtab7_unpacker")
       << "The BX in the AMC and in the FED does not match "<<bx<<" "<<bxCounter_<<std::endl;
   }

   if (0 || doHexDumping_) std::cout << "OGDT-INFO: Dump + AMC Header 1, BX: "<<bx<<" event: "<<event<<std::endl;

    // Second word header AMC: nothing relevant
    readLine(&dataWord);
    calcCRC(dataWord);

    if (0 || doHexDumping_) std::cout << "OGDT-INFO: Dump + number of slots: "<<slot_size[slot]<<std::endl;

    // Check on the number of slots... if large, we exclude the entry.
    if (slot_size[slot]>200) {
      if ( debug_ ) edm::LogWarning("oglez_dtab7_unpacker")
                      << "AMC Slot has too many digis/TP: "<<slot_size[slot]<<std::endl;
      if (slot_size[slot]>1000) return;  // Problematic events... code crashes due to crappy/crazy values?
    }

    // Reading the hits or trigger primitives (words!)
    for (int k=slot_size[slot]-1;k>2;--k) { // Just a counter of how many!
      readLine(&dataWord);
      calcCRC(dataWord);

      // Reading the AB7PayLoad:
      int type=(dataWord>>60)&0xF; // Bits 60-63 gives us the type of information:

      if (doHexDumping_) std::cout<<"OGDT-INFO: Dump + word info for digi/tp: "<<std::hex<<type<<std::dec<<std::endl;

      if (type==1) {  // Hit information: 2 hits of 30 bits!
        readAB7PayLoad_hitWord(dataWord,DTAB7FED,slot);
      }
      else if (((type>>2)&0x3)==2) {  // First trigger word... The other should come after wards!
        long firstWord=dataWord;

        // The second word is optional... not for correlated superlayer
        if ( ((firstWord>>58)&0x3)==0) {  // SL=0, correlated primitive!
          dataWord=(0xCL<<60);
        }
        else {
          readLine(&dataWord);  // Reading the second word
          calcCRC(dataWord);
          --k;
        }

//        if ( ((firstWord>>58)&0x3)==0) {  // SL=0, correlated primitive!
//          std::cout<<"OGDT-INFO: Correlated primitive with "<<std::hex<<firstWord<<" "<<dataWord<<std::dec<<" "<<((firstWord>>58)&0x3)<<std::endl;
//        }
//        else std::cout<<"OGDT-INFO: Uncorrelated primitive with "<<std::hex<<firstWord<<" "<<dataWord<<std::dec<<" "<<((firstWord>>58)&0x3)<<std::endl;

        // Checking it is fine...
        if (((dataWord>>60)&0xF)==0xC)  {  // It is ok! (v6)
          readAB7PayLoad_triggerPrimitive(firstWord,dataWord,DTAB7FED,slot);
        }
        else if (debug_) edm::LogWarning("oglez_dtab7_unpacker")
                           << "Expected second trigger word that is not there "
                           <<DTAB7FED<<" "<<slot<<std::hex<<type<<" "<<((dataWord>>62)&0x3)<<" "<<dataWord<<std::dec;
      }
      else {
        // Problems with the value of the word
        if (debug_) edm::LogWarning("oglez_dtab7_unpacker")
                      << "Error word for the ros information "
                      <<DTAB7FED<<" "<<slot<<" "<<std::hex<<type<<std::dec;
      }
    }

    // Trailer word of the AMC information
    readLine(&dataWord);
    calcCRC(dataWord);
  }

  // Trailer words for checks

  readLine(&dataWord);   // First trailer word (for the block) is not used, but it has some check information
  calcCRC(dataWord);

  if (doHexDumping_) std::cout << "OGDT-INFO: Dump + Trailer/muFOV, BX: "<<(dataWord&0xFFF)<<std::endl;

  readLine(&dataWord);   // Second trailer word (final one for the FED)
  calcCRC(dataWord&0xFFFFFFFF0000FFFF);   // EXCLUYENDO LOS VALORES DE CRC

  if ( ((dataWord>>60)&0xF)!=0xA) {   // Bits 60-63 are 0xA (control)
    if ( debug_ )  edm::LogWarning("oglez_dtab7_unpacker")
                     << "Trailer word "
                     << std::hex << dataWord << std::dec
                     << " does not start with 0xA";
    return;
  }

  int evtLgth = ( dataWord >> 32 ) & 0xFFFFFF; // Bits 32-55 is the number of lines
  int CRC     = ( dataWord >> 16 ) & 0xFFFF;   // Bits 16-31 is the expected CRC

  if ( newCRC_ != CRC ) {
    if ( debug_ ) {
      edm::LogWarning("oglez_dtab7_unpacker")
        << "Calculated CRC "
        << std::hex << newCRC_
        << " differs from CRC in trailer "
        << CRC << std::dec;
      std::cout<<"OGDT-ERROR: CRC does not match!!! "<<evtLgth<<" "<<lineCounter_<<" "<<std::hex<<CRC<<" "<<newCRC_<<" "<<std::dec<<std::endl;
    }
    return;
  }

  if ( lineCounter_ != evtLgth ) {
    if ( debug_ ) {
      edm::LogWarning("oglez_dtab7_unpacker")
        << "Number of words read != event lenght "
        << lineCounter_ << " " << evtLgth;
      std::cout<<"OGDT-ERROR: line counting does not match!!! "<<evtLgth<<" "<<lineCounter_<<" "<<std::hex<<CRC<<" "<<newCRC_<<" "<<std::dec<<std::endl;
    }
    return;
  }

//      std::cout<<"OGDT-CHECK: "<<evtLgth<<" "<<lineCounter_<<" "
//               <<std::hex<<CRC<<" "<<newCRC_<<" "<<std::dec<<std::endl;

  // Everything was read here!
  return;
}

//-----------------------------------------------------------------------
void OglezDTAB7RawToDigi::readAB7PayLoad_hitWord (long dataWord,int fedno, int slot)
// Read the HIT information from the Payload of the AB7.
{
  int ioff=0;
  while (ioff<31) {  // Two hits in the word
    int hitinfo=(dataWord>>ioff);   // Information for the hit (bits 0-29)
    ioff+=30;

    // I understand from Bilal's code that what it is called "offset" is in fact the BX in V3:
    int bx = (hitinfo>>5)&0xFFF; // positions   5 -> 16

    if (doHexDumping_) std::cout<<"OGDT-INFO: Dump + DIGI Info summary: BX: "<<bx<<std::endl;

    if (bx==0xFFF) {  // invalid (empty, not-needed because of odd-number) hits
      //if ( debug_ ) edm::LogWarning("oglez_dtab7_unpacker")
      //              << "Empty/invalid hit information because of an odd number of hits in the slot ";
      continue;
    }

    // I understand from Bilal's code that this is the time within the BX, using 25ns/30 units in V3
    int tdc_hit_t = hitinfo&0x1F;   // Bits 0-4: TDC value

    // Identifying the channel

    int channelId = (hitinfo>>17)&0x1FF;   // Bits 17-25: Channel index
    int slId = (hitinfo>>26)&0x3;   // Bits 26-27: SL number
    int stationId = ((hitinfo>>28)&0x3)+1;   // Bits 28-29: Station number (-1, C++ convention)

    if (slId==0) {
      std::cerr<<"OGDT-ERROR: Superlayer of a digi is zero... readout problem!!!"<<std::endl;
    }

    // std::cout<<"OGDT-INFO: Information: "<<channelId<<" "<<stationId<<" "<<slId<<" "<<tdc_hit_t<<std::endl;

    // Getting the channel index.
//    int dduId = theDDU(fedno, slot, link);
//    int rosId = theROS(fedno, slot, link);
//    int robId = theROB(fedno, slot, link);
//    DTROChainCoding channelIndex(dduId, rosId, robId, tdcId, tdcChannel);
//    uint32_t chCode=channelIndex.getCode();

    // Using the one for the Phase-2 SX5 proptype
    uint32_t chCode = channelId;  //

    if (hitOrder_.find(chCode) == hitOrder_.end()) hitOrder_[chCode] = 0;
    else hitOrder_[chCode]++;

    // Forcing the chamber to be in an specific wheel & sector
    int wheelId=2, sectorId=12;
    //if (stationId==1) stationId=2;  // for "v6" of the payload, before run 330160

    // Getting the values from the current mapping
    int layerId=-999, wire=-999;
    (*_channelMapping)(channelId,&slId,&layerId,&wire);  // slId needs to be overwritten!

    // Clean way:
    DTWireId detId = DTWireId(wheelId, stationId, sectorId, slId, layerId, wire);
//    std::cout<<"        Estamos en el canal "<<channelId<<" que se corresponde con "<<slId<<" "<<layerId<<" "
//             <<wire<<" [wire="<<detId.wire()<<"]"<<std::endl;

    // Storing the digi:

    // For tests... storing information (in 1/25 ns units... according to Cristina's equation)
    // Although I discovered that to use the legacy we should store "legacy TDC counts"
    // There is a *25 to convert from "BX units" to ns
    // There is a *(32/25) to convert from ns to TDCCounts  (and it needs to be positive!)
    // We need to subtract 1 in the tdc_hit, because it goes from 1-30, due to some
    //            convention (Alvaro indicated so)
    int tdccounts = int(32*(bx+(tdc_hit_t-1)/30.)+0.5)-32*bxCounter_;
    while (tdccounts<0) tdccounts+=114048;// 32*3564;

    DTDigi digi(wire,tdccounts, hitOrder_[chCode]);

    // To convert digi.time() en "BX" hay que dividir por 25, sumarle el BX_fed
    // y si lo que sale es mayor que 3564, restarle ese número.

    digis_->insertDigi(detId.layerId(),digi);
  }  // While for the two hits!
}

//-----------------------------------------------------------------------
void OglezDTAB7RawToDigi::readAB7PayLoad_triggerPrimitive (long firstWord,long secondWord,int fedno, int slot)
// Read the Trigger Primitive information from the Payload of the AB7.
{
  int stationId = ((firstWord>>60)&0x3)+1;   // Bits 60-61 (first word) is the station (-1, in C++ convention)
  int superlayer = ((firstWord>>58)&0x3);   // Bits 58-59 (first word) is the superlayer (1-3) or a phi-primitive (0)

  int quality = ((firstWord>>35)&0x3F);   // Bits 35-40 (first word) is the quality of the TP
  int time = ((firstWord>>41)&0x1FFFF) ; // Bits 41-57 (first word) is the time (in nanoseconds)
  // The time may need to be corrected to use the "L1A time" as the reference:
  if (correctTPTimeToL1A_) time -= 25*bxCounter_;

  int bx = (int) round(time/25.);   // Getting the associated bunch-crossing (as indicated by Jaime how they computed in the emulator).
  if (bx<0) bx += 3564;  // BX in previous orbit!

//v4  int position = ((firstWord)&0xFFFF);   // Bits 0-15 (first word) is the position (phi or theta depending on SL)
  int position = ((firstWord)&0x7FFF);   // Bits 0-14 (first word) is the position (phi or theta depending on SL)

//v4  int jmTanPhi = ((firstWord>>16)&0x7FFF);     // Bits 16-30 (first word) is the slope (phi or theta depending on SL)
  int jmTanPhi = ((firstWord>>15)&0x7FFF);     // Bits 15-29 (first word) is the slope (phi or theta depending on SL)
  // Acording to Jose Manuel (email 2019_04_03) this is 4096*tan(phi)
  // but we we need to account for the sign.
  if ( ((jmTanPhi>>14)&0x01)==0x01) { // Negative value!
    jmTanPhi = (jmTanPhi-32768);   // (1<<15)    // -1*(((~slope)&0x7FFF)+1);
  }

  int chi2 = ((firstWord>>30)&0x1F);    // Bits 30-34 (first word) is the chi2 (in v6)
//v4  int chi2 = (secondWord&0x1F);             // Bits 0-4 (second word) is the chi2 of the fit (in v4)
//v4  int tpindex = ((secondWord>>5)&0x07);     // Bits 5-7 (second word) is the index of this trigger primitive in the event (in v4)

  if (0 || doHexDumping_) std::cout<<"OGDT-INFO: Dump + TP Info summary: BX: "<<bx<<" "<<" Q="<<quality<<" SL="<<superlayer<<std::endl; // TEST:" "<<32*bx/25<<std::endl;

//  std::cout<<"            PRUEBA-TP(1): "<<bx<<" "<<stationId<<" "<<superlayer<<" Q="<<quality<<" "<<chi2<<" "<<jmTanPhi<<" "<<position<<std::endl;
//  std::cout<<"            PRUEBA-TP(2): "<<chi2<<" "<<superlayer<<" "<<std::endl;

//  std::cout<<"OGDTINFO Control version: "<<((firstWord>>31)&0xF)<<" "<<((secondWord>>52)&0x3FF)<<" "<<superlayer<<std::endl;


  // Getting the hits per layer (adapted to v6):
//  for (int i=0;i<4;++i) {  // Scanning layer number (inverted with respect to usual? Alvaro claims 4 is bottom...)
//    int lat = ((secondWord>>(3-i))&1);  // Laterality (1 is right, 0 is left)
//    int use = ((secondWord>>(7-i))&1);  // Was digi used in the primitive?
//    int driftime = ((secondWord>>(20-4*i))&0xF);  // 1/32 of Drift time in ns (?), 15 means greater than 14, calculated as hit_time-primitive time
//    int chan = ((secondWord>>(45-7*i))&0x3F);  // channel number inside its layer
//    std::cout<<"                 - Layer: "<<(i+1)<<" "<<lat<<" "<<use<<" "<<driftime<<" "<<chan<<std::endl;
//  }

  // Processing the position of the trigger primitive to change the parameters
  // to the global reference system:

  int wheel=2;   // FIXME: It is not clear who provides this? Hard-coded
  int sector=12;  // FIXME: It is not clear who provides this? Hard-coded
  //if (stationId==1) stationId=2;  // for "v6" of the payload, before run 330160

  // To change to the global we need to indicate where we are... for the
  // superlayer (although it is not clear who provide the superlayer)

  DTSuperLayerId slId(wheel,stationId,sector,superlayer);

  double phiAngle=0;
  double phiBending=0;
  OglezTransformJMSystem::instance()->getPhiAndPhiBending(slId,dtGeo_,position/4.,  // Using mm as this argument (JM uses mm as metric)
                                                          jmTanPhi/4096.,quality,&phiAngle,&phiBending);

  // Adding the trigger primitive, perhaps with some information modified
  int uind = 0;
  int uphi = (int) round(phiAngle*65536./0.8);  // phiAngle
  int uphib = (int) round(phiBending*2048./1.4);  // phiBending

  if (rawTPVars_) {   // Storing the raw (not transformed) values from the payload
    uind = 9999;
    uphi = position;   // phi stores the position bits in this mode
    uphib = jmTanPhi;   // phib stores the SIGNED tan phi (directly related to slope, but not exactly the same bit content)
  }

//  std::cout<<"PRIMITIVA "<<superlayer<<" "<<quality<<" "<<phiAngle<<" "<<phiBending<<" "<<(position/4.)<<" "<<(jmTanPhi/4096.)<<" RAW: "<<position<<" "<<jmTanPhi<<std::endl;

  L1Phase2MuDTPhDigi trigprim(bx,   // ubx (m_bx)
                              wheel,   // uwh (m_wheel)
                              sector-1,   // usc (m_sector)  USING L1 trigger primitives convention.
                              stationId,      // ust (m_station)
                              superlayer,   // usl (m_superlayer)

                              uphi,   // uphi (_phiAngle)
                              uphib,   // uphib (m_phiBending)

                              quality,  // uqua (m_qualityCode)
                              uind,   // uind (m_segmentIndex)
// v4 tpindex,  // uind (m_segmentIndex)
                              time,  // ut0 (m_t0Segment)
                              chi2,  // uchi2 (m_chi2Segment)
                              -10);  // urpc (m_rpcFlag)

  // Storing the primitive
  primitives_.push_back(trigprim);
}

//-----------------------------------------------------------------------
int OglezDTAB7RawToDigi::theDDU(int crate, int slot, int link) {

     if (crate == 1368) {
       if (slot < 7) return 770;
       return 771;
     }

     if (crate == 1370) {
       if (slot > 6) return 773;
       return 774;
     }

     return 772;
}

//-----------------------------------------------------------------------
int OglezDTAB7RawToDigi::theROS(int crate, int slot, int link) {

  if (slot%6 == 5) return link+1;

  int ros = (link/24) + 3*(slot%6) - 2;
  return ros;
}

//-----------------------------------------------------------------------
int OglezDTAB7RawToDigi::theROB(int crate, int slot, int link) {

  if (slot%6 == 5) return 23;

  int rob = link%24;
  if (rob < 15) return rob;
  if (rob == 15) return 24;
  return rob-1;
}

//-----------------------------------------------------------------------
void OglezDTAB7RawToDigi::calcCRC(long word) {
// Routine to compute the CRC using a new word.

  int myCRC[16], D[64], C[16];

  for ( int i = 0; i < 64; ++i ) { D[i]    = (word >> i) & 0x1; }
  for ( int i = 0; i < 16; ++i ) { C[i]    = (newCRC_>>i)  & 0x1; }

  myCRC[0] = ( D[63] + D[62] + D[61] + D[60] + D[55] + D[54] +
               D[53] + D[52] + D[51] + D[50] + D[49] + D[48] +
               D[47] + D[46] + D[45] + D[43] + D[41] + D[40] +
               D[39] + D[38] + D[37] + D[36] + D[35] + D[34] +
               D[33] + D[32] + D[31] + D[30] + D[27] + D[26] +
               D[25] + D[24] + D[23] + D[22] + D[21] + D[20] +
               D[19] + D[18] + D[17] + D[16] + D[15] + D[13] +
               D[12] + D[11] + D[10] + D[9]  + D[8]  + D[7]  +
               D[6]  + D[5]  + D[4]  + D[3]  + D[2]  + D[1]  +
               D[0]  + C[0]  + C[1]  + C[2]  + C[3]  + C[4]  +
               C[5]  + C[6]  + C[7]  + C[12] + C[13] + C[14] +
               C[15] )%2;

  myCRC[1] = ( D[63] + D[62] + D[61] + D[56] + D[55] + D[54] +
               D[53] + D[52] + D[51] + D[50] + D[49] + D[48] +
               D[47] + D[46] + D[44] + D[42] + D[41] + D[40] +
               D[39] + D[38] + D[37] + D[36] + D[35] + D[34] +
               D[33] + D[32] + D[31] + D[28] + D[27] + D[26] +
               D[25] + D[24] + D[23] + D[22] + D[21] + D[20] +
               D[19] + D[18] + D[17] + D[16] + D[14] + D[13] +
               D[12] + D[11] + D[10] + D[9]  + D[8]  + D[7]  +
               D[6]  + D[5]  + D[4]  + D[3]  + D[2]  + D[1]  +
               C[0]  + C[1]  + C[2]  + C[3]  + C[4]  + C[5]  +
               C[6]  + C[7]  + C[8]  + C[13] + C[14] + C[15] )%2;

  myCRC[2] = ( D[61] + D[60] + D[57] + D[56] + D[46] + D[42] +
               D[31] + D[30] + D[29] + D[28] + D[16] + D[14] +
               D[1]  + D[0]  + C[8]  + C[9]  + C[12] + C[13] )%2;

  myCRC[3] = ( D[62] + D[61] + D[58] + D[57] + D[47] + D[43] +
               D[32] + D[31] + D[30] + D[29] + D[17] + D[15] +
               D[2]  + D[1]  + C[9]  + C[10] + C[13] + C[14] )%2;

  myCRC[4] = ( D[63] + D[62] + D[59] + D[58] + D[48] + D[44] +
               D[33] + D[32] + D[31] + D[30] + D[18] + D[16] +
               D[3]  + D[2]  + C[0]  + C[10] + C[11] + C[14] +
               C[15] )%2;

  myCRC[5] = ( D[63] + D[60] + D[59] + D[49] + D[45] + D[34] +
               D[33] + D[32] + D[31] + D[19] + D[17] + D[4]  +
               D[3]  + C[1]  + C[11] + C[12] + C[15] )%2;

  myCRC[6] = ( D[61] + D[60] + D[50] + D[46] + D[35] + D[34] +
               D[33] + D[32] + D[20] + D[18] + D[5]  + D[4]  +
               C[2]  + C[12] + C[13] )%2;

  myCRC[7] = ( D[62] + D[61] + D[51] + D[47] + D[36] + D[35] +
               D[34] + D[33] + D[21] + D[19] + D[6]  + D[5]  +
               C[3]  + C[13] + C[14] )%2;

  myCRC[8] = ( D[63] + D[62] + D[52] + D[48] + D[37] + D[36] +
               D[35] + D[34] + D[22] + D[20] + D[7]  + D[6]  +
               C[0]  + C[4]  + C[14] + C[15] )%2;

  myCRC[9] = ( D[63] + D[53] + D[49] + D[38] + D[37] + D[36] +
               D[35] + D[23] + D[21] + D[8]  + D[7]  + C[1]  +
               C[5]  + C[15] )%2;

  myCRC[10] = ( D[54] + D[50] + D[39] + D[38] + D[37] + D[36] +
                D[24] + D[22] + D[9]  + D[8]  + C[2]  + C[6] )%2;

  myCRC[11] = ( D[55] + D[51] + D[40] + D[39] + D[38] + D[37] +
                D[25] + D[23] + D[10] + D[9]  + C[3]  + C[7] )%2;

  myCRC[12] = ( D[56] + D[52] + D[41] + D[40] + D[39] + D[38] +
                D[26] + D[24] + D[11] + D[10] + C[4]  + C[8] )%2;

  myCRC[13] = ( D[57] + D[53] + D[42] + D[41] + D[40] + D[39] +
                D[27] + D[25] + D[12] + D[11] + C[5]  + C[9] )%2;

  myCRC[14] = ( D[58] + D[54] + D[43] + D[42] + D[41] + D[40] +
                D[28] + D[26] + D[13] + D[12] + C[6]  + C[10] )%2;

  myCRC[15] = ( D[63] + D[62] + D[61] + D[60] + D[59] + D[54] +
                D[53] + D[52] + D[51] + D[50] + D[49] + D[48] +
                D[47] + D[46] + D[45] + D[44] + D[42] + D[40] +
                D[39] + D[38] + D[37] + D[36] + D[35] + D[34] +
                D[33] + D[32] + D[31] + D[30] + D[29] + D[26] +
                D[25] + D[24] + D[23] + D[22] + D[21] + D[20] +
                D[19] + D[18] + D[17] + D[16] + D[15] + D[14] +
                D[12] + D[11] + D[10] + D[9]  + D[8]  + D[7]  +
                D[6]  + D[5]  + D[4]  + D[3]  + D[2]  + D[1]  +
                D[0]  + C[0]  + C[1]  + C[2]  + C[3]  + C[4]  +
                C[5]  + C[6]  + C[11] + C[12] + C[13] + C[14] +
                C[15] )%2;

  int tempC = 0x0;
  for ( int i = 0; i < 16 ; ++i) { tempC = tempC + ( myCRC[i] << i ); }

  newCRC_ = tempC;
  return;
}

//-----------------------------------------------------------------------
void OglezDTAB7RawToDigi::sx5ChannelMapping_July2019 (int chanid, int *sl, int *layer, int *wire)
  // Static routine to process the current mapping between the channel ID and
  // the wire, Layer and SL of the digi for the Phase-2 prototype..
{
  // The (*sl) value is ignored...

  // From Cristina (and v6 of the payload after fixing the problem with the layer inversion)

  (*wire) = (chanid>>2)+1;
  (*layer) = (chanid&0x3)+1;
}

//-----------------------------------------------------------------------
void OglezDTAB7RawToDigi::sx5ChannelMapping_June2019 (int chanid, int *sl, int *layer, int *wire)
  // Static routine to process the current mapping between the channel ID and
  // the wire, Layer and SL of the digi for the Phase-2 prototype..
{
  // The (*sl) value is ignored...

  // NOTE: In practice this is no longer needed since the channel_id has been
  // set in such a way that explicitly stores the layer and the wire, perhaps
  // with an offset, but I keep it for now in case we need to play with it.

  // From Cristina (and v6 of the payload)

  // However the later needs to be fixed because it was inverted in this version.

  (*wire) = (chanid>>2)+1;
  (*layer) = 4-(chanid&0x3);
}

//-----------------------------------------------------------------------
void OglezDTAB7RawToDigi::sx5ChannelMapping_April2019 (int chanid, int *sl, int *layer, int *wire)
  // Static routine to process the current mapping between the channel ID and
  // the wire, Layer and SL of the digi for the Phase-2 prototype..
{
  // Not clear the underlying logic...
  if (chanid>=64 && chanid<=79) {
    (*sl) = 1;
    (*wire) = 1 + ((chanid)-64)/4;  // wire 1 to 4
  }
  else if (chanid>=80 && chanid<=95) {
    (*sl) = 3;
    (*wire) = 17 + ((chanid)-80)/4;  // wire 17 to 20
  }
  else if (chanid>=112 && chanid<=127) {
    (*sl) = 1;
    (*wire) = 17 + ((chanid)-112)/4;  // wire 17 to 20
  }

  else if (chanid>=0 && chanid<=15) {
    (*sl) = 3;
    (*wire) = 21 + (chanid)/4;  // wire 21 to 24
  }
  else if (chanid>=16 && chanid<=31) {
    (*sl) = 1;
    (*wire) = 5 + ((chanid)-16)/4;  // wire 5 to 8
    //std::cout<<"OGDT-INFO: Channel should be fine!!!"<<std::endl;
  }
  else if (chanid>=32 && chanid<=47) {
    (*sl) = 1;
    (*wire) = 13 + ((chanid)-32)/4;  // wire 13 to 16
  }
  else if (chanid>=48 && chanid<=63) {
    (*sl) = 1;
    (*wire) = 9 + ((chanid)-48)/4;  // wire 9 to 12
  }
  else if (chanid>=96 && chanid<=111) {
    (*sl) = 1;
    (*wire) = 21 + ((chanid)-96)/4;  // wire 21 to 24 (Nacho pone 25?)
  }

  else {
    std::cerr<<"OGDT-ERROR: Channel-ID does not seem to be correct in the currentSX5ChannelMapping: "<<chanid<<std::endl;
    (*sl) = -1;
    (*wire) = -999;
  }

  // Layer is simpler (If I got it right)
  int i = (chanid%4);
  (*layer) = 4 - 2*(i%2) - (i>=2);
}

//=======================================================================
