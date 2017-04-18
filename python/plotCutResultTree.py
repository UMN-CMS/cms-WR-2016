import ROOT
import ExoAnalysis.cmsWR.PlotUtils as plt
import numpy as np

def nminusone(tree,dir, hist_description=[]):
	"""
	hist_description is a list of (name, title, bin, low, high) to initialize
	the histograms
	"""
	# Get List of Cut names by finding branches with _result in name
	names = [name[:name.rfind("_result")] for name in [b.GetName() for b in tree.GetListOfBranches()] if "result" in name]
	if hist_description:
		hists_EB = {n:ROOT.TH1F(n + "EB",t + "EB",b,l,h) for n,t,b,l,h in hist_description}
		hists_EE = {n:ROOT.TH1F(n + "EE",t + "EE",b,l,h) for n,t,b,l,h in hist_description}
		hists_EB_pass = {n:ROOT.TH1F(n + "EB_pass",t + "EB pass",b,l,h) for n,t,b,l,h in hist_description}
		hists_EE_pass = {n:ROOT.TH1F(n + "EE_pass",t + "EE pass",b,l,h) for n,t,b,l,h in hist_description}
		hists_ALL = {n:ROOT.TH1F(n + "ALL",t + "ALL",b,l,h) for n,t,b,l,h in hist_description}
		hists_ALL_pass = {n:ROOT.TH1F(n + "ALL_pass",t + "ALL pass",b,l,h) for n,t,b,l,h in hist_description}
	else:
		hists_EB = {name:ROOT.TH1F(name + "EB",name + "EB",50,0,0) for name in names}
		hists_EE = {name:ROOT.TH1F(name + "EE",name + "EE",50,0,0) for name in names}
		hists_EB_pass = {name:ROOT.TH1F(name + "EB_pass",name + "EB pass",50,0,0) for name in names}
		hists_EE_pass = {name:ROOT.TH1F(name + "EE_pass",name + "EE pass",50,0,0) for name in names}
		hists_ALL = {name:ROOT.TH1F(name + "ALL",name + "ALL",50,0,0) for name in names}
		hists_ALL_pass = {name:ROOT.TH1F(name + "ALL_pass",name + "ALL pass",50,0,0) for name in names}

	for event in tree:
		if event.nele > 1: continue
		results = []
		values = []
		for name in names:
			result = event.__getattr__(name + "_result")
			value  = event.__getattr__(name + "_value")
			results.append(result)
			values.append(value)
		nminus = [all(results[:i] + results[i+1:]) for i in range(len(results))]

		for i in range(len(names)):
			if names[i] in hists_EB and nminus[i]:
				if abs(event.eta) < 1.47: 
					hists = hists_EB
					hists_pass = hists_EB_pass
				else:
					hists = hists_EE
					hists_pass = hists_EE_pass

				hists[names[i]].Fill(values[i])
				hists_ALL[names[i]].Fill(values[i])
				if results[i]:
					hists_pass[names[i]].Fill(values[i])
					hists_ALL_pass[names[i]].Fill(values[i])

	
	c = ROOT.TCanvas("c","c",1600,1200)
	c.SetLogy()
	for h,h_p in zip(hists_EB.values() + hists_EE.values() + hists_ALL.values(),hists_EB_pass.values() + hists_EE_pass.values() + hists_ALL_pass.values()):
		h.SetLineColor(ROOT.kBlack)
		h.SetFillColor(ROOT.kBlack)
		h.SetFillStyle(3005)
		h.Draw()
		c.Update()
		statbox =  h.GetListOfFunctions().FindObject('stats')

		h_p.SetLineColor(ROOT.kRed)
		h_p.SetFillColor(ROOT.kRed)
		h_p.SetFillStyle(3004)
		h_p.Draw("sames")
		c.Update()
		statbox =  h_p.GetListOfFunctions().FindObject('stats')
		statbox.SetTextColor(ROOT.kRed)
		statbox.SetY1NDC(.8 - .12)
		statbox.SetY2NDC(.8)

		c.SaveAs(dir + h.GetName() + ".png")

	eff_EB = {k:hists_EB_pass[k].GetEntries()/float(hists_EB[k].GetEntries()) for k in hists_EB}
	eff_EE = {k:hists_EE_pass[k].GetEntries()/float(hists_EE[k].GetEntries()) for k in hists_EE}
	eff_ALL = {k:hists_ALL_pass[k].GetEntries()/float(hists_ALL[k].GetEntries()) for k in hists_ALL}

	eff_EB_h = ROOT.TH1F("eff_EB","EB N-1 Efficiences",len(eff_EB),0,len(eff_EB))
	eff_EE_h = ROOT.TH1F("eff_EE","EE N-1 Efficiences",len(eff_EE),0,len(eff_EE))
	eff_ALL_h = ROOT.TH1F("eff_ALL","ALL N-1 Efficiences",len(eff_ALL),0,len(eff_ALL))

	bin = 1
	for k in eff_EB:
		print bin,k
		eff_EB_h.SetBinContent(bin, eff_EB[k])
		eff_EB_h.GetXaxis().SetBinLabel(bin,k)
		bin +=1

	bin = 1
	for k in eff_EE:
		eff_EE_h.SetBinContent(bin, eff_EE[k])
		eff_EE_h.GetXaxis().SetBinLabel(bin,k)
		bin +=1

	bin = 1
	for k in eff_ALL:
		eff_ALL_h.SetBinContent(bin, eff_ALL[k])
		eff_ALL_h.GetXaxis().SetBinLabel(bin,k)
		bin +=1

	plt.drawMultipleSame([eff_EB_h,eff_EE_h, eff_ALL_h],["EB","EE","ALL"],dir + "NminusOne.png", 
								colors=[ROOT.kBlue,ROOT.kRed,ROOT.kBlack], 
								width = 1600, height = 1200, 
								xtitle = "", ytitle = "Efficiency", leg="top",
								logy=False, ymin=.9, ymax=1.05)

if __name__ == "__main__":
	plt.customROOTstyle()
	ROOT.gStyle.SetOptStat(110010)
	hists = []
	
	hists.append(("MinPtCut_0","MinPtCut",50,0,300))
	hists.append(("GsfEleSCEtaMultiRangeCut_0","GsfEleSCEtaMultiRangeCut",50,0,3.0))
	hists.append(("GsfEleDEtaInSeedCut_0","GsfEleDEtaInSeedCut",50,0,.025))
	hists.append(("GsfEleDPhiInCut_0","GsfEleDPhiInCut",50,0,0.15))
	hists.append(("GsfEleFull5x5SigmaIEtaIEtaCut_0","GsfEleFull5x5SigmaIEtaIEtaCut",50,0,0.05))
	hists.append(("GsfEleFull5x5E2x5OverE5x5Cut_0","GsfEleFull5x5E2x5OverE5x5Cut",50,1,2.5))
	hists.append(("GsfEleHadronicOverEMLinearCut_0","GsfEleHadronicOverEMLinearCut",50,0,50))
	hists.append(("GsfEleTrkPtIsoCut_0","GsfEleTrkPtIsoCut",50,0,10))
	hists.append(("GsfEleEmHadD1IsoRhoCut_0","GsfEleEmHadD1IsoRhoCut",50,0,50))
	hists.append(("GsfEleDxyCut_0","GsfEleDxyCut",100,0,.1))
	hists.append(("GsfEleMissingHitsCut_0","GsfEleMissingHitsCut",8,0,8))
	hists.append(("GsfEleEcalDrivenCut_0","GsfEleEcalDrivenCut",2,0,2))

	file = ROOT.TFile.Open("signal.root")
	tree = file.Get("cutflowresult/CutFlowResultTree")
	dir = "plots/Signal0800_0400/"
	nminusone(tree,dir, hist_description = hists)

#	file = ROOT.TFile.Open("data.root")
#	tree = file.Get("ntupler/CutFlowResultTree")
#	dir = "plots/data/"
#	nminusone(tree,dir, hist_description = hists)
#
#	file = ROOT.TFile.Open("signalMC.root")
#	tree = file.Get("ntupler/CutFlowResultTree")
#	dir = "plots/signalMC/"
#	nminusone(tree,dir, hist_description = hists)
#
#	file = ROOT.TFile.Open("TTbar.root")
#	tree = file.Get("ntupler/CutFlowResultTree")
#	dir = "plots/TTbar/"
#	nminusone(tree,dir, hist_description = hists)
#
#	file = ROOT.TFile.Open("QCD.root")
#	tree = file.Get("ntupler/CutFlowResultTree")
#	dir = "plots/QCD/"
#	nminusone(tree,dir, hist_description = hists)
	
#	file = ROOT.TFile.Open("DYJets.root")
#	tree = file.Get("ntupler/CutFlowResultTree")
#	dir = "plots/DYJetsToLL/"
#	nminusone(tree,dir, hist_description = hists)
