/// @file
/// This file contains the declaration and the definition of the class that
/// allows to handle the transformations between the Reference system of Jose
/// Manuel and CMS-related points.
///
/// <PRE>
/// Written by Oscar Gonzalez (2019_06_11)
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
      std::cerr<<"OGDT-ERROR: Not possible to set the shifts on the X variable for OglezTransformJMSystem"<<std::endl;
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

  /// Apply the transformation to extract Phi and PhiBending.
  void getPhiAndPhiBending(const DTSuperLayerId &dtsl,
                        edm::ESHandle<DTGeometry> dtGeo,
                        float position, float tanPhi, int qual,double *phiAngle,double *phiBending) {

    DTWireId wireId(dtsl,2,1);

    int rawid = wireId.rawId();
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

    // For the single-SL "uncorrelated" primitives
    if (qual<6 || qual==7) {  // Quality identification

      double x_loc = (position/10.)+_xShifts[rawid];  // Converting to cm!

      // NOTE: Z is not used???? Hard-coded values???

      double z_loc = 0;

      if (station==3 || station==4) {
        if (sl==1) z_loc = 9.95;
        else if (sl==3) z_loc = -13.55;
      }
      else {
        if (sl==1) z_loc = 11.75;
        else if (sl==3) z_loc = -11.75;
      }

      // The second step is to get the phi and the phiBending from the global geometry:

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
    else {
      // For the multi-SL "correlated" primitives
      std::cerr<<"OGDT-ERROR: "<<std::endl;
    }
  }
};

#endif

//=======================================================================
