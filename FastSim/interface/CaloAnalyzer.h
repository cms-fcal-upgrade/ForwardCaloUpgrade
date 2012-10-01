#ifndef CaloAnalyzer_h
#define CaloAnalyzer_h

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
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "TString.h"

//
// class declaration
//

class TH1F;
class TH2F;

class CaloAnalyzer : public edm::EDAnalyzer {
   public:
      explicit CaloAnalyzer(const edm::ParameterSet&);
      ~CaloAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------

      std::map<TString, TH1F*> hMap;
      std::map<TString, TH2F*> hMap2D;
      TString outfilename;
};

//define this as a plug-in
DEFINE_FWK_MODULE(CaloAnalyzer);

#endif

