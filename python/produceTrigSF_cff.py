import FWCore.ParameterSet.Config as cms
import pickle
import math
import os

Additional_TR_Systematics = 0.01

f = open(os.path.join(os.environ['CMSSW_BASE'],'src/ExoAnalysis/cmsWR/data/Trigger_BCDEF.pkl'), 'r')
Trig_results = pickle.load(f)

ii = 0
SF_TR_C = []
SF_TR_E = []

for i in xrange(0,14):
    SF_TR_C.append(0)
    SF_TR_E.append(0)
    
ii=0
for key, result in sorted(Trig_results["Mu50_OR_TkMu50_EtaBins"]["histo_eta_DATA"].iteritems()) :
    SF_TR_C[ii] = result["value"]
    SF_TR_E[ii] = math.sqrt(pow(result["error"],2) + pow(Additional_TR_Systematics,2))
    ii += 1

    

# make a collection of TuneP muons which pass isHighPt ID
muonTrigSF = cms.EDProducer("produceLepTriggerScaleFactors",
		src = cms.InputTag("wRminiTreeMuon"),
                OutputCollectionName1 = cms.string("MuonSFTrigCentral"),
                OutputCollectionName2 = cms.string("MuonSFTrigError"),
                Scale_Factor_TR_Central = cms.vdouble(SF_TR_C),
                Scale_Factor_TR_Error = cms.vdouble(SF_TR_E)
)

#MuonIdIsoSFProdSequence = cms.Sequence(MuonIdIsoSFProd)
