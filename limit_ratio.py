from array import array
import ROOT,math

x = array('d')
y = array('d')
z = array('d')

for m in range(1,27) :
    mass = str(800+200*m)
    if mass == '2400':
        continue
    if mass == '3400':
        continue
    if mass == '5400':
        continue
    x.append(int(mass))
    f = open('/afs/cern.ch/work/s/skalafut/public/WR_starting2015/limitSetting/700toysAprilTwentyThree/_SHv19/_SHv19ee_'+mass+'_EXPECTED.log')
    ## Systematics OFF
    #f = open('/afs/cern.ch/user/j/jchavesb/work/limits/200_TOYS_2015_ns/_WRv07/_WRv07mumu_'+mass+'_EXPECTED.log')
    f_dc = open('/afs/cern.ch/work/s/skalafut/public/WR_starting2015/forJorge2015BackgroundEstimates/observedLimitDatacards/WRmumujj_MASS'+mass+'.txt')
    #g = open('/afs/cern.ch/user/j/jchavesb/work/limits/200_TOYS_UNC2_ns/_WRv07/_WRv07mumu_'+mass+'_EXPECTED.log')
    g_dc = open('/afs/cern.ch/user/j/jchavesb/CMSSW_8_0_25/src/ExoAnalysis/cmsWR/minlim_dc_new/WRmumujj_MASS'+mass+'.txt')

    #f = open('/afs/cern.ch/user/j/jchavesb/work/limits/100_TOYS_2015/_WRv06/_WRv06mumu_'+mass+'_EXPECTED.log')
    #g = open('/afs/cern.ch/user/j/jchavesb/work/limits/100_TOYS/_WRv06/_WRv06ee_'+mass+'_EXPECTED.log')
    ##g = open('/afs/cern.ch/user/j/jchavesb/work/limits/100_TOYS_2016/_WRv06/_WRv06mumu_'+mass+'_EXPECTED.log')
    ## Systematics ON
    #f = open('/afs/cern.ch/user/j/jchavesb/work/limits/100_TOYS_2015_syst/_WRv06/_WRv06mumu_'+mass+'_EXPECTED.log')
    #g = open('/afs/cern.ch/user/j/jchavesb/work/limits/100_TOYS_Punzi_syst/_WRv06/_WRv06mumu_'+mass+'_EXPECTED.log')
    g = open('/afs/cern.ch/user/j/jchavesb/work/limits/200_TOYS_UNC2/_WRv07/_WRv07ee_'+mass+'_EXPECTED.log')

    o = 0
    signal_2015 = 0
    signal_2016 = 0
    
    for line in f_dc:
        if 'rate' in line:
            signal_2015 = float(line.split(' ')[2])

    for line in g_dc:
        if 'rate' in line:
            signal_2016 = float(line.split(' ')[2])

            
    for line in f:
        if 'COMBINE' in line:
            #print mass,line.split('(')[2].replace(',','')
            y.append(float(line.split('(')[2].replace(',','')))
            o = float(line.split('(')[2].replace(',',''))
    for line in g:
        if 'COMBINE' in line:
            #print mass, line.split('(')[2].replace(',','')
            z.append(3.72*float(line.split('(')[2].replace(',',''))/o)
            #z.append(float(line.split('(')[2].replace(',',''))*signal_2016/(o*signal_2015)/3.72)

gr = ROOT.TGraph(23,x,y);
gr2 = ROOT.TGraph(23,x,z);

mycanvas = ROOT.TCanvas( "mycanvas", "", 0, 0, 800, 600 ) ;

gr2.GetYaxis().SetTitleOffset(1.45);

gr2.SetTitle("Limit ratio;Mass [GeV];2016 limits/2015 limits")
gr2.Draw("AC*")
#gr2.SetLineColor(ROOT.kRed)
f1 = ROOT.TF1("f1","0.07",0,6000)
f1 = ROOT.TF1("f1","1.0",0,6000)
f1.Draw("same")
f2 = ROOT.TF1("f2","0.27",0,6000)
#f2.Draw("same")

mg = ROOT.TMultiGraph()
mg.Add(gr)
mg.Add(gr2)

#mycanvas.Print('ratio_lim_mumu.pdf')

#mg.Draw("AC*")

