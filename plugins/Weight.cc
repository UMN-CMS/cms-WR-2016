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
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

class Weight : public edm::EDAnalyzer
{
public:
	explicit Weight(const edm::ParameterSet&);

private:
	virtual void analyze(const edm::Event&, const edm::EventSetup&);

	edm::EDGetToken evinfoToken_;
	edm::EDGetToken genparToken_;  

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

Weight::Weight(const edm::ParameterSet& cfg) :
  evinfoToken_ ( consumes<GenEventInfoProduct>(edm::InputTag("generator"))),
  genparToken_ ( consumes<edm::View<reco::GenParticle>>(edm::InputTag("prunedGenParticles")))
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

void Weight::analyze(const edm::Event& event, const edm::EventSetup&)
{
	nt.clear();
	nt.run = event.id().run();
	nt.lumi = event.luminosityBlock();
	nt.event = event.id().event();

	edm::Handle<GenEventInfoProduct> evinfo;
	event.getByToken(evinfoToken_, evinfo);
	nt.weight = evinfo->weight();

	edm::Handle<edm::View<reco::GenParticle> > genpars;
	event.getByToken(genparToken_, genpars);
	std::vector<reco::GenParticle> leps;
	std::vector<Int_t> ids;
	for(auto g: *genpars) {
	  if(g.isHardProcess() && (abs(g.pdgId())==11 || abs(g.pdgId())==13 || abs(g.pdgId())==15)) {
	    leps.push_back(g);
	    ids.push_back(g.pdgId());
	  }
	}

	//std::cout<<leps.size()<<std::endl;

	if(leps.size() != 2)
	  return;

	nt.genpT = (leps[0].p4()+leps[1].p4()).Pt();
	nt.genpdgId_1 = ids[0];
	nt.genpdgId_2 = ids[1];
	
	tree->Fill();
}

DEFINE_FWK_MODULE(Weight);
