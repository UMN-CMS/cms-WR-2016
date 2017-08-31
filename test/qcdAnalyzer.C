#include "TLorentzVector.h"
#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "DataFormats/Math/interface/deltaR.h"
#include <vector>
#include <TStopwatch.h>

#ifdef __CINT__
#pragma link C++ class std::vector<TLorentzVector>+;
#endif

// #include "loader.C"

float dR_TLV(TLorentzVector t1, TLorentzVector t2)
{
  return deltaR(t1.Eta(), t1.Phi(), t2.Eta(), t2.Phi());
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

  int MAX_PU_REWEIGHT = 50;
  
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

void Analyzer(bool data, bool dy, bool tt, bool other, bool qcd){


  TFile TrigSF1("data/EfficienciesAndSF_TrigBF.root");
  TFile TrigSF2("data/EfficienciesAndSF_TrigGH.root");
  TH2F *Trigh1 = (TH2F*)TrigSF1.Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
  TH2F *Trigh2 = (TH2F*)TrigSF2.Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
  TH2F *Trigg1 = (TH2F*)TrigSF1.Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesDATA/abseta_pt_DATA");
  TH2F *Trigg2 = (TH2F*)TrigSF2.Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesDATA/abseta_pt_DATA");

  TFile IDSF1("data/EfficienciesAndSF_IdBF.root");
  TFile IDSF2("data/EfficienciesAndSF_IdGH.root");
  TH2F *IDh1 = (TH2F*)IDSF1.Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/abseta_pair_ne_ratio");
  TH2F *IDh2 = (TH2F*)IDSF2.Get("MC_NUM_HighPtID_DEN_genTracks_PAR_newpt_eta/abseta_pair_ne_ratio");
  
  TFile ISOSF1("data/EfficienciesAndSF_IsoBF.root");
  TFile ISOSF2("data/EfficienciesAndSF_IsoGH.root");
  TH2F *ISOh1 = (TH2F*)ISOSF1.Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");
  TH2F *ISOh2 = (TH2F*)ISOSF2.Get("tkLooseISO_highptID_newpt_eta/abseta_pair_ne_ratio");
  
  TChain * chain = new TChain("miniTree_qcd/t");
    
  TString fname = "";
  TString inputDir = "root://eoscms.cern.ch//eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/ntuples/";
  std::cout<<"Begin"<<std::endl;
  if(data){
    // chain->Add("root://cmseos.fnal.gov//store/user/jchaves/ntuples/DoubleEG_RunB_v3_WRv07/*.root");
    // chain->Add("root://cmseos.fnal.gov//store/user/jchaves/ntuples/DoubleEG_RunC_WRv07/*.root");
    // chain->Add("root://cmseos.fnal.gov//store/user/jchaves/ntuples/DoubleEG_RunD_WRv07/*.root");
    // chain->Add("root://cmseos.fnal.gov//store/user/jchaves/ntuples/DoubleEG_RunE_WRv07/*.root");
    // chain->Add("root://cmseos.fnal.gov//store/user/jchaves/ntuples/DoubleEG_RunF_WRv07/*.root");
    // chain->Add("root://cmseos.fnal.gov//store/user/jchaves/ntuples/DoubleEG_RunG_WRv07/*.root");
    // chain->Add("root://cmseos.fnal.gov//store/user/jchaves/ntuples/DoubleEG_RunH_v2_WRv07/*.root");
    // chain->Add("root://cmseos.fnal.gov//store/user/jchaves/ntuples/DoubleEG_RunH_v3_WRv07/*.root");
    chain->Add(inputDir+"DoubleEG_RunB_v3_WRv07/*.root");
    chain->Add(inputDir+"DoubleEG_RunC_WRv07/*.root");
    chain->Add(inputDir+"DoubleEG_RunD_WRv07/*.root");
    chain->Add(inputDir+"DoubleEG_RunE_WRv07/*.root");
    chain->Add(inputDir+"DoubleEG_RunF_WRv07/*.root");
    chain->Add(inputDir+"DoubleEG_RunG_WRv07/*.root");
    chain->Add(inputDir+"DoubleEG_RunH_v2_WRv07/*.root");
    chain->Add(inputDir+"DoubleEG_RunH_v3_WRv07/*.root");
   fname = "data";
    
  }
  else if (dy){
    chain->Add(inputDir+"DYJets_amcatnlo_pt50_100_v1_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt50_100_v2_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt100_250_v1_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt100_250_v2_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt100_250_v3_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt100_250_v4_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt250_400_v1_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt250_400_v2_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt250_400_v3_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt250_400_v4_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt400_650_v1_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt400_650_v2_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt400_650_v3_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt650_Inf_v1_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt650_Inf_v2_WRv07/*.root");
    chain->Add(inputDir+"DYJets_amcatnlo_pt650_Inf_v3_WRv07/*.root");
    fname = "dy";
  }
  else if (tt){
    // chain->Add(inputDir+"TTJets_DiLept_amcatnlo_WRv07/*.root");
    chain->Add(inputDir+"TTJets_DiLept_amcatnlo_v1_WRv07/*.root");
    chain->Add(inputDir+"TTJets_DiLept_amcatnlo_v2_WRv07/*.root");
    fname = "tt";
  }

  else if (other){
    chain->Add(inputDir+"SingleTop_TTWLL_WRv06/*.root");
    chain->Add(inputDir+"SingleTop_TWNuNu_WRv06/*.root");
    chain->Add(inputDir+"SingleTop_tbarinc_WRv06/*.root");
    chain->Add(inputDir+"SingleTop_tinc_WRv06/*.root");
    chain->Add(inputDir+"WW_WRv07/*.root");
    chain->Add(inputDir+"WZ_WRv07/*.root");
    chain->Add(inputDir+"ZZ_WRv07/*.root");
    chain->Add(inputDir+"WJetsLNu_WRv07/*.root");
    fname = "other";
  }
  
  else if (qcd){
    chain->Add(inputDir+"QCD_bcToE_15_20_WRv07/*.root");
    chain->Add(inputDir+"QCD_bcToE_170_250_WRv07/*.root");
    chain->Add(inputDir+"QCD_bcToE_20_30_WRv07/*.root");
    chain->Add(inputDir+"QCD_bcToE_250_Inf_WRv07/*.root");
    chain->Add(inputDir+"QCD_bcToE_30_80_WRv07/*.root");
    chain->Add(inputDir+"QCD_bcToE_80_170_WRv07/*.root");

    chain->Add(inputDir+"QCD_EMEnriched_120_170_v1_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_120_170_v2_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_170_300_v1_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_20_30_v1_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_300_Inf_v1_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_30_50_v1_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_30_50_v2_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_50_80_v1_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_50_80_v2_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_80_120_v1_WRv07/*.root");
    chain->Add(inputDir+"QCD_EMEnriched_80_120_v2_WRv07/*.root");
    fname = "qcd";
  }

  Float_t WR_mass; // this is of Float_t because want to save it into a tree
  Float_t dilepton_pt;
  Float_t dilepton_mass;

  Float_t lead_lepton_pt; // flatten the collections for easy plotting
  Float_t sublead_lepton_pt;
  Float_t lead_lepton_eta;
  Float_t sublead_lepton_eta;
  Float_t lead_lepton_phi;
  Float_t sublead_lepton_phi;
  Float_t lead_lepton_bitmap;
  Float_t sublead_lepton_bitmap;
  Float_t lead_jet_pt;
  Float_t sublead_jet_pt;
  Float_t lead_jet_eta;
  Float_t sublead_jet_eta;
  Float_t lead_jet_phi;
  Float_t sublead_jet_phi;
  Float_t lead_jet_jec_unc;
  Float_t sublead_jet_jec_unc;
  Float_t lead_lepton_weight;
  Float_t sublead_lepton_weight;
  Float_t lead_jet_weight;
  Float_t sublead_jet_weight;
  Float_t dR_leadlepton_leadjet;
  Float_t dR_leadlepton_subleadjet;
  Float_t dR_subleadlepton_leadjet;
  Float_t dR_subleadlepton_subleadjet;
  Float_t lead_lepton_r9;
  Float_t sublead_lepton_r9;
  Float_t lead_lepton_IDSF_error;
  Float_t lead_lepton_IsoSF_error;
  Float_t lead_lepton_RecoSF_error;
  Float_t lead_lepton_HltSF_error;
  Float_t lead_lepton_EScaling_error;
  Float_t lead_lepton_ESmearing_sigma;
  Float_t lead_lepton_ESmearing_sigma_phi_up;
  Float_t lead_lepton_ESmearing_sigma_phi_down;
  Float_t lead_lepton_ESmearing_sigma_rho_up;
  Float_t lead_lepton_ESmearing_sigma_rho_down;
  Float_t sublead_lepton_IDSF_error;
  Float_t sublead_lepton_IsoSF_error;
  Float_t sublead_lepton_RecoSF_error;
  Float_t sublead_lepton_HltSF_error;
  Float_t sublead_lepton_EScaling_error;
  Float_t sublead_lepton_ESmearing_sigma;
  Float_t sublead_lepton_ESmearing_sigma_phi_up;
  Float_t sublead_lepton_ESmearing_sigma_phi_down;
  Float_t sublead_lepton_ESmearing_sigma_rho_up;
  Float_t sublead_lepton_ESmearing_sigma_rho_down;

  Float_t nPU;	///<positive only for MC
  Int_t nPV;
  Int_t njets;

  Float_t nweight; ///< this variable takes into accont the product of the global_event_weight and the single object weights   the cross sxn normalization is applied in analysis.cpp
  Float_t pu_weight;	///< the PU weight, equivalent to the absolute value of the global_event_weight

  Float_t HT;

  Int_t nEntries = chain->GetEntries();

  TStopwatch ts;
  ts.Start();
  
  std::vector<TLorentzVector> * muons_p4;
  std::vector<TLorentzVector> * electrons_p4;
  std::vector<TLorentzVector> * jets_p4;
  std::vector<TLorentzVector> * genps_p4;
  Float_t weight;
  char datasetName[30];
  std::vector<Int_t>  * genps_pdgId;
  std::vector<Int_t>  * electron_heepBitmap;
  std::vector<Int_t>  * electron_heepId;
  //Int_t nPV;
  
  muons_p4 = 0;
  electrons_p4 = 0;
  jets_p4 = 0;
  weight = 0;
  
  nPV = 0;
  nPU = 0;

  electron_heepId = 0;
  electron_heepBitmap = 0;
  
  chain->SetBranchAddress("muons_p4", &muons_p4);
  chain->SetBranchAddress("electrons_p4", &electrons_p4);
  chain->SetBranchAddress("jets_p4", &jets_p4);
  if(!data){
    genps_p4 = 0;
    genps_pdgId = 0;
    chain->SetBranchAddress("genps_p4", &genps_p4);
    chain->SetBranchAddress("genps_pdgId", &genps_pdgId);
  }
  chain->SetBranchAddress("weight", &weight);
  chain->SetBranchAddress("nPV", &nPV);
  chain->SetBranchAddress("nPU", &nPU);
  chain->SetBranchAddress("datasetName", datasetName);
  // chain->SetBranchAddress("electron_heepId", &electron_heepId);
  chain->SetBranchAddress("electron_heepBitmap", &electron_heepBitmap);
  
  TString dataPUfn = "data/MyDataPileupHistogram.root";
  std::map<float, double> pu_weights = PUreweight(dataPUfn);

  TFile f("selected_"+fname+".root", "recreate");  
  // TTree * tree = new TTree("tree", "");
  
  // tree->Branch("lead_lepton_pt", &lead_lepton_pt);
  // tree->Branch("sublead_lepton_pt", &sublead_lepton_pt);
  // tree->Branch("lead_lepton_eta", &lead_lepton_eta);
  // tree->Branch("sublead_lepton_eta", &sublead_lepton_eta);
  // tree->Branch("lead_lepton_phi", &lead_lepton_phi);
  // tree->Branch("sublead_lepton_phi", &sublead_lepton_phi);
  // tree->Branch("lead_lepton_bitmap", &lead_lepton_bitmap);
  // tree->Branch("sublead_lepton_bitmap", &sublead_lepton_bitmap);
  // tree->Branch("lead_jet_pt", &lead_jet_pt);
  // tree->Branch("sublead_jet_pt", &sublead_jet_pt);
  // tree->Branch("lead_jet_eta", &lead_jet_eta);
  // tree->Branch("sublead_jet_eta", &sublead_jet_eta);
  // tree->Branch("lead_jet_phi", &lead_jet_phi);
  // tree->Branch("sublead_jet_phi", &sublead_jet_phi);
  // tree->Branch("nPV", &nPV);
  // tree->Branch("nPU", &nPU);
  // tree->Branch("dR_leadlepton_leadjet", &dR_leadlepton_leadjet);
  // tree->Branch("dR_leadlepton_subleadjet", &dR_leadlepton_subleadjet);
  // tree->Branch("dR_subleadlepton_leadjet", &dR_subleadlepton_leadjet);
  // tree->Branch("dR_subleadlepton_subleadjet", &dR_subleadlepton_subleadjet);
  // tree->Branch("lead_lepton_r9", &lead_lepton_r9);
  // tree->Branch("sublead_lepton_r9", &sublead_lepton_r9);

  // tree->Branch("weight", &nweight);
  // tree->Branch("WR_mass", &WR_mass);
  // tree->Branch("dilepton_mass", &dilepton_mass);
  // tree->Branch("dilepton_pt", &dilepton_pt);
  // tree->Branch("pu_weight", &pu_weight);
  // tree->Branch("HT", &HT);
  // tree->Branch("njets", &njets);
  // tree->Branch("lead_jet_jec_unc", &lead_jet_jec_unc);
  // tree->Branch("sublead_jet_jec_unc", &sublead_jet_jec_unc);

  // tree->Branch("lead_lepton_IDSF_error", &lead_lepton_IDSF_error);
  // tree->Branch("lead_lepton_IsoSF_error", &lead_lepton_IsoSF_error);
  // tree->Branch("lead_lepton_RecoSF_error", &lead_lepton_RecoSF_error);
  // tree->Branch("lead_lepton_HltSF_error", &lead_lepton_HltSF_error);
  // tree->Branch("lead_lepton_EScaling_error", &lead_lepton_EScaling_error);
  // tree->Branch("lead_lepton_ESmearing_sigma", &lead_lepton_ESmearing_sigma);
  // tree->Branch("lead_lepton_ESmearing_sigma_phi_up", &lead_lepton_ESmearing_sigma_phi_up);
  // tree->Branch("lead_lepton_ESmearing_sigma_phi_down", &lead_lepton_ESmearing_sigma_phi_down);
  // tree->Branch("lead_lepton_ESmearing_sigma_rho_up", &lead_lepton_ESmearing_sigma_rho_up);
  // tree->Branch("lead_lepton_ESmearing_sigma_rho_down", &lead_lepton_ESmearing_sigma_rho_down);
  // tree->Branch("sublead_lepton_IDSF_error", &sublead_lepton_IDSF_error);
  // tree->Branch("sublead_lepton_IsoSF_error", &sublead_lepton_IsoSF_error);
  // tree->Branch("sublead_lepton_RecoSF_error", &sublead_lepton_RecoSF_error);
  // tree->Branch("sublead_lepton_HltSF_error", &sublead_lepton_HltSF_error);
  // tree->Branch("sublead_lepton_EScaling_error", &sublead_lepton_EScaling_error);
  // tree->Branch("sublead_lepton_ESmearing_sigma", &sublead_lepton_ESmearing_sigma);
  // tree->Branch("sublead_lepton_ESmearing_sigma_phi_up", &sublead_lepton_ESmearing_sigma_phi_up);
  // tree->Branch("sublead_lepton_ESmearing_sigma_phi_down", &sublead_lepton_ESmearing_sigma_phi_down);
  // tree->Branch("sublead_lepton_ESmearing_sigma_rho_up", &sublead_lepton_ESmearing_sigma_rho_up);
  // tree->Branch("sublead_lepton_ESmearing_sigma_rho_down", &sublead_lepton_ESmearing_sigma_rho_down);
 

  Float_t lumi = 35867.0;

  unsigned long long int nEntries_100 = nEntries / 100;


  std::cout<<"Nentries="<<nEntries<<std::endl;

  TH1F* h_wrmass1 = new TH1F("h_wrmass1","",140,0,7000);
  TH1F* h_wrmass2 = new TH1F("h_wrmass2","",140,0,7000);
  TH1F* h_wrmass3 = new TH1F("h_wrmass3","",140,0,7000);
  TH1F* h_wrmass4 = new TH1F("h_wrmass4","",140,0,7000);
  TH1F* h_wrmass5 = new TH1F("h_wrmass5","",140,0,7000);

  TH1F* h_bitmaps = new TH1F("h_bitmaps","",1200,3000,4200);
  TH1F* h_neles = new TH1F("h_neles","",10,0,10);
  
  for(int i = 0; i < nEntries; i++){
    //for(int i = 0; i < 100000; i++){

    if(nEntries > 100 && i % nEntries_100 == 1) {
      std::cout << "\b\b\b\b\b[" << std::setw (2) <<  (int)(i / nEntries_100) << "%]" << std::flush;
    }
    
    chain->GetEntry(i);

    //continue;
    
    std::vector<TLorentzVector> selMuons;
    std::vector<TLorentzVector> selElectrons;
    std::vector<TLorentzVector> badElectrons;
    std::vector<TLorentzVector> selJets;

    std::vector<Int_t> eleBitmaps;
    
    for(unsigned int i = 0; i < electrons_p4->size(); i++) {
      TLorentzVector e = electrons_p4->at(i);
      if(e.Pt() > 50 && (fabs(e.Eta()) < 1.4442 || (fabs(e.Eta()) < 2.4 && fabs(e.Eta()) > 1.566))){
	if(electron_heepBitmap->at(i) != 0xfff)
	    badElectrons.push_back(e);
	  
	//if(electron_heepBitmap->at(i) == 0xe7f || electron_heepBitmap->at(i) == 0xfff)
	  {
	    selElectrons.push_back(e);
	    eleBitmaps.push_back(electron_heepBitmap->at(i));
	  }
      }
    }

    for(auto j : *jets_p4) {
      if(j.Pt() > 40 && fabs(j.Eta()) < 2.4)
	selJets.push_back(j);
    }

    std::sort(selMuons.begin(), selMuons.end(),
    	      [](TLorentzVector const & a, TLorentzVector const & b) {
    		return a.Pt() > b.Pt();
    	      });
    std::sort(selElectrons.begin(), selElectrons.end(),
    	      [](TLorentzVector const & a, TLorentzVector const & b) {
    		return a.Pt() > b.Pt();
    	      });
    std::sort(badElectrons.begin(), badElectrons.end(),
    	      [](TLorentzVector const & a, TLorentzVector const & b) {
    		return a.Pt() > b.Pt();
    	      });
    std::sort(selJets.begin(), selJets.end(),
    	      [](TLorentzVector const & a, TLorentzVector const & b) {
    		return a.Pt() > b.Pt();
    	      });

    if(selJets.size()<2) continue;
    
    float weight_0 = 0.0;
    
    if(badElectrons.size()>0 && selElectrons.size()>0){
      if(selElectrons[0].Pt() > 53 && fabs(selElectrons[0].Eta()) < 2.4 && eleBitmaps[0] == 0xfff ){
	if(badElectrons[0].Pt() > 53 && fabs(badElectrons[0].Eta()) < 2.4){
	  if(dR_TLV(badElectrons[0],selJets[0]) > 0.4 && dR_TLV(badElectrons[0],selJets[1]) > 0.4 && dR_TLV(selElectrons[0],selJets[0]) > 0.4 && dR_TLV(selElectrons[0],selJets[1]) > 0.4){
	    
	    if (fabs(badElectrons[0].Eta())<1.4442){
	      if (badElectrons[0].Pt() < 61.9)
		weight_0 = 0.0628-0.000803*badElectrons[0].Pt();
	      else if (badElectrons[0].Pt() < 152.2)
		weight_0 = 0.0185-(8.84e-5)*badElectrons[0].Pt();
	      else if (badElectrons[0].Pt() < 408.1)
		weight_0 = 0.00505-(6.51e-8)*badElectrons[0].Pt();
	      else
		weight_0 = 0.00181+(7.88e-6)*badElectrons[0].Pt();
	    }

	    else if (fabs(badElectrons[0].Eta())<2.0){
	      if (badElectrons[0].Pt() < 78.5)
		weight_0 = 0.0955-0.000719*badElectrons[0].Pt();
	      else if (badElectrons[0].Pt() < 226.8)
		weight_0 = 0.0394-(4.06e-6)*badElectrons[0].Pt();
	      else if (badElectrons[0].Pt() < 345.4)
		weight_0 = 0.0229+(6.86e-6)*badElectrons[0].Pt();
	      else
		weight_0 = 0.0466;
	    }

	    else if (fabs(badElectrons[0].Eta())<2.4){
	      if (badElectrons[0].Pt() < 57.3)
		weight_0 = 0.1-0.000789*badElectrons[0].Pt();
	      else
		weight_0 = 0.0513+(6.29e-5)*badElectrons[0].Pt();
	    }
	    
	    if(selElectrons[0].Pt() > 60 || badElectrons[0].Pt() > 60)
	      h_wrmass5->Fill((selElectrons[0]+badElectrons[0]+selJets[0]+selJets[1]).M(), weight_0/(1-weight_0));
	  }
	}
      }      
    }

    float weight_1 = 0.0;
    float weight_2 = 0.0;
    
    if(badElectrons.size()>1){
      if(badElectrons[0].Pt() > 60 && fabs(badElectrons[0].Eta()) < 2.4){
	if(badElectrons[1].Pt() > 53 && fabs(badElectrons[1].Eta()) < 2.4){
	  if(dR_TLV(badElectrons[0],selJets[0]) > 0.4 && dR_TLV(badElectrons[0],selJets[1]) > 0.4 && dR_TLV(badElectrons[1],selJets[0]) > 0.4 && dR_TLV(badElectrons[1],selJets[1]) > 0.4){

	    if (fabs(badElectrons[0].Eta())<1.4442){
	      if (badElectrons[0].Pt() < 61.9)
		weight_1 = 0.0628-0.000803*badElectrons[0].Pt();
	      else if (badElectrons[0].Pt() < 152.2)
		weight_1 = 0.0185-(8.84e-5)*badElectrons[0].Pt();
	      else if (badElectrons[0].Pt() < 408.1)
		weight_1 = 0.00505-(6.51e-8)*badElectrons[0].Pt();
	      else
		weight_1 = 0.00181+(7.88e-6)*badElectrons[0].Pt();
	    }

	    else if (fabs(badElectrons[0].Eta())<2.0){
	      if (badElectrons[0].Pt() < 78.5)
		weight_1 = 0.0955-0.000719*badElectrons[0].Pt();
	      else if (badElectrons[0].Pt() < 226.8)
		weight_1 = 0.0394-(4.06e-6)*badElectrons[0].Pt();
	      else if (badElectrons[0].Pt() < 345.4)
		weight_1 = 0.0229+(6.86e-6)*badElectrons[0].Pt();
	      else
		weight_1 = 0.0466;
	    }

	    else if (fabs(badElectrons[0].Eta())<2.4){
	      if (badElectrons[0].Pt() < 57.3)
		weight_1 = 0.1-0.000789*badElectrons[0].Pt();
	      else
		weight_1 = 0.0513+(6.29e-5)*badElectrons[0].Pt();
	    }

	    
	    if (fabs(badElectrons[1].Eta())<1.4442){
	      if (badElectrons[1].Pt() < 61.9)
		weight_2 = 0.0628-0.000803*badElectrons[1].Pt();
	      else if (badElectrons[1].Pt() < 152.2)
		weight_2 = 0.0185-(8.84e-5)*badElectrons[1].Pt();
	      else if (badElectrons[1].Pt() < 408.1)
		weight_2 = 0.00505-(6.51e-8)*badElectrons[1].Pt();
	      else
		weight_2 = 0.00181+(7.88e-6)*badElectrons[1].Pt();
	    }

	    else if (fabs(badElectrons[1].Eta())<2.0){
	      if (badElectrons[1].Pt() < 78.5)
		weight_2 = 0.0955-0.000719*badElectrons[1].Pt();
	      else if (badElectrons[1].Pt() < 226.8)
		weight_2 = 0.0394-(4.06e-6)*badElectrons[1].Pt();
	      else if (badElectrons[1].Pt() < 345.4)
		weight_2 = 0.0229+(6.86e-6)*badElectrons[1].Pt();
	      else
		weight_2 = 0.0466;
	    }

	    else if (fabs(badElectrons[1].Eta())<2.4){
	      if (badElectrons[1].Pt() < 57.3)
		weight_2 = 0.1-0.000789*badElectrons[1].Pt();
	      else
		weight_2 = 0.0513+(6.29e-5)*badElectrons[1].Pt();
	    }

	    
	    h_wrmass4->Fill((badElectrons[0]+badElectrons[1]+selJets[0]+selJets[1]).M(), weight_1/(1-weight_1)*weight_2/(1-weight_2)  );
	  }
	}
      }
    }


    
    if(selElectrons.size()<2) continue;
    
    bool pass = true;

    if(selElectrons[0].Pt() < 60 || fabs(selElectrons[0].Eta()) > 2.4) pass = false;
    if(selElectrons[1].Pt() < 53 || fabs(selElectrons[1].Eta()) > 2.4) pass = false;
    
    if(!pass) continue; 
    
    // dilepton_mass = (selElectrons[0]+selElectrons[1]).M();
    // dilepton_pt = (selElectrons[0]+selElectrons[1]).Pt(); 
    // WR_mass = (selElectrons[0]+selElectrons[1]+selJets[0]+selJets[1]).M();

    // lead_lepton_pt = selElectrons[0].Pt();
    // sublead_lepton_pt = selElectrons[1].Pt();
    // lead_lepton_eta = selElectrons[0].Eta();
    // sublead_lepton_eta = selElectrons[1].Eta();
    // lead_lepton_phi = selElectrons[0].Phi();
    // sublead_lepton_phi = selElectrons[1].Phi();
    
    // lead_lepton_bitmap = eleBitmaps[0];
    // sublead_lepton_bitmap = eleBitmaps[1];

    // dR_leadlepton_leadjet = dR_TLV(selElectrons[0],selJets[0]);
    // dR_leadlepton_subleadjet = dR_TLV(selElectrons[0],selJets[1]);
    // dR_subleadlepton_leadjet = dR_TLV(selElectrons[1],selJets[0]);
    // dR_subleadlepton_subleadjet = dR_TLV(selElectrons[1],selJets[1]);

    // lead_jet_pt = selJets[0].Pt();
    // sublead_jet_pt = selJets[1].Pt();
    // lead_jet_eta = selJets[0].Eta();
    // sublead_jet_eta = selJets[1].Eta();
    // lead_jet_phi = selJets[0].Phi();
    // sublead_jet_phi = selJets[1].Phi();

    Float_t xsec = 1.0;
    Float_t xweight = 1.0;
    Float_t nevents = 1.0;  
    
    if(strcmp(datasetName,"DYJets_amcatnlo_pt50_100_v1") == 0){
      xsec = 3.467e+02;
      xweight = 2.7439;
      nevents = 130582589.0;
    }
    if(strcmp(datasetName,"DYJets_amcatnlo_pt50_100_v2") == 0){
      xsec = 3.467e+02;
      xweight = 2.7439;
      nevents = 130582589.0;
    }

    if(strcmp(datasetName,"DYJets_amcatnlo_pt100_250_v1") == 0){      
      xsec = 8.13e+01;
      xweight = 2.803702;
      nevents = 7983223.0;
    }
    if(strcmp(datasetName,"DYJets_amcatnlo_pt100_250_v2") == 0){      
      xsec = 8.13e+01;
      xweight = 2.803702;
      nevents = 7983223.0;
    }
    if(strcmp(datasetName,"DYJets_amcatnlo_pt100_250_v3") == 0){      
      xsec = 8.13e+01;
      xweight = 2.803702;
      nevents = 7983223.0;
    }
    
    else if(strcmp(datasetName,"DYJets_amcatnlo_pt250_400_v1") == 0){      
      xsec = 2.98e+00;
      xweight = 2.714562;
      nevents = 1609099.0;
    }
    else if(strcmp(datasetName,"DYJets_amcatnlo_pt250_400_v2") == 0){      
      xsec = 2.98e+00;
      xweight = 2.714562;
      nevents = 1609099.0;
    }
    if(strcmp(datasetName,"DYJets_amcatnlo_pt250_400_v3") == 0){      
      xsec = 2.98e+00;
      xweight = 2.714562;
      nevents = 1609099.0;
    }

    if(strcmp(datasetName,"DYJets_amcatnlo_pt400_650_v1") == 0){      
      xsec = 3.835e-01;
      xweight = 2.590890;
      nevents = 1625936.0;
    }
    if(strcmp(datasetName,"DYJets_amcatnlo_pt400_650_v2") == 0){      
      xsec = 3.835e-01;
      xweight = 2.590890;
      nevents = 1625936.0;
    }
    if(strcmp(datasetName,"DYJets_amcatnlo_pt400_650_v3") == 0){      
      xsec = 3.835e-01;
      xweight = 2.590890;
      nevents = 1625936.0;
    }
    if(strcmp(datasetName,"DYJets_amcatnlo_pt650_Inf_v1") == 0){      
      xsec = 3.556e-02;
      xweight = 2.433715;
      nevents = 1627882.0;
    }
    if(strcmp(datasetName,"DYJets_amcatnlo_pt650_Inf_v2") == 0){      
      xsec = 3.556e-02;
      xweight = 2.433715;
      nevents = 1627882.0;
    }
    if(strcmp(datasetName,"DYJets_amcatnlo_pt650_Inf_v3") == 0){      
      xsec = 3.556e-02;
      xweight = 2.433715;
      nevents = 1627882.0;
    }
    if(strcmp(datasetName,"TTJets_DiLept_amcatnlo") == 0){   
      xsec = 7.795e+01;
      xweight = 3.012935;
      nevents = 14529280.0;
    }
    if(strcmp(datasetName,"SingleTop_TTWLL") == 0){
      xsec = 7.795e+01;
      xweight = 3.012935;
      nevents = 14529280.0;
    }
    if(strcmp(datasetName,"SingleTop_TWNuNu") == 0){
      xsec = 7.795e+01;
      xweight = 3.012935;
      nevents = 14529280.0;
    }
    if(strcmp(datasetName,"SingleTop_tbarinc") == 0){
      xsec = 7.795e+01;
      xweight = 3.012935;
      nevents = 14529280.0;
    }
    if(strcmp(datasetName,"SingleTop_tinc") == 0){
      xsec = 7.795e+01;
      xweight = 3.012935;
      nevents = 14529280.0;
    }
    if(strcmp(datasetName,"WW") == 0){
      xsec = 6.425e+01;
      xweight = 1.0;
      nevents = 6987124.0;
    }
    if(strcmp(datasetName,"WZ") == 0){
      xsec = 2.344e+01;
      xweight = 1.0;
      nevents = 2995828.0;
    }
    if(strcmp(datasetName,"ZZ") == 0){
      xsec = 1.006e+01;
      xweight = 1.0;
      nevents = 998034.0;
    }
    if(strcmp(datasetName,"WJetsLNu") == 0){
      xsec = 5.020e+04;
      xweight = 1;
      nevents = 57026058.0;
    }
if(strcmp(datasetName,"QCD_EMEnriched_20_30_v1") == 0){
      xsec = 5.520e+06;
      xweight = 1;
      nevents = 9218954;
    }


if(strcmp(datasetName,"QCD_EMEnriched_30_50_v1") == 0){
      xsec = 6.967e+06;
      xweight = 1;
      nevents = 11498579;
    }


if(strcmp(datasetName,"QCD_EMEnriched_30_50_v2") == 0){
      xsec = 6.967e+06;
      xweight = 1;
      nevents = 4.216e+04;
    }


if(strcmp(datasetName,"QCD_EMEnriched_50_80_v1") == 0){
      xsec = 2.156e+06;
      xweight = 1;
      nevents = 45811241;
    }


if(strcmp(datasetName,"QCD_EMEnriched_50_80_v2") == 0){
      xsec = 2.156e+06;
      xweight = 1;
      nevents = 3.296e+04;
    }


if(strcmp(datasetName,"QCD_EMEnriched_80_120_v1") == 0){
      xsec = 4.174e+05;
      xweight = 1;
      nevents = 4.683e+03;
    }


if(strcmp(datasetName,"QCD_EMEnriched_80_120_v2") == 0){
      xsec = 4.174e+05;
      xweight = 1;
      nevents = 77695287;
    }


if(strcmp(datasetName,"QCD_EMEnriched_120_170_v1") == 0){
      xsec = 7.604e+04;
      xweight = 1;
      nevents = 77771316;
    }


if(strcmp(datasetName,"QCD_EMEnriched_120_170_v2") == 0){
      xsec = 7.604e+04;
      xweight = 1;
      nevents = 77771316;
    }


if(strcmp(datasetName,"QCD_EMEnriched_170_300_v1") == 0){
      xsec = 1.878e+04;
      xweight = 1;
      nevents = 1.045e+02;
    }


if(strcmp(datasetName,"QCD_EMEnriched_300_Inf_v1") == 0){
      xsec = 1.215e+03;
      xweight = 1;
      nevents = 2.033e+01;
    }

if(strcmp(datasetName,"QCD_bcToE_15_20") == 0){
      xsec = 2.598e+05;
      xweight = 1;
      nevents = 2685602;
    }


if(strcmp(datasetName,"QCD_bcToE_20_30") == 0){
      xsec = 3.754e+05;
      xweight = 1;
      nevents = 10987947;
    }


if(strcmp(datasetName,"QCD_bcToE_30_80") == 0){
      xsec = 4.114e+05;
      xweight = 1;
      nevents = 15328096;
    }


if(strcmp(datasetName,"QCD_bcToE_80_170") == 0){
      xsec = 4.103e+04;
      xweight = 1;
      nevents = 14976689;
    }


if(strcmp(datasetName,"QCD_bcToE_170_250") == 0){
      xsec = 2.586e+03;
      xweight = 1;
      nevents = 1.199e+01;
    }


if(strcmp(datasetName,"QCD_bcToE_250_Inf") == 0){
      xsec = 7.148e+02;
      xweight = 1;
      nevents = 2.285e+00;
    }


if(strcmp(datasetName,"QCD_MuEnrichedPt15") == 0){
      xsec = 2.688e+05;
      xweight = 1;
      nevents = 22094081;
    }    
    
    // float triggersf = TriggerSF(fabs(lead_lepton_eta), lead_lepton_pt,fabs(sublead_lepton_eta), sublead_lepton_pt,Trigh1,Trigh2,Trigg1,Trigg2).first;
    // float idsf1 = IdSF(fabs(lead_lepton_eta), lead_lepton_pt,IDh1,IDh2).first;
    // float idsf2 = IdSF(fabs(sublead_lepton_eta), sublead_lepton_pt,IDh1,IDh2).first;
    // float isosf1 = IsoSF(fabs(lead_lepton_eta), lead_lepton_pt,ISOh1,ISOh2).first;
    // float isosf2 = IsoSF(fabs(sublead_lepton_eta), sublead_lepton_pt,ISOh1,ISOh2).first;

    //std::cout<<"Factors="<<triggersf<<" "<<idsf1<<" "<<idsf2<<" "<<isosf1<<" "<<idsf2<<std::endl;
    
    if(data)
      nweight = 1.0;
    else    
      nweight = float((weight/fabs(weight)) * xsec * xweight * lumi * pu_weights[int(nPU)])/float(nevents);// * triggersf * idsf1 * idsf2 * isosf1 * isosf2;
      //nweight = float((weight/fabs(weight)) * xsec * xweight * lumi);///float(nevents);

    // tree->Fill();


    if(selElectrons.size()== 2){
      if(eleBitmaps[0] == 0xfff &&  eleBitmaps[1] == 0xfff){
	if(dR_TLV(selElectrons[0],selJets[0]) > 0.4 && dR_TLV(selElectrons[0],selJets[1]) > 0.4 && dR_TLV(selElectrons[1],selJets[0]) > 0.4 && dR_TLV(selElectrons[1],selJets[1]) > 0.4)
	  h_wrmass1->Fill((selElectrons[0]+selElectrons[1]+selJets[0]+selJets[1]).M(),nweight);
      }

      if((eleBitmaps[0] == 0xfff &&  eleBitmaps[1] == 0xe7f) || (eleBitmaps[0] == 0xe7f &&  eleBitmaps[1] == 0xfff))
	h_wrmass2->Fill((selElectrons[0]+selElectrons[1]+selJets[0]+selJets[1]).M(),nweight);

      if((eleBitmaps[0] == 0xe7f &&  eleBitmaps[1] == 0xe7f))
	h_wrmass3->Fill((selElectrons[0]+selElectrons[1]+selJets[0]+selJets[1]).M(),nweight);

    }

    if(selElectrons.size()== 3){
      if(eleBitmaps[0] == 0xfff &&  eleBitmaps[1] == 0xfff){
	if(dR_TLV(selElectrons[0],selJets[0]) > 0.4 && dR_TLV(selElectrons[0],selJets[1]) > 0.4 && dR_TLV(selElectrons[1],selJets[0]) > 0.4 && dR_TLV(selElectrons[1],selJets[1]) > 0.4)
	  h_wrmass1->Fill((selElectrons[0]+selElectrons[1]+selJets[0]+selJets[1]).M(),nweight);
      }

      if((eleBitmaps[0] == 0xfff &&  eleBitmaps[1] == 0xe7f) || (eleBitmaps[0] == 0xe7f &&  eleBitmaps[1] == 0xfff))
	h_wrmass2->Fill((selElectrons[0]+selElectrons[1]+selJets[0]+selJets[1]).M(),nweight);
      if((eleBitmaps[0] == 0xfff &&  eleBitmaps[2] == 0xe7f) || (eleBitmaps[0] == 0xe7f &&  eleBitmaps[2] == 0xfff))
	h_wrmass2->Fill((selElectrons[0]+selElectrons[2]+selJets[0]+selJets[1]).M(),nweight);
      if((eleBitmaps[1] == 0xfff &&  eleBitmaps[2] == 0xe7f) || (eleBitmaps[1] == 0xe7f &&  eleBitmaps[2] == 0xfff))
	h_wrmass2->Fill((selElectrons[1]+selElectrons[2]+selJets[0]+selJets[1]).M(),nweight);
      
      if((eleBitmaps[0] == 0xe7f &&  eleBitmaps[1] == 0xe7f))
	h_wrmass3->Fill((selElectrons[0]+selElectrons[1]+selJets[0]+selJets[1]).M(),nweight);
      if((eleBitmaps[0] == 0xe7f &&  eleBitmaps[2] == 0xe7f))
	h_wrmass3->Fill((selElectrons[0]+selElectrons[2]+selJets[0]+selJets[1]).M(),nweight);
      if((eleBitmaps[1] == 0xe7f &&  eleBitmaps[2] == 0xe7f))
	h_wrmass3->Fill((selElectrons[1]+selElectrons[2]+selJets[0]+selJets[1]).M(),nweight);

    }

    

    h_neles->Fill(selElectrons.size());
    
    if(selElectrons.size() != 2){
      for(auto eb:eleBitmaps)
	h_bitmaps->Fill(eb);
    }

    
  }
  
  ts.Stop();

  ts.Print();

  h_wrmass1->Write();
  h_wrmass2->Write();
  h_wrmass3->Write();
  h_wrmass4->Write();
  h_wrmass5->Write();

  h_neles->Write();
  h_bitmaps->Write();
  
  // tree->Write();
  
  delete chain;

}


void qcdAnalyzer(){
  // Analyzer(0,0,1,0,0);//ttbar
  // Analyzer(0,0,0,1,0);//Wjets
  // Analyzer(0,0,0,0,1);//qcd
  Analyzer(1,0,0,0,0);//data
  // Analyzer(0,1,0,0,0);//dy
 }

// int main(){
//   gROOT->ProcessLine(".L loader.C+");
//   qcdAnalyzer();
// }
