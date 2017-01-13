import FWCore.ParameterSet.Config as cms
import pickle
import math
import os

Additional_TR_Systematics = 0.01

f = open(os.path.join(os.environ['CMSSW_BASE'],'src/ExoAnalysis/cmsWR/data/Trigger_BCDEF.pkl'), 'r')
g = open(os.path.join(os.environ['CMSSW_BASE'],'src/ExoAnalysis/cmsWR/data/Trigger_GH.pkl'), 'r')
Trig_results = pickle.load(f)
Trig_results2 = pickle.load(g)

#Run2016B (5.929/fb)
#Run2016C (2.646/fb)
#Run2016D (4.353/fb)
#Run2016E (4.117/fb)
#Run2016F (3.186/fb)
#Run2016G (7.721/fb)
#Run2016H_v2 (8.636/fb)
#Run2016H_v3 (0.221/fb)

w1 = (5.929+2.646+4.353+4.117+3.186)/(5.929+2.646+4.353+4.117+3.186+7.721+8.636+0.221)
w2 = 1.0 - w1

ii = 0
SF_TR_C = []
SF_TR_E = []
SF_TR_C1 = []
SF_TR_E1 = []
SF_TR_C2 = []
SF_TR_E2 = []

for i in xrange(0,14):
    SF_TR_C.append(0)
    SF_TR_E.append(0)
    SF_TR_C1.append(0)
    SF_TR_E1.append(0)
    SF_TR_C2.append(0)
    SF_TR_E2.append(0)
    
ii=0
for key, result in sorted(Trig_results["Mu50_OR_TkMu50_EtaBins"]["histo_eta_DATA"].iteritems()) :
    SF_TR_C1[ii] = result["value"]
    SF_TR_E1[ii] = math.sqrt(pow(result["error"],2) + pow(Additional_TR_Systematics,2))
    ii += 1
ii=0
for key, result in sorted(Trig_results2["Mu50_OR_TkMu50_EtaBins"]["histo_eta_DATA"].iteritems()) :
    SF_TR_C2[ii] = result["value"]
    SF_TR_E2[ii] = math.sqrt(pow(result["error"],2) + pow(Additional_TR_Systematics,2))
    ii += 1
ii=0
for key, result in sorted(Trig_results2["Mu50_OR_TkMu50_EtaBins"]["histo_eta_DATA"].iteritems()) :
    SF_TR_C[ii] = SF_TR_C1[ii]*w1+SF_TR_C2[ii]*w2
    SF_TR_E[ii] = math.sqrt(pow(SF_TR_E1[ii]*w1,2) + pow(SF_TR_E2[ii]*w2,2))
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
