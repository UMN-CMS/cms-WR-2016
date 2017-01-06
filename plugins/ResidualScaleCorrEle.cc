#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

class ResidualScaleCorrEle : public edm::EDProducer
{
public:
  ResidualScaleCorrEle(const edm::ParameterSet&);

  virtual void produce(edm::Event&, const edm::EventSetup&);

private:
  edm::EDGetToken srcToken_;
  edm::EDGetTokenT<EcalRecHitCollection> ebReducedRecHitCollectionMiniAOD_;
};

ResidualScaleCorrEle::ResidualScaleCorrEle(const edm::ParameterSet& cfg)
{

  produces <pat::ElectronCollection>();
  srcToken_ = mayConsume<edm::View<pat::Electron> >(cfg.getParameter<edm::InputTag>("src"));
  ebReducedRecHitCollectionMiniAOD_ = mayConsume<EcalRecHitCollection>(cfg.getParameter<edm::InputTag>("ebReducedRecHitCollectionMiniAOD"));

}

void ResidualScaleCorrEle::produce(edm::Event& event, const edm::EventSetup&)
{
  edm::Handle<edm::View<pat::Electron> > electrons;
  event.getByToken(srcToken_, electrons);

  edm::Handle< EcalRecHitCollection > pEBRecHits;
  event.getByToken( ebReducedRecHitCollectionMiniAOD_, pEBRecHits );
  const EcalRecHitCollection *ebRecHits = pEBRecHits.product();
  
  std::auto_ptr<pat::ElectronCollection> eles(new pat::ElectronCollection);
  reco::Candidate::PolarLorentzVector tmp_ele;

  for(auto ele : *electrons) {
    
    DetId detid = ele.superCluster()->seed()->seed();
    const EcalRecHit * rh = NULL;
    double Ecorr=1;
    if (detid.subdetId() == EcalBarrel) {
      auto rh_i = ebRecHits->find(detid);
      if( rh_i != ebRecHits->end()) rh =  &(*rh_i);
      else rh = NULL;
    } 
    if(rh==NULL) Ecorr=1;
    else{
      if(rh->energy() > 200 && rh->energy()<300)  Ecorr=1.0199;
      else if(rh->energy()>300 && rh->energy()<400) Ecorr=  1.052;
      else if(rh->energy()>400 && rh->energy()<500) Ecorr = 1.015;
    }

	  
    if(event.isRealData())
      tmp_ele.SetCoordinates(ele.pt()*Ecorr, ele.eta(), ele.phi(), ele.mass());
    else
      tmp_ele.SetCoordinates(ele.pt(), ele.eta(), ele.phi(), ele.mass());
      
    ele.setP4(tmp_ele);
    eles->push_back(ele);
  }
  event.put(eles);
}

DEFINE_FWK_MODULE(ResidualScaleCorrEle);
