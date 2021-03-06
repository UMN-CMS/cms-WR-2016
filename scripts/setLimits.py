import ExoAnalysis.cmsWR.condorTools as condorTools
import os
import re
import subprocess
import sys

tag, toys = sys.argv[1:]

thisdir = os.getcwd()
#proddir = "/local/cms/user/phansen/limits/"
proddir = "/afs/cern.ch/work/j/jchavesb/limits/" + tag

datacardfolder = "/afs/cern.ch/user/j/jchavesb/CMSSW_8_0_25/src/ExoAnalysis/cmsWR/new_datacards/"
datacards = os.listdir(datacardfolder)
pattern = re.compile("WR(.*)jj_MASS(.*).txt")

configfile = open("configs/2016-v2.conf")
config = dict( [ line.strip().split('=') for line in configfile])


mode = "BayesianToyMC"
job = condorTools.Job(thisdir + "/scripts/batch_run", config["productionTAG"], prodSpace=proddir)
for datacard in datacards:
	m = pattern.match(datacard)
	if not m: continue
	channel, MWR = m.groups()
	if channel not in ["ee","mumu"]: continue
	if not MWR.isdigit(): continue

	print datacard
	datacard_file = datacardfolder + datacard

	jobname = channel + "_" + MWR + "_"
	systematics = True
#TODO: Make hybrid new work for observed
	if "Hybrid" in mode:
		jobid = jobname + "EXPECTED"
		command = "combine -M HybridNew --frequentist --testStat LHC -H ProfileLikelihood -S%d %s -n %s -T 5000 " %(systematics, datacard_file, datacard)
		prefix  = thisdir + "/python/combineTools.py " + jobid
		job.addJob( prefix + " " + command, jobid)
	elif "Bayes" in mode:
		jobid = jobname + "OBSERVED"
		command = "combine -M BayesianToyMC -H ProfileLikelihood -S%d %s -n %s " %(systematics, datacard_file, datacard)
		prefix  = thisdir + "/python/combineTools.py " + jobid
		#job.addJob( prefix + " " + command, jobid)
		jobid = jobname + "EXPECTED"
		command = "combine -M BayesianToyMC -H ProfileLikelihood -S%d %s -n %s --toys %s" %(systematics, datacard_file, datacard, toys)
		prefix  = thisdir + "/python/combineTools.py " + jobid
		job.addJob( prefix + " " + command, jobid)

job.submit(mode = "1nd")
