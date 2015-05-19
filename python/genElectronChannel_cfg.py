import FWCore.ParameterSet.Config as cms

process = cms.Process("GENEEJJ")

## load the filters, producers, and sequences defined in the config file fragment
process.load('ExoAnalysis.cmsWR.genElectronChannelModules_cff')
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

## run these analyzers after selecting gen electrons, jets, and quarks without any cuts applied
process.genAnalyzerOne = cms.EDAnalyzer('genAnalyzer',
		treeName = cms.string("genObjectsNoCuts"),
		genLeptonCollection = cms.InputTag("bareGenEle","","GENEEJJ"),
		genJetCollection = cms.InputTag("bareGenJet","","GENEEJJ")
		)

process.matchedGenAnalyzerOne = cms.EDAnalyzer('matchedAnalyzer',
		treeName = cms.string("matchedGenObjectsNoCuts"),
		doDeltaRcut = cms.bool(False),
		doFourObjMassCut = cms.bool(False),
		minFourObjMass = cms.double(-1),
		minDeltaRforLeptonJetExclusion = cms.double(-1),
		saveGenMatched = cms.bool(False),
		leadingLeptonCollection = cms.InputTag("bareMatchedLeadingGenEle","","GENEEJJ"),
		subleadingLeptonCollection = cms.InputTag("bareMatchedSubleadingGenEle","","GENEEJJ"),
		quarkCollection = cms.InputTag("bareMatchedGenQuark","","GENEEJJ"),
		matchingLeadingLeptonCollection = cms.InputTag("","",""),
		matchingSubleadingLeptonCollection = cms.InputTag("","",""),
		matchingQuarkCollection = cms.InputTag("","","")
		)


## run these analyzers after applying pt and eta cuts on the leptons and jets
process.genAnalyzerTwo = cms.EDAnalyzer('genAnalyzer',
		treeName = cms.string("genObjectsWithPtEtaCuts"),
		genLeptonCollection = cms.InputTag("ptEtaRestrictedSubleadingGenEle","","GENEEJJ"),
		genJetCollection = cms.InputTag("ptEtaRestrictedGenJet","","GENEEJJ")
		)

process.matchedGenAnalyzerTwo = cms.EDAnalyzer('matchedAnalyzer',
		treeName = cms.string("matchedGenObjectsWithPtEtaCuts"),
		doDeltaRcut = cms.bool(False),
		doFourObjMassCut = cms.bool(False),
		minFourObjMass = cms.double(-1),
		minDeltaRforLeptonJetExclusion = cms.double(-1),
		saveGenMatched = cms.bool(False),
		leadingLeptonCollection = cms.InputTag("ptEtaRestrictedMatchedLeadingGenEle","","GENEEJJ"),
		subleadingLeptonCollection = cms.InputTag("ptEtaRestrictedMatchedSubleadingGenEle","","GENEEJJ"),
		quarkCollection = cms.InputTag("ptEtaRestrictedMatchedGenQuark","","GENEEJJ"),
		matchingLeadingLeptonCollection = cms.InputTag("","",""),
		matchingSubleadingLeptonCollection = cms.InputTag("","",""),
		matchingQuarkCollection = cms.InputTag("","","")
		)


## run these analyzers after applying pt, eta, and dilepton mass cuts
process.genAnalyzerThree = cms.EDAnalyzer('genAnalyzer',
		treeName = cms.string("genObjectsWithPtEtaAndDileptonMassCuts"),
		genLeptonCollection = cms.InputTag("ptEtaRestrictedSubleadingGenEle","","GENEEJJ"),
		genJetCollection = cms.InputTag("ptEtaRestrictedGenJet","","GENEEJJ")
		)

process.matchedGenAnalyzerThree = cms.EDAnalyzer('matchedAnalyzer',
		treeName = cms.string("matchedGenObjectsWithPtEtaAndDileptonMassCuts"),
		doDeltaRcut = cms.bool(False),
		doFourObjMassCut = cms.bool(False),
		minFourObjMass = cms.double(-1),
		minDeltaRforLeptonJetExclusion = cms.double(-1),
		saveGenMatched = cms.bool(False),
		leadingLeptonCollection = cms.InputTag("ptEtaRestrictedMatchedLeadingGenEle","","GENEEJJ"),
		subleadingLeptonCollection = cms.InputTag("ptEtaRestrictedMatchedSubleadingGenEle","","GENEEJJ"),
		quarkCollection = cms.InputTag("ptEtaRestrictedMatchedGenQuark","","GENEEJJ"),
		matchingLeadingLeptonCollection = cms.InputTag("","",""),
		matchingSubleadingLeptonCollection = cms.InputTag("","",""),
		matchingQuarkCollection = cms.InputTag("","","")
		)

## analyzers to run with dR(lepton,hadron) > someValue cut applied
process.matchedGenAnalyzerFour = cms.EDAnalyzer('matchedAnalyzer',
		treeName = cms.string("matchedGenObjectsWithPtEtaDileptonMassAndDrCuts"),
		doDeltaRcut = cms.bool(True),
		doFourObjMassCut = cms.bool(False),
		minFourObjMass = cms.double(-1),
		minDeltaRforLeptonJetExclusion = cms.double(0.4),
		saveGenMatched = cms.bool(False),
		leadingLeptonCollection = cms.InputTag("ptEtaRestrictedMatchedLeadingGenEle","","GENEEJJ"),
		subleadingLeptonCollection = cms.InputTag("ptEtaRestrictedMatchedSubleadingGenEle","","GENEEJJ"),
		quarkCollection = cms.InputTag("ptEtaRestrictedMatchedGenQuark","","GENEEJJ"),
		matchingLeadingLeptonCollection = cms.InputTag("","",""),
		matchingSubleadingLeptonCollection = cms.InputTag("","",""),
		matchingQuarkCollection = cms.InputTag("","","")
		)

## analyzers to run with dR(lepton,hadron) > someValue cut and four obj inv mass cut applied
process.matchedGenAnalyzerFive = cms.EDAnalyzer('matchedAnalyzer',
		treeName = cms.string("matchedGenObjectsWithPtEtaDileptonMassDrAndFourObjMassCuts"),
		doDeltaRcut = cms.bool(True),
		doFourObjMassCut = cms.bool(True),
		minFourObjMass = cms.double(600),
		minDeltaRforLeptonJetExclusion = cms.double(0.4),
		saveGenMatched = cms.bool(False),
		leadingLeptonCollection = cms.InputTag("ptEtaRestrictedMatchedLeadingGenEle","","GENEEJJ"),
		subleadingLeptonCollection = cms.InputTag("ptEtaRestrictedMatchedSubleadingGenEle","","GENEEJJ"),
		quarkCollection = cms.InputTag("ptEtaRestrictedMatchedGenQuark","","GENEEJJ"),
		matchingLeadingLeptonCollection = cms.InputTag("","",""),
		matchingSubleadingLeptonCollection = cms.InputTag("","",""),
		matchingQuarkCollection = cms.InputTag("","","")
		)



process.printMuOrTauDecayChain = cms.Path(
		#process.hasGenNuMuOrTau
		#*process.hasGenNuMuOrTauFilter
		process.printParticleTree
		)

#process.runUnmatchedGenAnalysis = cms.Path(
#		process.bareGenParticleSeq
#		*process.genAnalyzerOne
#		*process.ptEtaConstrainedSeq
#		*process.genAnalyzerTwo
#		*process.genDiElectronCandidateSeq
#		*process.genAnalyzerThree
#		)

process.runMatchedGenAnalysis = cms.Path(
		process.bareMatchedGenParticleSeq
		*process.matchedGenAnalyzerOne
		*process.matchedPtEtaConstrainedSeq
		*process.matchedGenAnalyzerTwo
		*process.genMatchedDiElectronCandidateSeq
		*process.matchedGenAnalyzerThree
		*process.matchedGenAnalyzerFour
		*process.matchedGenAnalyzerFive
		)

process.schedule = cms.Schedule(process.runMatchedGenAnalysis)

process.TFileService = cms.Service("TFileService",
		fileName = cms.string('analysis_genElectronChannel_using_miniAOD.root')
		#fileName = cms.string('inspect_WR_to_LLJJ_GENSIM.root')

)

process.options = cms.untracked.PSet(
		wantSummary = cms.untracked.bool(True)
		)

process.source = cms.Source( "PoolSource",
    fileNames = cms.untracked.vstring(
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_1.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_23.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_17.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_24.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_16.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_13.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_22.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_12.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_20.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_26.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_10.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_11.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_18.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_25.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_14.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_21.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_4.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_19.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_15.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_9.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_6.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_3.root',
		#'file:/eos/uscms/store/user/skalafut/WR/13TeV/miniAOD_WR_signal/WR_signal_miniAODFile_2.root',

		##WR->LNu->LLJJ GEN-SIM files
		'file:/uscms/home/skalafut/WR/CMSSW_7_4_0_pre9/src/ExoAnalysis/cmsWR/WR_GEN_SIM.root'

    ),
    inputCommands = cms.untracked.vstring(
        'keep *'
    )
)

# limit the number of events to be processed
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)



