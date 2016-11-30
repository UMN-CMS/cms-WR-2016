import ExoAnalysis.cmsWR.combineTools as combineTools
import ExoAnalysis.cmsWR.cross_sections as xs 

import argparse

parser = argparse.ArgumentParser(description='Make datacards')
parser.add_argument('--no-syst', dest='nosyst', action='store_true',
  help='do not write systematics to datacards')
parser.add_argument('--draw-plots', dest='drawplots', action='store_true',
  help='draw plots')
parser.add_argument('-d', '--dir', dest='basedir',
  default="./",
  help='base dir for analysis tree files')
parser.add_argument('-t', '--tag', dest='tag',
  default="",
  help='tag name for analysis tree files')
parser.add_argument('-o', '--outdir', dest='outdir',
  default="datacards/",
  help='where to store the datacards')
parser.add_argument('-s', dest='scale', action='store_true',
  help='if present, rescale signal to .001 fb XS')


args = parser.parse_args()

minitrees = combineTools.AnalysisResultsInterface(
   base=args.basedir,
   tag =args.tag,
   makeplots=args.drawplots
   )

nuisance_params = []
nuisance_params.append(("lumi",        "lnN"))
nuisance_params.append(("TT_SF",       "lnN"))
nuisance_params.append(("DYAMC_SF",       "lnN"))
nuisance_params.append(("signal_unc",  "gmN"))
nuisance_params.append(("TT_unc",      "gmN"))
nuisance_params.append(("DYAMC_unc",   "gmN"))
for channel in ["ee", "mumu"]:
 if channel == "mumu":
     reduced_masses = {0: (600, 6500), 3200: (1950, 3770), 800: (600, 1020), 1600: (1050, 1860), 4000: (2280, 4750), 2400: (1510, 2770), 6000: (3420, 6480)}
 else:
     reduced_masses = {0: (600, 6500), 3200: (2060, 3470), 800: (600, 1010), 1600: (1060, 1840), 4000: (2550, 4300), 2400: (1600, 2690), 6000: (3600, 6300)}
 sig_name = "WR_" + channel + "jj"
 MWR = []
 signal = []
 bg = []
 systematics_list = []
 for mass in sorted(reduced_masses):
  try:
   systematics = combineTools.Systematics(["signal", "TT", "DYMADHT"], nuisance_params)
   if args.scale:
    scale =  .001/xs.WR_jj[channel][mass]
   else:
    scale = 1.0
   signalNevents = minitrees.getNEvents(mass, channel, "signal", systematics, scale = scale)

   TTBar = minitrees.getNEvents(mass, channel, "TT", systematics)
   DY = minitrees.getNEvents(mass, channel, "DYMADHT", systematics)

   MWR.append(mass)
   signal.append(signalNevents)
   bg.append([TTBar, DY])

   if args.nosyst: systematics = None
   systematics_list.append(systematics)
  except (IOError,KeyError,TypeError) as e:
   print mass, "not found"
   print e

 bg_names = ["TTBar", "DY"]

 for i in range(len(MWR)):
  print channel, MWR[i], signal[i]/sum(bg[i])
  signal_tuple = (sig_name, signal[i])
  bg_tuples = zip(bg_names, bg[i])
  nBG = sum(bg[i])

  datacard = "WR%sjj_MASS%04d" % (channel, MWR[i])
  datacard_file = args.outdir + "/" + datacard + ".txt"
  sig, bgs = combineTools.makeDataCardSingleBin(datacard_file, channel + "jj", nBG,
    signal_tuple, bg_tuples, systematics=systematics_list[i])
