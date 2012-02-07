
//#ifndef __CINT__
//#include "RooGlobalFunc.h"
//#endif
//#include "RooRealVar.h"
//#include "RooDataSet.h"
//#include "RooDataHist.h"
//#include "RooCBShape.h"
//#include "RooFitResult.h"
//#include "RooPlot.h"
//#include "RooBinning.h"
//#include "TCanvas.h"
//#include "TString.h"
//#include "TTree.h"
//#include "TH1.h"
//#include "TF1.h"
//#include "TFile.h"
//#include "TGraphErrors.h"

using namespace RooFit ;

TString sTitle("ELECTRON_FORWARD/calorimeter_histograms_");
TString sProd("PERFECT_LIGHT_COLL_RIGHT_");
###TString sProd("REALISTIC_LIGHT_COLLECTION_ETA2_");
TString sParticle("ELECTRON");
TString sRegion("FORWARD");




RooFitResult* FitHistWithCBShape(TH1* h)
{
  RooFitResult *res = 0;

  double avg = h->GetMean();
  double rms = h->GetRMS();
  double xmin = avg-5.0*rms;
  double xmax = avg+5.0*rms;

  RooRealVar E("E","E", xmin, xmax, "GeV");
  RooDataHist dataSet("dataSet", "dataSet", E, h ); 
  RooPlot* frame = E.frame();
  dataSet.plotOn(frame);

  RooRealVar mean("mean","mean", avg, xmin, xmax) ;
  RooRealVar sigma("sigma","width", rms, 0.001, 5.0*rms);
  RooRealVar alpha("alpha","alpha", 1.0, -10.0, 10.0);
  RooRealVar n("n","n", 1.0, 0.001, 10000.0);
  
  RooCBShape CBall("CBall","Crystal Ball", E, mean, sigma, alpha, n);
  
  res = CBall.fitTo(dataSet, Save() );

  CBall.plotOn(frame);
  frame->Draw();
  return res;

}




RooFitResult* Electron5x5(int indx)
{

  TString fname[7];

  for (int i = 1; i < 8; i++){

    int idx = TMath::Power(2,i-1)*10;
    TString sName = sTitle + "" + sProd + "" + sParticle + "_" + Form("%d",idx) + "_" + sRegion + ".root";
    fname[i-1] = sName.Data();

  }

  int idx = TMath::Power(2,indx)*10;

  if( indx<0 || indx>6 ) return;
  TFile* fin = new TFile( fname[indx].Data() );
  string shisto("caloEE_5to5_energy_"); shisto = shisto + "" + Form("%d",idx) + ";1";

  cout << shisto.c_str() << endl;

  TH1D* hist = (TH1D*)fin->Get(shisto.c_str());
  RooFitResult *r = FitHistWithCBShape(hist);
  r->Print();

  TString sNamePng(fname[indx]);
  sNamePng.ReplaceAll(".root", "_5to5_finebinning.png");
  cout << "sNamePng = " << sNamePng.Data() << endl;


  gPad->SaveAs(sNamePng.Data());

  return r;

}


void FitterExample(){

   TGraphErrors* gSigma = new TGraphErrors(7);

   gSigma->SetTitle("#sigma/#mu");

   TGraphErrors* gMean = new TGraphErrors(7);

   gMean->SetTitle("#mu/Ene");


   for (int i = 0; i < 7; i++){
      RooFitResult *r = Electron5x5(i);


      double sVal = ((RooAbsReal*) r->floatParsFinal().find("sigma"))->getVal();
      double sError = ((RooAbsReal*) r->floatParsFinal().find("sigma"))->getPropagatedError(*r);
 
      double sMean = ((RooAbsReal*) r->floatParsFinal().find("mean"))->getVal();
      double sErrorMean = ((RooAbsReal*) r->floatParsFinal().find("mean"))->getPropagatedError(*r);

 
      double Ene = TMath::Power(2,i)*10;

      cout << "Ene  =  " << Ene << " sMean = " << sMean << " errorMean = " << sErrorMean << " dSigma = " << sVal << " error = " << 
sError<< endl;
      gSigma->SetPoint(i, Ene, sVal/sMean);
      gSigma->SetPointError(i, 0, sError/sMean);

      gMean->SetPoint(i, Ene, sMean/Ene);
      gMean->SetPointError(i, 0, sErrorMean/Ene);


   }

   for (int i = 0; i < 7; i++){

      double Ene, dSigma, dMean, dMeanError;
      
      double sMean = ((RooAbsReal*) r->floatParsFinal().find("mean"))->getVal();

      gSigma->GetPoint(i, Ene, dSigma);
      gMean->GetPoint(i, Ene, dMean);

      cout << Ene << " " << dMean*Ene << " " << gMean->GetErrorY(i)*Ene << " " << dSigma << " " << gSigma->GetErrorY(i) << " " << endl;
   }


   string sName = sTitle + "" + sProd + "" + sParticle + "_" + sRegion + ".txt";

   ofstream outFileTxt;
   outFileTxt.open(sName.c_str());


   for (int i = 0; i < 7; i++){
  
      double Ene, dSigma;

      gSigma->GetPoint(i, Ene, dSigma);

      double sMean = ((RooAbsReal*) r->floatParsFinal().find("mean"))->getVal();

      outFileTxt << Ene << " " << dMean*Ene << " " << gMean->GetErrorY(i)*Ene << " " << dSigma << " " << gSigma->GetErrorY(i) << " " << 
endl;

   }

   outFileTxt.close();

   TCanvas* c = new TCanvas();

   c->Divide(1,2);
   c->cd(1);

   gSigma->Draw("APE");

   c->cd(2);
   gMean->Draw("APE");

   TLine* L = new TLine(0, 1, 700, 1);
   L->SetLineColor(kRed);
   L->SetLineWidth(2);
   L->Draw("SAME");



   string sName = sTitle + "" + sProd + "" + sParticle + "_" + sRegion + ".png";
   c->SaveAs(sName.c_str());

}
