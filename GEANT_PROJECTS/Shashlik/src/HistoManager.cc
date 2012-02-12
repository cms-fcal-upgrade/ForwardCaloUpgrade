//
// ====================================================================

#include "HistoManager.hh"
#include "RunAction.hh"

// ROOT headers
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

#include <stdio.h>

HistoManager* HistoManager::myanalysis= 0;

// ====================================================================

HistoManager::HistoManager()
{
 for(G4int ih=0; ih<nhist; ih++) histo[ih]=0;

 tree_tot  = 0;
 tree_vec  = 0;

 fileName = "test_01.root";
 
 gROOT->Reset();                         // ROOT style

}

HistoManager::~HistoManager()
{
  for(G4int ih=0; ih<nhist; ih++) delete histo[ih];

  delete tree_tot;
  delete tree_vec;

}

// determine pointer to histos

HistoManager* HistoManager::GetPointer()
{
  if (myanalysis == 0) {
    myanalysis = new HistoManager();
  }

  return myanalysis;
}

// histos definition
//-------------------
void HistoManager::Book(G4double Ekin, G4int nRtot, G4double dRbin,
                                       G4int nLtot, G4double dLbin,
                                       G4int nRtoth, G4double dRbinh)
{
  nhRtot = nRtot;      // number bins for Ecal transverse shower profile
  dhRbin = dRbin;      // bin width for Ecal transverse shower profile
  nhLtot = nLtot;      // number bins for Ecal longitudinal shower profile
  dhLbin = dLbin;      // bin width for Ecal longitudinal shower profile
  nhRtotc = nRtoth;    // number bins for Hcal transverse shower profile
  dhRbinc = dRbinh;    // bin width for Hcal transverse shower profile

  histo[0] = new TH1D("Ecal", "Deposited Energy", 100, 0., 1.10*Ekin/GeV);
  histo[0]-> GetXaxis()-> SetTitle("E [GeV]");
  histo[0]-> SetFillColor(kBlue);
  histo[0]-> SetStats(1);

  histo[1] = new TH1D("Hcal", "Deposited Energy", 100, 0., 20.*Ekin/GeV); 
  histo[1]-> GetXaxis()-> SetTitle("E [MeV]");
  histo[1]-> SetFillColor(kBlue);
  histo[1]-> SetStats(1);

  histo[2] = new TH1D("Zero", "Deposited Energy", 100, 0., 10.*Ekin/GeV); 
  histo[2]-> GetXaxis()-> SetTitle("E [MeV]");
  histo[2]-> SetFillColor(kBlue);
  histo[2]-> SetStats(1);

  tree_tot = new TTree("Total", "Calorimeter info");
  tree_tot-> Branch("ecal",&e_ecal,"e_ecal/D");
  tree_tot-> Branch("hcal",&e_hcal,"e_hcal/D");
  tree_tot-> Branch("zero",&e_zero,"e_zero/D");
  tree_tot-> Branch("hran",&r_hcal,"r_hcal/D");
  tree_tot-> Branch("eran",&r_ecal,"r_ecal/D");

  tree_vec = new TTree("Vector", "Layer energy info");
  tree_vec-> Branch("e_vec",e_vec,"e_vec[17]/D");

// Ecal transverse shower profile  in [mm]
//-----------------------------------------
    histo[3] = new TH1D("EcalTrShape","Ecal Transverse Shape",nhRtot,0.,nhRtot*dhRbin);
    histo[3]-> GetXaxis()-> SetTitle("rho / mm");
    histo[3]-> GetYaxis()-> SetTitle("1/E dE/dbin");   
    histo[3]-> SetFillColor(kBlue);
    histo[3]-> SetStats(1);

// Ecal longitudinal shower profile in [mm] or per layer's number 
//----------------------------------------------------------------
    histo[4] = new TH1D("EcalLoShape","Ecal Longitudinal Shape",nhLtot,0.,nhLtot*dhLbin);
    if ( (int)dhLbin !=1) { histo[4]-> GetXaxis()-> SetTitle("z / mm");    }
    else                  { histo[4]-> GetXaxis()-> SetTitle("z / layer"); }
    histo[4]-> GetYaxis()-> SetTitle("1/E dE/dbin");   
    histo[4]-> SetFillColor(kBlue);
    histo[4]-> SetStats(1);

// Hcal transverse shower profile  in [mm]
//-----------------------------------------
    histo[5] = new TH1D("HcalTrShape","Hcal Transverse Shape",nhRtotc,0.,nhRtotc*dhRbinc);
    histo[5]-> GetXaxis()-> SetTitle("rho / mm");
    histo[5]-> GetYaxis()-> SetTitle("1/E dE/dbin");
    histo[5]-> SetFillColor(kBlue);
    histo[5]-> SetStats(1);

// Hcal longitudinal shower profile per layer's number
//------------------------------------------------------
    histo[6] = new TH1D("HcalLoShape","Hcal Longitudinal Shape", 18, 0., 18.);
    histo[6]-> GetXaxis()-> SetTitle("z / layer"); 
    histo[6]-> GetYaxis()-> SetTitle("1/E dE/dbin");
    histo[6]-> SetFillColor(kBlue);
    histo[6]-> SetStats(1);

  return;
}

//==> reset histos ===============================

  void HistoManager::Clear()
{
  for(G4int ih=0;ih<nhist;ih++) histo[ih]=0;

  tree_tot = 0;
  tree_vec = 0;

  return;
}

//==> write out histos in the output file ===============

//  void HistoManager::Save(const G4String& fname)
  void HistoManager::Save()
{

//  TFile* file = new TFile(fname.c_str(),
  TFile* file = new TFile(fileName,
                          "RECREATE", "Geant4 ROOT analysis");

  for(G4int ih=0; ih<nhist; ih++) histo[ih]->Write();

  tree_tot->  Write();
  tree_vec->  Write();

  file-> Close();
  delete file;

  return;
}

//==> Fill histo with deposited energy ====================

   void HistoManager::FillHisto(G4double ecal, G4double hcal, G4double zero,
                                              G4double rhcal, G4double recal)
   {

     histo[0]-> Fill(ecal/GeV);
     histo[1]-> Fill(hcal/MeV);
     histo[2]-> Fill(zero/MeV);   

     e_ecal = ecal;
     e_hcal = hcal;
     e_zero = zero;
     r_hcal = rhcal;
     r_ecal = recal;

     tree_tot-> Fill();
   }

   void HistoManager::FillTransShape(G4double* p_tra)
   {
     EdepEcalRad = 0.;
     for(G4int itr=0; itr<nhRtot; itr++) EdepEcalRad += p_tra[itr];
     if( EdepEcalRad > 0. ) { 
       for(G4int jtr=0; jtr<nhRtot; jtr++) {
          G4double xrbin = dhRbin*jtr + 0.5*dhRbin;
          histo[3]-> Fill(xrbin,p_tra[jtr]/EdepEcalRad);
       }
     }
   }

   void HistoManager::FillLongShape(G4double* p_lon)
   {
     EdepEcalLong = 0.;
     for(G4int ilo=0; ilo<nhLtot; ilo++) EdepEcalLong += p_lon[ilo];
     if( EdepEcalLong > 0. ) {
       for(G4int jlo=0; jlo<nhLtot; jlo++) {
          G4double xlbin = dhLbin*jlo + 0.5*dhLbin; 
          histo[4]-> Fill(xlbin,p_lon[jlo]/EdepEcalLong);
       }
     }
   }

   void HistoManager::FillHcalTransShape(G4double* ph_tra)
   {
     EdepHcalRad = 0.;
     for(G4int ihtr=0; ihtr<nhRtot; ihtr++) EdepHcalRad += ph_tra[ihtr];
     if( EdepHcalRad > 0. ) {
       for(G4int jhtr=0; jhtr<nhRtot; jhtr++) {
          G4double xhrbin = dhRbin*jhtr + 0.5*dhRbin;
          histo[5]-> Fill(xhrbin,ph_tra[jhtr]/EdepHcalRad);
       }
     }
   }

   void HistoManager::FillHcalLongShape(G4double* ph_lon)
   {
     EdepHcalLong = 0.;
     for(G4int ihlo=0; ihlo<18; ihlo++) EdepHcalLong += ph_lon[ihlo];
     if( EdepHcalLong > 0. ) {
       for(G4int jhlo=0; jhlo<18; jhlo++) {
          G4double xhlbin = jhlo + 0.5;
          histo[6]-> Fill(xhlbin,ph_lon[jhlo]/EdepHcalLong);
       }
     }  
     for(G4int kfi=0; kfi<17; kfi++) e_vec[kfi] = ph_lon[kfi];
     tree_vec-> Fill();
   }    

//===========================================================
