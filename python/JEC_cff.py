import FWCore.ParameterSet.Config as cms


    
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetCorrFactorsUpdated
patJetCorrFactorsReapplyJEC = patJetCorrFactorsUpdated.clone(
    src = cms.InputTag("slimmedJets"),
    levels = cms.vstring('L1FastJet', 
                         'L2Relative', 
                         'L3Absolute'),
    payload = cms.string('AK4PFchs') ) # Make sure to choose the appropriate levels and payload here!


from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import patJetsUpdated
patJetsReapplyJEC = patJetsUpdated.clone(
    jetSource = cms.InputTag("slimmedJets"),
    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJEC"))
    )

jetUnc = cms.EDProducer('produceJECUnc',
		src = cms.InputTag("patJetCorrFactorsReapplyJEC"),
		)

#jecDataOnly = cms.Sequence(jec * es_prefer_jec)

jecSequence = cms.Sequence(patJetCorrFactorsReapplyJEC * patJetsReapplyJEC * jetUnc)
