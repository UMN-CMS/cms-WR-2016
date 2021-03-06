import FWCore.ParameterSet.Config as cms
### \todo add deltaR cut between leptons?
### \todo add HEEP ID for electrons
### \todo add muon ID
### \todo check composite candidate cut string for pt ordering: pt_0 >= pt_1
### \todo make sure that the miniTree is using objects with looser selection


###

leadingPt=60.
subleadingPt=50.
miniTreeLeptonPt=40.
maxEtaLeptons=2.4
jetPt = 20. # same as preselection


### need the reference for this selection, with link to the presentation and twiki page
jetID=" (neutralHadronEnergyFraction<0.90 && neutralEmEnergyFraction<0.9 && (chargedMultiplicity+neutralMultiplicity)>1 && muonEnergyFraction<0.8) && ((abs(eta)<=2.4 && chargedHadronEnergyFraction>0 && chargedMultiplicity>0 && chargedEmEnergyFraction<0.90) || abs(eta)>2.4)"
#muonIDIso=" isolationR03().sumPt/pt < 0.1"
muonIDIso=' isolationR03().sumPt/pt < 0.1 && userInt("highPtID") == 1'
muonID=' userInt("highPtID") == 1'



############################################################ Jets
#patJetsReapplyJEC
from ExoAnalysis.cmsWR.JEC_cff import *
from ExoAnalysis.cmsWR.produceJERsf_cff import *

### select tight-ID jets
wRtightJets = cms.EDFilter("PATJetSelector",
                          src = cms.InputTag("updatedPatJetsUpdatedJEC"),
                          #src = cms.InputTag("slimmedJets"),
                          cut = cms.string(jetID),
                          )

wRJets = cms.EDFilter("PATJetSelector",
                      src = cms.InputTag("wRtightJets"),
                      cut = cms.string(''),
		)


wRJECUncert = JECUnc.clone(
    src = cms.InputTag('wRJets')
    )


wRJetFilter = cms.EDFilter("CandViewCountFilter",
                                 src = cms.InputTag("wRJets"),
                                 minNumber = cms.uint32(2)
                             )

wRdiJetCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
                                  decay = cms.string("wRJets wRJets"),
                                  #role = cms.string("leading subleading"),
                                  checkCharge = cms.bool(False),
                                  # the cut on the pt of the daughter is to respect the order of leading and subleading:
                                  # if both electrons have pt>60 GeV there will be two di-electron candidates with inversed order
                                  cut = cms.string("mass > 0 && daughter(0).pt>daughter(1).pt"),
)

jetSelectionSeq = cms.Sequence( wRtightJets * wRJets * wRJECUncert * jetResolutionSF) #* wRdiJetCandidate)

############################################################ Electrons
from ExoAnalysis.cmsWR.produceEleScaleSmearing_cff import *

from RecoEgamma.ElectronIdentification.heepIdVarValueMapProducer_cfi import heepIDVarValueMaps

wRHEEPExtrasMap = heepIDVarValueMaps.clone()
wRHEEPExtrasMap.elesMiniAOD = cms.InputTag("calibratedPatElectrons")



wRHEEPElectron = cms.EDProducer('HEEPIDSelector',
                                skipHEEP = cms.bool(False),
                                #electrons= cms.InputTag("slimmedElectrons"),
                                electrons= cms.InputTag("calibratedPatElectrons"),
                                eleHEEPIdMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV70"),
                                eleHEEPIdMap2 = cms.InputTag("wRHEEPExtrasMap","eleTrkPtIso"),
                                eleHEEPIdMap3 = cms.InputTag("wRHEEPExtrasMap","eleNrSaturateIn5x5"),
                                HEEPBitMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV70Bitmap"),
)

wRLooseElectron = cms.EDProducer('HEEPIDSelector',
                                 skipHEEP = cms.bool(True),
                                 #electrons= cms.InputTag("slimmedElectrons"),
                                 electrons= cms.InputTag("calibratedPatElectrons"),
                                 eleHEEPIdMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV70"),
                                 eleHEEPIdMap2 = cms.InputTag("wRHEEPExtrasMap","eleTrkPtIso"),
                                 eleHEEPIdMap3 = cms.InputTag("wRHEEPExtrasMap","eleNrSaturateIn5x5"),
                                 HEEPBitMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV70Bitmap"),
)

wRscaledElectrons = cms.EDProducer("ResidualScaleCorrEle",
                                   src = cms.InputTag("wRHEEPElectron"),
                                   ebReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma:reducedEBRecHits"),
                                  )
wRscaledLooseElectron = cms.EDProducer("ResidualScaleCorrEle",
                                   src = cms.InputTag("wRLooseElectron"),
                                   ebReducedRecHitCollectionMiniAOD = cms.InputTag("reducedEgamma:reducedEBRecHits"),
                                  )
#######ADD FILTER HERE
wRDiEle33CaloIdLMWPMS2Unseeded = cms.EDFilter("DiEle33CaloIdLMWPMS2UnseededFilter",
                                              src = cms.InputTag("wRscaledElectrons")
                                             )

wRminiTreeElectron = cms.EDFilter("PATElectronSelector",
                                  src = cms.InputTag("wRscaledElectrons"),
                                  #src = cms.InputTag("wRHEEPElectron"),
                                  cut = cms.string(''),
)

### create di-electron pair in signal region
wRdiElectronCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
                                       decay = cms.string("wRminiTreeElectron wRminiTreeElectron"),
                                       role = cms.string("leading subleading"),
                                       checkCharge = cms.bool(False),
                                       # the cut on the pt of the daughter is to respect the order of leading and subleading:
                                       # if both electrons have pt>60 GeV there will be two di-electron candidates with inversed order
                                       cut = cms.string("mass > 0 && daughter(0).pt>daughter(1).pt"),
                                   )

wRLoosediElectronCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
                                       decay = cms.string("wRscaledLooseElectron wRscaledLooseElectron"),
                                       role = cms.string("leading subleading"),
                                       checkCharge = cms.bool(False),
                                       # the cut on the pt of the daughter is to respect the order of leading and subleading:
                                       # if both electrons have pt>60 GeV there will be two di-electron candidates with inversed order
                                       cut = cms.string("mass > 0 && daughter(0).pt>daughter(1).pt"),
                                   )

### filter: at least one di-electron candidate in signal region
wRdiElectronCandidateFilter = cms.EDFilter("CandViewCountFilter",
                                           src = cms.InputTag("wRdiElectronCandidate"),
                                           minNumber = cms.uint32(1)
                                           )

edDumper = cms.EDAnalyzer('EventContentAnalyzer')

eleLooseScaleSmearing = eleScaleSmearing.clone()
eleLooseScaleSmearing.electrons_src = cms.InputTag("wRscaledLooseElectron")

electronHEEPSeq = cms.Sequence(wRHEEPExtrasMap * wRHEEPElectron * wRLooseElectron)
electronSelectionSeq = cms.Sequence( wRscaledElectrons * wRscaledLooseElectron * wRminiTreeElectron * wRdiElectronCandidate * wRLoosediElectronCandidate * eleScaleSmearing * eleLooseScaleSmearing)

############################################################ Muons
from ExoAnalysis.cmsWR.produceIdIsoSF_cff import *
from ExoAnalysis.cmsWR.produceTrigSF_cff import *

badGlobalMuonTagger = cms.EDFilter("BadGlobalMuonTagger",
    muons = cms.InputTag("slimmedMuons"),
    vtx   = cms.InputTag("offlineSlimmedPrimaryVertices"),
    muonPtCut = cms.double(20),
    selectClones = cms.bool(False),
    taggingMode = cms.bool(True),
    verbose     = cms.untracked.bool(False)
)
cloneGlobalMuonTagger = badGlobalMuonTagger.clone(
    selectClones = cms.bool(True)
)

removeBadAndCloneGlobalMuons = cms.EDProducer("MuonRefPruner",
    input = cms.InputTag("slimmedMuons"),
    toremove = cms.InputTag("badGlobalMuonTagger", "bad"),
    toremove2 = cms.InputTag("cloneGlobalMuonTagger", "bad")
)

tunePMuons = cms.EDProducer("TunePMuonProducer",
		src = cms.InputTag("removeBadAndCloneGlobalMuons")
		#src = cms.InputTag("slimmedMuons")
		)

### muon ID and isolation
# make a collection of TuneP muons which pass isHighPt ID
tunePIDIsoMuons = cms.EDFilter("PATMuonSelector",
                               src = cms.InputTag("tunePMuons"),
                               cut = cms.string(muonIDIso),
                               )

### add here the trigger matching


wRminiTreeMuon = cms.EDFilter("PATMuonSelector",
                              src = cms.InputTag("tunePIDIsoMuons"),
                              cut = cms.string(''),
                              )

### create di-muon pair in signal region
wRdiMuonCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
                                   decay = cms.string("wRminiTreeMuon wRminiTreeMuon"),
                                   role = cms.string("leading subleading"),
                                   checkCharge = cms.bool(False),
                                   # the cut on the pt of the daughter is to respect the order of leading and subleading:
                                   # if both muons have pt>60 GeV there will be two di-muon candidates with inversed order
                                   #cut = cms.string("mass > 200 && daughter(0).pt>daughter(1).pt"),
                                   cut = cms.string("mass > 0  && daughter(0).pt>daughter(1).pt"),
                                   
									   )

### filter: at least one di-muon candidate in signal region
wRdiMuonCandidateFilter = cms.EDFilter("CandViewCountFilter",
                                           src = cms.InputTag("wRdiMuonCandidate"),
                                           minNumber = cms.uint32(1)
                                           )

muonSelectionSeq = cms.Sequence(badGlobalMuonTagger * cloneGlobalMuonTagger * removeBadAndCloneGlobalMuons * tunePMuons * tunePIDIsoMuons * wRminiTreeMuon *  muonIdIsoSF * muonTrigSF * wRdiMuonCandidate)
############################################################ E-Mu candidate
#mixed flavour candidates
wReleMuCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
                                  decay = cms.string("wRminiTreeElectron wRminiTreeMuon"),
                                  role = cms.string("leading subleading"),
                                  checkCharge = cms.bool(False),
                                  # the cut on the pt of the daughter is to respect the order of leading and subleading:
                                  # if both electrons have pt>60 GeV there will be two di-electron candidates with inversed order
                                  cut = cms.string("mass > 0 && daughter(0).pt>daughter(1).pt"),
)

wRmuEleCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
                                  decay = cms.string("wRminiTreeMuon wRminiTreeElectron"),
                                  role = cms.string("leading subleading"),
                                  checkCharge = cms.bool(False),
                                  # the cut on the pt of the daughter is to respect the order of leading and subleading:
                                  # if both electrons have pt>60 GeV there will be two di-electron candidates with inversed order
                                  cut = cms.string("mass > 0 && daughter(0).pt>daughter(1).pt"),
)


############################################################# di lepton candidate
wRdiLeptonCandidate = cms.EDProducer("CandViewMerger",
                                     src = cms.VInputTag("wRdiElectronCandidate", "wRdiMuonCandidate", "wReleMuCandidate", "wRmuEleCandidate")
                                     )
wRLoosediLeptonCandidate = cms.EDProducer("CandViewMerger",
                                     src = cms.VInputTag("wRLoosediElectronCandidate", "wRdiMuonCandidate", "wReleMuCandidate", "wRmuEleCandidate")
                                     )

wRdiLeptonCandidateFilter = cms.EDFilter("CandViewCountFilter",
                                           src = cms.InputTag("wRdiLeptonCandidate"),
                                           minNumber = cms.uint32(1)
                                           )

wRCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
                                  decay = cms.string("wRdiLeptonCandidate wRdiJetCandidate"),
                             # role = cms.string("leading subleading"),
                                  checkCharge = cms.bool(False),
                                  # the cut on the pt of the daughter is to respect the order of leading and subleading:
                                  # if both electrons have pt>60 GeV there will be two di-electron candidates with inversed order
                                  cut = cms.string("mass > 0"),
)
############################################################# Flavour sideband filter
flavourSidebandSelection='((daughter(0).isElectron && daughter(1).isMuon) || (daughter(1).isElectron && daughter(0).isMuon))'
diLeptonSelection = '(mass>200)'


lowFourObjectSidebandSelector = cms.EDFilter("CandViewSelector",
                                     src = cms.InputTag("wRCandidate"),
                                     cut = cms.string('(mass< 600)')
                                     )
lowFourObjectSidebandFilter  = cms.EDFilter('CandViewCountFilter',
                                          src = cms.InputTag('lowFourObjectSidebandSelector'),
                                          minNumber = cms.uint32(1)
)


flavourSidebandSelector = cms.EDFilter("CandViewSelector",
                                     src = cms.InputTag("wRdiLeptonCandidate"),
                                     cut = cms.string(flavourSidebandSelection),
                                     )

flavourSidebandFilter = cms.EDFilter('CandViewCountFilter',
                                     src = cms.InputTag('flavourSidebandSelector'),
                                     minNumber = cms.uint32(1)
)

## to be fixed
lowDiLeptonSidebandSelector = cms.EDFilter("CandViewSelector",
                                     src = cms.InputTag("wRdiLeptonCandidate"),
                                     cut = cms.string('(mass< 200) && (!'+flavourSidebandSelection+')' )
                                     )
lowLooseDiLeptonSidebandSelector = cms.EDFilter("CandViewSelector",
                                     src = cms.InputTag("wRLoosediLeptonCandidate"),
                                     cut = cms.string('(mass< 200) && (!'+flavourSidebandSelection+')' )
                                     )
lowDiLeptonSidebandFilter  = cms.EDFilter('CandViewCountFilter',
                                          src = cms.InputTag('lowDiLeptonSidebandSelector'),
                                          minNumber = cms.uint32(1)
)

####
signalRegionSelector = cms.EDFilter("CandViewSelector",
                                     src = cms.InputTag("wRdiLeptonCandidate"),
                                     cut = cms.string("(! "+flavourSidebandSelection+') && '+diLeptonSelection),
                                     )

signalRegionFilter =  cms.EDFilter('CandViewCountFilter',
                                     src = cms.InputTag('signalRegionSelector'),
                                     minNumber = cms.uint32(1),
)

signalRegionEESelector = cms.EDFilter("CandViewSelector",
                                      src = cms.InputTag('signalRegionSelector'),
                                      cut = cms.string('(daughter(0).isElectron && daughter(1).isElectron)'),
                                      )
signalRegionMuMuSelector = cms.EDFilter("CandViewSelector",
                                      src = cms.InputTag('signalRegionSelector'),
                                      cut = cms.string('(daughter(0).isMuon && daughter(1).isMuon)'),
                                      )
signalRegionEEFilter = cms.EDFilter("CandViewCountFilter",
                                    src = cms.InputTag("signalRegionEESelector"),
                                    minNumber = cms.uint32(1),
                                    )
signalRegionMuMuFilter = cms.EDFilter("CandViewCountFilter",
                                    src = cms.InputTag("signalRegionMuMuSelector"),
                                    minNumber = cms.uint32(1),
                                    )

signalRegionFilterSeq = cms.Sequence(~flavourSidebandFilter * ~lowDiLeptonSidebandSelector)



selectionSequence = cms.Sequence(
    ( jetSelectionSeq + electronSelectionSeq + muonSelectionSeq ) * wReleMuCandidate * wRmuEleCandidate * wRdiLeptonCandidate * wRLoosediLeptonCandidate *  flavourSidebandSelector * lowDiLeptonSidebandSelector * lowLooseDiLeptonSidebandSelector * signalRegionSelector * signalRegionEESelector * signalRegionMuMuSelector
        )

filterSequence = cms.Sequence(
    #wRJetFilter *
    wRdiLeptonCandidateFilter
)
