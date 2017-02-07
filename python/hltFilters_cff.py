import FWCore.ParameterSet.Config as cms
import HLTrigger.HLTfilters.hltHighLevel_cfi as hlt


### \page triggers Triggers
## Set of triggers used in the analysis
## 
## 
## Here the list of triggers:
##  - \b HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v*  
##    - electron channel
##  - \b HLT_Mu50_v*'                          
##    - muon channel trigger
##  - \b HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v*  
##    - emujj sideband
##  - \b HLT_Ele30WP60_Ele8_Mass55_v*          
##    - electron tag&probe
##  - \b HLT_Ele30WP60_SC4_Mass55_v*           
##    -  electron tag&probe
##  - \b HLT_IsoMu22_v*                        
##    - muon tag&probe
##  - \b HLT_IsoMu27_v*                        
##    - muon tag&probe
## 

wReejjHLTFilterGsfTrkIdVL = hlt.hltHighLevel.clone(
    throw = cms.bool(False),
    HLTPaths = [
        'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v*', ### \ingroup hlt_Group electron channel trigger
        #'HLT_DoubleEle33_CaloIdL_MW_v*'
    ]
)

wReejjHLTFilterMW = hlt.hltHighLevel.clone(
    throw = cms.bool(False),
    HLTPaths = [
        #'HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v*', ### \ingroup hlt_Group electron channel trigger
        'HLT_DoubleEle33_CaloIdL_MW_v*'
    ]
)

wRmumujjHLTFilter = hlt.hltHighLevel.clone(
    throw = cms.bool(False),
    HLTPaths = [
        'HLT_Mu50_v*',        ## \ingroup hlt_Group muon channel trigger
        'HLT_TkMu50_v*'
        ]
)

wRemujjHLTFilter =  hlt.hltHighLevel.clone(
    throw = cms.bool(False),
    HLTPaths = [
        'HLT_Mu33_Ele33_CaloIdL_GsfTrkIdVL_v*', ## \ingroup hlt_Group flavour sideband trigger
        'HLT_Mu30_Ele30_CaloIdL_GsfTrkIdVL_v*',
        ]
)

tagAndProbeDoubleEleHLTFilter = hlt.hltHighLevel.clone(
    throw = cms.bool(False),
    HLTPaths = [
        'HLT_Ele27_WPTight_Gsf_v*',  ## \ingroup hlt_Group electron tagAndProbe trigger
    ]
)

tagAndProbeDoubleMuHLTFilter = hlt.hltHighLevel.clone(
    throw = cms.bool(False),
    HLTPaths = [
        'HLT_IsoMu24_v*', ## \ingroup hlt_Group muon tagAndProbe trigger
        'HLT_IsoTkMu24_v*' 
    ]
)


wRHLTFilter_MC =  hlt.hltHighLevel.clone(
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    throw = cms.bool(False),
    HLTPaths = wReejjHLTFilterMW .HLTPaths + wRmumujjHLTFilter.HLTPaths + wRemujjHLTFilter.HLTPaths
)

tagAndProbeHLTFilter_MC = hlt.hltHighLevel.clone(
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    throw = cms.bool(False),
    HLTPaths = tagAndProbeDoubleMuHLTFilter.HLTPaths + tagAndProbeDoubleEleHLTFilter.HLTPaths
)

wRHLTFilter_data =  hlt.hltHighLevel.clone(
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    throw = cms.bool(False),
    HLTPaths = wReejjHLTFilterMW.HLTPaths + wRmumujjHLTFilter.HLTPaths + wRemujjHLTFilter.HLTPaths
)

tagAndProbeHLTFilter_data = hlt.hltHighLevel.clone(
    TriggerResultsTag = cms.InputTag("TriggerResults","","HLT"),
    throw = cms.bool(False),
    HLTPaths = tagAndProbeDoubleMuHLTFilter.HLTPaths + tagAndProbeDoubleEleHLTFilter.HLTPaths
)

