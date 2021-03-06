import FWCore.ParameterSet.Config as cms

MiniTTree = cms.EDAnalyzer('miniTTree',
                            muons_src = cms.InputTag('wRminiTreeMuon'),
                            electrons_src = cms.InputTag('wRminiTreeElectron'),
                            jets_src = cms.InputTag('wRJets'),
                            genparticles_src = cms.InputTag('prunedGenParticles'),
                            jec_unc_src = cms.InputTag('wRJECUncert','JECUncertainty'),
                            jetResolution_src = cms.InputTag('jetResolutionSF','JetResolution'),
                            JERsf_src = cms.InputTag('jetResolutionSF','JERsf'),
                            JERsf_up_src = cms.InputTag('jetResolutionSF','JERsfUp'),
                            JERsf_down_src = cms.InputTag('jetResolutionSF','JERsfDown'),
                            genJetPt_src = cms.InputTag('jetResolutionSF','GenJetPt'),
                            genJetMatch_src = cms.InputTag('jetResolutionSF','GenJetMatch'),
                            ele_scale_error_src = cms.InputTag('eleScaleSmearing','EleScaleError'),
                            ele_smearing_sigma_src = cms.InputTag('eleScaleSmearing','EleSmearingSigma'),
                            ele_smearing_sigma_phi_up_src = cms.InputTag('eleScaleSmearing','EleSmearingSigmaPhiUp'),
                            ele_smearing_sigma_phi_down_src = cms.InputTag('eleScaleSmearing','EleSmearingSigmaPhiDown'),
                            ele_smearing_sigma_rho_up_src = cms.InputTag('eleScaleSmearing','EleSmearingSigmaRhoUp'),
                            ele_smearing_sigma_rho_down_src = cms.InputTag('eleScaleSmearing','EleSmearingSigmaRhoDown'),
                            trigObjs = cms.InputTag("selectedPatTrigger"),
                            trigResults = cms.InputTag("TriggerResults","","HLT"),
                            muon_IDSF_central_src = cms.InputTag('muonIdIsoSF','MuonSFIdCentral'),                            
                            muon_IsoSF_central_src = cms.InputTag('muonIdIsoSF', 'MuonSFIsoCentral'),
                            muon_TrigSF_central_src = cms.InputTag('muonTrigSF', 'MuonSFTrigCentral'),
                            muon_IDSF_error_src = cms.InputTag('muonIdIsoSF','MuonSFIdError'),
                            muon_IsoSF_error_src = cms.InputTag('muonIdIsoSF','MuonSFIsoError'),
                            muon_TrigSF_error_src = cms.InputTag('muonTrigSF','MuonSFTrigError'),
                            PUWeights_src = cms.InputTag('PUWeights','PileupWeights'),
                            datasetName = cms.InputTag("addStringIdentifier", "datasetIdentifier"),
                            filtersToPass = cms.vstring("hltDiEle33CaloIdLPixelMatchUnseededFilter","hltDiEle33CaloIdLMWPMS2UnseededFilter"),
                            pathsToPass = cms.vstring("HLT_DoubleEle33_CaloIdL_MW_v1",),
                            LHE_available = cms.bool(True),
                            )
