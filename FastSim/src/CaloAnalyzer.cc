// -*- C++ -*-
//
// Package:    CaloAnalyzer
// Class:      CaloAnalyzer
// 
/**\class CaloAnalyzer CaloAnalyzer.cc ForwardCaloUpgrade/CaloAnalyzer/src/CaloAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Maxime Gouzevitch,40 4-B16,+41227671558,
//         Created:  Wed Oct 19 15:43:22 CEST 2011
// $Id: CaloAnalyzer.cc,v 1.4 2011/10/27 17:58:20 mgouzevi Exp $
//
//

// Global FWCore clases
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// user include files
#include "ForwardCaloUpgrade/FastSim/interface/CaloAnalyzer.h"

//Ecal Rec hits 
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"

// Provides the Calorimeter geometry
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

//
#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

using namespace edm;
using namespace std;
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CaloAnalyzer::CaloAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


CaloAnalyzer::~CaloAnalyzer()
{



}


//
// member functions
//

// ------------ method called for each event  ------------
void
CaloAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  // get geometry
  iSetup.get<CaloGeometryRecord>().get(geometry);
  if (!geometry.isValid()) {
    edm::LogWarning("CaloAnalyser")
       << "Unable to find CaloGeometry in event!";
  }


  // Reco hits
  Handle<EcalRecHitCollection> EERecHits;
  bool bEE = iEvent.getByLabel("ecalRecHit","EcalRecHitsEE", EERecHits);
  if (bEE) hMap["caloEE_size"]->Fill(EERecHits->size());
  
  Handle<EcalRecHitCollection> EBRecHits;
  bool bEB = iEvent.getByLabel("ecalRecHit","EcalRecHitsEB", EBRecHits);
  if (bEB) hMap["caloEB_size"]->Fill(EBRecHits->size());
   
  double totalEneEB = 0, totalEneEE = 0;
   
  
  if (bEB) totalEneEB = fillEB(EBRecHits);
  if (bEB) totalEneEE = fillEE(EERecHits);
    

  double totalEneE = totalEneEB + totalEneEE;
  
  hMap["caloE_totenergy"]->Fill(totalEneE);
  hMap["caloE_totenergy_zoom"]->Fill(totalEneE);


   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
 

}






double
CaloAnalyzer::fillEE(Handle<EcalRecHitCollection> EERecHits){

  double totalEneEE = 0, fV2_X = 0, fMean_X = 0, fV2_Y = 0, fMean_Y = 0;

   for (EcalRecHitCollection::const_iterator hit = EERecHits->begin(); hit!=EERecHits->end(); ++hit) {
       /*
       int ix = EEDetId((*hit).id()).ix();
       int iy = EEDetId((*hit).id()).iy();
       int iz = EEDetId((*hit).id()).zside();
       */
       double ene = hit->energy();
       //     hMap["caloEE_radius"]->Fill(ene);
       hMap["caloEE_energy"]->Fill(ene);
       hMap["caloEE_energy_zoom"]->Fill(ene);
       hMap["caloEE_energy_zoom_zoom"]->Fill(ene);
       
       EEDetId cell((*hit).id());
       const CaloCellGeometry* cellGeometry = geometry->getSubdetectorGeometry(cell)->getGeometry(cell);

       double fX = cellGeometry->getPosition().x();
       double fY = cellGeometry->getPosition().y();
      
       fV2_X += fX*fX*ene;
       fMean_X += fX*ene;
       fV2_Y += fY*fY*ene;
       fMean_Y += fY*ene;

       totalEneEE += ene;
       
   }
     
   hMap["caloEE_totenergy"]->Fill(totalEneEE);
   hMap["caloEE_totenergy_zoom"]->Fill(totalEneEE);
 
   fV2_X /= totalEneEE;  fV2_Y /= totalEneEE;  
   fMean_X /= totalEneEE;  fMean_Y /= totalEneEE;  


   double fSigma_X = sqrt(fV2_X - fMean_X*fMean_X);
   double fSigma_Y = sqrt(fV2_Y - fMean_Y*fMean_Y);


   if (fSigma_X > 1e-4) hMap2D["caloEE_xsize"]->Fill(fSigma_X, totalEneEE);
   if (fSigma_Y > 1e-4) hMap2D["caloEE_ysize"]->Fill(fSigma_Y, totalEneEE);

   return totalEneEE;


}

double
CaloAnalyzer::fillEB(Handle<EcalRecHitCollection> EBRecHits){


  double totalEneEB = 0;

  for (EcalRecHitCollection::const_iterator hit = EBRecHits->begin(); hit!=EBRecHits->end(); ++hit) {
            
    double ene = hit->energy();
    //     hMap["caloEB_radius"]->Fill(ene);
    hMap["caloEB_energy"]->Fill(ene);
    hMap["caloEB_energy_zoom"]->Fill(ene);
    hMap["caloEB_energy_zoom_zoom"]->Fill(ene);
    
    totalEneEB += ene;
      
  }
  
  hMap["caloEB_totenergy"]->Fill(totalEneEB);
  hMap["caloEB_totenergy_zoom"]->Fill(totalEneEB);

  return totalEneEB;
}














// ------------ method called once each job just before starting event loop  ------------
void 
CaloAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CaloAnalyzer::endJob() 
{
  outfilename = "toto.root";

  TFile* outfile = new TFile(outfilename, "RECREATE");
  outfile->cd();
  for(std::map<TString, TH1F*>::iterator it = hMap.begin(); it!=hMap.end(); it++){
    it->second->Write();
  }

  for(std::map<TString, TH2F*>::iterator it = hMap2D.begin(); it!=hMap2D.end(); it++){
    it->second->Write();
  }

  outfile->Close();
}

// ------------ method called when starting to processes a run  ------------
void 
CaloAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
  
  hMap["caloEB_size"] = new TH1F("caloEB_size", "N hits in ECAL barrel", 1000, 0, 1000);
  hMap["caloEE_size"] = new TH1F("caloEE_size", "N hits in ECAL endcap", 1000, 0, 1000);
    
  hMap["caloEB_energy"] = new TH1F("caloEB_energy", "Hits energy in ECAL barrel", 100, 0, 100);
  hMap["caloEE_energy"] = new TH1F("caloEE_energy", "Hits energy in ECAL endcap", 100, 0, 100);

  hMap["caloEB_energy_zoom"] = new TH1F("caloEB_energy_zoom", "Hits energy in ECAL barrel zoom to 10 GeV", 100, 0, 10);
  hMap["caloEE_energy_zoom"] = new TH1F("caloEE_energy_zoom", "Hits energy in ECAL endcap zoom to 10 GeV", 100, 0, 10);

  hMap["caloEB_energy_zoom_zoom"] = new TH1F("caloEB_energy_zoom_zoom", "Hits energy in ECAL barrel zoom to 1 GeV", 100, 0, 1);
  hMap["caloEE_energy_zoom_zoom"] = new TH1F("caloEE_energy_zoom_zoom", "Hits energy in ECAL endcap zoom to 1 GeV", 100, 0, 1);

  hMap["caloEB_totenergy"] = new TH1F("caloEB_totenergy", "Total Hits energy in ECAL barrel zoom to 10 GeV", 100, 0, 100);
  hMap["caloEE_totenergy"] = new TH1F("caloEE_totenergy", "Total hits energy in ECAL endcap zoom to 10 GeV", 100, 0, 100);

  hMap["caloEB_totenergy_zoom"] = new TH1F("caloEB_totenergy_zoom", "Total Hits energy in ECAL barrel zoom to 10 GeV", 100, 0, 10);
  hMap["caloEE_totenergy_zoom"] = new TH1F("caloEE_totenergy_zoom", "Total hits energy in ECAL endcap zoom to 10 GeV", 100, 0, 10);

  hMap["caloE_totenergy"] = new TH1F("caloE_totenergy", "Total hits energy in ECAL endcap zoom to 10 GeV", 100, 0, 100);
  hMap["caloE_totenergy_zoom"] = new TH1F("caloE_totenergy_zoom", "Total hits energy in ECAL endcap zoom to 10 GeV", 100, 0, 10);

  hMap2D["caloEE_xsize"] = new TH2F("caloEE_xsize", "Transverse x size (cm)", 50, 0, 10, 100, 0, 100); 
  hMap2D["caloEE_ysize"] = new TH2F("caloEE_ysize", "Transverse y size (cm)", 50, 0, 10, 100, 0, 100); 



}

// ------------ method called when ending the processing of a run  ------------
void 
CaloAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
CaloAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
CaloAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CaloAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

