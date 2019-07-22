#
# Testing configuration file for the Phase-2 unpacker.
#
# Written by Oscar Gonzalez (2019_06_15)
#

import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras

process = cms.Process("DTNT",eras.Run2_2018)  ## ESTO ES LA HOSTIA DE LIOSO CON LO QUE CAMBIA... !OJITO!
process.load('Configuration/StandardSequences/Services_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")  #DB v2, at least since GR_E_V42

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(101) )   # Number of events

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource",fileNames = cms.untracked.vstring(
        'file:/eos/cms/store/data/Commissioning2019/Cosmics/RAW/v1/000/330/161/00000/969B8A72-1A03-224C-9B2F-CA479658075D.root'
        ))

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.GlobalTag.globaltag = '101X_dataRun2_Express_v7' ## 2018 data

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cff")
process.load("Geometry.DTGeometry.dtGeometry_cfi")
process.DTGeometryESModule.applyAlignment = False


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
