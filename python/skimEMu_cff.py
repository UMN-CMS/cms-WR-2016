import FWCore.ParameterSet.Config as cms

# Selection of electrons from the slimmedElectrons collection (PAT)
""" \addtogroup electronSkim_Group electronSkim sequences
@{
"""

### make sure the evt has at least two jets, and one has a nontrivial pT
emuwRhardJet = cms.EDFilter("PATJetRefSelector",
		src = cms.InputTag("slimmedJets"),
		cut = cms.string("pt>20")
		)

emuwRsoftJet = cms.EDFilter("PATJetRefSelector",
		src = cms.InputTag("slimmedJets"),
		cut = cms.string("pt>8")
		)

emuwRdiJetCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
		decay = cms.string("emuwRhardJet emuwRsoftJet"),
		role = cms.string("leadingJet subleadingJet"),
		checkCharge = cms.bool(False),
		cut = cms.string("mass > 0")
		)

emuwRdiJetCandidateFilter = cms.EDFilter("CandViewCountFilter",
		src = cms.InputTag("emuwRdiJetCandidate"),
		minNumber = cms.uint32(1)
		)

### select electron \ingroup electronSkim_Group
emuwRelectron = cms.EDFilter("PATElectronRefSelector",
                                 src = cms.InputTag("slimmedElectrons"),
                                 cut = cms.string("pt>30"),
                                 )

### select muon
emuwRmuon = cms.EDFilter("PATElectronRefSelector",
                                 src = cms.InputTag("slimmedMuons"),
                                 cut = cms.string("pt>30"),
                                 )
#emuwRpreSelectedElectrons = cms.EDProducer("CandViewMerger",

### create di-lepton pair in signal region
emuwRdiLeptonCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
                                       decay = cms.string("emuwRelectron emuwRmuon"),
                                       role = cms.string("leading subleading"),
                                       checkCharge = cms.bool(False),
                                       # the cut on the pt of the daughter is to respect the order of leading and subleading:
                                           # if both electrons have pt>60 GeV there will be two di-lepton candidates with inversed order
                                       #cut = cms.string("mass > 200 && daughter(0).pt>daughter(1).pt"),
                                       cut = cms.string("mass > 0"),
									   )

### filter: at least one di-lepton candidate in signal region
emuwRdiLeptonCandidateFilter = cms.EDFilter("CandViewCountFilter",
                                           src = cms.InputTag("emuwRdiLeptonCandidate"),
                                           minNumber = cms.uint32(1)
                                           )

### create an object from two jets and two electrons in the evt, and cut on its mass
emuwRdiLeptonDijetCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
		decay = cms.string("emuwRdiJetCandidate emuwRdiLeptonCandidate"),
		role = cms.string("dijet dilepton"),
		checkCharge = cms.bool(False),
		cut = cms.string("mass > 500")
		)

### filter: require at least one LLJJ object in the evt
emuwRdiLeptonDijetCandidateFilter = cms.EDFilter("CandViewCountFilter",
		src = cms.InputTag("emuwRdiLeptonDijetCandidate"),
		minNumber = cms.uint32(1)
		)


### create di-lepton pair in sideband region
emuwRdiElectronSidebandCandidate = cms.EDProducer("CandViewShallowCloneCombiner",
                                       decay = cms.string("emuwRelectron emuwRmuon"),
                                       checkCharge = cms.bool(False),
                                       cut = cms.string("0< mass < 200 && daughter(0).pt>daughter(1).pt")
                                       )

### filter: at least one di-lepton candidate in sideband region
emuwRdiElectronSidebandCandidateFilter = cms.EDFilter("CandViewCountFilter",
                                                   src = cms.InputTag("emuwRdiElectronSidebandCandidate"),
                                                   minNumber = cms.uint32(1)
                                                   )

### di-jet selection sequence
emuwRjetSelectionSeq = cms.Sequence(emuwRhardJet + emuwRsoftJet)

### di-lepton selection sequence
emuwRleptonSelectionSeq = cms.Sequence(emuwRelectron + emuwRmuon)
### di-lepton selection in signal region sequence
emuwRdiLeptonSignalSeq = cms.Sequence(emuwRleptonSelectionSeq * emuwRdiLeptonCandidate * emuwRdiLeptonCandidateFilter)
### di-lepton and four object selection in signal region sequence
emuwRdiLeptonAndFourObjSignalSeq = cms.Sequence(
		emuwRjetSelectionSeq
		*emuwRdiLeptonSignalSeq
		*emuwRdiJetCandidate
		*emuwRdiJetCandidateFilter
		*emuwRdiLeptonDijetCandidate
		*emuwRdiLeptonDijetCandidateFilter
		)


### di-lepton selection in sideband region sequence
emuwRdiElectronSidebandSeq = cms.Sequence(emuwRleptonSelectionSeq * emuwRdiElectronSidebandCandidate * emuwRdiElectronSidebandCandidateFilter)

### low mass emujj selection sequence
emuwRdiLeptonAndLowMassSeq = cms.Sequence(
		emuwRjetSelectionSeq
		*emuwRdiLeptonSignalSeq
		*emuwRdiJetCandidate
		*emuwRdiJetCandidateFilter
		*emuwRdiLeptonDijetCandidate
		*~emuwRdiLeptonDijetCandidateFilter
		)

### @}
