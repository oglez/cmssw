// Modified version of the DTAB7RawToDigi unpacker by Oscar Gonzalez to improve
// readability and maintenance.
// Started on (2019_01_22)

//-------------------------------------------------
//
/**  \class DTAB7RawToDigi
 *
 *   L1 DT AB7 Raw-to-Digi
 *
 *
 *
 *   C. Heidemann - RWTH Aachen
 *   J. Troconiz  - UAM Madrid
 */
//
//--------------------------------------------------

#ifndef Oglez_AB7Test_DTAB7RawToDigi_h
#define Oglez_AB7Test_DTAB7RawToDigi_h

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/MuonDetId/interface/DTLayerId.h"

#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Utilities/interface/InputTag.h>

#include "DataFormats/L1DTTrackFinder/interface/L1Phase2MuDTPhContainer.h"

#include <string>

class DTReadOutMapping;

class DTGeometry;

class OglezDTAB7RawToDigi : public edm::EDProducer {

public:

  /// Constructor of the class.
  OglezDTAB7RawToDigi(const edm::ParameterSet& pset);

  /// Destructor of the class.
  virtual ~OglezDTAB7RawToDigi();

  /// Run at the beginning of the task to initialize parameters.
  void beginRun(edm::Run const& iRun, const edm::EventSetup& iEventSetup);


  /// Produce digis out of raw data
  void produce(edm::Event& e, const edm::EventSetup& c);

  /// Generate and fill FED raw data for a full event
  bool fillRawData(edm::Event& e,
                   const edm::EventSetup& c);

private:

  // Configuration parameters to be read from the python configuration.

  /// Collection of the Raw data to be processed.
  edm::InputTag DTAB7InputTag_;

  /// To enable the debugging messages.
  bool debug_;

  /// To enable the dumping option. All the hexadecimal values that are read,
  /// but also some "orientative" information.
  bool doHexDumping_;

  /// To enable the option that makes that for the trigger primitives we store
  /// the slope and the position as read from the payload, instead of the phi
  /// and phiBending.
  bool rawTPVars_;

  /// Feds that should be read in the task.
  std::vector<int> feds_;

  /// Maps with the shifts in X for the transformation from JM's system to the
  /// center of the chambers (using some channel ID as keys).
  std::map<int,float> xShifts_;

  /// Maps with the shifts in Z for the transformation from JM's system to the
  /// center of the chambers (using some channel ID as keys).
  std::map<int,float> zShifts_;

  // Additional variables

  /// Token with the collection associated to the raw data (FEDs).
  edm::EDGetTokenT<FEDRawDataCollection> rawToken_;

  unsigned char* fedLinePointer_;  ///< Pointer to read the words.

  int lineCounter_;   ///< Counter of the read lines.

  int newCRC_;  ///< Computed CRC during the reading process.

  int bxCounter_; ///< Value of the BX as provided by the header FED (and
                  ///< checked with the AMC information).

  std::map<uint32_t, int> hitOrder_;  ///< Hit counter

  DTDigiCollection *digis_;  ///< Digis collection to be generated
  //  std::vector<L1MuDTChambPhDigi> primitives_;  ///< Trigger primitives to be read (original)
  std::vector<L1Phase2MuDTPhDigi> primitives_;  ///< Trigger primitives to be read (Phase2, by F. Primavera)

  edm::ESHandle<DTGeometry> dtGeo_;  ///< To handle the DTGeometry tools.

  /// Pointer to the method used to the mapping of channels for the card.
  void (*_channelMapping)(int,int *,int *,int *);

  // Operations

  //process data

  void process(int DTAB7FED,
               edm::Handle<FEDRawDataCollection> data,
               edm::ESHandle<DTReadOutMapping> mapping);

  /// Command to read a 64-bit word at the position of the pointer, increasing
  /// the counter.
  inline void readLine (long *dataWord) {
    (*dataWord)= *((long*)fedLinePointer_);
    fedLinePointer_+=8;
    ++lineCounter_;
    if (doHexDumping_)
      std::cout << "OGINFO-INFO: Dump      HEX: "<<std::hex<<(*dataWord)<<std::dec<<std::endl;
  }

  /// Read the HIT information from the Payload of the AB7.
  void readAB7PayLoad_hitWord (long dataWord,int fedno,int slot);

  /// Read the Trigger Primitive information from the Payload of the AB7.
  void readAB7PayLoad_triggerPrimitive (long firstWord,long secondWord,int fedno,int slot);

  /// Routine to compute the CRC for checking out the raw data has been
  /// properly read.
  void calcCRC(long word);

  int theDDU(int crate, int slot, int link);

  int theROS(int crate, int slot, int link);

  int theROB(int crate, int slot, int link);

  /// To get the collection tag of the raw data to be processed.
  edm::InputTag getDTAB7InputTag() { return DTAB7InputTag_; }

  /// Static routine to process the april-2019 mapping between the channel ID and
  /// the Layer and SL of the digi for the Phase-2 prototype..
  static void sx5ChannelMapping_April2019 (int ch_id, int *sl, int *layer, int *wire);

  /// Static routine to process the june-2019 mapping between the channel ID
  /// and the Layer of the digi for the Phase-2 prototype. It should correspond
  /// to the v5 of the payload. SL should not be used.
  static void sx5ChannelMapping_June2019 (int ch_id, int *sl, int *layer, int *wire);

  /// Static routine to process the july-2019 mapping between the channel ID
  /// and the Layer of the digi for the Phase-2 prototype. It should correspond
  /// to the v5 of the payload after fixing the problem with the layer
  /// inversion. SL should not be used.
  static void sx5ChannelMapping_July2019 (int ch_id, int *sl, int *layer, int *wire);

  /// Dummy static routine for the mapping, which is the default.
  static void sx5ChannelMapping_dummy (int ch_id, int *sl, int *layer, int *wire) {
    (*layer)=1+(ch_id%4);
    (*sl)=1;
    (*wire)=1+ (ch_id/4);
  }
};

#endif
// ======================================================================
