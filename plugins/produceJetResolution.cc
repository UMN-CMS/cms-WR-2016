#include "DataFormats/Common/interface/ValueMap.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include <memory>
#include <iomanip>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <DataFormats/PatCandidates/interface/Jet.h>
#include "DataFormats/Math/interface/deltaR.h"
//#include "TLorentzVector.h"
#include <vector>
using namespace std;

class produceJetResolution : public edm::EDProducer
{
  typedef float v_t;
  typedef edm::ValueMap<v_t> scale_factors_Map;
  typedef edm::ValueMap<bool> valid_Map;

public:
  produceJetResolution(const edm::ParameterSet&);
  virtual void produce(edm::Event&, const edm::EventSetup&);

private:
  edm::EDGetToken srcToken_;
  edm::EDGetToken gensrcToken_;
  edm::EDGetToken rhoToken_;
  std::string outputCollName1;     ///<label name of collection made by this producer
  std::string outputCollName2;     ///<label name of collection made by this producer
  std::string outputCollName3;     ///<label name of collection made by this producer
  std::string outputCollName4;     ///<label name of collection made by this producer
  std::string outputCollName5;     ///<label name of collection made by this producer
  std::string outputCollName6;     ///<label name of collection made by this producer

};

produceJetResolution::produceJetResolution(const edm::ParameterSet& cfg)
  : srcToken_ ( consumes<edm::View<pat::Jet> >(cfg.getParameter<edm::InputTag>("jets_src"))),
    gensrcToken_ ( consumes<edm::View<reco::GenJet> >(cfg.getParameter<edm::InputTag>("genjets_src"))),
    rhoToken_ ( consumes<double>(cfg.getParameter<edm::InputTag>("rho_src"))),
    outputCollName1(cfg.getParameter<std::string>("OutputCollectionName1")),
    outputCollName2(cfg.getParameter<std::string>("OutputCollectionName2")),
    outputCollName3(cfg.getParameter<std::string>("OutputCollectionName3")),
    outputCollName4(cfg.getParameter<std::string>("OutputCollectionName4")),
    outputCollName5(cfg.getParameter<std::string>("OutputCollectionName5")),
    outputCollName6(cfg.getParameter<std::string>("OutputCollectionName6"))
{
  produces<scale_factors_Map>(outputCollName1);
  produces<scale_factors_Map>(outputCollName2);
  produces<scale_factors_Map>(outputCollName3);
  produces<scale_factors_Map>(outputCollName4);
  produces<scale_factors_Map>(outputCollName5);
  produces<valid_Map>(outputCollName6);
}

  void produceJetResolution::produce(edm::Event& event, const edm::EventSetup& setup)
  {
    edm::Handle<edm::View<pat::Jet> > jets;
    event.getByToken(srcToken_, jets);
    edm::Handle<edm::View<reco::GenJet> > gjets;
    event.getByToken(gensrcToken_, gjets);
    edm::Handle<double> rho;
    event.getByToken(rhoToken_, rho);
    
    JME::JetResolution resolution = JME::JetResolution::get(setup, "AK4PFchs_pt");
    JME::JetResolutionScaleFactor res_sf = JME::JetResolutionScaleFactor::get(setup, "AK4PFchs");
    
    std::vector<v_t>  jetResolution;
    std::vector<v_t>  JER_sf;
    std::vector<v_t>  JER_sf_up;
    std::vector<v_t>  JER_down;
    std::vector<v_t>  genjetPt;
    std::vector<bool>  genjetmatch;

    std::auto_ptr<scale_factors_Map> jetResolutionMap(new scale_factors_Map());
    std::auto_ptr<scale_factors_Map> JER_sfMap(new scale_factors_Map());
    std::auto_ptr<scale_factors_Map> JER_sf_upMap(new scale_factors_Map());
    std::auto_ptr<scale_factors_Map> JER_downMap(new scale_factors_Map());
    std::auto_ptr<scale_factors_Map> genjetPtMap(new scale_factors_Map());
    std::auto_ptr<valid_Map> genjetmatchMap(new valid_Map());
    
    
    for (const auto& jet: *jets) {
      if(!event.isRealData()) {
	// First, using 'set' functions
	JME::JetParameters parameters_1;
	parameters_1.setJetPt(jet.pt());
	parameters_1.setJetEta(jet.eta());
	parameters_1.setRho(*rho);
	
	// Now, get the resolution
	float r = resolution.getResolution(parameters_1);
	
	// We do the same thing to access the scale factors
	float sf = res_sf.getScaleFactor({{JME::Binning::JetEta, jet.eta()}});
	
	// Access up and down variation of the scale factor
	float sf_up = res_sf.getScaleFactor({{JME::Binning::JetEta, jet.eta()}}, Variation::UP);
	float sf_down = res_sf.getScaleFactor({{JME::Binning::JetEta, jet.eta()}}, Variation::DOWN);
	
	jetResolution.push_back(r);
	JER_sf.push_back(sf);
	JER_sf_up.push_back(sf_up);
	JER_down.push_back(sf_down);
	
	float gpt = -999;
	bool match = false;
	for (auto gj: *gjets) {
	  float dR = deltaR(jet,gj);
	  float dpt = abs(jet.pt()-gj.pt());
	  if (dR < (0.4/2) && dpt < (3*r)) {
	    gpt = gj.pt();
	    match = true;
	  }
	}
	genjetPt.push_back(gpt);
	genjetmatch.push_back(match);
	
      } else {
	jetResolution.push_back(1);
	JER_sf.push_back(1);
	JER_sf_up.push_back(1);
	JER_down.push_back(1);
	genjetPt.push_back(-999);
	genjetmatch.push_back(false);
      }
    }
    
    scale_factors_Map::Filler jetResolution_filler(*jetResolutionMap);
    scale_factors_Map::Filler JER_sf_filler(*JER_sfMap);
    scale_factors_Map::Filler JER_sf_up_filler(*JER_sf_upMap);
    scale_factors_Map::Filler JER_down_filler(*JER_downMap);
    scale_factors_Map::Filler genjetPt_filler(*genjetPtMap);
    valid_Map::Filler genjetmatch_filler(*genjetmatchMap);
    
    
    jetResolution_filler.insert(jets, jetResolution.begin(), jetResolution.end());
    JER_sf_filler.insert(jets, JER_sf.begin(), JER_sf.end());
    JER_sf_up_filler.insert(jets, JER_sf_up.begin(), JER_sf_up.end());
    JER_down_filler.insert(jets, JER_down.begin(), JER_down.end());
    genjetPt_filler.insert(jets, genjetPt.begin(), genjetPt.end());
    genjetmatch_filler.insert(jets, genjetmatch.begin(), genjetmatch.end());
    
    jetResolution_filler.fill();
    JER_sf_filler.fill();
    JER_sf_up_filler.fill();
    JER_down_filler.fill();
    genjetPt_filler.fill();
    genjetmatch_filler.fill();
    

    event.put(jetResolutionMap, outputCollName1);
    event.put(JER_sfMap, outputCollName2);
    event.put(JER_sf_upMap, outputCollName3);
    event.put(JER_downMap, outputCollName4);
    event.put(genjetPtMap, outputCollName5);
    event.put(genjetmatchMap, outputCollName6);
    
    jetResolution.clear();
    JER_sf.clear();
    JER_sf_up.clear();
    JER_down.clear();
    genjetPt.clear();
    genjetmatch.clear();
    
  }

DEFINE_FWK_MODULE(produceJetResolution);
