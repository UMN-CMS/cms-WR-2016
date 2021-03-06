#include "TTree.h"
#include "TLorentzVector.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

class highMlljj : public edm::EDAnalyzer
{
public:
  explicit highMlljj(const edm::ParameterSet&);

private:
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  edm::EDGetToken muonsMiniAODToken_;  
  edm::EDGetToken jetsMiniAODToken_;  
  edm::EDGetToken electronsMiniAODToken_;  
  edm::EDGetToken photonsMiniAODToken_;  
  edm::EDGetToken metMiniAODToken_;  

  struct tree_t {
    unsigned run;
    unsigned lumi;
    unsigned event;

    float weight;
    float genpT;
    float genpdgId_1;
    float genpdgId_2;
	  

    tree_t()
    {
      clear();
    }

    void clear()
    {
      run = lumi = event = 0;
      weight = 0;
      genpT = 0;
      genpdgId_1 = genpdgId_2 = 0;
    }
  };

  TTree* tree;
  tree_t nt;
};

highMlljj::highMlljj(const edm::ParameterSet& cfg) :
  muonsMiniAODToken_ ( consumes<edm::View<pat::Muon> >(cfg.getParameter<edm::InputTag>("muons_src"))),
  jetsMiniAODToken_ ( consumes<edm::View<pat::Jet> >(cfg.getParameter<edm::InputTag>("jets_src"))),
  electronsMiniAODToken_ ( consumes<edm::View<pat::Electron> >(cfg.getParameter<edm::InputTag>("electrons_src"))),
  photonsMiniAODToken_ ( consumes<edm::View<pat::Photon> >(cfg.getParameter<edm::InputTag>("photons_src"))),
  metMiniAODToken_ ( consumes<edm::View<pat::MET> >(cfg.getParameter<edm::InputTag>("met_src")))
{
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("t", "");
  tree->Branch("run", &nt.run, "run/i");
  tree->Branch("lumi", &nt.lumi, "lumi/i");
  tree->Branch("event", &nt.event, "event/i");

  tree->Branch("weight", &nt.weight, "weight/F");
  tree->Branch("genpT", &nt.genpT, "genpT/F");
  tree->Branch("genpdgId_1", &nt.genpdgId_1, "genpdgId_1/F");
  tree->Branch("genpdgId_2", &nt.genpdgId_2, "genpdgId_2/F");
}

void highMlljj::analyze(const edm::Event& event, const edm::EventSetup&)
{
  nt.clear();
  nt.run = event.id().run();
  nt.lumi = event.luminosityBlock();
  nt.event = event.id().event();

  edm::Handle<edm::View<pat::Muon> > muons;
  event.getByToken(muonsMiniAODToken_, muons);
  edm::Handle<edm::View<pat::Jet> > jets;
  event.getByToken(jetsMiniAODToken_, jets);
  edm::Handle<edm::View<pat::Electron> > electrons;
  event.getByToken(electronsMiniAODToken_, electrons);
  edm::Handle<edm::View<pat::Photon> > photons;
  event.getByToken(photonsMiniAODToken_, photons);

  edm::Handle<edm::View<pat::MET> > mets;
  event.getByToken(metMiniAODToken_, mets);

  const pat::MET &met = mets->front();
  printf("MET: pt %5.1f, phi %+4.2f, sumEt (%.1f). MET with JES up/down: %.1f/%.1f\n",
	 met.pt(), met.phi(), met.sumEt(),
	 met.shiftedPt(pat::MET::JetEnUp), met.shiftedPt(pat::MET::JetEnDown));
  
  printf("\n");
    
  std::cout<<"NJets="<<jets->size()<<std::endl;
  for (auto j:*jets)
    {
      std::cout<<j.pt()<<" "<<j.eta()<<" "<<j.phi()<<std::endl;
    }
  
  std::cout<<"NMuons="<<muons->size()<<std::endl;
  for (auto m:*muons)
    {
      std::cout<<m.pt()<<" "<<m.eta()<<" "<<m.phi()<<std::endl;
    }
  
  std::cout<<"NElectrons="<<electrons->size()<<std::endl;
  for (auto e:*electrons)
    {
      std::cout<<e.pt()<<" "<<e.eta()<<" "<<e.phi()<<std::endl;
    }
  
  std::cout<<"NPhotons="<<photons->size()<<std::endl;
  for (auto p:*photons)
    {
      std::cout<<p.pt()<<" "<<p.eta()<<" "<<p.phi()<<std::endl;
    }
  
	
  tree->Fill();
}

DEFINE_FWK_MODULE(highMlljj);
