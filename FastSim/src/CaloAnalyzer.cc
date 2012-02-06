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
// $Id: CaloAnalyzer.cc,v 1.8 2012/02/06 09:43:16 mgouzevi Exp $
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
  if (bEE) totalEneEE = fillEE(EERecHits);
    

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
 

}






double
CaloAnalyzer::fillEE(Handle<EcalRecHitCollection> EERecHits){

  double totalEneEE = 0, EneEE_5to5 = 0, fV2_X = 0, fMean_X = 0, fV2_Y = 0, fMean_Y = 0, Emax = 0;

  double ix_max = -1000, iy_max = -1000, iz_max = -1000;

  for (EcalRecHitCollection::const_iterator hit = EERecHits->begin(); hit!=EERecHits->end(); ++hit) {

       double ene = hit->energy();
       
	if (Emax < ene) {
	        Emax = ene;
        	ix_max = EEDetId((*hit).id()).ix();
        	iy_max = EEDetId((*hit).id()).iy();
        	iz_max = EEDetId((*hit).id()).zside();
	}

       hMap["caloEE_hit_energy"]->Fill(ene);
       hMap["caloEE_hit_energy_zoom"]->Fill(ene);
       hMap["caloEE_hit_energy_zoom_zoom"]->Fill(ene);
       
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


  if (Emax > 1){

        EneEE_5to5 = Emax;


  	for (EcalRecHitCollection::const_iterator hit = EERecHits->begin(); hit!=EERecHits->end(); ++hit) {

	    double ene = hit->energy();


	    double ix = EEDetId((*hit).id()).ix();
            double iy = EEDetId((*hit).id()).iy();
            double iz = EEDetId((*hit).id()).zside();

            bool b5to5 = (iz=iz_max) && abs(ix-ix_max) < 3 && abs(iy-iy_max) < 3 && !( iy == iy_max && ix == ix_max);

            if (b5to5)EneEE_5to5 += ene;

	}


   hMap["caloEE_5to5_energy"]->Fill(EneEE_5to5);
   hMap["caloEE_5to5_energy_10"]->Fill(EneEE_5to5);
   hMap["caloEE_5to5_energy_20"]->Fill(EneEE_5to5);
   hMap["caloEE_5to5_energy_40"]->Fill(EneEE_5to5);
   hMap["caloEE_5to5_energy_80"]->Fill(EneEE_5to5);
   hMap["caloEE_5to5_energy_160"]->Fill(EneEE_5to5);
   hMap["caloEE_5to5_energy_320"]->Fill(EneEE_5to5);  
   hMap["caloEE_5to5_energy_640"]->Fill(EneEE_5to5);

  }
    

   hMap["caloEE_total_energy"]->Fill(totalEneEE);
   hMap["caloEE_total_energy_10"]->Fill(totalEneEE);
   hMap["caloEE_total_energy_20"]->Fill(totalEneEE);
   hMap["caloEE_total_energy_40"]->Fill(totalEneEE);
   hMap["caloEE_total_energy_80"]->Fill(totalEneEE);
   hMap["caloEE_total_energy_160"]->Fill(totalEneEE);
   hMap["caloEE_total_energy_320"]->Fill(totalEneEE);
   hMap["caloEE_total_energy_640"]->Fill(totalEneEE);



 
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
    hMap["caloEB_hit_energy"]->Fill(ene);
    hMap["caloEB_hit_energy_zoom"]->Fill(ene);
    hMap["caloEB_hit_energy_zoom_zoom"]->Fill(ene);
    
    totalEneEB += ene;
      
  }
  
  hMap["caloEB_total_energy"]->Fill(totalEneEB);
  hMap["caloEB_total_energy_10"]->Fill(totalEneEB);
  hMap["caloEB_total_energy_20"]->Fill(totalEneEB);
  hMap["caloEB_total_energy_40"]->Fill(totalEneEB);
  hMap["caloEB_total_energy_80"]->Fill(totalEneEB);
  hMap["caloEB_total_energy_160"]->Fill(totalEneEB);
  hMap["caloEB_total_energy_320"]->Fill(totalEneEB);
  hMap["caloEB_total_energy_640"]->Fill(totalEneEB);


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
  outfilename = "calorimeter_histograms.root";

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

  hMap["caloEB_hit_energy"] = new TH1F("caloEB_hit_energy", "Hit energy in ECAL barrel", 100, 0, 1000);
  hMap["caloEE_hit_energy"] = new TH1F("caloEE_hit_energy", "Hit energy in ECAL endcap", 100, 0, 1000);    

  hMap["caloEB_hit_energy_zoom"] = new TH1F("caloEB_hit_energy_zoom", "Hit energy in ECAL barrel zoom", 100, 0, 100);
  hMap["caloEE_hit_energy_zoom"] = new TH1F("caloEE_hit_energy_zoom", "Hit energy in ECAL endcap zoom", 100, 0, 100);    

  hMap["caloEB_hit_energy_zoom_zoom"] = new TH1F("caloEB_hit_energy_zoom_zoom", "Hit energy in ECAL barrel zoom zoom", 100, 0, 10);
  hMap["caloEE_hit_energy_zoom_zoom"] = new TH1F("caloEE_hit_energy_zoom_zoom", "Hit energy in ECAL endcap zoom zoom", 100, 0, 10);    




  hMap["caloEB_5to5_energy"] = new TH1F("caloEB_5to5_energy", "5to5 energy in ECAL barrel in 5to5 cluster", 5000, 0, 1000);
  hMap["caloEE_5to5_energy"] = new TH1F("caloEE_5to5_energy", "5to5 energy in ECAL endcap in 5to5 cluster", 5000, 0, 1000);    

  hMap["caloEB_5to5_energy_10"] = new TH1F("caloEB_5to5_energy_10", "Total energy in ECAL barrel zoom to 10 GeV", 1200, 0, 15);
  hMap["caloEE_5to5_energy_10"] = new TH1F("caloEE_5to5_energy_10", "Total energy in ECAL endcap zoom to 10 GeV", 1200, 0, 15);

  hMap["caloEB_5to5_energy_20"] = new TH1F("caloEB_5to5_energy_20", "Total energy in ECAL barrel zoom to 20 GeV", 800, 10, 30);
  hMap["caloEE_5to5_energy_20"] = new TH1F("caloEE_5to5_energy_20", "Total energy in ECAL endcap zoom to 20 GeV", 800, 10, 30);

  hMap["caloEB_5to5_energy_40"] = new TH1F("caloEB_5to5_energy_40", "Total energy in ECAL barrel zoom to 40 GeV", 800, 15, 55);
  hMap["caloEE_5to5_energy_40"] = new TH1F("caloEE_5to5_energy_40", "Total energy in ECAL endcap zoom to 40 GeV", 800, 15, 55);

  hMap["caloEB_5to5_energy_80"] = new TH1F("caloEB_5to5_energy_80", "Total energy in ECAL barrel zoom to 80 GeV", 500, 55, 105);
  hMap["caloEE_5to5_energy_80"] = new TH1F("caloEE_5to5_energy_80", "Total energy in ECAL endcap zoom to 80 GeV", 500, 55, 105);

  hMap["caloEB_5to5_energy_160"] = new TH1F("caloEB_5to5_energy_160", "Total energy in ECAL barrel zoom to 160 GeV", 400, 100, 180);
  hMap["caloEE_5to5_energy_160"] = new TH1F("caloEE_5to5_energy_160", "Total energy in ECAL endcap zoom to 160 GeV", 400, 100, 180);

  hMap["caloEB_5to5_energy_320"] = new TH1F("caloEB_5to5_energy_320", "Total energy in ECAL barrel zoom to 320 GeV", 500, 200, 400);
  hMap["caloEE_5to5_energy_320"] = new TH1F("caloEE_5to5_energy_320", "Total energy in ECAL endcap zoom to 320 GeV", 500, 200, 400);

  hMap["caloEB_5to5_energy_640"] = new TH1F("caloEB_5to5_energy_640", "Total energy in ECAL barrel zoom to 640 GeV", 500, 400, 700);
  hMap["caloEE_5to5_energy_640"] = new TH1F("caloEE_5to5_energy_640", "Total energy in ECAL endcap zoom to 640 GeV", 500, 400, 700);





  hMap["caloEB_total_energy"] = new TH1F("caloEB_total_energy", "Total energy in ECAL barrel", 5000, 0, 1000);
  hMap["caloEE_total_energy"] = new TH1F("caloEE_total_energy", "Total energy in ECAL endcap", 5000, 0, 1000);    

  hMap["caloEB_total_energy_10"] = new TH1F("caloEB_total_energy_10", "Total energy in ECAL barrel zoom to 10 GeV", 1200, 0, 15);
  hMap["caloEE_total_energy_10"] = new TH1F("caloEE_total_energy_10", "Total energy in ECAL endcap zoom to 10 GeV", 1200, 0, 15);

  hMap["caloEB_total_energy_20"] = new TH1F("caloEB_total_energy_20", "Total energy in ECAL barrel zoom to 20 GeV", 800, 10, 30);
  hMap["caloEE_total_energy_20"] = new TH1F("caloEE_total_energy_20", "Total energy in ECAL endcap zoom to 20 GeV", 800, 10, 30);

  hMap["caloEB_total_energy_40"] = new TH1F("caloEB_total_energy_40", "Total energy in ECAL barrel zoom to 40 GeV", 800, 25, 55);
  hMap["caloEE_total_energy_40"] = new TH1F("caloEE_total_energy_40", "Total energy in ECAL endcap zoom to 40 GeV", 800, 25, 55);

  hMap["caloEB_total_energy_80"] = new TH1F("caloEB_total_energy_80", "Total energy in ECAL barrel zoom to 80 GeV", 500, 55, 105);
  hMap["caloEE_total_energy_80"] = new TH1F("caloEE_total_energy_80", "Total energy in ECAL endcap zoom to 80 GeV", 500, 55, 105);

  hMap["caloEB_total_energy_160"] = new TH1F("caloEB_total_energy_160", "Total energy in ECAL barrel zoom to 160 GeV", 400, 100, 180);
  hMap["caloEE_total_energy_160"] = new TH1F("caloEE_total_energy_160", "Total energy in ECAL endcap zoom to 160 GeV", 400, 100, 180);

  hMap["caloEB_total_energy_320"] = new TH1F("caloEB_total_energy_320", "Total energy in ECAL barrel zoom to 320 GeV", 500, 200, 400);
  hMap["caloEE_total_energy_320"] = new TH1F("caloEE_total_energy_320", "Total energy in ECAL endcap zoom to 320 GeV", 500, 200, 400);

  hMap["caloEB_total_energy_640"] = new TH1F("caloEB_total_energy_640", "Total energy in ECAL barrel zoom to 640 GeV", 400, 400, 700);
  hMap["caloEE_total_energy_640"] = new TH1F("caloEE_total_energy_640", "Total energy in ECAL endcap zoom to 640 GeV", 400, 400, 700);

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

