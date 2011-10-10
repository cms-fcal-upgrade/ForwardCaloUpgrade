//
// ********************************************************************
//
// ********************************************************************

#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Step.hh"

// Constructor determination with the pointers assignment
//
// detector(det)=> detector=det and the same for
// eventaction(evt) => eventaction=evt

SteppingAction::SteppingAction(DetectorConstruction* det,
                                         EventAction* evt)
:detector(det), eventaction(evt)					 
{ }

// Distructor

SteppingAction::~SteppingAction()
{ }

// Determination of SteppingAction function

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

// get volume of the current step
//
// GetPreStepPoint() - member of G4Step
// GetVolume() - member of G4Track
// GetTouchableHandle() - member of G4Track

//  G4VPhysicalVolume* volume 
//  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

    G4StepPoint* point1 = aStep->GetPreStepPoint();
//    G4StepPoint* point2 = aStep->GetPostStepPoint();

// Geometrical informations are available from preStepPoint.
// G4VTouchable and its derivates keep these geometrical informations.

    G4TouchableHandle touch1 = point1->GetTouchableHandle(); 
     
// To get the current volume

    G4VPhysicalVolume* volume = touch1->GetVolume();
  
// Collect energy step by step

    G4double edep = aStep->GetTotalEnergyDeposit();

// To get the copy number of the mother volume (layer number)
//         (Sens -> Gap -> Wrap -> Layer)
// ( depth = 0   ->  1  ->  2   ->   3  )
  
  G4int copyNumber = touch1->GetCopyNumber(3);

// Check, in which volume the present step is ? 
// GetEcal(), GetHcal() and GetZero() member functions of 
// DetectorConstruction-class which return pointer to physic volume
//
// AddEcal(edep), AddHcal(edep) and AddZero(edep) are member functions 
// of EventAction-class, which accumulate deposited energy
      
  if (volume == detector->GetEcal() && edep > 0.) eventaction->AddEcal(edep);
  if (volume == detector->GetHcal() && edep > 0.) 
                                       eventaction->AddHcal(edep,copyNumber);
  if (volume == detector->GetZero() && edep > 0.) eventaction->AddZero(edep);

// Example of Birk attenuation law in organic scintillators.
// adapted from Geant3 PHYS337. See MIN 80 (1970) 239-244

// G4Material* material = aStep->GetTrack()->GetMaterial();
// G4double birk1       = material->GetIonisation()->GetBirksConstant();
// G4double destep      = aStep->GetTotalEnergyDeposit();
// G4double stepl       = aStep->GetStepLength();
// G4double charge      = 
// aStep->GetTrack()->GetDefinition()->GetPDGCharge();

// G4double response = destep;
// if (birk1*destep*stepl*charge != 0.)
//   {
//     response = destep/(1. + birk1*destep/stepl);
//   }
// return response;

  
//example of saving random number seed of this event, under condition
//// if (condition) G4RunManager::GetRunManager()->rndmSaveThisEvent(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
