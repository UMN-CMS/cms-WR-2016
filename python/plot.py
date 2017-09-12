
import numpy as np
from ExoAnalysis.cmsWR.PlotUtils import customROOTstyle
import ROOT 
# import CMS_lumi


def CMS_lumi():
	H = ROOT.gPad.GetWh()
	W = ROOT.gPad.GetWw()
	l = ROOT.gPad.GetLeftMargin()
	t = ROOT.gPad.GetTopMargin() 
	r = ROOT.gPad.GetRightMargin()
	b = ROOT.gPad.GetBottomMargin()
	y = 1-t+0.01 

	lumiText = "35.9 fb^{-1} (13 TeV)"
	lumiTextSize = 0.75 #0.6
	lumiTextFont = 42

	cmsText = "CMS"
	cmsTextFont = 61 # default is helvetic-bold
	cmsTextSize = 0.8 #0.65

	extraText = "Preliminary"
	extraTextFont = 52 # default is helvetica-italics
	extraTextSize = 0.75 #0.6

	latex = ROOT.TLatex()
	latex.SetNDC()
	latex.SetTextAngle(0)
	latex.SetTextColor(1)#kBlack)

	latex.SetTextFont(lumiTextFont)
	latex.SetTextAlign(31)
	latex.SetTextSize(lumiTextSize*t)
	latex.DrawLatex(1-r,y,lumiText)

	latex.SetTextFont(cmsTextFont)
	latex.SetTextAlign(11)
	latex.SetTextSize(cmsTextSize*t)
	latex.DrawLatex(l,y,cmsText)	

	latex.SetTextFont(extraTextFont)
	latex.SetTextAlign(11)
	latex.SetTextSize(extraTextSize*t)
	latex.DrawLatex(l + 0.09, y, extraText)	



class limit1d:
	def __init__(self,xs=None):
		self.plus1sig = []
		self.min1sig = []
		self.plus2sig = []
		self.min2sig = []
		self.medians = []
		self.errors = []
		self.masses = []
		self.observed = []
		self.theory = None
		if xs is None:
			xs = 1
		self.xs = xs
	def add(self, mass, point):
		median, (onesig_minus,onesig_plus), (twosig_minus,twosig_plus) = point
		self.masses.append(float(mass))
		self.medians.append(float(median))
		self.plus1sig.append(float(onesig_plus))
		self.min1sig.append(float(onesig_minus))
		self.plus2sig.append(float(twosig_plus))
		self.min2sig.append(float(twosig_minus))
	def addObserved(self, mass, point):
		self.observed.append(float(point))
	def addTheory(self, theory):
		self.theory = theory

	def plot(self, filename, x_title="", y_title="",
			x_limits=(600,6000), y_limits=(1e-3, 1e-1), 
			# x_limits=(2500,4200), y_limits=(1e-3, 1e-1), 
			leg_x = .55, leg_y=.66, SetLogx=False):
		customROOTstyle()
		#ROOT.gROOT.LoadMacro("scripts/tdrStyle.C")
		c1 = ROOT.TCanvas("c1","c1",800,800);
		c1.SetTopMargin(0.05);
		c1.SetLeftMargin(0.15);
		c1.SetRightMargin(0.05);
		c1.SetBottomMargin(0.13);
		c1.SetTicks(1,1);
		c1.SetLogy();
		if SetLogx:
			c1.SetLogx()

		mass_array = np.array(self.masses, dtype=float)
		expected_limit_array = np.array(self.medians, dtype=float)
		expected_limit_error_array = np.array(self.errors, dtype=float)
		onesig_array = np.array(self.plus1sig + self.min1sig[::-1] + [self.plus1sig[0]], dtype=float)
		twosig_array = np.array(self.plus2sig + self.min2sig[::-1] + [self.plus2sig[0]], dtype=float)
		mass_band_array = np.array(self.masses + self.masses[::-1] + [self.masses[0]], dtype=float)

		if self.theory:
			#expected_limit_array = np.array([ l*self.theory[mass] for mass, l in zip(mass_array, expected_limit_array)], dtype=float)
			#onesig_array         = np.array([ l*self.theory[mass] for mass, l in zip(mass_band_array, onesig_array)   ], dtype=float) 
			#twosig_array         = np.array([ l*self.theory[mass] for mass, l in zip(mass_band_array, twosig_array)   ], dtype=float) 
			expected_limit_array *= self.xs
			onesig_array *= self.xs
			twosig_array *= self.xs

		dummy = ROOT.TH1F("dummy","",30,x_limits[0], x_limits[1])
		dummy.SetMinimum(y_limits[0])
		dummy.SetMaximum(y_limits[1])
		dummy.SetStats(0)
		dummy.GetXaxis().SetNdivisions(507)
		dummy.GetXaxis().SetTitle(x_title)
		dummy.GetXaxis().SetTitleSize(0.045)
		dummy.GetXaxis().SetLabelSize(.04)
		dummy.GetYaxis().SetLabelSize(.035)
		dummy.GetYaxis().SetTitleSize(.045)
		dummy.GetYaxis().SetTitleOffset(1.25)
		dummy.GetYaxis().SetTitle(y_title)

		dummy.Draw("HIST")
		leg_w = .44
		leg_h = .21
		leg = ROOT.TLegend(leg_x,leg_y,leg_x + leg_w, leg_y + leg_h)
		latex = ROOT.TLatex(leg_x + 0.02, leg_y + 0.23, "#it{m}_{#scale[1.25]{N_{R}}}= #it{m}_{#scale[1.25]{W_{R}}}/2");
		latex.SetNDC();
		latex.SetTextSize(0.032);
		latex.SetTextFont(42);
		latex.Draw();

		leg.SetTextFont(42);
		leg.SetTextSize(0.032);
		leg.SetFillStyle(0);
		leg.SetBorderSize(0);

		n = len(self.masses)

		g_twosig = ROOT.TGraph(n*2+1, mass_band_array, twosig_array)
		g_twosig.SetFillColor(ROOT.kYellow)
		g_twosig.SetLineWidth(0)
		g_twosig.Draw("F SAME")

		g_onesig = ROOT.TGraph(n*2+1, mass_band_array, onesig_array)
		g_onesig.SetFillColor(ROOT.kGreen)
		g_onesig.SetLineWidth(0)
		g_onesig.Draw("F SAME")

		g_exp = ROOT.TGraph(n,mass_array, expected_limit_array);
		g_exp.SetLineWidth(2);
		g_exp.SetLineColor(ROOT.kBlue); g_exp.SetLineStyle(2);
		g_exp.Draw("L SAME");

		if self.theory:
			ntheory = len(self.theory)
			theory_mass = np.array(sorted(self.theory.keys()), dtype=float)
			theory_limit = np.array( [ self.theory[mass] for mass in theory_mass], dtype=float)
			print theory_limit
			if self.xs == 1:
				g_theory = ROOT.TGraph(ntheory, theory_mass, theory_limit*1000)
			else: 
				g_theory = ROOT.TGraph(ntheory, theory_mass, theory_limit)
			g_theory.SetLineWidth(3);
			g_theory.SetLineColor(ROOT.kRed+2);
			g_theory.SetLineStyle(0);
			g_theory.SetFillStyle(3002);
			g_theory.SetFillColor(ROOT.kRed);
			g_theory.Draw("LSAME")

		if self.observed:
			nobs = len(self.observed)
			observed_limit_array = np.array( self.observed, dtype=float)
			if self.theory:
				#observed_limit_array = np.array([ l*self.theory[mass] for mass, l in zip(mass_array, observed_limit_array)], dtype=float)
				observed_limit_array *= self.xs
			g_obs = ROOT.TGraph(nobs, mass_array, observed_limit_array)
			g_obs.SetLineWidth(3);
			g_obs.SetLineColor(ROOT.kBlue+2);
			g_obs.SetLineStyle(0);
			g_obs.SetFillStyle(3002);
			g_obs.SetFillColor(ROOT.kBlue+2);
			g_obs.Draw("L SAME")
		
		if self.observed:
			leg.AddEntry(g_obs,"Observed limit","L")
		leg.AddEntry(g_exp, "Expected limit","L")
		leg.AddEntry(g_onesig, "Expected #pm 1 #sigma", "F")
		leg.AddEntry(g_twosig, "Expected #pm 2 #sigma", "F")
		if self.theory:
			leg.AddEntry(g_theory,"Theory (g_{R}= g_{L})","L")
		leg.Draw("same")

		# text = ROOT.TText(0.17,0.97,"CMS Preliminary")
		# text.SetNDC();
		# text.SetTextFont(62);
		# text.SetTextSize(0.04);
		# text.Draw()
		# text2 = ROOT.TLatex(0.72,0.97,"35.9 fb^{-1} (13 TeV)")
		# text2.SetNDC();
		# text2.SetTextFont(62);
		# text2.SetTextSize(0.03);
		# text2.Draw("same")
		CMS_lumi()

		c1.RedrawAxis()
		c1.SaveAs(filename + ".png")
		c1.SaveAs(filename + ".pdf")
		c1.SaveAs(filename + ".root")
		c1.SaveAs(filename + ".C")


from collections import defaultdict
class limit2d:
	def __init__(self, ratio_file, name,channelname,  mwr_range=(27 , 800, 6200) , mnu_range = (124, 0, 6200 ), xs=None):
		self.eff = defaultdict(dict)
		self.effratio = ROOT.TH2F(name + "_eff_ratio", name + " eff ratio" , *(mwr_range + mnu_range))
		self.limits = ROOT.TH2F(name + "_limits", name + " limits", *(mwr_range + mnu_range))
		self.r = ROOT.TH2F(name + "_r", name + " r", *(mwr_range + mnu_range))
		self.exclusion = ROOT.TH2F(name + "_exc", name + " exclusion", *(mwr_range + mnu_range))
		self.crosssection = ROOT.TH2F(name + "_xs", name + " xs", *(mwr_range + mnu_range))
		self.channelname = channelname
		#these TH2Fs are made only for the 2D WR mass exclusion plot
		#exclusionTwo is only for the observed mass exclusion limit, the others are for the expected limit plus/minus one sigma
		self.exclusionTwo = ROOT.TH2F(name + "_excTwo", name + " exclusionTwo", *(mwr_range + mnu_range))
		self.exclusionExpMinusOneSigma = ROOT.TH2F(name + "_excExpMinusOneSigma", name + " exclusionExpMinusOneSigma", *(mwr_range + mnu_range))
		self.exclusionExpPlusOneSigma = ROOT.TH2F(name + "_excExpPlusOneSigma", name + " exclusionExpPlusOneSigma", *(mwr_range + mnu_range))
	
		with open(ratio_file,"r") as inf:
			for line in inf:
				line = line.strip()
				if "#" == line[0]: continue
				mwr, mnu, eff = map(float,line.split())
				self.eff[int(mwr)][int(mnu)] = eff
		if xs is None:
			xs = 1
		self.xs = xs

	def addTheory(self, theory):
		self.theory = theory
	
	def addObserved(self, mwr, point):
		#this method is only for the observed 2D limit
		mwr = int(mwr)
		median = float(point)
		#median *= self.xs
		for mnu in self.eff[mwr]:

			ratio = self.eff[mwr][mnu]/self.eff[mwr][mwr/2]
			if np.isnan(ratio): continue
			if ratio == 0:
				ratio = .00001

			limit = median * self.xs / ratio
			self.exclusionTwo.Fill(mwr, mnu, limit/self.theory[(mwr,mnu)])

	def add(self, mwr, point):
		#this method is only for the expected 2D limit
		mwr = int(mwr)
		median, (onesig_minus,onesig_plus), (twosig_minus,twosig_plus) = point
		for mnu in self.eff[mwr]:

			ratio = self.eff[mwr][mnu]/self.eff[mwr][mwr/2]
			if np.isnan(ratio): continue
			if ratio == 0:
				ratio = .00001

			limit = median * self.xs / ratio
			self.effratio.Fill(mwr, mnu, ratio)
			self.limits.Fill(mwr, mnu, limit)
			#self.r.Fill(mwr, mnu, median * self.xs)
			self.exclusion.Fill(mwr, mnu, limit/self.theory[(mwr,mnu)])
			self.crosssection.Fill(mwr, mnu, self.theory[(mwr,mnu)])
			self.exclusionExpMinusOneSigma.Fill(mwr, mnu, limit*onesig_minus/(median*self.theory[(mwr,mnu)]))
			self.exclusionExpPlusOneSigma.Fill(mwr, mnu, limit*onesig_plus/(median*self.theory[(mwr,mnu)]))
	#end add method, only for expected 2D limits


	#draw multiple 2D limit curves on one canvas
	# def drawOverlay(self, hOne, filename, zrange, ztitle, logz=False, contOne=None, hTwo=None, contTwo=None, contExpMinusOneSigma=None, contExpPlusOneSigma=None, hExpMinusOneSigma=None, hExpPlusOneSigma=None):
	def drawOverlay(self, hOne, hTwo, filename, zrange, ztitle, logz=False, contOne=None, contTwo=None, contExpMinusOneSigma=None, contExpPlusOneSigma=None, hExpMinusOneSigma=None, hExpPlusOneSigma=None):
		customROOTstyle()
		ROOT.gStyle.SetOptTitle(0)
		ROOT.gROOT.SetBatch(True)
		# ROOT.gStyle.SetPalette(55)

		number  = 257
		steps = np.array([0., 0.00390625, 0.0078125, 0.01171875, 0.015625, 0.01953125, 0.0234375, 0.02734375, 0.03125, 0.03515625, 0.0390625, 0.04296875, 0.046875, 0.05078125, 0.0546875, 0.05859375, 0.0625, 0.06640625, 0.0703125, 0.07421875, 0.078125, 0.08203125, 0.0859375, 0.08984375, 0.09375, 0.09765625, 0.1015625, 0.10546875, 0.109375, 0.11328125, 0.1171875, 0.12109375, 0.125, 0.12890625, 0.1328125, 0.13671875, 0.140625, 0.14453125, 0.1484375, 0.15234375, 0.15625, 0.16015625, 0.1640625, 0.16796875, 0.171875, 0.17578125, 0.1796875, 0.18359375, 0.1875, 0.19140625, 0.1953125, 0.19921875, 0.203125, 0.20703125, 0.2109375, 0.21484375, 0.21875, 0.22265625, 0.2265625, 0.23046875, 0.234375, 0.23828125, 0.2421875, 0.24609375, 0.25, 0.25390625, 0.2578125, 0.26171875, 0.265625, 0.26953125, 0.2734375, 0.27734375, 0.28125, 0.28515625, 0.2890625, 0.29296875, 0.296875, 0.30078125, 0.3046875, 0.30859375, 0.3125, 0.31640625, 0.3203125, 0.32421875, 0.328125, 0.33203125, 0.3359375, 0.33984375, 0.34375, 0.34765625, 0.3515625, 0.35546875, 0.359375, 0.36328125, 0.3671875, 0.37109375, 0.375, 0.37890625, 0.3828125, 0.38671875, 0.390625, 0.39453125, 0.3984375, 0.40234375, 0.40625, 0.41015625, 0.4140625, 0.41796875, 0.421875, 0.42578125, 0.4296875, 0.43359375, 0.4375, 0.44140625, 0.4453125, 0.44921875, 0.453125, 0.45703125, 0.4609375, 0.46484375, 0.46875, 0.47265625, 0.4765625, 0.48046875, 0.484375, 0.48828125, 0.4921875, 0.49609375, 0.5, 0.50390625, 0.5078125, 0.51171875, 0.515625, 0.51953125, 0.5234375, 0.52734375, 0.53125, 0.53515625, 0.5390625, 0.54296875, 0.546875, 0.55078125, 0.5546875, 0.55859375, 0.5625, 0.56640625, 0.5703125, 0.57421875, 0.578125, 0.58203125, 0.5859375, 0.58984375, 0.59375, 0.59765625, 0.6015625, 0.60546875, 0.609375, 0.61328125, 0.6171875, 0.62109375, 0.625, 0.62890625, 0.6328125, 0.63671875, 0.640625, 0.64453125, 0.6484375, 0.65234375, 0.65625, 0.66015625, 0.6640625, 0.66796875, 0.671875, 0.67578125, 0.6796875, 0.68359375, 0.6875, 0.69140625, 0.6953125, 0.69921875, 0.703125, 0.70703125, 0.7109375, 0.71484375, 0.71875, 0.72265625, 0.7265625, 0.73046875, 0.734375, 0.73828125, 0.7421875, 0.74609375, 0.75, 0.75390625, 0.7578125, 0.76171875, 0.765625, 0.76953125, 0.7734375, 0.77734375, 0.78125, 0.78515625, 0.7890625, 0.79296875, 0.796875, 0.80078125, 0.8046875, 0.80859375, 0.8125, 0.81640625, 0.8203125, 0.82421875, 0.828125, 0.83203125, 0.8359375, 0.83984375, 0.84375, 0.84765625, 0.8515625, 0.85546875, 0.859375, 0.86328125, 0.8671875, 0.87109375, 0.875, 0.87890625, 0.8828125, 0.88671875, 0.890625, 0.89453125, 0.8984375, 0.90234375, 0.90625, 0.91015625, 0.9140625, 0.91796875, 0.921875, 0.92578125, 0.9296875, 0.93359375, 0.9375, 0.94140625, 0.9453125, 0.94921875, 0.953125, 0.95703125, 0.9609375, 0.96484375, 0.96875, 0.97265625, 0.9765625, 0.98046875, 0.984375, 0.98828125, 0.9921875, 0.99609375, 1],dtype=float)
		red = np.array([0.2298057, 0.234299935 ,0.238810063 ,0.243336663 ,0.247880265 ,0.25244136  ,0.257020396 ,0.261617779 ,0.26623388  ,0.270869029 ,0.275523523 ,0.28019762  ,0.284891546 ,0.289605495 ,0.294339624 ,0.299094064 ,0.30386891  ,0.308664231 ,0.313480065 ,0.318316422 ,0.323173283 ,0.328050603 ,0.332948312 ,0.337866311 ,0.342804478 ,0.347762667 ,0.352740705, 0.357738399, 0.362755532, 0.367791863, 0.372847134 ,0.37792106  ,0.38301334  ,0.38812365  ,0.39325165  ,0.398396976 ,0.40355925  ,0.408738074 ,0.413933033 ,0.419143694 ,0.424369608 ,0.429610311 ,0.434865321 ,0.440134144 ,0.445416268 ,0.450711169 ,0.456018308 ,0.461337134 ,0.46666708  ,0.472007569 ,0.477358011 ,0.482717804 ,0.488086336 ,0.493462982 ,0.498847107 ,0.504238066 ,0.509635204 ,0.515037856 ,0.520445349 ,0.525857    ,0.531272118 ,0.536690004 ,0.542109949 ,0.54753124  ,0.552953156 ,0.558374965 ,0.563795935 ,0.569215322 ,0.574632379 ,0.580046354 ,0.585456486 ,0.590862011 ,0.596262162 ,0.601656165 ,0.607043242 ,0.61242261  ,0.617793485 ,0.623155076 ,0.628506592 ,0.633847237 ,0.639176211 ,0.644492714 ,0.649795942 ,0.655085089 ,0.660359348 ,0.665617908 ,0.670859959 ,0.676084688 ,0.681291281 ,0.686478925 ,0.691646803 ,0.696794099 ,0.701919999 ,0.707023684 ,0.712104339 ,0.717161148 ,0.722193294 ,0.727199962 ,0.732180337 ,0.737133606 ,0.742058956 ,0.746955574 ,0.751822652 ,0.756659379 ,0.761464949 ,0.766238556 ,0.770979397 ,0.775686671 ,0.780359577 ,0.78499732  ,0.789599105 ,0.79416414  ,0.798691636 ,0.803180808 ,0.807630872 ,0.812041048 ,0.81641056  ,0.820738635 ,0.825024503 ,0.829267397 ,0.833466556 ,0.837621221 ,0.841730637 ,0.845794055 ,0.849810727 ,0.853779913 ,0.857700874 ,0.861572878 ,0.865395197 ,0.86977749  ,0.874064226 ,0.878255583 ,0.882351728 ,0.886352818 ,0.890259    ,0.89407041  ,0.897787179 ,0.901409427 ,0.904937269 ,0.908370816 ,0.911710171 ,0.914955433 ,0.918106696 ,0.921164054 ,0.924127593 ,0.926997401 ,0.929773562 ,0.932456159 ,0.935045272 ,0.937540984 ,0.939943375 ,0.942252526 ,0.944468518 ,0.946591434 ,0.948621357 ,0.950558373 ,0.952402567 ,0.954154029 ,0.955812849 ,0.957379123 ,0.958852946 ,0.960234418 ,0.961523642 ,0.962720725 ,0.963825777 ,0.964838913 ,0.965760251 ,0.966589914 ,0.96732803  ,0.967974729 ,0.96853015  ,0.968994435 ,0.969367729 ,0.969650186 ,0.969841963 ,0.969943224 ,0.969954137 ,0.969874878 ,0.969705626 ,0.96944657  ,0.969097901 ,0.968659818 ,0.968132528 ,0.967516241 ,0.966811177 ,0.966017559 ,0.965135621 ,0.964165599 ,0.963107739 ,0.961962293 ,0.960729521 ,0.959409687 ,0.958003065 ,0.956509936 ,0.954930586 ,0.95326531  ,0.951514411 ,0.949678196 ,0.947756983 ,0.945751096 ,0.943660866 ,0.941486631 ,0.939228739 ,0.936887543 ,0.934463404 ,0.931956691 ,0.929367782 ,0.92669706  ,0.923944917 ,0.921111753 ,0.918197974 ,0.915203996 ,0.912130241 ,0.908977139 ,0.905745128 ,0.902434654 ,0.89904617  ,0.895580136 ,0.892037022 ,0.888417303 ,0.884721464 ,0.880949996 ,0.877103399 ,0.873182178 ,0.869186849 ,0.865117934 ,0.860975962 ,0.85676147  ,0.852475004 ,0.848117114 ,0.843688361 ,0.839189312 ,0.834620542 ,0.829982631 ,0.82527617  ,0.820501754 ,0.815659988 ,0.810751482 ,0.805776855 ,0.800736732 ,0.795631745 ,0.790462533 ,0.785229744 ,0.779934029 ,0.774576051 ,0.769156474 ,0.763675975 ,0.758135232 ,0.752534934 ,0.746875773 ,0.741158452 ,0.735383675 ,0.729552157 ,0.723664618 ,0.717721782 ,0.711724383, 0.705673158],dtype=float)
		green = np.array([0.298717966, 0.305559204, 0.312388385, 0.319205292, 0.326009656, 0.332801165, 0.339579464, 0.346344164, 0.353094838, 0.359831032, 0.36655226 ,0.373258014, 0.379947761, 0.386620945, 0.393276993, 0.399915313, 0.406535296, 0.413136319, 0.419717745, 0.426278924, 0.432819194, 0.439337884, 0.445834313, 0.45230779 ,0.458757618, 0.465183092, 0.471583499, 0.477958123, 0.484306241, 0.490627125, 0.496920043, 0.503184261, 0.50941904 ,0.515623638, 0.521797312, 0.527939316, 0.534048902, 0.540125323, 0.546167829, 0.552175668, 0.558148092, 0.564084349, 0.56998369 ,0.575845364, 0.581668623, 0.587452719, 0.593196905, 0.598900436, 0.604562568, 0.61018256 ,0.615759672, 0.621293167, 0.626782311, 0.632226371, 0.637624618, 0.642976326, 0.648280772, 0.653537236, 0.658745003, 0.66390336 ,0.669011598, 0.674069012, 0.679074903, 0.684028574, 0.688929332, 0.693776492, 0.698569369, 0.703307287, 0.707989572, 0.712615557, 0.717184578, 0.721695979, 0.726149107, 0.730543315, 0.734877964, 0.739152418, 0.743366047, 0.747518228, 0.751608345, 0.755635786, 0.759599947, 0.763500228, 0.767336039, 0.771106793, 0.774811913, 0.778450826, 0.782022968, 0.78552778 ,0.788964712, 0.792333219, 0.795632765, 0.798862821, 0.802022864, 0.805112381, 0.808130864, 0.811077814, 0.813952739, 0.816755156, 0.81948459 ,0.82214057 ,0.824722639, 0.827230344, 0.829663241, 0.832020895, 0.834302879, 0.836508774, 0.838638169, 0.840690662, 0.842665861, 0.84456338 ,0.846382843, 0.848123884, 0.849786142, 0.85136927 ,0.852872925, 0.854296776, 0.855640499, 0.856903782, 0.85808632 ,0.859187816, 0.860207984, 0.861146547, 0.862003236, 0.862777795, 0.863469972, 0.864079527, 0.864606232, 0.865049863, 0.86541021 ,0.863633958, 0.861776352, 0.859837644, 0.857818097, 0.85571798 ,0.853537573, 0.851277164, 0.848937047, 0.846517528, 0.844018919, 0.841441541, 0.838785722, 0.836051799, 0.833240115, 0.830351023, 0.827384882, 0.824342058, 0.821222926, 0.818027865, 0.814757264, 0.811411517, 0.807991025, 0.804496196, 0.800927443, 0.797285187, 0.793569853, 0.789781872, 0.785921682, 0.781989725, 0.777986449, 0.773912305, 0.769767752, 0.765553251, 0.761269267, 0.756916272, 0.752494738, 0.748005143, 0.743447967, 0.738823693, 0.734132809, 0.729375802, 0.724553162, 0.719665383, 0.714712956, 0.709696378, 0.704616143, 0.699472746, 0.694266682, 0.688998447, 0.683668532, 0.678277431, 0.672825633, 0.667313624, 0.661741889, 0.656110908, 0.650421156, 0.644673104, 0.638867216, 0.63300395 ,0.627083758, 0.621107082, 0.615074355, 0.608986   ,0.602842431, 0.596644046, 0.590391232, 0.584084361, 0.57772379 ,0.571309856, 0.564842879, 0.558323158, 0.551750968, 0.545126562, 0.538450165, 0.531721972, 0.524942147, 0.518110821, 0.511228087, 0.504293997, 0.49730856 ,0.490271735, 0.483183431, 0.476043498, 0.468851724, 0.461607831, 0.454311462, 0.446962183, 0.439559467, 0.43210269 ,0.424591118, 0.417023898, 0.409400045, 0.401718425, 0.393977745, 0.386176527, 0.378313092, 0.370385535, 0.362391695, 0.354329127, 0.346195061, 0.337986361, 0.329699471, 0.32133036 ,0.312874446, 0.304326513, 0.295680611, 0.286929926, 0.278066636, 0.269081721, 0.259964733, 0.250703507, 0.24128379 ,0.231688768, 0.221898442, 0.211888813, 0.201630762, 0.191088518, 0.180217488, 0.168961101, 0.157246067, 0.144974956, 0.132014017, 0.1181719  ,0.103159409, 0.086504694, 0.067344036, 0.043755173, 0.01555616],dtype=float)
		blue = np.array([0.753683153, 0.759874796, 0.766005866, 0.772075394, 0.778082421, 0.784026001, 0.789905199, 0.79571909, 0.801466763, 0.807147315, 0.812759858, 0.818303516, 0.823777422, 0.829180725, 0.834512584, 0.839772171, 0.84495867, 0.850071279, 0.855109207, 0.860071679, 0.864957929, 0.869767207, 0.874498775, 0.87915191, 0.883725899, 0.888220047, 0.892633669, 0.896966095, 0.90121667, 0.905384751, 0.909469711, 0.913470934, 0.917387822, 0.921219788, 0.924966262, 0.928626686, 0.932200518, 0.93568723, 0.939086309, 0.942397257, 0.945619588, 0.948752835, 0.951796543, 0.954750272, 0.957613599, 0.960386113, 0.96306742, 0.96565714, 0.968154911, 0.970560381, 0.972873218, 0.975093102, 0.97721973, 0.979252813, 0.981192078, 0.983037268, 0.98478814, 0.986444467, 0.988006036, 0.989472652, 0.990844132, 0.99212031, 0.993301037, 0.994386177, 0.995375608, 0.996269227, 0.997066945, 0.997768685, 0.99837439, 0.998884016, 0.999297533, 0.999614929, 0.999836203, 0.999961374, 0.999990472, 0.999923544, 0.999760652, 0.999501871, 0.999147293, 0.998697024, 0.998151185, 0.99750991, 0.996773351, 0.995941671, 0.995015049, 0.993993679, 0.992877768, 0.991667539, 0.990363227, 0.988965083, 0.987473371, 0.985888369, 0.984210369, 0.982439677, 0.980576612, 0.978621507, 0.976574709, 0.974436577, 0.972207484, 0.969887816, 0.967477972, 0.964978364, 0.962389418, 0.959711569, 0.956945269, 0.95409098, 0.951149176, 0.948120345, 0.945004985, 0.941803607, 0.938516733, 0.935144898, 0.931688648, 0.928148539, 0.92452514, 0.92081903, 0.917030798, 0.913161047, 0.909210387, 0.90517944, 0.901068838, 0.896879224, 0.892611249, 0.888265576, 0.883842876, 0.87934383, 0.874769128, 0.870119469, 0.865395561, 0.859948576, 0.854466231, 0.848949435, 0.843399101, 0.837816138, 0.832201453, 0.826555954, 0.820880546, 0.815176131, 0.809443611, 0.803683885, 0.79789785, 0.792086401, 0.786250429, 0.780390824, 0.774508472, 0.768604257, 0.76267906, 0.756733758, 0.750769226, 0.744786333, 0.738785947, 0.732768931, 0.726736146, 0.720688446, 0.714626683, 0.708551706, 0.702464356, 0.696365473, 0.690255891, 0.68413644, 0.678007945, 0.671871226, 0.665727098, 0.659576372, 0.653419853, 0.647258341, 0.64109263, 0.634923509, 0.628751763, 0.62257817, 0.616403502, 0.610228525, 0.604054002, 0.597880686, 0.591709328, 0.585540669, 0.579375448, 0.573214394, 0.567058232, 0.560907681, 0.554763452, 0.54862625, 0.542496774, 0.536375716, 0.530263762, 0.524161591, 0.518069875, 0.511989279, 0.505920462, 0.499864075, 0.493820764, 0.487791167, 0.481775914, 0.475775629, 0.46979093, 0.463822426, 0.457870719, 0.451936407, 0.446020077, 0.440122312, 0.434243684, 0.428384763, 0.422546107, 0.41672827, 0.410931798, 0.40515723, 0.399405096, 0.393675922, 0.387970225, 0.382288516, 0.376631297, 0.370999065, 0.36539231, 0.359811513, 0.354257151, 0.348729691, 0.343229596, 0.33775732, 0.332313313, 0.326898016, 0.321511863, 0.316155284, 0.310828702, 0.305532531, 0.300267182, 0.295033059, 0.289830559, 0.284660075, 0.279521991, 0.27441669, 0.269344545, 0.264305927, 0.259301199, 0.254330723, 0.249394851, 0.244493934, 0.239628318, 0.234798343, 0.230004348, 0.225246666, 0.220525627, 0.215841558, 0.211194782, 0.20658562, 0.202014392, 0.197481414, 0.192987001, 0.188531467, 0.184115123, 0.179738284, 0.175401259, 0.171104363, 0.166847907, 0.162632207, 0.158457578, 0.154324339, 0.150232812],dtype=float)

		ROOT.TColor.CreateGradientColorTable(number, steps, red, green, blue, number) 

		c1 = ROOT.TCanvas("c1","c1",800,800);
		c1.SetTopMargin(0.05);
		c1.SetLeftMargin(0.2);
		c1.SetRightMargin(0.2);
		c1.SetBottomMargin(0.13);
		c1.SetTicks(1,1);
		#c1.SetLogz()

		c1.SetLogz(logz)
		hOne.Draw("colz")
		hOne.SetAxisRange(zrange[0], zrange[1],"Z")
		hOne.SetXTitle("#it{m}_{W_{R}} (GeV)")
		hOne.SetYTitle("#it{m}_{N_{R}} (GeV)")
		hOne.SetZTitle(ztitle)
		hOne.GetYaxis().SetTitleOffset(2)
		hOne.GetZaxis().SetTitleOffset(1.7)
		hOne.GetXaxis().SetLabelSize(.027)

		if hExpMinusOneSigma:
			hExpMinusOneSigma.Draw("colzsame")
			hExpMinusOneSigma.SetAxisRange(zrange[0], zrange[1],"Z")
			hExpMinusOneSigma.SetXTitle("#it{m}_{W_{R}} (GeV)")
			hExpMinusOneSigma.SetYTitle("#it{m}_{N_{R}} (GeV)")
			hExpMinusOneSigma.SetZTitle(ztitle)
			hExpMinusOneSigma.GetYaxis().SetTitleOffset(2)
			hExpMinusOneSigma.GetZaxis().SetTitleOffset(1.7)
		
		if hExpPlusOneSigma:
			hExpPlusOneSigma.Draw("colzsame")
			hExpPlusOneSigma.SetAxisRange(zrange[0], zrange[1],"Z")
			hExpPlusOneSigma.SetXTitle("#it{m}_{W_{R}} (GeV)")
			hExpPlusOneSigma.SetYTitle("#it{m}_{N_{R}} (GeV)")
			hExpPlusOneSigma.SetZTitle(ztitle)
			hExpPlusOneSigma.GetYaxis().SetTitleOffset(2)
			hExpPlusOneSigma.GetZaxis().SetTitleOffset(1.7)
		
		if hTwo:
			hTwo.Draw("colzsame")
			hTwo.SetAxisRange(zrange[0], zrange[1],"Z")
			hTwo.SetXTitle("#it{m}_{W_{R}} (GeV)")
			hTwo.SetYTitle("#it{m}_{N_{R}} (GeV)")
			hTwo.SetZTitle(ztitle)
			hTwo.GetYaxis().SetTitleOffset(2)
			hTwo.GetZaxis().SetTitleOffset(1.7)

		#set the legend box size
		#yw of 550 worked well when showing expected limit, observed limit, and expected +/- 1 sigma limit curves
		x1 = 900
		y1 = 4000
		xw = 1200
		yw = 200

		leg = ROOT.TLegend(x1, y1, x1 + xw, y1 + yw,"","")
		if contExpMinusOneSigma and contExpPlusOneSigma: 
			leg = ROOT.TLegend(x1, y1, x1 + xw, y1 + 3*yw,"","")
			
		if contOne:
			#draw kinematic region which cannot be probed, MNu > MWR, before any 2D limit curves
			#these two arrays define the four points of an area which will be colored yellow
			x = np.array([700,4950,700,700],dtype=float)   
			y = np.array([700,4950,4950,700],dtype=float)

			#this is the area which cannot be excluded by this analysis, where MNu is greater than MWR
			area = ROOT.TPolyLine(4,x,y)
			area.SetFillColor(ROOT.kWhite)
			area.SetLineWidth(0)
			area.Draw("F")
			latex2 = ROOT.TLatex()
			latex2.SetTextSize(0.045)  #original value was 0.05 when X and Y axes extended to 4.0 TeV
			#specify the lower left corner in x, y coordinates
			latex2.DrawLatex(1400,3600, "#it{m}_{N_{R}} > #it{m}_{W_{R}} ")
			
			#update style of expected limit line, then draw it
			contOne.SetLineStyle(7)  #small dashes
			# contOne.SetLineColor(ROOT.kRed)
			contOne.SetLineColor(ROOT.kBlue)
			contOne.SetLineWidth(3)
			contOne.Draw("Lsame")
			leg.AddEntry(contOne, "Expected","l")
		#end if contOne

		if contExpMinusOneSigma:
			contExpMinusOneSigma.SetLineStyle(7)   #small dashes
			# contExpMinusOneSigma.SetLineColor(ROOT.kViolet)
			# contExpMinusOneSigma.SetLineColor(ROOT.kRed-3)
			contExpMinusOneSigma.SetLineColor(ROOT.kBlue+2)
			contExpMinusOneSigma.SetLineWidth(3)
			contExpMinusOneSigma.Draw("Lsame")
			leg.AddEntry(contExpMinusOneSigma, "Expected-1#sigma","l")
		#end if contExpMinusOneSigma

		if contExpPlusOneSigma:
			contExpPlusOneSigma.SetLineStyle(7)   #small dashes
			# contExpPlusOneSigma.SetLineColor(ROOT.kGreen+1)
			# contExpPlusOneSigma.SetLineColor(ROOT.kOrange+7)
			# contExpPlusOneSigma.SetLineColor(ROOT.kBlue-7)
			contExpPlusOneSigma.SetLineColor(ROOT.kAzure-2)
			contExpPlusOneSigma.SetLineWidth(3)
			contExpPlusOneSigma.Draw("Lsame")
			leg.AddEntry(contExpPlusOneSigma, "Expected+1#sigma","l")
		#end if contExpPlusOneSigma

		#draw contTwo, the observed limit contour, last
		if contTwo:
			contTwo.SetLineStyle(1)  #solid
			# contTwo.SetLineColor(ROOT.kBlue)
			contTwo.SetLineColor(ROOT.kRed-3)
			contTwo.SetLineWidth(3)
			contTwo.Draw("Lsame")
			leg.AddEntry(contTwo, "Observed","l")
		#end if contTwo

		leg.SetTextFont(42);
		leg.SetTextSize(0.032);
		leg.SetFillStyle(0);
		leg.SetBorderSize(0);
		leg.Draw()

		# latex = ROOT.TLatex()
		# latex.SetNDC(True)
		# latex.SetTextSize(0.03)
		# latex.SetTextFont(62)
		# latex.DrawLatex(0.62, 0.96, "35.9 fb^{-1} (13 TeV)")

		# text = ROOT.TText(0.2,0.96,"CMS Preliminary")
		# text.SetNDC();
		# text.SetTextFont(62);
		# text.SetTextSize(0.03);
		# text.Draw()

		CMS_lumi()

		c1.RedrawAxis()
		c1.SaveAs(filename + ".png")
		c1.SaveAs(filename + ".pdf")
		c1.SaveAs(filename + ".root")
		c1.SaveAs(filename + ".C")

	#end drawOverlay method

	
	def draw(self, h, filename, zrange, ztitle, logz=False, cont=None, isObserved=False):
		customROOTstyle()
		ROOT.gStyle.SetOptTitle(0)
		ROOT.gROOT.SetBatch(True)
		ROOT.gStyle.SetPalette(55)

		c1 = ROOT.TCanvas("c1","c1",800,800);
		c1.SetTopMargin(0.05);
		c1.SetLeftMargin(0.2);
		c1.SetRightMargin(0.2);
		c1.SetBottomMargin(0.13);
		c1.SetTicks(1,1);
		#c1.SetLogz()

		c1.SetLogz(logz)
		h.Draw("colz")
		h.SetAxisRange(zrange[0], zrange[1],"Z")
		h.SetXTitle("#it{m}_{W_{R}} (GeV)")
		h.SetYTitle("#it{m}_{N_{R}} (GeV)")
		h.SetZTitle(ztitle)
		h.GetYaxis().SetTitleOffset(2)
		h.GetZaxis().SetTitleOffset(1.7)
		h.GetXaxis().SetLabelSize(.027)

		if cont:
			x = np.array([700,4950,700,700],dtype=float)
			y = np.array([700,4950,4950,700],dtype=float)

			area = ROOT.TPolyLine(4,x,y)
			area.SetFillColor(ROOT.kWhite)
			area.SetLineWidth(0)
			area.Draw("F")
			latex2 = ROOT.TLatex()
			latex2.SetTextSize(0.05)
			#specify the lower left corner in x, y coordinates
			latex2.DrawLatex(1400,3600, "M_{N_{l}} > M_{W_{R}} ")
			cont.SetLineStyle(1)  #solid
			cont.SetLineColor(ROOT.kBlue)
			cont.SetLineWidth(3)
			cont.Draw("L")

			x1 = 900
			y1 = 4000
			xw = 1200
			yw = 200

			leg = ROOT.TLegend(x1, y1, x1 + xw, y1 + yw,"","")
			if not isObserved: leg.AddEntry(cont, "Expected","l")
			if isObserved: leg.AddEntry(cont, "Observed","l")
			leg.SetTextFont(42);
			leg.SetTextSize(0.032);
			leg.SetFillStyle(0);
			leg.SetBorderSize(0);
			leg.Draw()

		# latex = ROOT.TLatex()
		# latex.SetNDC(True)
		# latex.SetTextSize(0.03)
		# latex.SetTextFont(62)
		# latex.DrawLatex(0.62, 0.96, "35.9 fb^{-1} (13 TeV)")

		# text = ROOT.TText(0.2,0.96,"CMS Preliminary")
		# text.SetNDC();
		# text.SetTextFont(62);
		# text.SetTextSize(0.03);
		# text.Draw()
		CMS_lumi()

		c1.RedrawAxis()
		c1.SaveAs(filename + ".png")
		c1.SaveAs(filename + ".pdf")
		c1.SaveAs(filename + ".root")
		c1.SaveAs(filename + ".C")
	#end draw method


	def plot(self,filename,isObserved):

		#exclusion, exclusionTwo, and exclusionExp... all have the same binning structure
		nx = self.exclusion.GetNbinsX()
		ny = self.exclusion.GetNbinsY()
		for ix in range(1, nx+1):
			for iy in range(1,ny+1):
				if not self.exclusionTwo.GetBinContent(ix,iy):
					self.exclusionTwo.SetBinContent(ix, iy, 1000);
				if not self.exclusion.GetBinContent(ix,iy):
					self.exclusion.SetBinContent(ix, iy, 1000);
				mwr = self.exclusion.GetXaxis().GetBinCenter(ix)
				mnr = self.exclusion.GetYaxis().GetBinCenter(iy)
				mwrTwo = self.exclusionTwo.GetXaxis().GetBinCenter(ix)
				mnrTwo = self.exclusionTwo.GetYaxis().GetBinCenter(iy)
				if mnr > mwr:  #default bin content is 2
					self.exclusion.SetBinContent(ix, iy, 3);
					self.exclusionExpMinusOneSigma.SetBinContent(ix, iy, 3);
					self.exclusionExpPlusOneSigma.SetBinContent(ix, iy, 3);
				elif mnr == mwr:  #default bin content is 1.5
					self.exclusion.SetBinContent(ix, iy, 3);
					self.exclusionExpMinusOneSigma.SetBinContent(ix, iy, 3);
					self.exclusionExpPlusOneSigma.SetBinContent(ix, iy, 3);
				if mnrTwo > mwrTwo:  #default bin content is 2
					self.exclusionTwo.SetBinContent(ix, iy, 3);
				elif mnrTwo == mwrTwo:  #default bin content is 1.5
					self.exclusionTwo.SetBinContent(ix, iy, 3);

		graphs = contourFromTH2(self.exclusion, 1)
		# print "len(graphs) = ", len(graphs)

		if isObserved:
			graphsTwo = contourFromTH2(self.exclusionTwo, 1)
			# print "len(graphsTwo) = ", len(graphsTwo)

		c1 = ROOT.TCanvas("c1","c1",800,800);

		#self.draw(self.exclusion,    filename + "_exclusion", (0   , 3), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj})" % self.channelname, logz=False, cont = graphs[0], isObserved=False)
		#self.draw(self.exclusionTwo,     filename + "_exclusionTwo", (0   , 3), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj})" % self.channelname, logz=False, cont = graphsTwo[0], isObserved=True)
		
		#self.draw(self.exclusionExpMinusOneSigma,   filename + "_exclusionExpMinusOneSigma", (0    , 3), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj})" % self.channelname, logz=False, cont = graphsExpMinusOneSigma[0], isObserved=False)
	
		#draw several exclusion limits overlaid on each other
		#the first histo arg must be the expected curve, and the second must be the observed limit curve (exclusionTwo)
		

		# #with +/- 1sigma expected limit curves
		graphsExpMinusOneSigma = contourFromTH2(self.exclusionExpMinusOneSigma, 1)
		graphsExpPlusOneSigma = contourFromTH2(self.exclusionExpPlusOneSigma, 1)
		# #self.drawOverlay(self.exclusion, filename + "_exclusionOverlayWithExpPlusMinusOneSigma", (0   , 3), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj})" % self.channelname, logz=False, contOne = graphs[0], self.exclusionTwo, contTwo = graphsTwo[0], contExpMinusOneSigma = graphsExpMinusOneSigma[0], contExpPlusOneSigma = graphsExpPlusOneSigma[0], hExpMinusOneSigma = self.exclusionExpMinusOneSigma, hExpPlusOneSigma = self.exclusionExpPlusOneSigma)

		if isObserved:
			# self.drawOverlay(self.exclusion, self.exclusionTwo, filename + "_exclusionOverlayWithExpPlusMinusOneSigma", (0   , 3), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj}')" % self.channelname, logz=False, contOne = graphs[0], contTwo = graphsTwo[0], contExpMinusOneSigma = graphsExpMinusOneSigma[0], contExpPlusOneSigma = graphsExpPlusOneSigma[0], hExpMinusOneSigma = self.exclusionExpMinusOneSigma, hExpPlusOneSigma = self.exclusionExpPlusOneSigma)
			#log scale:
			# self.drawOverlay(self.exclusion, self.exclusionTwo, filename + "_exclusionOverlayWithExpPlusMinusOneSigma", (0.01, 3.), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj}')" % self.channelname, logz=True, contOne = graphs[0], contTwo = graphsTwo[0], contExpMinusOneSigma = graphsExpMinusOneSigma[0], contExpPlusOneSigma = graphsExpPlusOneSigma[0], hExpMinusOneSigma = self.exclusionExpMinusOneSigma, hExpPlusOneSigma = self.exclusionExpPlusOneSigma)		
			self.drawOverlay(self.exclusion, self.exclusionTwo, filename + "_exclusionOverlayWithExpPlusMinusOneSigma", (0.01, 5.), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj})" % self.channelname, logz=True, contOne = graphs[0], contTwo = graphsTwo[0], contExpMinusOneSigma = graphsExpMinusOneSigma[0], contExpPlusOneSigma = graphsExpPlusOneSigma[0], hExpMinusOneSigma = self.exclusionExpMinusOneSigma, hExpPlusOneSigma = self.exclusionExpPlusOneSigma)		
			# self.drawOverlay(self.exclusion, self.exclusionTwo, filename + "_exclusionOverlayWithExpPlusMinusOneSigma", (0.01, 10.), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj}')" % self.channelname, logz=True, contOne = graphs[0], contTwo = graphsTwo[0], contExpMinusOneSigma = graphsExpMinusOneSigma[0], contExpPlusOneSigma = graphsExpPlusOneSigma[0], hExpMinusOneSigma = self.exclusionExpMinusOneSigma, hExpPlusOneSigma = self.exclusionExpPlusOneSigma)

		else:  #for expected
			self.drawOverlay(self.exclusion, filename + "_exclusionOverlayWithExpPlusMinusOneSigma", (0   , 3), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj})" % self.channelname, logz=False, contOne = graphs[0], contExpMinusOneSigma = graphsExpMinusOneSigma[0], contExpPlusOneSigma = graphsExpPlusOneSigma[0], hExpMinusOneSigma = self.exclusionExpMinusOneSigma, hExpPlusOneSigma = self.exclusionExpPlusOneSigma)	


		# #without +/- 1sigma expected limit curves
		# #self.drawOverlay(self.exclusion, filename + "_exclusionOverlay", (0  , 3), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj})" % self.channelname, logz=False, contOne = graphs[0], self.exclusionTwo, contTwo = graphsTwo[0])
		# if isObserved:
		# 	self.drawOverlay(self.exclusion, self.exclusionTwo, filename + "_exclusionOverlay", (0  , 3), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj})" % self.channelname, logz=False, contOne = graphs[0], contTwo = graphsTwo[0])
		# else:  #for expected
		# 	self.drawOverlay(self.exclusion, filename + "_exclusionOverlay", (0  , 3), "Limit / #sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj})" % self.channelname, logz=False, contOne = graphs[0])


		# self.draw(self.limits,       filename + "_limit",     (1e-5, 1), "#sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{W_{R}#rightarrow %sjj}) [pb]" % self.channelname,    logz=True,  cont = graphs[0])
		# self.draw(self.effratio,     filename + "_effratio",  (0   , 2), "efficiency #times acceptance (W_{R}, N_{l}) / (W_{R}, W_{R}/2) ",                         logz=False )
		# self.draw(self.crosssection, filename + "_xs",        (1e-7, 1), "#sigma(#it{pp#rightarrow W_{R}}) #times BR(#it{%sjj}) [pb]" % self.channelname,                     logz=True  )



def contourFromTH2(h2in, threshold, minPoints=20):
	ROOT.gROOT.SetBatch(True)   #dont render plots in GUI
	print "Getting contour at threshold ", threshold, " from ", h2in.GetName()
	#http://root.cern.ch/root/html/tutorials/hist/ContourList.C.html
	contours = np.array([threshold], dtype=float)
	
	if (h2in.GetNbinsX() * h2in.GetNbinsY() > 10000): minPoints = 50;
	if (h2in.GetNbinsX() * h2in.GetNbinsY() <= 100): minPoints = 10;
	
	#frameTH2D returns a TH2D object whose bins are all set to the same, very high content (order 1000)
	h2 = frameTH2D(h2in,threshold);
	
	#make 1 contour on h2 (first arg), and set the value of the contour to the one element stored in the numpy array
	#named contours
	h2.SetContour(1, contours);
	
	# Draw contours as filled regions, and Save points
	h2.Draw("CONT Z LIST");
	ROOT.gPad.Update(); # Needed to force the plotting and retrieve the contours in TGraphs
	
	
	# Get Contours
	conts = ROOT.gROOT.GetListOfSpecials().FindObject("contours");
	
	if not conts: 
		 print "*** No Contours Were Extracted!"
		 return None
	else:
		 print "found contors, contors size = ", conts.GetSize()

	ret = []
	for i in range(conts.GetSize()):
		contLevel = conts.At(i)
		# print "Contour ",  i, " has ", contLevel.GetSize(), " Graphs"

		for j in range(contLevel.GetSize()):
			gr1 = contLevel.At(j)
			# print "Graph ", j, " has ", gr1.GetN(), " points"

			if (gr1.GetN() > minPoints):
				ret.append(gr1.Clone())
			#break;

		if (contLevel.GetSize() == 0):  
			ret.append(contLevel.At(0))

		return ret;


def frameTH2D(h, threshold):
	# NEW LOGIC:
	#   - pretend that the center of the last bin is on the border if the frame
	#   - add one tiny frame with huge values
	frameValue = 1000;
	#if (TString(h.GetName()).Contains("bayes")) frameValue = -1000;
	
	xw = h.GetXaxis().GetBinWidth(1);
	yw = h.GetYaxis().GetBinWidth(1);
	
	nx = h.GetNbinsX();
	ny = h.GetNbinsY();
	
	x0 = h.GetXaxis().GetXmin();
	x1 = h.GetXaxis().GetXmax();
	
	y0 = h.GetYaxis().GetXmin();
	y1 = h.GetYaxis().GetXmax();
	xbins = np.zeros(999,dtype=float)
	ybins = np.zeros(999,dtype=float) 
	eps = 0.1;
	
	xbins[0] = x0 - eps*xw - xw
	xbins[1] = x0 + eps*xw - xw
	#for (int ix = 2; ix <= nx; ++ix):
	for ix in range(2,nx+1):
		xbins[ix] = x0 + (ix-1)*xw

	xbins[nx+1] = x1 - eps*xw + 0.5*xw
	xbins[nx+2] = x1 + eps*xw + xw
	
	ybins[0] = y0 - eps*yw - yw;
	ybins[1] = y0 + eps*yw - yw;
	#for (int iy = 2; iy <= ny; ++iy)
	for iy in range(2,ny+1):
		ybins[iy] = y0 + (iy-1)*yw;

	ybins[ny+1] = y1 - eps*yw + yw;
	ybins[ny+2] = y1 + eps*yw + yw;
	
	framed = ROOT.TH2D( h.GetName() + "framed", h.GetTitle() + "framed", nx + 2, xbins, ny + 2, ybins )
	
	#Copy over the contents
	#for(int ix = 1; ix <= nx ; ix++){
	for ix in range(1, nx+1):
		#for(int iy = 1; iy <= ny ; iy++){
		for iy in range(1,ny+1):
			framed.SetBinContent(1+ix, 1+iy, h.GetBinContent(ix,iy));

	#Frame with huge values
	nx = framed.GetNbinsX();
	ny = framed.GetNbinsY();
	#for(int ix = 1; ix <= nx ; ix++){
	for ix in xrange(1, nx+1):
		framed.SetBinContent(ix,  1, frameValue);
		framed.SetBinContent(ix, ny, frameValue);

	#for(int iy = 2; iy <= ny-1 ; iy++){
	for iy in xrange(2, ny):
		framed.SetBinContent( 1, iy, frameValue);
		framed.SetBinContent(nx, iy, frameValue);
	
	return framed