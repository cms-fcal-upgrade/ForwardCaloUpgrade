//
//*********************************************************************
//*********************************************************************

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class DetectorConstruction;
class EventAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(DetectorConstruction*, EventAction*, HistoManager*);
  virtual ~SteppingAction();

  void UserSteppingAction(const G4Step*);

private:
  DetectorConstruction* detector;
  EventAction*          eventaction;  
  HistoManager*         histo;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
