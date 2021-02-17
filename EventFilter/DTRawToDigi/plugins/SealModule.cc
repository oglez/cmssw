#include <FWCore/Framework/interface/MakerMacros.h>

#include <EventFilter/DTRawToDigi/plugins/DTUnpackingModule.h>
#include <EventFilter/DTRawToDigi/plugins/DTDigiToRawModule.h>

DEFINE_FWK_MODULE(DTUnpackingModule);
DEFINE_FWK_MODULE(DTDigiToRawModule);

#include "EventFilter/DTRawToDigi/interface/OglezDTAB7RawToDigi.h"
DEFINE_FWK_MODULE(OglezDTAB7RawToDigi);
