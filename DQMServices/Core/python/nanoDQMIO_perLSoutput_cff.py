import FWCore.ParameterSet.Config as cms

# Configuration file for per-lumisection DQMIO
# Use this file to specify which monitoring elements (MEs) will be stored per lumisection in the DQMIO format.
# For more information, see https://twiki.cern.ch/twiki/bin/view/CMS/PerLsDQMIO.

# Use the full ME path, as displayed for example in the DQM GUI.

# The current selection of MEs is for the processing of 2024 data.

nanoDQMIO_perLSoutput = cms.PSet(
  MEsToSave = cms.untracked.vstring(*(
    "Hcal/DigiTask/Occupancy/depth/depth1",
    "Hcal/DigiTask/Occupancy/depth/depth2",
    "Hcal/DigiTask/Occupancy/depth/depth3",
    "Hcal/DigiTask/Occupancy/depth/depth4",
    "Hcal/DigiTask/Occupancy/depth/depth5",
    "Hcal/DigiTask/Occupancy/depth/depth6",
    "Hcal/DigiTask/Occupancy/depth/depth7",
    "Hcal/DigiTask/Occupancy/depth/depthHO",
    "Hcal/DigiTask/OccupancyCut/depth/depth1",
    "Hcal/DigiTask/OccupancyCut/depth/depth2",
    "Hcal/DigiTask/OccupancyCut/depth/depth3",
    "Hcal/DigiTask/OccupancyCut/depth/depth4",
    "Hcal/DigiTask/OccupancyCut/depth/depth5",
    "Hcal/DigiTask/OccupancyCut/depth/depth6",
    "Hcal/DigiTask/OccupancyCut/depth/depth7",
    "Hcal/DigiTask/OccupancyCut/depth/depthHO",

    "EcalBarrel/EBOccupancyTask/EBOT digi occupancy",
    "EcalEndcap/EEOccupancyTask/EEOT digi occupancy EE -",
    "EcalEndcap/EEOccupancyTask/EEOT digi occupancy EE +",
    "EcalBarrel/EBOccupancyTask/EBOT DCC entries",
    "EcalEndcap/EEOccupancyTask/EEOT DCC entries",
    "Ecal/EventInfo/processedEvents",

    "PixelPhase1/Tracks/charge_PXBarrel",
    "PixelPhase1/Tracks/charge_PXForward",
    "PixelPhase1/Tracks/PXBarrel/charge_PXLayer_1",
    "PixelPhase1/Tracks/PXBarrel/charge_PXLayer_2",
    "PixelPhase1/Tracks/PXBarrel/charge_PXLayer_3",
    "PixelPhase1/Tracks/PXBarrel/charge_PXLayer_4",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_+1",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_+2",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_+3",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_-1",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_-2",
    "PixelPhase1/Tracks/PXForward/charge_PXDisk_-3",
    "PixelPhase1/Tracks/PXBarrel/size_PXLayer_1",
    "PixelPhase1/Tracks/PXBarrel/size_PXLayer_2",
    "PixelPhase1/Tracks/PXBarrel/size_PXLayer_3",
    "PixelPhase1/Tracks/PXBarrel/size_PXLayer_4",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_+1",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_+2",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_+3",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_-1",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_-2",
    "PixelPhase1/Tracks/PXForward/size_PXDisk_-3",

    "CSC/CSCOfflineMonitor/recHits/hRHGlobalm1",
    "CSC/CSCOfflineMonitor/recHits/hRHGlobalm2",
    "CSC/CSCOfflineMonitor/recHits/hRHGlobalm3",
    "CSC/CSCOfflineMonitor/recHits/hRHGlobalm4",
    "CSC/CSCOfflineMonitor/recHits/hRHGlobalp1",
    "CSC/CSCOfflineMonitor/recHits/hRHGlobalp2",
    "CSC/CSCOfflineMonitor/recHits/hRHGlobalp3",
    "CSC/CSCOfflineMonitor/recHits/hRHGlobalp4",

    "HLT/Vertexing/hltPixelVertices/hltPixelVertices/goodvtxNbr",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/mon_eta",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/mon_hits",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/mon_phi",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/mon_pt",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/mon_unMatched_eta",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/mon_unMatched_hits",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/mon_unMatched_phi",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/mon_unMatched_pt",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/ref_eta",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/ref_hits",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/ref_matched_eta",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/ref_matched_hits",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/ref_matched_phi",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/ref_matched_pt",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/ref_phi",
    "HLT/Tracking/ValidationWRTOffline/hltMergedWrtHighPurityPV/ref_pt",
    "HLT/Tracking/pixelTracks/GeneralProperties/Chi2Prob_GenTk",
    "HLT/Tracking/pixelTracks/GeneralProperties/Chi2oNDFVsEta_ImpactPoint_GenTk",
    "HLT/Tracking/pixelTracks/GeneralProperties/DeltaZToPVZoom_GenTk",
    "HLT/Tracking/pixelTracks/GeneralProperties/DistanceOfClosestApproachToPVVsPhi_GenTk",
    "HLT/Tracking/pixelTracks/GeneralProperties/DistanceOfClosestApproachToPVZoom_GenTk",
    "HLT/Tracking/pixelTracks/GeneralProperties/NumberOfTracks_GenTk",
    "HLT/Tracking/tracks/GeneralProperties/Chi2Prob_GenTk",
    "HLT/Tracking/tracks/GeneralProperties/Chi2oNDFVsEta_ImpactPoint_GenTk",
    "HLT/Tracking/tracks/GeneralProperties/DeltaZToPVZoom_GenTk",
    "HLT/Tracking/tracks/GeneralProperties/DistanceOfClosestApproachToPVVsPhi_GenTk",
    "HLT/Tracking/tracks/GeneralProperties/DistanceOfClosestApproachToPVZoom_GenTk",
    "HLT/Tracking/tracks/GeneralProperties/NumberOfTracks_GenTk",
    "HLT/Tracking/tracks/LUMIanalysis/NumberEventsVsLUMI",
    "HLT/Tracking/tracks/PUmonitoring/NumberEventsVsGoodPVtx",

    "PixelPhase1/Tracks/num_clusters_ontrack_PXBarrel",
    "PixelPhase1/Tracks/num_clusters_ontrack_PXForward",
    "PixelPhase1/Tracks/clusterposition_zphi_ontrack",
    "PixelPhase1/Tracks/PXBarrel/clusterposition_zphi_ontrack_PXLayer_1",
    "PixelPhase1/Tracks/PXBarrel/clusterposition_zphi_ontrack_PXLayer_2",
    "PixelPhase1/Tracks/PXBarrel/clusterposition_zphi_ontrack_PXLayer_3",
    "PixelPhase1/Tracks/PXBarrel/clusterposition_zphi_ontrack_PXLayer_4",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_+1",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_+2",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_+3",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_-1",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_-2",
    "PixelPhase1/Tracks/PXForward/clusterposition_xy_ontrack_PXDisk_-3",

    "PixelPhase1/Phase1_MechanicalView/PXBarrel/digi_occupancy_per_SignedModuleCoord_per_SignedLadderCoord_PXLayer_1",
    "PixelPhase1/Phase1_MechanicalView/PXBarrel/digi_occupancy_per_SignedModuleCoord_per_SignedLadderCoord_PXLayer_2",
    "PixelPhase1/Phase1_MechanicalView/PXBarrel/digi_occupancy_per_SignedModuleCoord_per_SignedLadderCoord_PXLayer_3",
    "PixelPhase1/Phase1_MechanicalView/PXBarrel/digi_occupancy_per_SignedModuleCoord_per_SignedLadderCoord_PXLayer_4",
    "PixelPhase1/Phase1_MechanicalView/PXForward/digi_occupancy_per_SignedDiskCoord_per_SignedBladePanelCoord_PXRing_1",
    "PixelPhase1/Phase1_MechanicalView/PXForward/digi_occupancy_per_SignedDiskCoord_per_SignedBladePanelCoord_PXRing_2",
    "PixelPhase1/Phase1_MechanicalView/PXBarrel/clusters_per_SignedModuleCoord_per_SignedLadderCoord_PXLayer_1",
    "PixelPhase1/Phase1_MechanicalView/PXBarrel/clusters_per_SignedModuleCoord_per_SignedLadderCoord_PXLayer_2",
    "PixelPhase1/Phase1_MechanicalView/PXBarrel/clusters_per_SignedModuleCoord_per_SignedLadderCoord_PXLayer_3",
    "PixelPhase1/Phase1_MechanicalView/PXBarrel/clusters_per_SignedModuleCoord_per_SignedLadderCoord_PXLayer_4",
    "PixelPhase1/Phase1_MechanicalView/PXForward/clusters_per_SignedDiskCoord_per_SignedBladePanelCoord_PXRing_1",
    "PixelPhase1/Phase1_MechanicalView/PXForward/clusters_per_SignedDiskCoord_per_SignedBladePanelCoord_PXRing_2",

    "SiStrip/MechanicalView/TEC/PLUS/wheel_1/NormalizedHitResiduals_TEC__wheel__1",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_2/NormalizedHitResiduals_TEC__wheel__2",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_3/NormalizedHitResiduals_TEC__wheel__3",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_4/NormalizedHitResiduals_TEC__wheel__4",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_5/NormalizedHitResiduals_TEC__wheel__5",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_6/NormalizedHitResiduals_TEC__wheel__6",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_7/NormalizedHitResiduals_TEC__wheel__7",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_8/NormalizedHitResiduals_TEC__wheel__8",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_9/NormalizedHitResiduals_TEC__wheel__9",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_1/NormalizedHitResiduals_TEC__wheel__1",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_2/NormalizedHitResiduals_TEC__wheel__2",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_3/NormalizedHitResiduals_TEC__wheel__3",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_4/NormalizedHitResiduals_TEC__wheel__4",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_5/NormalizedHitResiduals_TEC__wheel__5",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_6/NormalizedHitResiduals_TEC__wheel__6",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_7/NormalizedHitResiduals_TEC__wheel__7",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_8/NormalizedHitResiduals_TEC__wheel__8",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_9/NormalizedHitResiduals_TEC__wheel__9",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_1/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__1",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_2/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__2",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_3/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__3",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_4/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__4",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_5/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__5",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_6/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__6",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_7/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__7",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_8/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__8",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_9/Summary_ClusterStoNCorr__OnTrack__TEC__PLUS__wheel__9",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_1/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__1",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_2/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__2",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_3/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__3",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_4/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__4",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_5/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__5",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_6/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__6",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_7/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__7",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_8/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__8",
    "SiStrip/MechanicalView/TEC/MINUS/wheel_9/Summary_ClusterStoNCorr__OnTrack__TEC__MINUS__wheel__9",
    "SiStrip/MechanicalView/TIB/layer_1/NormalizedHitResiduals_TIB__Layer__1",
    "SiStrip/MechanicalView/TIB/layer_2/NormalizedHitResiduals_TIB__Layer__2",
    "SiStrip/MechanicalView/TIB/layer_3/NormalizedHitResiduals_TIB__Layer__3",
    "SiStrip/MechanicalView/TIB/layer_4/NormalizedHitResiduals_TIB__Layer__4",
    "SiStrip/MechanicalView/TIB/layer_1/Summary_ClusterStoNCorr__OnTrack__TIB__layer__1",
    "SiStrip/MechanicalView/TIB/layer_2/Summary_ClusterStoNCorr__OnTrack__TIB__layer__2",
    "SiStrip/MechanicalView/TIB/layer_3/Summary_ClusterStoNCorr__OnTrack__TIB__layer__3",
    "SiStrip/MechanicalView/TIB/layer_4/Summary_ClusterStoNCorr__OnTrack__TIB__layer__4",
    "SiStrip/MechanicalView/TID/PLUS/wheel_1/NormalizedHitResiduals_TID__wheel__1",
    "SiStrip/MechanicalView/TID/PLUS/wheel_2/NormalizedHitResiduals_TID__wheel__2",
    "SiStrip/MechanicalView/TID/PLUS/wheel_3/NormalizedHitResiduals_TID__wheel__3",
    "SiStrip/MechanicalView/TID/MINUS/wheel_1/NormalizedHitResiduals_TID__wheel__1",
    "SiStrip/MechanicalView/TID/MINUS/wheel_2/NormalizedHitResiduals_TID__wheel__2",
    "SiStrip/MechanicalView/TID/MINUS/wheel_3/NormalizedHitResiduals_TID__wheel__3",
    "SiStrip/MechanicalView/TID/PLUS/wheel_1/Summary_ClusterStoNCorr__OnTrack__TID__PLUS__wheel__1",
    "SiStrip/MechanicalView/TID/PLUS/wheel_2/Summary_ClusterStoNCorr__OnTrack__TID__PLUS__wheel__2",
    "SiStrip/MechanicalView/TID/PLUS/wheel_3/Summary_ClusterStoNCorr__OnTrack__TID__PLUS__wheel__3",
    "SiStrip/MechanicalView/TID/MINUS/wheel_1/Summary_ClusterStoNCorr__OnTrack__TID__MINUS__wheel__1",
    "SiStrip/MechanicalView/TID/MINUS/wheel_2/Summary_ClusterStoNCorr__OnTrack__TID__MINUS__wheel__2",
    "SiStrip/MechanicalView/TID/MINUS/wheel_3/Summary_ClusterStoNCorr__OnTrack__TID__MINUS__wheel__3",
    "SiStrip/MechanicalView/TOB/layer_1/NormalizedHitResiduals_TOB__Layer__1",
    "SiStrip/MechanicalView/TOB/layer_2/NormalizedHitResiduals_TOB__Layer__2",
    "SiStrip/MechanicalView/TOB/layer_3/NormalizedHitResiduals_TOB__Layer__3",
    "SiStrip/MechanicalView/TOB/layer_4/NormalizedHitResiduals_TOB__Layer__4",
    "SiStrip/MechanicalView/TOB/layer_5/NormalizedHitResiduals_TOB__Layer__5",
    "SiStrip/MechanicalView/TOB/layer_6/NormalizedHitResiduals_TOB__Layer__6",
    "SiStrip/MechanicalView/TOB/layer_1/Summary_ClusterStoNCorr__OnTrack__TOB__layer__1",
    "SiStrip/MechanicalView/TOB/layer_2/Summary_ClusterStoNCorr__OnTrack__TOB__layer__2",
    "SiStrip/MechanicalView/TOB/layer_3/Summary_ClusterStoNCorr__OnTrack__TOB__layer__3",
    "SiStrip/MechanicalView/TOB/layer_4/Summary_ClusterStoNCorr__OnTrack__TOB__layer__4",
    "SiStrip/MechanicalView/TOB/layer_5/Summary_ClusterStoNCorr__OnTrack__TOB__layer__5",
    "SiStrip/MechanicalView/TOB/layer_6/Summary_ClusterStoNCorr__OnTrack__TOB__layer__6",
    "SiStrip/MechanicalView/MainDiagonal Position",
    "SiStrip/MechanicalView/NumberOfClustersInPixel",
    "SiStrip/MechanicalView/NumberOfClustersInStrip",
    "SiStrip/MechanicalView/TID/PLUS/wheel_1/TkHMap_NumberOfDigi_TIDP_D1",
    "SiStrip/MechanicalView/TID/PLUS/wheel_1/TkHMap_NumberOfCluster_TIDP_D1",
    "SiStrip/MechanicalView/TIB/layer_1/TkHMap_NumberOfDigi_TIB_L1",
    "SiStrip/MechanicalView/TIB/layer_1/TkHMap_NumberOfCluster_TIB_L1",
    "SiStrip/MechanicalView/TOB/layer_1/TkHMap_NumberOfDigi_TOB_L1",
    "SiStrip/MechanicalView/TOB/layer_1/TkHMap_NumberOfCluster_TOB_L1",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_1/TkHMap_NumberOfDigi_TECP_W1",
    "SiStrip/MechanicalView/TEC/PLUS/wheel_1/TkHMap_NumberOfCluster_TECP_W1",

    "Tracking/TrackParameters/generalTracks/LSanalysis/Chi2oNDF_lumiFlag_GenTk",  
    "Tracking/TrackParameters/generalTracks/LSanalysis/NumberOfRecHitsPerTrack_lumiFlag_GenTk", 
    "Tracking/TrackParameters/generalTracks/LSanalysis/NumberOfTracks_lumiFlag_GenTk", 
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/SIPDxyToPV_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/SIPDzToPV_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/SIP3DToPV_GenTk",
    "Tracking/TrackParameters/generalTracks/HitProperties/NumberOfMissingOuterRecHitsPerTrack_GenTk",
    "Tracking/TrackParameters/generalTracks/HitProperties/NumberMORecHitsPerTrackVsPt_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/TrackEtaPhi_ImpactPoint_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/NumberOfTracks_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/HitProperties/NumberOfRecHitsPerTrack_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/TrackPt_ImpactPoint_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/Chi2oNDF_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/TrackPhi_ImpactPoint_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/TrackEta_ImpactPoint_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/HitProperties/NumberOfRecHitsPerTrack_Strip_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/HitProperties/NumberOfRecHitsPerTrack_Pixel_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/DistanceOfClosestApproachToBS_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/DistanceOfClosestApproachToBSdz_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/DistanceOfClosestApproachToBSVsPhi_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/DistanceOfClosestApproachToBSVsEta_GenTk",
    "Tracking/TrackParameters/highPurityTracks/pt_1/GeneralProperties/TrackQoverP_ImpactPoint_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/Quality_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/NumberofTracks_Hardvtx_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/NumberofTracks_PUvtx_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TrackPtHighpurity_ImpactPoint_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TrackPtTight_ImpactPoint_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TrackPtLoose_ImpactPoint_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TrackEtaHighpurity_ImpactPoint_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TrackEtaTight_ImpactPoint_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TrackEtaLoose_ImpactPoint_GenTk",
    "Tracking/PrimaryVertices/highPurityTracks/pt_0to1/offline/NumberOfGoodPVtx_offline",
    "Tracking/PrimaryVertices/highPurityTracks/pt_0to1/offline/GoodPVtxNumberOfTracks_offline",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/NumberofTracks_Hardvtx_PUvtx_GenTk",
    "Tracking/PrimaryVertices/highPurityTracks/pt_0to1/offline/FractionOfGoodPVtx_offline",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TkEtaPhi_Ratio_byFoldingmap_ImpactPoint_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TkEtaPhi_Ratio_byFoldingmap_op_ImpactPoint_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TkEtaPhi_RelativeDifference_byFoldingmap_ImpactPoint_GenTk",
    "Tracking/TrackParameters/generalTracks/GeneralProperties/TkEtaPhi_RelativeDifference_byFoldingmap_op_ImpactPoint_GenTk",

    "OfflinePV/offlinePrimaryVertices/tagVtxProb",
    "OfflinePV/offlinePrimaryVertices/tagType",
    "OfflinePV/Resolution/PV/pull_x",
    "OfflinePV/Resolution/PV/pull_y",
    "OfflinePV/Resolution/PV/pull_z",
    "OfflinePV/offlinePrimaryVertices/tagDiffX",
    "OfflinePV/offlinePrimaryVertices/tagDiffY",

    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_highPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_highPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_mediumPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_mediumPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_lowPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/CHFrac_lowPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_highPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_highPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_mediumPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_mediumPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_lowPt_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/ChMultiplicity_lowPt_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Constituents",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Eta",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Eta_uncor",
    "JetMET/Jet/Cleanedak4PFJetsCHS/JetEnergyCorr",
    "JetMET/Jet/Cleanedak4PFJetsCHS/NJets",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Phi",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Phi_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Phi_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsCHS/Pt",
    "JetMET/Jet/Cleanedak4PFJetsPuppi/Pt"
    "JetMET/Jet/Cleanedak4PFJetsPuppi/Phi",
    "JetMET/Jet/Cleanedak4PFJetsPuppi/Phi_Barrel",
    "JetMET/Jet/Cleanedak4PFJetsPuppi/Phi_EndCap",
    "JetMET/Jet/Cleanedak4PFJetsPuppi/JetEnergyCorr",
    "JetMET/Jet/Cleanedak4PFJetsPuppi/NJets",
    "JetMET/Jet/Cleanedak4PFJetsPuppi/Eta",
    "JetMET/Jet/Cleanedak4PFJetsPuppi/Eta_uncor",
    "JetMET/MET/pfMETT1/Cleaned/METSig",
    "JetMET/vertices",     

    "Muons/MuonRecoAnalyzer/GlbMuon_Glb_pt",
    "Muons/MuonRecoAnalyzer/GlbMuon_Glb_eta",
    "Muons/MuonRecoAnalyzer/GlbMuon_Glb_phi",
    "Muons/MuonRecoAnalyzer/Res_TkGlb_qOverlap",
    "Muons/diMuonHistograms/GlbGlbMuon_LM",
    "Muons/diMuonHistograms/GlbGlbMuon_HM",
    "Muons/Isolation/global/relPFIso_R03",
    "Muons/globalMuons/GeneralProperties/NumberOfMeanRecHitsPerTrack_glb",
    "Muons/standAloneMuonsUpdatedAtVtx/HitProperties/NumberOfValidRecHitsPerTrack_sta",
    "Muons/MuonRecoOneHLT/GlbMuon_Glb_pt",
    "Muons/MuonRecoOneHLT/GlbMuon_Glb_eta",

    "Egamma/Electrons/Ele5_TagAndProbe/ele0_vertexPt_barrel",
    "Egamma/Electrons/Ele5_TagAndProbe/ele1_vertexPt_endcaps",
    "Egamma/Electrons/Ele5_TagAndProbe/ele2_vertexEta",
    "Egamma/Electrons/Ele5_TagAndProbe/ele5_vertexZ",
    "Egamma/Electrons/Ele5_TagAndProbe/ele10_Eop_barrel",
    "Egamma/Electrons/Ele5_TagAndProbe/ele10_Eop_endcaps",
    "Egamma/Electrons/Ele5_TagAndProbe/ele101_etaEff",
    "Egamma/Electrons/Ele5_TagAndProbe/ele102_phiEff",
    "Egamma/Electrons/Ele5_TagAndProbe/ele201_mee_os",
  ) )
)
