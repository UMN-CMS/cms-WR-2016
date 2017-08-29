import FWCore.ParameterSet.Config as cms
import os,glob,sys

process = cms.Process('ANA')

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
                            #fileNames=cms.untracked.vstring('/store/mc/RunIISpring16MiniAODv2/TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/00000/001AFDCE-C33B-E611-B032-0025905D1C54.root',
                            #fileNames=cms.untracked.vstring('/store/mc/RunIISpring16MiniAODv2/DYJetsToLL_Pt-400To650_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14_ext1-v1/50000/20BE2D8A-E758-E611-99DE-002590DC03AC.root',
                            fileNames=cms.untracked.vstring(
                              # "file:pickevents_highMlljj.root",
                              # 'file:pickevents2.root'
                              "file:pickevents_2HighMassMuons.root",
                              # "file:pickevents_5HighMassMuons.root"
                            )
)

outfile = 'highMlljj.root'

process.TFileService = cms.Service('TFileService', fileName = cms.string(outfile))


# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.GlobalTag.globaltag = '80X_dataRun2_2016SeptRepro_v7'

jetID=" (neutralHadronEnergyFraction<0.90 && neutralEmEnergyFraction<0.9 && (chargedMultiplicity+neutralMultiplicity)>1 && muonEnergyFraction<0.8) && ((abs(eta)<=2.4 && chargedHadronEnergyFraction>0 && chargedMultiplicity>0 && chargedEmEnergyFraction<0.90) || abs(eta)>2.4)"


from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection

updateJetCollection(
    process,
    jetSource = cms.InputTag('slimmedJets'),
    labelName = 'UpdatedJEC',
    jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None')  # Do not forget 'L2L3Residual' on data!
    )

updateJetCollection.jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L2L3Residual', 'L3Absolute']), 'None')

process.tightJets = cms.EDFilter("PATJetSelector",
                           src = cms.InputTag("updatedPatJetsUpdatedJEC"),
                           cut = cms.string(jetID),
)

muonIDIso=' isolationR03().sumPt/pt < 0.1 && userInt("highPtID") == 1'

process.tunePMuons = cms.EDProducer("TunePMuonProducer",
		                    #src = cms.InputTag("removeBadAndCloneGlobalMuons")
		                    src = cms.InputTag("slimmedMuons")
		)

process.tunePIDIsoMuons = cms.EDFilter("PATMuonSelector",
                               src = cms.InputTag("tunePMuons"),
                               cut = cms.string(muonIDIso),
                               )


process.highMlljj = cms.EDAnalyzer('highMlljj',
                                   muons_src = cms.InputTag('tunePIDIsoMuons'),
                                   jets_src = cms.InputTag('tightJets'),
                                   electrons_src = cms.InputTag('slimmedElectrons'),
                                   photons_src = cms.InputTag('slimmedPhotons'),
                                   met_src = cms.InputTag('slimmedMETs'),
                                   vertex_src = cms.InputTag('offlineSlimmedPrimaryVertices'),
                                   tracks_src = cms.InputTag('packedPFCandidates')
)


process.p = cms.Path(process.patJetCorrFactorsUpdatedJEC * process.updatedPatJetsUpdatedJEC * process.tightJets * process.tunePMuons * process.tunePIDIsoMuons * process.highMlljj)
