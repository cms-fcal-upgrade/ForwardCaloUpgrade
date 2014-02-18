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
 ph_ecal  = 0;
 ph_hcal  = 0;
 ph_zero  = 0;

 tree_tot  = 0;
 tree_vec  = 0;
 fileName = "test_01.root";
 
//  char str[64];
//  sprintf(str, "test001.root", 1);
//  G4String fname(str);

// ROOT style

  gROOT->Reset();

}

HistoManager::~HistoManager()
{
  delete ph_ecal;
  delete ph_hcal;
  delete ph_zero;
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

// hidtograms defenitions

void HistoManager::Book()
{

  ph_ecal = new TH1D("Ecal", "Deposited Energy", 100, 0., 50.);
  ph_ecal-> GetXaxis()-> SetTitle("E [GeV]");
  ph_ecal-> SetFillColor(kBlue);
  ph_ecal-> SetStats(1);

  ph_hcal = new TH1D("Hcal", "Deposited Energy", 100, 0., 500.); 
  ph_hcal-> GetXaxis()-> SetTitle("E [MeV]");
  ph_hcal-> SetFillColor(kBlue);
  ph_hcal-> SetStats(1);

  ph_zero = new TH1D("Zero", "Deposited Energy", 100, 0., 100.); 
  ph_zero-> GetXaxis()-> SetTitle("E [MeV]");
  ph_zero-> SetFillColor(kBlue);
  ph_zero-> SetStats(1);

  tree_tot = new TTree("Total", "Calorimeter info");
  tree_tot-> Branch("ecal",&e_ecal,"e_ecal/D");
  tree_tot-> Branch("hcal",&e_hcal,"e_hcal/D");
  tree_tot-> Branch("zero",&e_zero,"e_zero/D");

  tree_vec = new TTree("Vector", "Layer energy info");
  tree_vec-> Branch("e_vec",e_vec,"e_vec[17]/D");

  return;
}

//==> reset histos ===============================

  void HistoManager::Clear()
{

  ph_ecal = 0;
  ph_hcal = 0;
  ph_zero = 0;
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
  ph_ecal->  Write();
  ph_hcal->  Write();
  ph_zero->  Write();

  tree_tot->  Write();
  tree_vec->  Write();

  file-> Close();
  delete file;

  return;
}

//==> Fill histo with deposited energy ====================

   void HistoManager::FillHisto(G4double edx1, G4double edx2, G4double edx3)
{

   ph_ecal-> Fill(edx1/GeV);
   ph_hcal-> Fill(edx2/MeV);
   ph_zero-> Fill(edx3/MeV);   

   e_ecal = edx1;
   e_hcal = edx2;
   e_zero = edx3;

   tree_tot-> Fill();
}

   void HistoManager::FillHistoLayer(G4double* p_arr)
{
     for(G4int kfil=0; kfil<17; kfil++) e_vec[kfil] = p_arr[kfil];

     tree_vec-> Fill();
}

//===========================================================
