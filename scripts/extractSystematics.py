import ROOT

systs = ['WRv07_elec_reco_id_trig', 'WRv07_elec_res', 'WRv07_elec_scale', 'WRv07_jet_res', 'WRv07_jet_scale']

for s in systs:
    print s
    f = ROOT.TFile('/eos/cms/store/user/aevans/WR-2016-LimitCalculations/'+s+'/selected_tree_WRtoEEJJ_2200_1100_signal_eeEE.root')
    t = f.Get('syst_tree')
    h = ROOT.TH1F()
    t.Draw('NEventsInRange[11]','','goff')

    h = ROOT.gDirectory.Get("htemp")

    print h.GetMean(),100*h.GetStdDev()/h.GetMean()
