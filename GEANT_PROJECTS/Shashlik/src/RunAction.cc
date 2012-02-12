//
// ********************************************************************
// ********************************************************************
//

#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//RunAction::RunAction()
RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* kin)
:Det(det),Kin(kin)
{}

RunAction::~RunAction()
{}


void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

//inform the runManager to save random number seed

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);

// histos cleaning and after definition

  HistoManager* myana= HistoManager::GetPointer();
  myana-> Clear();
//  myana-> Book();

  G4double Ekin = Kin->GetParticleGun()->GetParticleEnergy();
  G4int    nRtot = Det->GetnRtot();
  G4double dRbin = Det->GetdRbin();
  G4int    nLtot = Det->GetnLtot();
  G4double dLbin = Det->GetdLbin();
  G4int    nRtoth = Det->GetHcalnRtot();
  G4double dRbinh = Det->GetHcaldRbin(); 
  if( Det->GetNbOfEcalLayers() !=1) {
      nLtot = Det->GetNbOfEcalLayers(); 
      dLbin = 1.;
  }

  myana-> Book(Ekin,nRtot,dRbin,nLtot,dLbin,nRtoth,dRbinh);

// initialize cumulative quantities

  sumEcal  = sumHcal  = sumZero  = 0.;
  sum2Ecal = sum2Hcal = sum2Zero = 0.;

}


void RunAction::fillPerEvent(G4double eEcal, G4double eHcal, G4double eZero)
{

// accumulate statistics

  sumEcal += eEcal;  sum2Ecal += eEcal*eEcal;
  sumHcal += eHcal;  sum2Hcal += eHcal*eHcal;
  sumZero += eZero;  sum2Zero += eZero*eZero;

}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;

// save histos

  HistoManager* myana= HistoManager::GetPointer();
  myana-> Save();
  
// compute statistics: mean and rms

  sumEcal /= NbOfEvents; sum2Ecal /= NbOfEvents;
  G4double rmsEcal = sum2Ecal - sumEcal*sumEcal;
  if (rmsEcal >0.) rmsEcal = std::sqrt(rmsEcal); else rmsEcal = 0.;
  
  sumHcal /= NbOfEvents; sum2Hcal /= NbOfEvents;
  G4double rmsHcal = sum2Hcal - sumHcal*sumHcal;
  if (rmsHcal >0.) rmsHcal = std::sqrt(rmsHcal); else rmsHcal = 0.;

  sumZero /= NbOfEvents; sum2Zero /= NbOfEvents;
  G4double rmsZero = sum2Zero - sumZero*sumZero;
  if (rmsZero >0.) rmsZero = std::sqrt(rmsZero); else rmsZero = 0.;

// print
  
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     << "\n mean Energy in ECAL : " << G4BestUnit(sumEcal,"Energy")
     << " +- "                      << G4BestUnit(rmsEcal,"Energy")  
     << "\n mean Energy in HCAL : " << G4BestUnit(sumHcal,"Energy")
     << " +- "                      << G4BestUnit(rmsHcal,"Energy")
     << "\n mean Energy in Zero : " << G4BestUnit(sumZero,"Energy")
     << " +- "                      << G4BestUnit(rmsZero,"Energy")
     << "\n------------------------------------------------------------\n"
     << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
