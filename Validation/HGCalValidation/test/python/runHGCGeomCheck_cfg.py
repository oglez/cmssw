###############################################################################
# Way to use this:
#   cmsRun runHGCGeomCheck_cfg.py geometry=D88
#
#   Options for geometry D88, D92, D93
#
###############################################################################
import FWCore.ParameterSet.Config as cms
import os, sys, imp, re
import FWCore.ParameterSet.VarParsing as VarParsing

####################################################################
### SETUP OPTIONS
options = VarParsing.VarParsing('standard')
options.register('geometry',
                 "D88",
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                  "geometry of operations: D88, D92, D93")

### get and parse the command line arguments
options.parseArguments()

print(options)

####################################################################
# Use the options
from Configuration.Eras.Era_Phase2C11M9_cff import Phase2C11M9
process = cms.Process('GeomCheck',Phase2C11M9)

geomFile = "Configuration.Geometry.GeometryExtended2026" + options.geometry + "Reco_cff"
inFile = "file:testHGCalSimWatcher" + options.geometry + ".root"
outFile = "hgcGeomCheck" + options.geometry + ".root"

print("Geometry file: ", geomFile)
print("Input file:    ", inFile)
print("Output file:   ", outFile)

process.load(geomFile)
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')    
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T21', '')

process.MessageLogger.cerr.FwkReport.reportEvery = 5
if hasattr(process,'MessageLogger'):
    process.MessageLogger.HGCalValid=dict()
    process.MessageLogger.HGCalGeom=dict()

process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(inFile)
)

process.load('Validation.HGCalValidation.hgcGeomCheck_cff')

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(outFile),
                                   closeFileFast = cms.untracked.bool(True)
)

SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",ignoreTotal = cms.untracked.int32(1) )

process.p = cms.Path(process.hgcGeomCheck)
