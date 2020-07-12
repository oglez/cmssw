#
# Testing configuration file for the Phase-2 unpacker.
#
# Written by Oscar Gonzalez (2019_06_15)
#                            2019_10_16  adapted to v8 of the payload
#

import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("DTNT",eras.Run2_2018)  ## ESTO ES LA HOSTIA DE LIOSO CON LO QUE CAMBIA... !OJITO!
process.load('Configuration/StandardSequences/Services_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")  #DB v2, at least since GR_E_V42

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1001) )   # Number of events

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(
    'file:/eos/cms/store/data/Commissioning2019/MiniDaq/RAW/v1/000/333/413/00000/FC5B8DCC-F475-B145-98A6-2DA07749A3DF.root'
))

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.GlobalTag.globaltag = '101X_dataRun2_Express_v7' ## 2018 data

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')

# Setting the module for the SX5 unpacker we are testing!
process.load("EventFilter.DTRawToDigi.dtab7unpacker_cfi")

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
