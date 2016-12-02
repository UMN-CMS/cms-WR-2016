import FWCore.ParameterSet.Config as cms

cutflowresult = cms.EDAnalyzer('cutResultTree',
      electrons= cms.InputTag("slimmedElectrons"),
      eleHEEPIdMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV70")
      )

