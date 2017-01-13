import FWCore.ParameterSet.Config as cms
import pickle
import math
import os

Additional_ID_Systematics = 0.01
Additional_ISO_Systematics = 0.01

f1 = open(os.path.join(os.environ['CMSSW_BASE'],'src/ExoAnalysis/cmsWR/data/EfficienciesAndSF_BCDEF.pkl'), 'r')
g1 = open(os.path.join(os.environ['CMSSW_BASE'],'src/ExoAnalysis/cmsWR/data/Isolation_BCDEF.pkl'), 'r')
f2 = open(os.path.join(os.environ['CMSSW_BASE'],'src/ExoAnalysis/cmsWR/data/EfficienciesAndSF_GH.pkl'), 'r')
g2 = open(os.path.join(os.environ['CMSSW_BASE'],'src/ExoAnalysis/cmsWR/data/Isolation_GH.pkl'), 'r')
ID_results = pickle.load(f1)
Iso_results = pickle.load(g1)
ID_results2 = pickle.load(f2)
Iso_results2 = pickle.load(g2)

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
SF_ID_C = []
SF_ID_E = []
SF_ISO_C = []
SF_ISO_E = []
SF_ID_C1 = []
SF_ID_E1 = []
SF_ISO_C1 = []
SF_ISO_E1 = []
SF_ID_C2 = []
SF_ID_E2 = []
SF_ISO_C2 = []
SF_ISO_E2 = []

for i in xrange(0,13):
    SF_ID_C.append(0)
    SF_ID_E.append(0)
    SF_ISO_C.append(0)
    SF_ISO_E.append(0)
    SF_ID_C1.append(0)
    SF_ID_E1.append(0)
    SF_ISO_C1.append(0)
    SF_ISO_E1.append(0)
    SF_ID_C2.append(0)
    SF_ID_E2.append(0)
    SF_ISO_C2.append(0)
    SF_ISO_E2.append(0)

for key, result in sorted(ID_results["MC_NUM_HighPtID_DEN_genTracks_PAR_eta"]["eta_ratio"].iteritems()) : 
    SF_ID_C1[ii] = result["value"]
    SF_ID_E1[ii] = math.sqrt(pow(result["error"],2) + pow(Additional_ID_Systematics,2))
    ii += 1

ii=0
for key, result in sorted(Iso_results["tkLooseISO_highptID_eta"]["eta_ratio"].iteritems()) :
    SF_ISO_C1[ii] = result["value"]
    SF_ISO_E1[ii] = math.sqrt(pow(result["error"],2) + pow(Additional_ISO_Systematics,2))
    ii += 1

ii=0
for key, result in sorted(ID_results2["MC_NUM_HighPtID_DEN_genTracks_PAR_eta"]["eta_ratio"].iteritems()) : 
    SF_ID_C2[ii] = result["value"]
    SF_ID_E2[ii] = math.sqrt(pow(result["error"],2) + pow(Additional_ID_Systematics,2))
    ii += 1
ii=0
for key, result in sorted(Iso_results2["tkLooseISO_highptID_eta"]["eta_ratio"].iteritems()) :
    SF_ISO_C2[ii] = result["value"]
    SF_ISO_E2[ii] = math.sqrt(pow(result["error"],2) + pow(Additional_ISO_Systematics,2))
    ii += 1

ii=0
for key, result in sorted(ID_results2["MC_NUM_HighPtID_DEN_genTracks_PAR_eta"]["eta_ratio"].iteritems()) : 
    SF_ID_C[ii] = SF_ID_C1[ii]*w1 + SF_ID_C2[ii]*w2
    SF_ID_E[ii] = math.sqrt(pow(SF_ID_E1[ii]*w1,2) + pow(SF_ID_E2[ii]*w2,2))
    ii += 1
ii=0
for key, result in sorted(Iso_results2["tkLooseISO_highptID_eta"]["eta_ratio"].iteritems()) :
    SF_ISO_C[ii] = SF_ISO_C1[ii]*w1 + SF_ISO_C2[ii]*w2
    SF_ISO_E[ii] = math.sqrt(pow(SF_ISO_E1[ii]*w1,2) + pow(SF_ISO_E2[ii]*w2,2))
    ii += 1

    

# make a collection of TuneP muons which pass isHighPt ID
muonIdIsoSF = cms.EDProducer("produceLepIdIsoScaleFactors",
		src = cms.InputTag("wRminiTreeMuon"),
                OutputCollectionName1 = cms.string("MuonSFIdCentral"),
                OutputCollectionName2 = cms.string("MuonSFIdError"),
                OutputCollectionName3 = cms.string("MuonSFIsoCentral"),
                OutputCollectionName4 = cms.string("MuonSFIsoError"),
                Scale_Factor_ID_Central = cms.vdouble(SF_ID_C),
                Scale_Factor_ID_Error = cms.vdouble(SF_ID_E),
                Scale_Factor_ISO_Central = cms.vdouble(SF_ISO_C),
                Scale_Factor_ISO_Error = cms.vdouble(SF_ISO_E)
)

#MuonIdIsoSFProdSequence = cms.Sequence(MuonIdIsoSFProd)
