import ROOT

# systs = ['WRv07_elec_reco_id_trig', 'WRv07_elec_res', 'WRv07_elec_scale', 'WRv07_jet_res', 'WRv07_jet_scale']
# for s in systs:
#     print s
#     f = ROOT.TFile('/eos/cms/store/user/aevans/WR-2016-LimitCalculations/'+s+'/selected_tree_WRtoEEJJ_2200_1100_signal_eeEE.root')
#     t = f.Get('syst_tree')
#     h = ROOT.TH1F()
#     t.Draw('NEventsInRange[11]','','goff')
#     h = ROOT.gDirectory.Get("htemp")
#     print h.GetMean(),100*h.GetStdDev()/h.GetMean()

# systs = ['Smear_Muon_ID_Iso_Trig', 'Smear_Muon_Resolution', 'Smear_Muon_Scale']
# for s in systs:
#     print s
#     f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/WRv07/selected_tree_WRtoMuMuJJ_2200_1100_signal_mumuMuMu.root')
#     # f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/WRv07/selected_tree_DYAMCPT_signal_mumuMuMu.root')
#     t = f.Get('syst_tree')
#     h = ROOT.TH1F()
#     t.Draw('NEventsInRange[11]','','goff')
#     # t.Draw('NEventsInRange[8]','','goff')
#     h = ROOT.gDirectory.Get("htemp")
#     print h.GetMean(),100*h.GetStdDev()/h.GetMean()


# for index in range(1, 27):
# 	mass = 800 + ((index-1) * 200)
# 	print mass

# 	systs = ['jet_res', 'jet_scale', 'elec_res', 'elec_scale', 'elec_reco_id_trig'] 
# 	for s in systs:
# 	    f = ROOT.TFile('/eos/cms/store/user/aevans/WR-2016-LimitCalculations/WRv07_'+s+'/selected_tree_WRtoEEJJ_'+str(mass)+'_'+str(mass/2)+'_signal_eeEE.root')
# 	    # f = ROOT.TFile('/afs/cern.ch/user/j/jchavesb/work/WRv07_'+s+'/selected_tree_DYAMCPT_signal_eeEE.root')
# 	    t = f.Get('syst_tree')
# 	    h = ROOT.TH1F()
# 	    t.Draw('NEventsInRange[index]','','goff') 
# 	    h = ROOT.gDirectory.Get("htemp")
# 	    print s, " = ", 100*h.GetStdDev()/h.GetMean()

# 	systs = ['Smear_Muon_ID_Iso_Trig', 'Smear_Muon_Resolution', 'Smear_Muon_Scale']
# 	for s in systs:
# 		f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/WRv07/selected_tree_WRtoMuMuJJ_'+str(mass)+'_'+str(mass/2)+'_signal_mumuMuMu.root')
# 	    # f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/WRv07/selected_tree_DYAMCPT_signal_mumuMuMu.root')
# 	    t = f.Get('syst_tree')
# 	    h = ROOT.TH1F()
# 	    t.Draw('NEventsInRange[index]','','goff')
# 	    h = ROOT.gDirectory.Get("htemp")
# 	    print s, " = ", 100*h.GetStdDev()/h.GetMean()


DY_pdf_ee = [1.0893530087817573, 1.0892126712597168, 1.0973998725171585, 1.0953847977789655, 1.0927640018130709, 1.0983982172166515, 1.1021031438101079, 1.105884764692278, 1.1067715217841758, 1.1063922504537698, 1.1047001761986774, 1.1140268668676494, 1.1063470203908117, 1.1082864789778444, 1.1126397979288316, 1.1162847727782781, 1.1078129139720647, 1.1100529103885586, 1.1151378514383867, 1.117408703827643, 1.1417526638490141, 1.1442480820991991, 1.1442480820991991, 1.1604304896536641, 1.1442480820991991, 1.1442480820991991, 1.1417526638490141]
DY_fact_ee = [1.0699778262709547, 1.0764610705716682, 1.0756526995868501, 1.0856972428834162, 1.0829951001161633, 1.0804150575364713, 1.0823402842670136, 1.0696689771025518, 1.079788241127478, 1.0854550657826727, 1.068409750819973, 1.0432093079412179, 1.0477725215870295, 1.0765132281219552, 1.1093697009222314, 1.1581510806701023, 1.1586799389905189, 1.1643048427749265, 1.169937711074345, 1.1771217380202175, 1.115198810846405, 1.1284401650889635, 1.1284401650889635, 1.1292117031996685, 1.1284401650889635, 1.1284401650889635, 1.115198810846405]
DY_pdf_mumu = [1.0096261370916695, 1.0092216804795999, 1.0095723678995385, 1.0100791083317244, 1.0122078938905341, 1.0132785546841292, 1.0140623890269926, 1.0149964989330509, 1.0194360626751791, 1.0213084120553544, 1.0266670991267361, 1.0273150163756253, 1.0281297140778443, 1.0356278497474951, 1.0285966951856311, 1.0441316607809439, 1.0517900371314755, 1.0732166944809101, 1.1115462044202087, 1.6354028909918492, 1.9468591735052336, 2.316460031328848, 1.7220948488351153, 1.3219630085231109, 1.5056580378756377, 1.5056580378756377, 1.9358274841292848]
DY_fact_mumu = [1.0853667177186932, 1.0806780023152645, 1.0799286030084718, 1.0920318710894521, 1.0812357990482453, 1.0958298003101439, 1.0803595413691292, 1.0852661749477079, 1.0803211722753887, 1.0648891496238537, 1.0807006882602244, 1.0879730969782695, 1.0772149699705331, 1.0818163312185782, 1.0713741190911772, 1.0340852651335657, 1.0655396350760176, 1.0540386106936925, 1.0983040103435739, 2.3647106780265279, 2.463021900757948, 4.5792853520633248, 2.3780646604573747, 1.8455740473031024, 2.0714655032844695, 2.0714655032844695, 2.4672271210715122]

print "ee jet_res jet_scale elec_res elec_scale elec_reco_id_trig DY_pdf DY_fact-renorm"

for index in range(1, 28):
	mass = 800 + ((index-1) * 200)
	# print mass

	systUnc = []
	systs = ['jet_res', 'jet_scale', 'elec_res', 'elec_scale', 'elec_reco_id_trig'] 
	for s in systs:
		# f = ROOT.TFile('/eos/cms/store/user/aevans/WR-2016-LimitCalculations/WRv07_'+s+'/selected_tree_WRtoEEJJ_'+str(mass)+'_'+str(mass/2)+'_signal_eeEE.root')
		f = ROOT.TFile('/afs/cern.ch/user/j/jchavesb/work/WRv07_'+s+'/selected_tree_DYAMCPT_signal_eeEE.root')
		# f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/selected_tree_data_flavoursidebandEMu_EE.root') 
		# f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/selected_tree_Other_signal_eeEE.root')
		t = f.Get('syst_tree')
		h = ROOT.TH1F()
		t.Draw('NEventsInRange[index]','','goff') 
		h = ROOT.gDirectory.Get("htemp")
		if h.GetMean()!= 0:
			systUnc.append(100*h.GetStdDev()/h.GetMean())
		else: 
			systUnc.append(0)
		# print s, " = ", 100*h.GetStdDev()/h.GetMean()
	# print "ee ", mass, systUnc
	print mass, systUnc, (DY_pdf_ee[index-1]-1)*100, (DY_fact_ee[index-1]-1)*100


print " "
print "mumu jet_res jet_scale Smear_Muon_Resolution Smear_Muon_Scale Smear_Muon_ID_Iso_Trig  DY_pdf DY_fact-renorm"

for index in range(1, 28):
	mass = 800 + ((index-1) * 200)
	
	systUnc = []
	systs = ['jet_res', 'jet_scale'] 
	for s in systs:
		# f = ROOT.TFile('/eos/cms/store/user/aevans/WR-2016-LimitCalculations/WRv07_'+s+'/selected_tree_WRtoMuMuJJ_'+str(mass)+'_'+str(mass/2)+'_signal_mumuMuMu.root')
		f = ROOT.TFile('/afs/cern.ch/user/j/jchavesb/work/WRv07_'+s+'/selected_tree_DYAMCPT_signal_mumuMuMu.root')
		# f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/selected_tree_data_flavoursidebandEMu_MuMu.root')
		# f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/selected_tree_Other_signal_mumuMuMu.root')
		t = f.Get('syst_tree')
		h = ROOT.TH1F()
		t.Draw('NEventsInRange[index]','','goff') 
		h = ROOT.gDirectory.Get("htemp")
		if h.GetMean()!= 0:
			systUnc.append(100*h.GetStdDev()/h.GetMean())
		else: 
			systUnc.append(0)
		# print s, " = ", 100*h.GetStdDev()/h.GetMean()

	systs = ['Smear_Muon_Resolution', 'Smear_Muon_Scale', 'Smear_Muon_ID_Iso_Trig']
	for s in systs:
		# f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/WRv07/selected_tree_WRtoMuMuJJ_'+str(mass)+'_'+str(mass/2)+'_signal_mumuMuMu.root')
		f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/WRv07/selected_tree_DYAMCPT_signal_mumuMuMu.root')
		# f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/WRv07/selected_tree_data_flavoursidebandEMu_MuMu.root')
		# f = ROOT.TFile('/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/Systematics_signalRegion/'+s+'/WRv07/selected_tree_Other_signal_mumuMuMu.root')
		t = f.Get('syst_tree')
		h = ROOT.TH1F()
		t.Draw('NEventsInRange[index]','','goff')
		h = ROOT.gDirectory.Get("htemp")
		if h.GetMean()!= 0:
			systUnc.append(100*h.GetStdDev()/h.GetMean())
		else: 
			systUnc.append(0)
		# print s, " = ", 100*h.GetStdDev()/h.GetMean()
	# print "mumu ", mass, systUnc
	print mass, systUnc, (DY_pdf_mumu[index-1]-1)*100, (DY_fact_mumu[index-1]-1)*100


