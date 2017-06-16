import ExoAnalysis.cmsWR.combineTools as combineTools
import ExoAnalysis.cmsWR.plot as plt
from ExoAnalysis.cmsWR.PlotUtils import ave_tuple
from ExoAnalysis.cmsWR.PlotUtils import ave
import ExoAnalysis.cmsWR.cross_sections as xs 
import os 
import sys
# import pickle

 
def write_new_card(old_dc_filename, outfilename, obs):
	with open(old_dc_filename,'r') as dcf:
		dc = dcf.read()
	start = dc.find("observation")
	end = dc.find("\n",start)
	new_dc = dc.replace(dc[start:end], "observation %d" % obs)
	with open(outfilename,'w') as outf:
		outf.write(new_dc)



configfile = open("configs/2016-v2.conf")  
config = dict( [ line.strip().split('=') for line in configfile])

tag = sys.argv[1] 

# prodSpace = "/afs/cern.ch/user/j/jchavesb/work/limits/" + tag + "/"
prodSpace = "/afs/cern.ch/user/g/gnegro/work/public/WR16/limits/200_TOYS/_WRv07/"

name = ""#config["productionTAG"]
results = []

for log in os.listdir(prodSpace + name):
	if log[-4:] != ".log": continue   
	# print log
	with open(prodSpace + name + "/" + log) as f:
		for line in f:
			if "COMBINE" in line:
				results.append(eval(line.strip()))   

results.sort()

# try:
# 	with open("obs_limit.p","r") as obsp:
# 		print "loading from pickle file"
# 		obs_limits = pickle.load(obsp)
# except IOError:
# 	obs_limits = {"ee":[], "mumu":[]}
# 	for ch in ["ee","mumu"]:
# 		for obs in range(3):
# 			print "calculating Observed = ", obs
# 			jobname =  ch + "obs" + str(obs)
# 			new_dc = "datacards/" + jobname + ".txt"
# 			write_new_card("datacards/WR{ch}jj_MASS5200.txt".format(ch=ch),new_dc, obs)
# 			obs_limits[ch].append( float(combineTools.runCombine("combine -M BayesianToyMC -n " +jobname + " " + new_dc, jobname)))
# 	with open("obs_limit.p","w") as obsp:
# 		pickle.dump(obs_limits, obsp)

plotters = {"ee":plt.limit1d(.001), "mumu":plt.limit1d(.001)}
plotters["ee"].addTheory(xs.WR_jj["ee"])
plotters["mumu"].addTheory(xs.WR_jj["mumu"])

#set x and y ranges in 2D plots such that each bin is 200 GeV wide in x, and 100 GeV tall in y
plotters2d = {
		"ee"  : plt.limit2d("data/offlineEEefficienciesVsMassesWithMassWindowCuts.txt",    "ee2d" , "ee",     (25, 700, 5700), (49, 50, 4950), xs=.001),
		"mumu": plt.limit2d("data/offlineMuMuEfficienciesVsMassesWithMassWindowCuts.txt", "mumu2d", "#mu#mu", (25, 700, 5700), (49, 50, 4950), xs=.001)
		}

plotters2d["ee"].addTheory(xs.WR_jj_offdiagonal["ee"])
plotters2d["mumu"].addTheory(xs.WR_jj_offdiagonal["mumu"])

for res in results: 
	_, m, ret = res
	channel,mass,mode = m.split('_')
	if mode == "EXPECTED":
		# print m, ret
		plotters[channel].add(mass, ret)
		plotters2d[channel].add(mass,ret)
	
	elif mode == "OBSERVED":
		#print m, ret
		plotters[channel].addObserved(mass, ret)
		plotters2d[channel].addObserved(mass, ret)

plotters["ee"].plot("/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/limitsWithNewTtbarSF/limWReejj" + name + tag, x_title = "M_{W_{R}} [GeV]",y_title="#sigma(pp#rightarrowW_{R}#rightarroweejj) (pb)", y_limits = (1e-4,1e-1), leg_y = .58 )
plotters["mumu"].plot("/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/limitsWithNewTtbarSF/limWRmumujj" + name + tag, x_title = "M_{W_{R}} [GeV]",y_title="#sigma(pp#rightarrowW_{R}#rightarrow#mu#mujj) (pb)", y_limits = (1e-4,1e-1), leg_y = .58 )
plotters2d["ee"].plot("/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/limitsWithNewTtbarSF/lim2dWReejj" + name)
plotters2d["mumu"].plot("/afs/cern.ch/user/g/gnegro/www/cmsWR/preApproval/limitsWithNewTtbarSF/lim2dWRmumujj" + name)

