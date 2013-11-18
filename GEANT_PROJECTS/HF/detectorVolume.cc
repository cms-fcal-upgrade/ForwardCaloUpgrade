//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: detectorVolume.cc,v 1.1 2013/05/20 21:57:37 cowden Exp $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// Description: Test of Continuous Process G4Cerenkov
//              and RestDiscrete Process G4Scintillation
//              -- Generation Cerenkov Photons --
//              -- Generation Scintillation Photons --
//              -- Transport of optical Photons --
// Version:     5.0
// Created:     1996-04-30
// Author:      Juliet Armstrong
// mail:        gum@triumf.ca
//     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <fstream>
#include <unistd.h>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"
#include "G4EmUserPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4VModularPhysicsList.hh"

#include "LHEP.hh"
#include "QGSP_BERT.hh"

#include "HFPrimaryGeneratorAction.hh"
#include "CMSHFDetectorConstruction.hh"
#include "HFRunAction.hh"
#include "HFStackingAction.hh"
#include "HFSteppingVerbose.hh"
#include "HFEventAction.hh"
#include "HFDataFormat.hh"
#include "HFSteppingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// print help message
void printUsage(int argc, char **argv)
{
  if ( argv ) std::cout << argv[0] << " ";
  std::cout << " [options] [macroFile]" << std::endl;
  std::cout << "\t -o outputFile\t the name of the output ROOT file\n"
	    << "\t -p physics\t the name of the physics list [default: FTFP_BERT]\n"
            << "\t -r randomize the seed or not (fixed random seed for testing purpose\n"
            << "\t -n max number of photons to be stored (default is 10k)\n"
            << "\t -h\t print this help message and quit" << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  std::string outputFileName("output.root");
  std::string macro("b.mac");
  std::string physName("");
  int isRandomSeed = 0;
  int maxPhotonNum = 10000;

  //--------- command line argument parsing
  int c;
  while ((c = getopt(argc,argv,"o:p:r:n:h")) != -1 )
    switch (c) {
      case 'h':
  	printUsage(argc,argv);
	return 0;
      case 'o':
	outputFileName = optarg;
	break;
      case 'p':
	physName = optarg;
	break;
      case 'r':
        isRandomSeed = atoi(optarg); 
        break;
      case 'n':
        maxPhotonNum = atoi(optarg);
        break;
    }
  
  // Seed the random number generator manually
  //
  G4long myseed;
  if( isRandomSeed ){
     fstream randRead; 
     randRead.open("/dev/urandom",std::fstream::binary|std::fstream::in);
     randRead.read((char*)(&myseed),4);
  }else{
     myseed = 782365;
     std::cout<<"\nFor testing purpose... random seed is fixed to 782365 ...\n"<<std::endl;
  }
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  CLHEP::HepRandom::setTheSeed(myseed);
  std::cout << "THE RANDOM SEED IS: " << myseed << std::endl;

  std::cout << "maxPhotonNum is : " << maxPhotonNum <<std::endl;

  const unsigned nIndex = argc-optind+1U;
  std::cout<<"nIndex : "<<nIndex<<std::endl;

  //char *nArgv[nIndex];
  std::vector<char *> nArgv(nIndex);
  nArgv[0] = argv[0];

  unsigned p=1;
  for ( int i = optind; i < argc; i++ ) {
    if ( i == optind ) macro = argv[i];

    nArgv[p++] = argv[i];
  } 
  

  // data format
  HFDataFormat df(outputFileName.c_str());

  df.setMaxPhotonNum(maxPhotonNum);
  
  G4PhysListFactory factory;
  const std::vector<G4String> & names = factory.AvailablePhysLists();
  for ( unsigned n=0; n != names.size(); n++ ) {
    G4cout << "PhysicsList: " << names[n] << G4endl;
  }

  if ( "" == physName ) { 
    char * path = getenv("PHYSLIST");
    if ( path ) { physName = G4String(path); }
  }

  if ( "" == physName || factory.IsReferencePhysList(physName)) {
    physName = "FTFP_BERT";
  } 
 
  std::cout << "Using physics list: " << physName << std::endl; 

  // User Verbose output class
  //
  G4VSteppingVerbose* verbosity = new HFSteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);
  
  // Run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // UserInitialization classes - mandatory
  //
  //
  G4VUserDetectorConstruction* detector = new CMSHFDetectorConstruction;
  runManager-> SetUserInitialization(detector);

  // set physics list
  G4VModularPhysicsList * physics = factory.GetReferencePhysList(physName);
  physics->RegisterPhysics(new G4EmUserPhysics(0));
  runManager-> SetUserInitialization(physics);

  // setup generator
  HFPrimaryGeneratorAction* gen_action = new HFPrimaryGeneratorAction();
  gen_action->SetDataFormat(&df);
  runManager->SetUserAction(gen_action);
  

  G4UserEventAction *event_action = new HFEventAction(&df);
  runManager->SetUserAction(event_action);

  //
  G4UserStackingAction* stacking_action = new HFStackingAction(&df);
  runManager->SetUserAction(stacking_action);

  dynamic_cast<CMSHFDetectorConstruction*>(detector)->SetStackingAction(dynamic_cast<HFStackingAction*>(stacking_action));
  
  // UserAction classes
  //
  G4UserRunAction* run_action = new HFRunAction;
  runManager->SetUserAction(run_action);

  // user stepping action
  G4UserSteppingAction * step_action = new HFSteppingAction(&df);
  runManager->SetUserAction(step_action);

  // Initialize G4 kernel
  //
  runManager->Initialize();
  
#ifdef G4VIS_USE
  // visualization manager
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

    
  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer(); 
   
  if (nIndex == 1)   // Define UI session for interactive mode
    {
#ifdef G4UI_USE
      std::cout<<"\nG4UI_USE G4UIExecutive ... \n"<<std::endl;
      G4UIExecutive * ui = new G4UIExecutive(nIndex,&nArgv[0]);
#ifdef G4VIS_USE
      std::cout<<"\nG4VIS_USE UImanager->ApplyCommand ... \n"<<std::endl;
//      UImanager->ApplyCommand("/control/execute detVis.mac");     
#endif
      std::cout<<"\nui->SessionStart ... \n"<<std::endl;
      ui->SessionStart();
      delete ui;
#endif
    }
  else         // Batch mode
    {
      std::cout<<"\nBatching mode ...\n"<<std::endl;
      G4String command = "/control/execute ";
      G4String fileName = macro;
      UImanager->ApplyCommand(command+fileName);
    }
   
  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  // dump output file
  df.fileDump();

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
  delete verbosity;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
