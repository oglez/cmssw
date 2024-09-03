import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

import Configuration.Geometry.defaultPhase2ConditionsEra_cff as _settings
###################################################################
# Setup 'standard' options
###################################################################
options = VarParsing.VarParsing()
options.register('Scenario',
                 _settings.DEFAULT_VERSION, # default value
                 VarParsing.VarParsing.multiplicity.singleton, # singleton or list
                 VarParsing.VarParsing.varType.string, # string, int, or float
                 "geometry version to use: 2026DXXX")
options.parseArguments()

###################################################################
# get Global Tag and ERA
###################################################################
GLOBAL_TAG, ERA = _settings.get_era_and_conditions(options.Scenario)

from Configuration.ProcessModifiers.dd4hep_cff import dd4hep
process = cms.Process("GeometryTest", ERA, dd4hep)
process.load("FWCore.MessageLogger.MessageLogger_cfi")

# Choose Tracker Geometry
if(options.Scenario == _settings.DEFAULT_VERSION):
    print("Loading default scenario: ", _settings.DEFAULT_VERSION)
    process.load('Configuration.Geometry.GeometryDD4hepExtended2026DefaultReco_cff')
else:
    process.load('Configuration.Geometry.GeometryDD4hepExtended'+options.Scenario+'Reco_cff')

process.TrackerGeometricDetESModule = cms.ESProducer( "TrackerGeometricDetESModule",
                                                      fromDDD = cms.bool( False ),
                                                      fromDD4hep = cms.bool( True )
                                                     )

process.es_prefer_geomdet = cms.ESPrefer("TrackerGeometricDetESModule","")

process.load("Geometry.TrackerGeometryBuilder.TrackerAdditionalParametersPerDet_cfi")

process.load("Alignment.CommonAlignmentProducer.FakeAlignmentSource_cfi")
process.preferFakeAlign = cms.ESPrefer("FakeAlignmentSource") 

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.out = cms.OutputModule("AsciiOutputModule")

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.debugModules.append('*')
process.MessageLogger.cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG'),
            default = cms.untracked.PSet( limit = cms.untracked.int32(0) ),
            TrackerNumberingBuilder = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
            TrackerGeometryBuilder = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
            ModuleInfo = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
            )

process.prod = cms.EDAnalyzer("ModuleInfo",
    fromDDD = cms.bool(False),
    printDDD = cms.untracked.bool(False),
    tolerance = cms.untracked.double(1.0e-23)
)

process.p1 = cms.Path(process.prod)
process.ep = cms.EndPath(process.out)


