import subprocess
import ExoAnalysis.cmsWR.combineTools as combineTools
import numpy as np
import math

###########
#for this script to work, the file python/combineTools.py must define functions named getBranchMean() and getBranchStdDev()
#that return the mean and RMS of a TTree branch
#if these functions are not defined, add the following lines of code to python/combineTools.py

#def getBranchMean(fileName, treeName, branchName):
#	r.gROOT.SetBatch(True)
#	infile = r.TFile.Open(fileName)
#	tree = infile.Get(treeName)
#	r.gROOT.cd()
#	tree.Draw(branchName + ">>tempHist()")
#	h = r.gROOT.FindObject("tempHist")
#	return h.GetMean(1)
###end getBranchMean
#
#def getBranchStdDev(fileName, treeName, branchName):
#	r.gROOT.SetBatch(True)
#	infile = r.TFile.Open(fileName)
#	tree = infile.Get(treeName)
#	r.gROOT.cd()
#	tree.Draw(branchName + ">>tempHist()")
#	h = r.gROOT.FindObject("tempHist")
#	return h.GetStdDev(1)
###end getBranchStdDev

##########
	
#put this script in the scripts/ directory
#run this script from one directory above scripts/, and pipe the output to a .tex file using the '>' operator

#change fileDir to point to the directory from which expected bkgnd and signal should be read
#update the input root file names to read the correct files
#update the top SFs and uncertainty magnitudes
treeName="syst_tree"
# fileDir="limits_fixed/WRv07/"
# fileDir="newLimits/WRv07/"
fileDir="/eos/cms/store/group/phys_exotica/leptonsPlusJets/WR/limits_unblinding_5000toys/WRv07/"

#input root files
expTopMuMuFile = "selected_tree_data_flavoursidebandEMuMuMu.root"
expDyMuMuFile = "selected_tree_DYAMCPT_signal_mumuMuMu.root"
expOtherMuMuFile = "selected_tree_Other_signal_mumuMuMu.root"

expTopEEFile = "selected_tree_data_flavoursidebandEMuEE.root"
expDyEEFile = "selected_tree_DYAMCPT_signal_eeEE.root"
expOtherEEFile = "selected_tree_Other_signal_eeEE.root"
#the input WR root files are declared below, within the loop over entries in the mass_cuts.txt file which defines the mass window lower and upper bounds

#top SFs
eMuToEESF = 0.418
eMuToMuMuSF = 0.691

#total uncertainty magnitudes (0.4 is 40 percent) applied to expected signal and backgrounds
#expressed as a fraction of the total number of expected events in a mass window
wrNormUnc = 0.037

dyNormEEUnc = [1.06178,1.05516,1.06346,1.05686,1.07727,1.07198,1.07539,1.09793,1.12256,1.14182,1.18037,1.18536,1.20261,1.22923,1.26532,1.26705,1.33989,1.40681,1.41051,1.54843,1.5483,1.8344,1.8344,1.8327,1.8327,1.8327,1.54712]
dypdfEEUnc = [1.0893530087817573, 1.0892126712597168, 1.0973998725171585, 1.0953847977789655, 1.0927640018130709, 1.0983982172166515, 1.1021031438101079, 1.105884764692278, 1.1067715217841758, 1.1063922504537698, 1.1047001761986774, 1.1140268668676494, 1.1063470203908117, 1.1082864789778444, 1.1126397979288316, 1.1162847727782781, 1.1078129139720647, 1.1100529103885586, 1.1151378514383867, 1.117408703827643, 1.1417526638490141, 1.1442480820991991, 1.1442480820991991, 1.1604304896536641, 1.1442480820991991, 1.1442480820991991, 1.1417526638490141]
dyfactEEUnc = [1.0699778262709547, 1.0764610705716682, 1.0756526995868501, 1.0856972428834162, 1.0829951001161633, 1.0804150575364713, 1.0823402842670136, 1.0696689771025518, 1.079788241127478, 1.0854550657826727, 1.068409750819973, 1.0432093079412179, 1.0477725215870295, 1.0765132281219552, 1.1093697009222314, 1.1581510806701023, 1.1586799389905189, 1.1643048427749265, 1.169937711074345, 1.1771217380202175, 1.115198810846405, 1.1284401650889635, 1.1284401650889635, 1.1292117031996685, 1.1284401650889635, 1.1284401650889635, 1.115198810846405]

dyNormMuMuUnc = [1.03872,1.03841,1.05461,1.04913,1.05961,1.06251,1.06596,1.06341,1.07742,1.11064,1.12368,1.15685,1.19883,1.20536,1.21791,1.18596,1.18609,1.1855,1.27836,1.27491,1.42228,1.39748,1.39748,1.49849,1.49849,1.49849,1.49849]
dypdfMuMuUnc = [1.0096261370916695, 1.0092216804795999, 1.0095723678995385, 1.0100791083317244, 1.0122078938905341, 1.0132785546841292, 1.0140623890269926, 1.0149964989330509, 1.0194360626751791, 1.0213084120553544, 1.0266670991267361, 1.0273150163756253, 1.0281297140778443, 1.0356278497474951, 1.0285966951856311, 1.0441316607809439, 1.0517900371314755, 1.0732166944809101, 1.1115462044202087, 1.6354028909918492, 1.9468591735052336, 2.316460031328848, 1.7220948488351153, 1.3219630085231109, 1.5056580378756377, 1.5056580378756377, 1.9358274841292848]
dyfactMuMuUnc = [1.0853667177186932, 1.0806780023152645, 1.0799286030084718, 1.0920318710894521, 1.0812357990482453, 1.0958298003101439, 1.0803595413691292, 1.0852661749477079, 1.0803211722753887, 1.0648891496238537, 1.0807006882602244, 1.0879730969782695, 1.0772149699705331, 1.0818163312185782, 1.0713741190911772, 1.0340852651335657, 1.0655396350760176, 1.0540386106936925, 1.0983040103435739, 2.3647106780265279, 2.463021900757948, 4.5792853520633248, 2.3780646604573747, 1.8455740473031024, 2.0714655032844695, 2.0714655032844695, 2.4672271210715122]

dyNormUnc = 0.41
topNormEEUnc = 0.067
topNormMuMuUnc = 0.139
otherNormUnc = 0.027

##now make a LaTex table with the following format (copied twice, once for each lepton channel):
## WR mass hypothesis | exp WR evts +/- unc | exp DY evts +/- unc | exp top evts +/- unc | exp bkgnd +/- unc | data |
tex= "\\begin{{tabular}}{{{col}}}\n{table}\\end{{tabular}}"
header = " & \\multicolumn{5}{c|}{Electron channel}  \\\\\n \\MWR (GeV) & Signal (mean $\\pm$ stat $\\pm$ syst) & $Z/\\gamma^*$ (mean $\\pm$ stat $\\pm$ syst) & Top quark (mean $\\pm$ stat $\\pm$ syst) & Other (mean $\\pm$ stat $\\pm$ syst) & All BG (mean $\\pm$ stat $\\pm$ syst) \\\\\\hline\n"
muonHeader = " & \\multicolumn{5}{c|}{Muon channel}  \\\\\n \\MWR (GeV) & Signal (mean $\\pm$ stat $\\pm$ syst) & $Z/\\gamma^*$ (mean $\\pm$ stat $\\pm$ syst) & Top quark (mean $\\pm$ stat $\\pm$ syst) & Other (mean $\\pm$ stat $\\pm$ syst) & All BG (mean $\\pm$ stat $\\pm$ syst) \\\\\\hline\n"
table = {}  #dictionary named table
masses = set()	#unordered sequence which cannot be indexed, and does not record order of insertion or element position
index = 0   #mass window index used to read correct element in an array branch from root file
with open("configs/mass_cuts.txt") as f:
	for line in f:
		if line[0] == "#": continue
		
		#now split the iterator named line into four strings, two of which (low and hi) will not be used
		ch, mass, low, hi = line.split()
		intMass = int(mass)
		if mass == "0":
			index += 1
			continue

		#initialize floating point variables to hold the number of mean expected and observed evts, and uncertainties in a mass window
		numWREvts = 0.0
		wrStatUnc = 0.0
		wrSystUnc = 0.0
		numDYEvts = 0.0
		dyStatUnc = 0.0
		dySystUnc = 0.0
		numTopEvts = 0.0
		topStatUnc = 0.0
		topSystUnc = 0.0
		numOtherEvts = 0.0
		otherStatUnc = 0.0
		otherSystUnc = 0.0
		numExpEvts = 0.0
		expEvtStatUnc = 0.0
		expEvtSystUnc = 0.0
		#based on the string named ch, fill the two num Evts vars declared immediately above
		#the mass_cuts.txt file is ordered from lowest mass to highest mass, so a simple index which increments after each
		#iteration of this loop will allow the correct array element to be read from the input root files
		if ch == 'EE':
			wrEEFile = ("selected_tree_WRtoEEJJ_%i_%i_signal_eeEE.root" % (intMass, intMass/2) )
			numWREvts = combineTools.getBranchMean(fileDir+wrEEFile, treeName, "NEventsInRange["+str(index)+"]")
			wrStatUnc = combineTools.getBranchMean(fileDir+wrEEFile, treeName, "ErrorEventsInRange["+str(index)+"]")
			wrSystUnc = math.sqrt(pow(wrNormUnc*numWREvts, 2) + pow(combineTools.getBranchStdDev(fileDir+wrEEFile, treeName, "NEventsInRange["+str(index)+"]"), 2) )
			numDYEvts = combineTools.getBranchMean(fileDir+expDyEEFile, treeName, "NEventsInRange["+str(index)+"]")
			dyStatUnc = combineTools.getBranchMean(fileDir+expDyEEFile, treeName, "ErrorEventsInRange["+str(index)+"]")
			dySystUnc = math.sqrt(pow((dypdfEEUnc[index-1]-1)*numDYEvts, 2) + pow((dyfactEEUnc[index-1]-1)*numDYEvts, 2) + pow(combineTools.getBranchStdDev(fileDir+expDyEEFile, treeName, "NEventsInRange["+str(index)+"]"), 2) )
			numTopEvts = (eMuToEESF)*(combineTools.getBranchMean(fileDir+expTopEEFile, treeName, "NEventsInRange["+str(index)+"]"))
			topStatUnc = (eMuToEESF)*(combineTools.getBranchMean(fileDir+expTopEEFile, treeName, "ErrorEventsInRange["+str(index)+"]"))
			topSystUnc = math.sqrt( pow(topNormEEUnc*numTopEvts, 2)  + pow((eMuToEESF)*(combineTools.getBranchStdDev(fileDir+expTopEEFile, treeName, "NEventsInRange["+str(index)+"]")), 2) )
			if numTopEvts < 0.1 : topStatUnc = eMuToEESF
			numOtherEvts = combineTools.getBranchMean(fileDir+expOtherEEFile, treeName, "NEventsInRange["+str(index)+"]")
			otherStatUnc = combineTools.getBranchMean(fileDir+expOtherEEFile, treeName, "ErrorEventsInRange["+str(index)+"]")
			otherSystUnc = math.sqrt(pow(otherNormUnc*numOtherEvts, 2) + pow(combineTools.getBranchStdDev(fileDir+expOtherEEFile, treeName, "NEventsInRange["+str(index)+"]"), 2) )
			numExpEvts = numDYEvts + numTopEvts + numOtherEvts
			expEvtStatUnc = math.sqrt(pow(dyStatUnc,2) + pow(topStatUnc,2) + pow(otherStatUnc,2) )
			expEvtSystUnc = math.sqrt(pow(dySystUnc,2) + pow(topSystUnc,2) + pow(otherSystUnc,2) )
		if ch == 'MuMu':
			wrMuMuFile = ("selected_tree_WRtoMuMuJJ_%i_%i_signal_mumuMuMu.root" % (intMass, intMass/2) )
			numWREvts = combineTools.getBranchMean(fileDir+wrMuMuFile, treeName, "NEventsInRange["+str(index)+"]")
			wrStatUnc = combineTools.getBranchMean(fileDir+wrMuMuFile, treeName, "ErrorEventsInRange["+str(index)+"]")
			wrSystUnc = math.sqrt(pow(wrNormUnc*numWREvts, 2) + pow(combineTools.getBranchStdDev(fileDir+wrMuMuFile, treeName, "NEventsInRange["+str(index)+"]"), 2) )
			numDYEvts = combineTools.getBranchMean(fileDir+expDyMuMuFile, treeName, "NEventsInRange["+str(index)+"]")
			dyStatUnc = combineTools.getBranchMean(fileDir+expDyMuMuFile, treeName, "ErrorEventsInRange["+str(index)+"]")
			dySystUnc = math.sqrt(pow((dypdfMuMuUnc[index-1]-1)*numDYEvts, 2) + pow((dyfactMuMuUnc[index-1]-1)*numDYEvts, 2) + pow(combineTools.getBranchStdDev(fileDir+expDyMuMuFile, treeName, "NEventsInRange["+str(index)+"]"), 2) )
			numTopEvts = (eMuToMuMuSF)*(combineTools.getBranchMean(fileDir+expTopMuMuFile, treeName, "NEventsInRange["+str(index)+"]"))
			topStatUnc = (eMuToMuMuSF)*(combineTools.getBranchMean(fileDir+expTopMuMuFile, treeName, "ErrorEventsInRange["+str(index)+"]"))
			topSystUnc = math.sqrt( pow(topNormMuMuUnc*numTopEvts, 2)  + pow((eMuToMuMuSF)*(combineTools.getBranchStdDev(fileDir+expTopMuMuFile, treeName, "NEventsInRange["+str(index)+"]")), 2) )
			if numTopEvts < 0.1 : topStatUnc = eMuToMuMuSF
			numOtherEvts = combineTools.getBranchMean(fileDir+expOtherMuMuFile, treeName, "NEventsInRange["+str(index)+"]")
			otherStatUnc = combineTools.getBranchMean(fileDir+expOtherMuMuFile, treeName, "ErrorEventsInRange["+str(index)+"]")
			otherSystUnc = math.sqrt(pow(otherNormUnc*numOtherEvts, 2) + pow(combineTools.getBranchStdDev(fileDir+expOtherMuMuFile, treeName, "NEventsInRange["+str(index)+"]"), 2) )
			numExpEvts = numDYEvts + numTopEvts + numOtherEvts
			expEvtStatUnc = math.sqrt(pow(dyStatUnc,2) + pow(topStatUnc,2) + pow(otherStatUnc,2) )
			expEvtSystUnc = math.sqrt(pow(dySystUnc,2) + pow(topSystUnc,2) + pow(otherSystUnc,2) )
			

		index += 1
		
		#mass_cuts.txt begins with EE channel, then changes to MuMu channel
		#reset index to 1 when line[1] equals 6000
		if mass == "6000": index = 0
		
		#now add a new key and value to the dictionary. the key is a pair containing the mass and lepton channel, and the value is a string containing the number of expected events
		#the numbers 6 and 7 control how many digits and white space are shown in each dict value
		#round controls the number of decimal points shown
		#these if statements are only used to force a certain number of decimal points to be shown for expected event counts, stat and syst uncertainties
		if mass == "800" or mass == "1000" or mass == "1200" or mass == "1400" or mass == "1600" or mass == "1800" or mass == "2000" or mass == "2200": table[(mass , ch)] = "& {sig:<6} & {dy:<6} & {top:<6} & {other:<6} & {total:<6}".format(sig = (str(round(numWREvts,0))+" $\\pm$ "+str(round(wrStatUnc,1))+" $\\pm$ "+str(round(wrSystUnc,1)) ), dy = (str(round(numDYEvts,2))+" $\\pm$ "+str(round(dyStatUnc,2))+" $\\pm$ "+str(round(dySystUnc,2))), top = (str(round(numTopEvts,2))+" $\\pm$ "+str(round(topStatUnc,2))+" $\\pm$ "+str(round(topSystUnc,2))), other = (str(round(numOtherEvts,2))+" $\\pm$ "+str(round(otherStatUnc,2))+" $\\pm$ "+str(round(otherSystUnc,2))), total = (str(round(numExpEvts,2))+" $\\pm$ "+str(round(expEvtStatUnc,2))+" $\\pm$ "+str(round(expEvtSystUnc,2))) )
		
		if mass == "2400" or mass == "2600" or mass == "2800" or mass == "3000" or mass == "3200" or mass == "3400" or mass == "3600" or mass == "3800" or mass == "4000" or mass == "4200": table[(mass , ch)] = "& {sig:<6} & {dy:<6} & {top:<6} & {other:<6} & {total:<6}".format(sig = (str(round(numWREvts,1))+" $\\pm$ "+str(round(wrStatUnc,2))+" $\\pm$ "+str(round(wrSystUnc,2)) ), dy = (str(round(numDYEvts,2))+" $\\pm$ "+str(round(dyStatUnc,2))+" $\\pm$ "+str(round(dySystUnc,2))), top = (str(round(numTopEvts,2))+" $\\pm$ "+str(round(topStatUnc,2))+" $\\pm$ "+str(round(topSystUnc,2))), other = (str(round(numOtherEvts,2))+" $\\pm$ "+str(round(otherStatUnc,2))+" $\\pm$ "+str(round(otherSystUnc,2))), total = (str(round(numExpEvts,2))+" $\\pm$ "+str(round(expEvtStatUnc,2))+" $\\pm$ "+str(round(expEvtSystUnc,2))) )

		if mass == "4400" or mass == "4600" or mass == "4800" or mass == "5000" or mass == "5200" or mass == "5400" or mass == "5600" or mass == "5800" or mass == "6000": table[(mass , ch)] = "& {sig:<6} & {dy:<6} & {top:<6} & {other:<6} & {total:<6}".format(sig = (str(round(numWREvts,2))+" $\\pm$ "+str(round(wrStatUnc,4))+" $\\pm$ "+str(round(wrSystUnc,4)) ), dy = (str(round(numDYEvts,2))+" $\\pm$ "+str(round(dyStatUnc,2))+" $\\pm$ "+str(round(dySystUnc,2))), top = (str(round(numTopEvts,2))+" $\\pm$ "+str(round(topStatUnc,2))+" $\\pm$ "+str(round(topSystUnc,2))), other = (str(round(numOtherEvts,2))+" $\\pm$ "+str(round(otherStatUnc,2))+" $\\pm$ "+str(round(otherSystUnc,2))), total = (str(round(numExpEvts,2))+" $\\pm$ "+str(round(expEvtStatUnc,2))+" $\\pm$ "+str(round(expEvtSystUnc,2))) )
		
		masses.add(mass)
#end reading mass_cuts.txt

#define the table structure (number of columns and column titles)
#do electron channel first
tex_table = header
for mass in sorted(masses):
	if mass == "0": continue
	tex_table += "{mass:4d} {ee} \\\\ \\hline\n".format(mass=int(mass), ee=table[(mass, "EE")])
	
	
#now do muon channel
tex_table += muonHeader
for mass in sorted(masses):
	if mass == "0": continue
	tex_table += "{mass:4d} {mumu} \\\\ \\hline\n".format(mass=int(mass), mumu=table[(mass, "MuMu")])
	

print tex.format(table=tex_table,   col="|c|c|c|c|c|c|")
