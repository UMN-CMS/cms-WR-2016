#include <map>
#include <utility>
#include <fstream>
#include <string>
#include <map>
#include "TH2F.h"
#include "Selector.h"

#include "RooAbsPdf.h"
#include "RooRealVar.h"
using namespace RooFit;

typedef std::map< std::pair<Selector::tag_t, int>, std::pair<int, int> > mass_cut_map_t;
static const int MAX_PU_REWEIGHT = 59;

mass_cut_map_t getMassCutMap()
{
  mass_cut_map_t mass_cut;
  std::ifstream ifs;
  ifs.open("configs/mass_cuts.txt", std::ifstream::in);
  std::string line;
  std::string mass, ch, low, high;
  while (std::getline(ifs, line)) {
    if (line[0] == '#' || line.size() == 0) continue;
    std::stringstream ss(line);
    std::getline(ss, ch, ' ');
    std::getline(ss, mass, ' ');
    std::getline(ss, low, ' ');
    std::getline(ss, high, ' ');
    mass_cut[std::make_pair(Selector::getTag(ch), std::stoi(mass))] = std::make_pair(std::stoi(low), std::stoi(high));
  }

  return mass_cut;
}

std::vector<int> getMassVec()
{
  std::vector<int> mass;
  std::ifstream ifs;
  ifs.open("configs/mass_cuts.txt", std::ifstream::in);
  std::string line;
  std::string m;
  while (std::getline(ifs, line)) {
    if (line[0] == '#' || line.size() == 0) continue;
    std::stringstream ss(line);
    std::getline(ss, m, ' ');
    std::getline(ss, m, ' ');
    mass.push_back(std::stoi(m));
  }

  return mass;
}

double NormalizedIntegral(RooAbsPdf * function, RooRealVar& integrationVar, double lowerLimit, double upperLimit)
{
  integrationVar.setRange("integralRange", lowerLimit, upperLimit) ;
  RooAbsReal* integral = (*function).createIntegral(integrationVar, NormSet(integrationVar), Range("integralRange")) ;
  double normalizedIntegralValue = integral->getVal();
  return normalizedIntegralValue;
}

std::map<float, double> PUreweight(TString PileupDataFilename)
{
  TFile data(PileupDataFilename);
  TFile mc("data/MCPileup.root");

  if(!data.IsOpen() || !mc.IsOpen()) {
    std::cerr << "[ERROR] data or mc PU file not opened" << std::endl;
    exit(1);
  }

  TH1D* puMC_hist = (TH1D*) mc.Get("pileup");
  if(puMC_hist == NULL) {
    std::cerr << "[ERROR] mc pileup histogram is NULL" << std::endl;
    exit(1);
  }

  TH1D *puData_hist = (TH1D *) data.Get("pileup");
  if(puData_hist == NULL) {
    std::cerr << "[ERROR] data pileup histogram is NULL" << std::endl;
    exit(1);
  }

  // they should have bin width = 1
  if(puMC_hist->GetBinWidth(2) != 1) {
    std::cerr << "[ERROR] Bin width for mc pileup distribution different from 1" << std::endl;
    exit(1);
  }

  if(puData_hist->GetBinWidth(2) != 1) {
    std::cerr << "[ERROR] Bin width for data pileup distribution different from 1" << std::endl;
    exit(1);
  }

  // they should have the same binning otherwise PU weights until the max between them
  int nBins = MAX_PU_REWEIGHT;
  if(puData_hist->GetNbinsX() != puMC_hist->GetNbinsX()) {
    std::cerr << "[WARNING] Different binning between data and mc pileup distributions:" << puData_hist->GetNbinsX() << "\t" << puMC_hist->GetNbinsX() << std::endl;
    nBins = std::min(puData_hist->GetNbinsX(), puMC_hist->GetNbinsX());
    //std::cerr << "Pileup reweighted until nPU max = " << nBins;
  }

  nBins = std::min(MAX_PU_REWEIGHT, nBins);

  // Normalize Histograms
  float puMC_int = puMC_hist->Integral(0, nBins);
  float puData_int = puData_hist->Integral(0, nBins);
  puMC_hist->Scale(1. / puMC_int);
  puData_hist->Scale(1. / puData_int);

  double puMCweight_int = 0;
  std::map<float, double> pu_weights;
  for (int i = 0; i < nBins; i++) {
    double binContent = puMC_hist->GetBinContent(i + 1);
    if(binContent == 0 && puData_hist->GetBinContent(i + 1) != 0) {
      if(puData_hist->GetBinContent(i + 1) < 1e-4 || i < 4) {
	std::cerr << "[WARNING] mc bin empty while data not: iBin = " << i + 1 << std::endl;
	std::cerr << "          data bin = " << puData_hist->GetBinContent(i + 1) << std::endl;
      } else {
	std::cerr << "[WARNING] mc bin empty while data not: iBin = " << i + 1 << std::endl;
	std::cerr << "        data bin = " << puData_hist->GetBinContent(i + 1) << std::endl;
	puData_hist->SetBinContent(i + 1, 1e-4);
      }
    }
    double weight = binContent > 0 ? puData_hist->GetBinContent(i + 1) / binContent : 0;
    // the MC normalization should not change, so calculate the
    // integral of the MC and rescale the weights by that
    puMCweight_int += weight * binContent;
    pu_weights[i] = weight;
  }

  for (int i = 0; i < nBins; i++) {
    pu_weights[i] /= puMCweight_int;
  }


  return pu_weights;
}

std::pair<float,float> ElectronRecoSF(float eta, float pt, TH2F *h){
  Int_t eta_bin = h->GetXaxis()->FindBin(eta);
  Int_t pt_bin = h->GetYaxis()->FindBin(pt);
  float sf = h->GetBinContent(eta_bin,pt_bin);
  float e = h->GetBinError(eta_bin,pt_bin);
  if(sf==0)
    sf=1.0;
  return std::make_pair(sf,e);
}

std::pair<float,float> IdSF(float eta, float pt, TH2F *h1, TH2F *h2){
  Int_t eta_bin1 = h1->GetXaxis()->FindBin(eta);
  Int_t pt_bin1 = h1->GetYaxis()->FindBin(pt);
  Int_t eta_bin2 = h2->GetXaxis()->FindBin(eta);
  Int_t pt_bin2 = h2->GetYaxis()->FindBin(pt);

  float w1 = (5.929+2.646+4.353+4.117+3.186)/(5.929+2.646+4.353+4.117+3.186+7.721+8.636+0.221);
  float w2 = 1.0 - w1;
  
  float sf1 = h1->GetBinContent(eta_bin1,pt_bin1);
  float sf2 = h2->GetBinContent(eta_bin2,pt_bin2);
  float e1 = h1->GetBinError(eta_bin1,pt_bin1);
  float e2 = h2->GetBinError(eta_bin2,pt_bin2);

  if(sf1==0)
    sf1=1.0;
  if(sf2==0)
    sf2=1.0;
  
  return std::make_pair(sf1*w1+sf2*w2,TMath::Sqrt(e1*e1*w1*w1+e2*e2*w2*w2));  
}

std::pair<float,float> IsoSF(float eta, float pt, TH2F *h1, TH2F *h2){
  Int_t eta_bin1 = h1->GetXaxis()->FindBin(eta);
  Int_t pt_bin1 = h1->GetYaxis()->FindBin(pt);
  Int_t eta_bin2 = h2->GetXaxis()->FindBin(eta);
  Int_t pt_bin2 = h2->GetYaxis()->FindBin(pt);

  float w1 = (5.929+2.646+4.353+4.117+3.186)/(5.929+2.646+4.353+4.117+3.186+7.721+8.636+0.221);
  float w2 = 1.0 - w1;
  
  float sf1 = h1->GetBinContent(eta_bin1,pt_bin1);
  float sf2 = h2->GetBinContent(eta_bin2,pt_bin2);
  float e1 = h1->GetBinError(eta_bin1,pt_bin1);
  float e2 = h2->GetBinError(eta_bin2,pt_bin2);

  if(sf1==0)
    sf1=1.0;
  if(sf2==0)
    sf2=1.0;
  
  return std::make_pair(sf1*w1+sf2*w2,TMath::Sqrt(e1*e1*w1*w1+e2*e2*w2*w2));
}

