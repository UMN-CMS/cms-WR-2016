#include "DataFormats/PatCandidates/interface/Lepton.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include <string>
//#include "TLorentzVector.h"
#include <vector>
using namespace std;

int FindTrigBin(double Grid[], double Value, const int size);

class produceLepTriggerScaleFactors : public edm::EDProducer
{

	typedef float v_t;
	typedef edm::ValueMap<v_t> scale_factors_Map;

public:
	produceLepTriggerScaleFactors(const edm::ParameterSet&);
	virtual void produce(edm::Event&, const edm::EventSetup&);

private:
	const edm::InputTag src;	///<input particle objects
	edm::EDGetToken srcToken_;
	std::string outputCollName1;     ///<label name of collection made by this producer
	std::string outputCollName2;     ///<label name of collection made by this producer

	std::vector<double> Scale_Factor_TR_Central;
	std::vector<double> Scale_Factor_TR_Error;

};

produceLepTriggerScaleFactors::produceLepTriggerScaleFactors(const edm::ParameterSet& cfg)
	: srcToken_ ( consumes<edm::View<pat::Muon> >(cfg.getParameter<edm::InputTag>("src"))),
	  outputCollName1(cfg.getParameter<std::string>("OutputCollectionName1")),
	  outputCollName2(cfg.getParameter<std::string>("OutputCollectionName2")),
	  Scale_Factor_TR_Central(cfg.getParameter<std::vector<double>>("Scale_Factor_TR_Central")),
	  Scale_Factor_TR_Error(cfg.getParameter<std::vector<double>>("Scale_Factor_TR_Error"))
{
	produces<scale_factors_Map>(outputCollName1);
	produces<scale_factors_Map>(outputCollName2);
}

void produceLepTriggerScaleFactors::produce(edm::Event& event, const edm::EventSetup&)
{
	edm::Handle<edm::View<pat::Muon> > muons;
	event.getByToken(srcToken_, muons);
	std::auto_ptr<pat::MuonCollection> mus(new pat::MuonCollection);
	std::vector<v_t>  scale_factor_TR_central;
	std::vector<v_t>  scale_factor_TR_error;

	const int Bins = 15;
	double Grid[Bins] = { -2.4, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0.0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.4};
	
	std::auto_ptr<scale_factors_Map> scale_factor_TR_centralMap(new scale_factors_Map());
	std::auto_ptr<scale_factors_Map> scale_factor_TR_errorMap(new scale_factors_Map());

	for(auto mu : *muons) {
		if(!event.isRealData()) {
			if(FindTrigBin(Grid, mu.eta(), Bins) != -1) {
				scale_factor_TR_central.push_back(Scale_Factor_TR_Central[FindTrigBin(Grid, mu.eta(), Bins)]);
				scale_factor_TR_error.push_back(Scale_Factor_TR_Error[FindTrigBin(Grid, mu.eta(), Bins)]);

			} else {
				scale_factor_TR_central.push_back(-1);
				scale_factor_TR_error.push_back(-1);
			}
		} else {
			scale_factor_TR_central.push_back(1);
			scale_factor_TR_error.push_back(1);
		}
	}

	scale_factors_Map::Filler scale_factor_TR_central_filler(*scale_factor_TR_centralMap);
	scale_factors_Map::Filler scale_factor_TR_error_filler(*scale_factor_TR_errorMap);


	scale_factor_TR_central_filler.insert(muons, scale_factor_TR_central.begin(), scale_factor_TR_central.end());
	scale_factor_TR_error_filler.insert(muons, scale_factor_TR_error.begin(), scale_factor_TR_error.end());

	scale_factor_TR_central_filler.fill();
	scale_factor_TR_error_filler.fill();


	event.put(scale_factor_TR_centralMap, outputCollName1);
	event.put(scale_factor_TR_errorMap, outputCollName2);

	scale_factor_TR_central.clear();
	scale_factor_TR_error.clear();

}

int FindTrigBin(double Grid[], double Value, const int size)
{
	for(int iii = 0; iii < size - 1; iii++) {
		if(Value >= Grid[iii] && Value < Grid[iii + 1]) {
			if(iii < 7) return (6 - iii);
			else return iii;
		}
	}
	return -1;
}


DEFINE_FWK_MODULE(produceLepTriggerScaleFactors);
