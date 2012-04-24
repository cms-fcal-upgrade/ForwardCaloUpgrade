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
:detector(det), eventaction(evt), histo(hist), oldEvtNumber(0)				 
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

// deposited energy in Ecal cells
//-------------------------------
       G4int    IndCell = G4int( sqrt(detector->GetNbOfEcalCells()) );
       G4double DxCell  = detector->GetEcalCellSize();
       G4double maxSize = 0.5*DxCell*IndCell;
       if( fabs(aPoint.y())<=maxSize && fabs(aPoint.z())<=maxSize) {
         G4int iy_Ind = int( fabs(-maxSize-aPoint.y()) / DxCell );
         G4int iz_Ind = int( fabs(-maxSize-aPoint.z()) / DxCell );
         G4int cell_Ind = iz_Ind + iy_Ind*IndCell;
         eventaction->fillEcalCell(cell_Ind,edep);
       }

// hit point (Y vs Z) of first point in Ecal  
//------------------------------------------
       if( eventaction->GetEventNb() == oldEvtNumber) {
         oldEvtNumber = eventaction->GetEventNb() + 1;
         G4int    IndHit = histo->GetnRtot();
         G4double DxHits = histo->GetdRbin();
         G4double maxHit = 0.5*DxHits*IndHit;
         if( fabs(aPoint.y())<=maxHit && fabs(aPoint.z())<=maxHit ) {
           G4int iy_Hit = int( fabs(-maxHit-aPoint.y()) / DxHits );
           G4int iz_Hit = int( fabs(-maxHit-aPoint.z()) / DxHits );
           G4int hit_Ind = iz_Hit + iy_Hit*IndHit;
           eventaction->fillEcalHits(hit_Ind,edep);
         }
       }

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

//       G4double birk1  = mat->GetIonisation()->GetBirksConstant();
       G4double response = edep;
       G4double* birks = detector->GetHcalBirksConstant();
       if(birks[0]*edep*stepl*charge != 0.) {
         G4double density = mat->GetDensity() / (g/cm3);
         G4double rkb     = birks[0]/density;
         G4double c       = birks[1]*rkb*rkb;
         G4double dedx    = edep/(stepl / cm);
         if( fabs(charge) >= 2. && birks[2] !=0. ) rkb /= birks[2];
         response = edep/(1.+rkb*dedx+c*dedx*dedx);
       }

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

   if(volume == detector->GetZero() && edep > 0.) {
     G4double response = edep;
     G4double* birks = detector->GetHcalBirksConstant();
     if(birks[0]*edep*stepl*charge != 0.) {
       G4double density = mat->GetDensity() / (g/cm3);
       G4double rkb     = birks[0]/density;
       G4double c       = birks[1]*rkb*rkb;
       G4double dedx    = edep/(stepl / cm);
       if( fabs(charge) >= 2. && birks[2] !=0. ) rkb /= birks[2];
       response = edep/(1.+rkb*dedx+c*dedx*dedx);
     }
     eventaction->AddZero(response);
   }
 
//example of saving random number seed of this event, under condition
//// if (condition) G4RunManager::GetRunManager()->rndmSaveThisEvent(); 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
