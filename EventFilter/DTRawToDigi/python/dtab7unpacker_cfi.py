import FWCore.ParameterSet.Config as cms
import os

dtAB7unpacker = cms.EDProducer("OglezDTAB7RawToDigi",
                               DTAB7_FED_Source = cms.InputTag("rawDataCollector"),
                               feds = cms.untracked.vint32( 1368,),
                               debug = cms.untracked.bool(False),
                               doHexDumping = cms.untracked.bool(False),
                               rawTPVars = cms.untracked.bool(False),
                               channelMapping = cms.untracked.string("july2019"),
                               xShiftFilename = cms.string(os.environ['CMSSW_BASE']+'/src/L1Trigger/DTPhase2Trigger/data/wire_rawId_x.txt'),
                               zShiftFilename = cms.string(os.environ['CMSSW_BASE']+'/src/L1Trigger/DTPhase2Trigger/data/wire_rawId_z.txt')
)
