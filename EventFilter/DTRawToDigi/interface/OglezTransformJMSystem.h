/// @file
/// This file contains the declaration and the definition of the class that
/// allows to handle the transformations between the Reference system of Jose
/// Manuel and CMS-related points.
///
/// <PRE>
/// Written by Oscar Gonzalez (2019_06_11)
///                            2019_06_28  Using the transformation for the correlated primitives (SL=0)
/// </PRE>

#ifndef __DTOglezAna_OglezTransformJMSystem_h
#define __DTOglezAna_OglezTransformJMSystem_h



#include "FWCore/Framework/interface/ESHandle.h"
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/DTGeometry/interface/DTGeometry.h>

#include "DataFormats/MuonDetId/interface/DTWireId.h"



// General ROOT and C++ classes

#include "TMath.h"

#include <string>
#include <map>
#include <iostream>
#include <fstream>

//-----------------------------------------------------------------------
/// This is the class (singleton) to perform the transformations between the
/// reference system by Jose Manuel and the CMS-related systems for the DTs.
class OglezTransformJMSystem
{
  /// Pointer to the single instance of the class. This should be explcitly
  /// defined in the code as needed:
  ///        OglezTransformJMSystem* OglezTransformJMSystem::_instance = nullptr;
  ///
  static OglezTransformJMSystem *_instance;



  // Configuration parameters needed for the transformation

  std::map<int,float> _xShifts;  ///< Shifts in X as


  /// Private constructor for a Singleton.
  explicit OglezTransformJMSystem (void) {}




public:

  /// Access to the singleton via the instance parameter.
  static OglezTransformJMSystem *instance() {
    if (_instance==nullptr) _instance = new OglezTransformJMSystem();
    return _instance;
  }

  /// Destructor of the class
  ~OglezTransformJMSystem (void) {
    delete _instance;
    _instance=nullptr;
  }

  /// Configuration of the hardcoded files to perform the transformation.
  void xShiftConfigure (std::string xshiftfile) {
    if (_xShifts.size()>0) {
      std::cerr<<"OGDT-WARNING: Not possible to set (again) the shifts on the X variable for OglezTransformJMSystem"<<std::endl;
      return;
    }

    // Loading the values from the file:
    std::ifstream shiftfile(xshiftfile);
    int rawId;
    float sh;
    while (shiftfile.good()) {
      shiftfile >> rawId >> sh;
      _xShifts[rawId] = sh;
    }
    shiftfile.close();
    if (_xShifts.size()==0) {
      std::cerr<<"OGDT-ERROR: Shifts on the X variable for the reference system transformations not found! "
               <<_xShifts.size()<<std::endl;
    }
  }

  /// Get the value of the shift for a given SL in a given chamber.
  float getXShift (int wheel, int sector, int station, int sl) {
    DTWireId wireId(wheel,station,sector,sl,2,1);

    int rawid = wireId.rawId();
    return _xShifts[rawid];
  }

  /// Apply the transformation to extract Phi and PhiBending.
  void getPhiAndPhiBending(const DTSuperLayerId &dtsl,
                        edm::ESHandle<DTGeometry> dtGeo,
                        float position, float tanPhi, int qual,double *phiAngle,double *phiBending) {

    int wheel = dtsl.wheel();
    int station = dtsl.station();
    int sl = dtsl.superLayer();

    int sector = dtsl.sector();

    // Hardcoded values for tests!
    //rawid=576235528; wheel=-2;  sector=6;  station=1;  sl=1;  position=753.902;  tanPhi=0.436489;
    //rawid=586213384;   wheel=-2;  sector=12;  station=3;  sl=3;  position=1741.69;  tanPhi=-0.267323;

    // Performing the basic transformation: from JM to the center of the chamber
    // using the hardcoded values.

    double tanPhi_loc = -1*tanPhi;
    int thisec = sector;
    if(thisec==13) thisec = 4;
    else if(thisec==14) thisec = 10;

    double x_loc=0;
    double z_loc=0;

    // For the single-SL "uncorrelated" primitives
    //    if (qual<6 || qual==7) {  // Quality identification
    if (sl!=0) {
      DTWireId wireId(dtsl,2,1);
      int rawid = wireId.rawId();

      x_loc = (position/10.)+_xShifts[rawid];  // Converting to cm!

      // NOTE: Z is not used???? Hard-coded values???

      if (station==3 || station==4) {
        if (sl==1) z_loc = 9.95;
        else if (sl==3) z_loc = -13.55;
      }
      else {
        if (sl==1) z_loc = 11.75;
        else if (sl==3) z_loc = -11.75;
      }
    }
    else {
      // For the multi-SL "correlated" primitives the origin of JM reference
      // system seems to be SL1 (?), so we correct using that

      DTWireId wireId(DTSuperLayerId(wheel,station,dtsl.sector(),1),2,1);
      int rawid = wireId.rawId();

      x_loc = (position/10.)+_xShifts[rawid];  // Converting to cm from milimeters!

      if(station>=3) z_loc=-1.8;  // "position" of the segment associated to the primitive
      // Minus sign added on 2019_07_29 when bug was found in the emulator.
    }

    // The second step is to get the phi and the phiBending from the global
    // geometry, after fixing correctly the position in the local system

    GlobalPoint x_global = dtGeo->chamber(DTChamberId(wheel,station,sector))->toGlobal(LocalPoint(x_loc,0.,z_loc));

    double phi= x_global.phi()-0.5235988*(thisec-1);  // (Pi/6) rads
    double psi=TMath::ATan(tanPhi_loc);

    double phiB=-psi-phi;
    if ( wheel>0 || (wheel==0 && (thisec%4)>1) ) {  // They call this "hasPosRF"
      phiB=psi-phi;   // Changing sign
    }
    (*phiAngle) = phi;
    (*phiBending) = phiB;
  }
};

#endif

//=======================================================================
