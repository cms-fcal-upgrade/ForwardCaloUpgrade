//
// ********************************************************************
//
// ********************************************************************

#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "G4Material.hh"
#include "G4Step.hh"
//#include "G4DynamicParticle.hh"

// Constructor determination with the pointers assignment
//
// detector(det)=> detector=det and the same for
// eventaction(evt) => eventaction=evt

SteppingAction::SteppingAction(DetectorConstruction* det,
                               EventAction* evt, HistoManager* hist)
:detector(det), eventaction(evt), histo(hist)					 
{ }

// Distructor

SteppingAction::~SteppingAction()
{ }

// Determination of SteppingAction function

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

// Geometrical informations are available from preStepPoint.
// G4VTouchable and its derivates keep these geometrical informations.
//---------------------------------------------------------------------
   G4TouchableHandle touch = aStep->GetPreStepPoint()->GetTouchableHandle();

// Get volume on the current step 
//-------------------------------- 
// GetPreStepPoint() - member of G4Step 
// GetVolume() - member of G4Track 
// GetTouchableHandle() - member of G4Track 
// G4VPhysicalVolume* volume 
// = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

   G4VPhysicalVolume* volume = touch->GetVolume(); 

// Information on the current step 
//---------------------------------
   G4double edep = aStep->GetTotalEnergyDeposit(); 
   G4ThreeVector prePoint  = aStep->GetPreStepPoint()->GetPosition();
   G4ThreeVector postPoint = aStep->GetPostStepPoint()->GetPosition();
   G4double stepl  = aStep->GetStepLength();
   G4double charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
   G4double totalP = aStep->GetTrack()->GetDynamicParticle()->GetTotalMomentum();
   G4double totalE = aStep->GetTrack()->GetTotalEnergy();
   G4Material* mat = aStep->GetTrack()->GetMaterial();
   G4MaterialPropertiesTable* propertiesTable = mat->GetMaterialPropertiesTable();

// To get the copy number of the mother volume (layer number)
//         (Sens -> Gap -> Wrap -> Layer)
// ( depth = 0   ->  1  ->  2   ->   3  )
// G4int copyNumber = touch->GetCopyNumber(3);
//  
// Check, in which volume the present step is ? 
// GetEcal(), GetHcal() and GetZero() member functions of 
// DetectorConstruction-class which return pointer to physics volume
//
// AddEcal(edep), AddHcal(edep) and AddZero(edep) are member functions 
// of EventAction-class, which accumulate deposited energy

// collect information for Ecal sensitive media
//--------------------------------------------- 
   if( volume == detector->GetEcal()) 
   {
     G4int nEcalLayer = touch->GetCopyNumber(1);
     if( edep > 0. ) {
       G4ThreeVector aPoint  = prePoint + 0.5*(postPoint - prePoint);
       G4double radius = std::sqrt(aPoint.y()*aPoint.y()+aPoint.z()*aPoint.z());
       G4double offset = detector->GetEcalOffset();

       G4int SlideNb = int( (aPoint.x() - offset) / histo->GetdLbin() );
       if( SlideNb > histo->GetnLtot()   ) SlideNb = histo->GetnLtot();
       if( detector->GetNbOfEcalLayers() !=1) SlideNb = nEcalLayer; 

       G4int RingNb  = int( radius / histo->GetdRbin() );        
       if( RingNb > histo->GetnRtot() ) RingNb = histo->GetnRtot();

       eventaction->fillEcalStep(edep,SlideNb,RingNb);
       eventaction->AddEcal(edep);
     }

     if( charge != 0.) {
       G4MaterialPropertyVector* Rindex = propertiesTable->GetProperty("RINDEX");
       G4double rind_ecal = Rindex->GetMaxProperty();
       G4double beta = (totalE > 0.) ? totalP / totalE : 1.;
       if( beta*rind_ecal > 1.) eventaction->AddEcalRange(stepl,nEcalLayer); 
     }
   }

// collect information in Ecal absorber 
//-------------------------------------
   if( volume == detector->GetAbsEcal())
   {
     G4int nEcalLayer = touch->GetCopyNumber(1);
     if( edep > 0. ) {  
       G4ThreeVector aPoint  = prePoint + 0.5*(postPoint - prePoint);
       G4double radius = std::sqrt(aPoint.y()*aPoint.y()+aPoint.z()*aPoint.z());
       G4double offset = detector->GetEcalOffset();
       
       G4int SlideAbs = int( (aPoint.x() - offset) / histo->GetAbsdLbin() );
       if( SlideAbs > histo->GetAbsnLtot()  ) SlideAbs = histo->GetAbsnLtot();
       if( detector->GetNbOfEcalLayers() !=1) SlideAbs = nEcalLayer;
       G4int RingAbs  = int( radius / histo->GetAbsdRbin() );
       if( RingAbs > histo->GetAbsnRtot() ) RingAbs = histo->GetAbsnRtot();

       eventaction->fillAbsStep(edep,SlideAbs,RingAbs);
       eventaction->AddAbs(edep);
     }
   }

// collect Hcal information
//-------------------------- 
   if( volume == detector->GetHcal() ) {
     G4int nHcalLayer = touch->GetCopyNumber(3); 
     if( edep > 0. ) {
       G4ThreeVector aPoint  = prePoint + 0.5*(postPoint - prePoint);
       G4double radius = std::sqrt(aPoint.y()*aPoint.y()+aPoint.z()*aPoint.z());
       G4int RingHcal  = int( radius / histo->GetHcaldRbin() );        
       if( RingHcal > histo->GetHcalnRtot() ) RingHcal = histo->GetHcalnRtot();

       G4double birk1  = mat->GetIonisation()->GetBirksConstant();
       G4double response = edep;
       if(birk1*edep*stepl*charge != 0.) response = edep/(1.+birk1*edep/stepl);

       eventaction->AddHcal(response);
       eventaction->fillHcalStep(response,nHcalLayer,RingHcal);
     }

     if( charge != 0.) {
       G4MaterialPropertyVector* Rindex = propertiesTable->GetProperty("RINDEX");
       G4double rind_hcal = Rindex->GetMaxProperty();
       G4double beta = (totalE > 0.) ? totalP / totalE : 1.;
       if( beta*rind_hcal > 1.) eventaction->AddHcalRange(stepl,nHcalLayer);
     }
   }

   if(volume == detector->GetZero() && edep > 0.) eventaction->AddZero(edep);

//example of saving random number seed of this event, under condition
//// if (condition) G4RunManager::GetRunManager()->rndmSaveThisEvent(); 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
