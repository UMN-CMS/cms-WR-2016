#include "TTree.h"
#include "TLorentzVector.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "../interface/miniTreeEvent.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/Common/interface/View.h"

#include <vector>
#include <string>
#include <iostream>

//little helper function to get handles easier
//note the use of annoymous namespace to avoid linking conflicts
namespace{
  template<typename T>
    edm::Handle<T> getHandle(const edm::Event& event,const edm::EDGetTokenT<T>& token)
  {
    edm::Handle<T> handle;
    event.getByToken(token,handle);
    return handle;
  }
}


typedef double JECUnc_t;
typedef edm::ValueMap<JECUnc_t> JECUnc_Map;

//the functions which actually match the trigger objects and see if it passes
namespace{
  std::vector<const pat::TriggerObjectStandAlone*> getMatchedObjs(const float eta,const float phi,const std::vector<pat::TriggerObjectStandAlone>& trigObjs,const float maxDeltaR=0.1)
  {
    std::vector<const pat::TriggerObjectStandAlone*> matchedObjs;
    const float maxDR2 = maxDeltaR*maxDeltaR;
    for(auto& trigObj : trigObjs){
      const float dR2 = reco::deltaR2(eta,phi,trigObj.eta(),trigObj.phi());
      if(dR2<maxDR2) matchedObjs.push_back(&trigObj);
    }
    return matchedObjs;
  }

  bool checkFilters(const float eta,const float phi,const std::vector<pat::TriggerObjectStandAlone>& trigObjs,const std::vector<std::string>& filterNames,const float maxDeltaR=0.1)
  {
    bool passAnyFilter=false;
    const auto matchedObjs = getMatchedObjs(eta,phi,trigObjs,maxDeltaR);
    for(auto& filterName : filterNames){
      for(const auto trigObj : matchedObjs){
        //normally would auto this but to make it clearer for the example
        const std::vector<std::string>& objFilters = trigObj->filterLabels();
        //I dont think filterLabels are sorted so use std::find to see if filterName is in 
        //the list of passed filters for this object
        if(std::find(objFilters.begin(),objFilters.end(),filterName)!=objFilters.end()){
          //std::cout <<" object "<<eta<<" "<<phi<<" passes "<<filterName<<std::endl;
          passAnyFilter=true;
        }
      }//end loop over matched trigger objects
    }//end loop over filter lables
    return passAnyFilter;
  }
}


class miniTTree : public edm::EDAnalyzer
{
public:
  explicit miniTTree(const edm::ParameterSet&);

private:
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  edm::InputTag trigObjsTag_;
  edm::InputTag trigResultsTag_;
  std::vector<std::string> filtersToPass_;
  std::vector<std::string> pathsToPass_;

  edm::EDGetToken electronsMiniAODToken_;
  edm::EDGetToken muonsMiniAODToken_;
  edm::EDGetToken jetsMiniAODToken_;
  edm::EDGetToken genparticlesMiniAODToken_;
  edm::EDGetToken pileUpInfoToken_;
  edm::EDGetToken pileUpReweightToken_;
  edm::EDGetToken primaryVertexToken_;
  edm::EDGetToken evinfoToken_;
  edm::EDGetToken lheEventToken_;
  
  edm::EDGetToken  jec_unc_src;
  edm::EDGetToken  jetResolution_src;
  edm::EDGetToken  JERsf_src;
  edm::EDGetToken  JERsf_up_src;
  edm::EDGetToken  JERsf_down_src;
  edm::EDGetToken  genJetPt_src;
  edm::EDGetToken  genJetMatch_src;
  
  edm::EDGetToken  ele_scale_error_src;
  edm::EDGetToken  ele_smearing_sigma_src;
  edm::EDGetToken  ele_smearing_sigma_phi_up_src;
  edm::EDGetToken  ele_smearing_sigma_phi_down_src;
  edm::EDGetToken  ele_smearing_sigma_rho_up_src;
  edm::EDGetToken  ele_smearing_sigma_rho_down_src;
  
  edm::EDGetToken  muon_IDSF_central_src;
  edm::EDGetToken  muon_IsoSF_central_src;
  edm::EDGetToken  muon_TrigSF_central_src;
  edm::EDGetToken  muon_IDSF_error_src;
  edm::EDGetToken  muon_IsoSF_error_src;
  edm::EDGetToken  muon_TrigSF_error_src;
  edm::EDGetTokenT<edm::TriggerResults> trigResultsToken_;
  edm::EDGetTokenT<std::vector<pat::TriggerObjectStandAlone> > trigObjsToken_;

  edm::EDGetToken datasetNameToken_;

 
  //trigger results stores whether a given path passed or failed
  //a path is series of filters
  //triggers are the objects the trigger is run on, with a list of filters they pass
  //match to these to see if a given electron/photon/whatever passed a given filter

  bool LHE_available;
  
  TTree* tree;
  miniTreeEvent myEvent;

};

miniTTree::miniTTree(const edm::ParameterSet& cfg):

  trigObjsTag_(cfg.getParameter<edm::InputTag>("trigObjs")),
  trigResultsTag_(cfg.getParameter<edm::InputTag>("trigResults")),
  filtersToPass_(cfg.getParameter<std::vector<std::string> >("filtersToPass")), 
  pathsToPass_(cfg.getParameter<std::vector<std::string> >("pathsToPass")),

  electronsMiniAODToken_   ( consumes<edm::View<pat::Electron> >(cfg.getParameter<edm::InputTag>("electrons_src"))),
  muonsMiniAODToken_ ( consumes<edm::View<pat::Muon> >(cfg.getParameter<edm::InputTag>("muons_src"))),
  jetsMiniAODToken_ ( consumes<edm::View<pat::Jet> >(cfg.getParameter<edm::InputTag>("jets_src"))),
  genparticlesMiniAODToken_ ( consumes<edm::View<reco::GenParticle> >(cfg.getParameter<edm::InputTag>("genparticles_src"))),
  pileUpInfoToken_ ( consumes<edm::View<PileupSummaryInfo> >(edm::InputTag("slimmedAddPileupInfo"))),
  pileUpReweightToken_ ( consumes<float >(cfg.getParameter<edm::InputTag>("PUWeights_src"))),
  primaryVertexToken_ ( consumes<edm::View<reco::Vertex> >(edm::InputTag("offlineSlimmedPrimaryVertices"))),
  evinfoToken_ ( consumes<GenEventInfoProduct>(edm::InputTag("generator"))),
  lheEventToken_ ( consumes<LHEEventProduct>(edm::InputTag("externalLHEProducer"))),
  jec_unc_src ( consumes<JECUnc_Map >(cfg.getParameter<edm::InputTag>("jec_unc_src"))),
  jetResolution_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("jetResolution_src"))),
  JERsf_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("JERsf_src"))),
  JERsf_up_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("JERsf_up_src"))),
  JERsf_down_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("JERsf_down_src"))),
  genJetPt_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("genJetPt_src"))),
  genJetMatch_src ( consumes<edm::ValueMap<bool> >(cfg.getParameter<edm::InputTag>("genJetMatch_src"))),
  ele_scale_error_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("ele_scale_error_src"))),
  ele_smearing_sigma_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("ele_smearing_sigma_src"))),
  ele_smearing_sigma_phi_up_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("ele_smearing_sigma_phi_up_src"))),
  ele_smearing_sigma_phi_down_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("ele_smearing_sigma_phi_down_src"))),
  ele_smearing_sigma_rho_up_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("ele_smearing_sigma_rho_up_src"))),
  ele_smearing_sigma_rho_down_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("ele_smearing_sigma_rho_down_src"))),
  muon_IDSF_central_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("muon_IDSF_central_src"))),
  muon_IsoSF_central_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("muon_IsoSF_central_src"))),
  muon_TrigSF_central_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("muon_TrigSF_central_src"))),
  muon_IDSF_error_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("muon_IDSF_error_src"))),
  muon_IsoSF_error_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("muon_IsoSF_error_src"))),
  muon_TrigSF_error_src ( consumes<edm::ValueMap<float> >(cfg.getParameter<edm::InputTag>("muon_TrigSF_error_src"))),
  trigResultsToken_(consumes<edm::TriggerResults>(trigResultsTag_)),
  trigObjsToken_(consumes<std::vector<pat::TriggerObjectStandAlone> >(trigObjsTag_)),	
  datasetNameToken_ ( consumes<std::string>(cfg.getParameter<edm::InputTag>("datasetName"))),
  LHE_available ( cfg.getParameter<bool>("LHE_available"))
{
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("t", "");

  myEvent.SetBranches(tree);

}

void miniTTree::analyze(const edm::Event& event, const edm::EventSetup&)
{
  myEvent.clear();
  myEvent.run = event.id().run();
  myEvent.lumi = event.luminosityBlock();
  myEvent.event = event.id().event();

  edm::Handle<edm::View<pat::Electron> > electrons;
  event.getByToken(electronsMiniAODToken_, electrons);
  edm::Handle<edm::View<pat::Muon> > muons;
  event.getByToken(muonsMiniAODToken_, muons);
  edm::Handle<edm::View<pat::Jet> > jets;
  event.getByToken(jetsMiniAODToken_, jets);
  edm::Handle<edm::View<reco::GenParticle> > genparticles;

  edm::Handle<JECUnc_Map > jec_unc;
  event.getByToken(jec_unc_src, jec_unc);
  edm::Handle< edm::ValueMap<float> > jetResolution;
  event.getByToken(jetResolution_src, jetResolution);
  edm::Handle< edm::ValueMap<float> > JERsf;
  event.getByToken(JERsf_src, JERsf);
  edm::Handle< edm::ValueMap<float> > JERsf_up;
  event.getByToken(JERsf_up_src, JERsf_up);
  edm::Handle< edm::ValueMap<float> > JERsf_down;
  event.getByToken(JERsf_down_src, JERsf_down);
  edm::Handle< edm::ValueMap<float> > genjetPt;
  event.getByToken(genJetPt_src, genjetPt);
  edm::Handle< edm::ValueMap<bool> > genjetMatch;
  event.getByToken(genJetMatch_src, genjetMatch);

  edm::Handle< edm::ValueMap<float> > ele_scale_error;
  event.getByToken(ele_scale_error_src, ele_scale_error);
  edm::Handle< edm::ValueMap<float> > ele_smearing_sigma;
  event.getByToken(ele_smearing_sigma_src, ele_smearing_sigma);
  edm::Handle< edm::ValueMap<float> > ele_smearing_sigma_phi_up;
  event.getByToken(ele_smearing_sigma_phi_up_src, ele_smearing_sigma_phi_up);
  edm::Handle< edm::ValueMap<float> > ele_smearing_sigma_phi_down;
  event.getByToken(ele_smearing_sigma_phi_up_src, ele_smearing_sigma_phi_down);
  edm::Handle< edm::ValueMap<float> > ele_smearing_sigma_rho_up;
  event.getByToken(ele_smearing_sigma_rho_up_src, ele_smearing_sigma_rho_up);
  edm::Handle< edm::ValueMap<float> > ele_smearing_sigma_rho_down;
  event.getByToken(ele_smearing_sigma_rho_up_src, ele_smearing_sigma_rho_down);


  edm::Handle< edm::ValueMap<float> > muon_IDSF;
  event.getByToken(muon_IDSF_central_src, muon_IDSF);
  edm::Handle< edm::ValueMap<float> > muon_IsoSF;
  event.getByToken(muon_IsoSF_central_src, muon_IsoSF);
  edm::Handle< edm::ValueMap<float> > muon_TrigSF;
  event.getByToken(muon_TrigSF_central_src, muon_TrigSF);
  edm::Handle< edm::ValueMap<float> > muon_IDSF_error;
  event.getByToken(muon_IDSF_error_src, muon_IDSF_error);
  edm::Handle< edm::ValueMap<float> > muon_IsoSF_error;
  event.getByToken(muon_IsoSF_error_src, muon_IsoSF_error);
  edm::Handle< edm::ValueMap<float> > muon_TrigSF_error;
  event.getByToken(muon_TrigSF_error_src, muon_TrigSF_error);

  auto trigResultsHandle = getHandle(event,trigResultsToken_) ;
  auto trigObjsHandle = getHandle(event,trigObjsToken_);

  edm::Handle<GenEventInfoProduct> evinfo;		
  edm::Handle<edm::View<PileupSummaryInfo> > PU_Info;		
  edm::Handle<float > PU_Weights;
  edm::Handle<LHEEventProduct> EvtHandle ;  

  edm::Handle<edm::View<reco::Vertex> > primary_vertex;
  event.getByToken(primaryVertexToken_, primary_vertex);

  edm::Handle<std::string> datasetName;
  event.getByToken(datasetNameToken_, datasetName);
  
  sprintf(myEvent.datasetName, "%s", datasetName->c_str());

  if(primary_vertex->size() > 0) {
    for(auto pv : *primary_vertex)
      myEvent.nPV++;
  }

  if(!event.isRealData()) {		
    event.getByToken(evinfoToken_, evinfo);
    myEvent.weight = evinfo->weight();		
    event.getByToken(pileUpInfoToken_, PU_Info);		
    for(auto p : *PU_Info) {		
      int BX = p.getBunchCrossing();		
      if(BX == 0)		
	myEvent.nPU = p.getTrueNumInteractions();		
    }  

    if(LHE_available){
      event.getByToken( lheEventToken_ , EvtHandle ) ;
      for (unsigned int i=0; i<EvtHandle->weights().size(); i++) {
	myEvent.RF_weights->push_back(fabs(EvtHandle->weights()[i].wgt/EvtHandle->originalXWGTUP()));
      }
    }
  }

  //const edm::TriggerNames& trigNames = event.triggerNames(*trigResultsHandle);
  //std::cout <<"checking paths "<<std::endl;
  //for(auto& pathName : pathsToPass_){
  //we need to figure out which path index the pathName corresponds too
  //size_t pathIndx = trigNames.triggerIndex(pathName);
  //if(pathIndx>=trigNames.size()) std::cout <<" path "<<pathName<<" not found in menu"<<std::endl;
  //else{
  //if(trigResultsHandle->accept(pathIndx)) std::cout <<" path "<<pathName<<" passed"<<std::endl;
  //else std::cout <<" path "<<pathName<<" failed"<<std::endl;
  //}
  //}
      
  for (size_t i = 0; i < electrons->size(); ++i) {
    const auto ele = electrons->ptrAt(i);
    TLorentzVector p4;
    p4.SetPtEtaPhiM(ele->pt(), ele->eta(), ele->phi(), ele->mass());
    myEvent.electrons_p4->push_back(p4);
    myEvent.electron_scale_error->push_back((*ele_scale_error)[ele]);
    myEvent.electron_smearing_sigma->push_back((*ele_smearing_sigma)[ele]);
    myEvent.electron_smearing_sigma_phi_up->push_back((*ele_smearing_sigma_phi_up)[ele]);
    myEvent.electron_smearing_sigma_phi_down->push_back((*ele_smearing_sigma_phi_down)[ele]);
    myEvent.electron_smearing_sigma_rho_up->push_back((*ele_smearing_sigma_rho_up)[ele]);
    myEvent.electron_smearing_sigma_rho_down->push_back((*ele_smearing_sigma_rho_down)[ele]);
    myEvent.electron_charge->push_back(ele->charge());
    myEvent.electron_r9->push_back(ele->full5x5_r9());
    myEvent.electron_SC_eta->push_back(ele->superCluster()->eta());
    myEvent.electron_HltDiEle33CaloIdLMWPMS2UnseededFilter->push_back(checkFilters(ele->superCluster()->eta(),ele->superCluster()->phi(),*trigObjsHandle,filtersToPass_));
    myEvent.electron_HoverEM->push_back(ele->hadronicOverEm());
    myEvent.electron_dxy->push_back(ele->gsfTrack()->dxy());
    myEvent.electron_dB->push_back(ele->dB());
    myEvent.electron_dxyError->push_back(ele->gsfTrack()->dxyError());
    myEvent.electron_pfIso->push_back((ele->chargedHadronIso() + TMath::Max(float(0.0),ele->neutralHadronIso()) + ele->photonIso() - 0.5*ele->puChargedHadronIso())/ele->et());
    myEvent.electron_GsfEleEmHadD1IsoRhoCut->push_back(ele->dr03EcalRecHitSumEt() + ele->dr03HcalDepth1TowerSumEt());
    myEvent.electron_GsftrkIso->push_back(ele->dr03TkSumPt());
    myEvent.electron_heepId->push_back(ele->userInt("heepID"));
    myEvent.electron_trkIso->push_back(ele->userFloat("tkrIso"));
    myEvent.electron_satCrystal->push_back(ele->userInt("satCrystals"));
    myEvent.electron_heepBitmap->push_back(ele->userInt("heepBitmap"));  
  }
  
  for (size_t i = 0; i < muons->size(); ++i) {
    const auto mu = muons->ptrAt(i);
    TLorentzVector p4;
    p4.SetPtEtaPhiM(mu->pt(), mu->eta(), mu->phi(), mu->mass());
    myEvent.muons_p4->push_back(p4);
    myEvent.muon_charge->push_back(mu->charge());
    myEvent.muon_IDSF_central->push_back((*muon_IDSF)[mu]);
    myEvent.muon_IsoSF_central->push_back((*muon_IsoSF)[mu]);
    myEvent.muon_TrigSF_central->push_back((*muon_TrigSF)[mu]);
    myEvent.muon_IDSF_error->push_back((*muon_IDSF_error)[mu]);
    myEvent.muon_IsoSF_error->push_back((*muon_IsoSF_error)[mu]);
    myEvent.muon_TrigSF_error->push_back((*muon_TrigSF_error)[mu]);
    myEvent.muon_trackerLayersWithMeasurement->push_back(mu->bestTrack()->hitPattern().trackerLayersWithMeasurement());
    myEvent.muon_trkIso->push_back(mu->isolationR03().sumPt/mu->pt());
    myEvent.muon_ptRel->push_back(mu->muonBestTrack()->ptError()/mu->muonBestTrack()->pt());
    myEvent.muon_muonChambers->push_back(mu->globalTrack()->hitPattern().numberOfValidMuonHits());
    myEvent.muon_matchedStations->push_back(mu->numberOfMatchedStations());
    myEvent.muon_dxy->push_back(fabs(mu->muonBestTrack()->dxy(primary_vertex->at(0).position())));
    myEvent.muon_dxyError->push_back(fabs(mu->muonBestTrack()->dxyError()));
    myEvent.muon_pixelHits->push_back(mu->innerTrack()->hitPattern().numberOfValidPixelHits());
    myEvent.muon_stripHits->push_back(mu->innerTrack()->hitPattern().trackerLayersWithMeasurement());
  }

  for (size_t i = 0; i < jets->size(); ++i) {
    const auto jet = jets->ptrAt(i);
    TLorentzVector p4;
    p4.SetPtEtaPhiM(jet->pt(), jet->eta(), jet->phi(), jet->mass());
    myEvent.jets_p4->push_back(p4);
    myEvent.jec_uncertainty->push_back((*jec_unc)[jet]);
    myEvent.jetResolution->push_back((*jetResolution)[jet]);
    myEvent.JER_sf->push_back((*JERsf)[jet]);
    myEvent.JER_sf_up->push_back((*JERsf_up)[jet]);
    myEvent.JER_sf_down->push_back((*JERsf_down)[jet]);
    myEvent.genJetPt->push_back((*genjetPt)[jet]);
    myEvent.genJetMatch->push_back((*genjetMatch)[jet]);
  }

  if(!event.isRealData()) {
    event.getByToken(genparticlesMiniAODToken_, genparticles);
    for (size_t i = 0; i < genparticles->size(); ++i) {
      const auto genp = genparticles->ptrAt(i);
      TLorentzVector p4;
      p4.SetPtEtaPhiM(genp->pt(), genp->eta(), genp->phi(), genp->mass());
      if(genp->isHardProcess()){
	myEvent.genps_p4->push_back(p4);
	myEvent.genps_pdgId->push_back(genp->pdgId());
	myEvent.genps_status->push_back(genp->status());
	if(genp->mother() != 0)
	  myEvent.genps_motherpdgId->push_back(genp->mother()->pdgId());
      }
    }
  }

  tree->Fill();
}

DEFINE_FWK_MODULE(miniTTree);
