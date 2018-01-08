#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"


// void CMS_lumi(TPad* pad, TString extraText = "Preliminary") {
void CMS_lumi(TPad* pad, TString extraText) {
	float H = pad->GetWh();
	float W = pad->GetWw();
	float l = pad->GetLeftMargin();
	float t = pad->GetTopMargin(); 
	float r = pad->GetRightMargin();
	float b = pad->GetBottomMargin();

	// float x = l;
	float x = l + 0.045*(1-l-r);
	
	TString lumiText = "35.9 fb^{-1} (13 TeV)";
	float lumiTextSize = 0.4; //0.6;
	float lumiTextOffset = 0.4; //0.2;

	TString cmsText = "CMS";
	float cmsTextFont = 61; // default is helvetic-bold
	float cmsTextSize = 0.45; //0.75;

	// TString extraText = "Preliminary";
	float extraTextFont = 52; // default is helvetica-italics
	float extraTextSize = 0.4; 

	float yLumi = 1-t+lumiTextOffset*t;

	// float y = yLumi;
	float y = 1 - t - 0.035*(1-t-b);
	if (extraText == "Simulation") y = 1-t+0.01; 

	TLatex latex;
	latex.SetNDC();
	latex.SetTextAngle(0);
	latex.SetTextColor(kBlack);

	latex.SetTextFont(42);
	latex.SetTextAlign(31);
	latex.SetTextSize(lumiTextSize*t);
	latex.DrawLatex(1-r,yLumi,lumiText);

	latex.SetTextFont(cmsTextFont);
	latex.SetTextAlign(11);
	latex.SetTextSize(cmsTextSize*t);
	latex.DrawLatex(x,y,cmsText);

	latex.SetTextFont(extraTextFont);
	latex.SetTextSize(extraTextSize*t);
	latex.DrawLatex(x + 0.1, y, extraText);

	return;
}
