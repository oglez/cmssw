#
# Testing configuration file for the Phase-2 unpacker.
#
# Written by Oscar Gonzalez (2019_06_15)
#                            2019_10_16  adapted to v8 of the payload
#                            2023_02_09  adapted to the v11.4 of the unpacker, developed for CMSSW_12_4_X
#

import os

import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("DTNT",eras.Run3)
process.load('Configuration/StandardSequences/Services_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(int(os.getenv('DTTESTUNPACKER_NEVTS',1001))) )   # Number of events

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(
#    'file:/eos/cms/store/data/Commissioning2019/MiniDaq/RAW/v1/000/333/413/00000/FC5B8DCC-F475-B145-98A6-2DA07749A3DF.root'
#     'file:/eos/cms/tier0/store/data/Commissioning2022/Cosmics/RAW-RECO/CosmicTP-PromptReco-v1/000/347/868/00000/2c1c5e3f-69e8-4369-b8f4-173889c3f5a9.root'
    '/store/data/Commissioning2023/Cosmics/RAW/v1/000/363/604/00000/df38f996-1144-439f-9a30-32b0e5ee605d.root'
))

# We may overwrite the file to run over using
if os.getenv('DTTESTUNPACKER_FILES',None) is not None:
    process.source.fileNames = cms.untracked.vstring(os.getenv('DTTESTUNPACKER_FILES').split(','))

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# We may overwrite the Global Tag using other parameter of the environment!
process.GlobalTag.globaltag = os.getenv('DTTESTUNPACKER_GT','124X_dataRun3_Express_v4')

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')

# Setting the module for the SX5 unpacker we are testing!
process.load("EventFilter.DTRawToDigi.dtab7unpacker_cfi")

process.dtAB7unpacker.produceExtendedPrimitives = cms.untracked.bool(True)

# ###################
# Setting up the path
# ###################

process.p = cms.Path(process.dtAB7unpacker)

# #################################
# Saving the output of the unpacker
# #################################

process.out = cms.OutputModule("PoolOutputModule"
                               , outputCommands = cms.untracked.vstring(
                                   "drop *",
                                   "keep *_TriggerResults_*_HLT",
                                   "keep *_dtAB7unpacker_*_*")
                               , fileName = cms.untracked.string("unpacker_output.root")
                               , SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("p"))
)

process.ep = cms.EndPath(process.out)

# =======================================================================
