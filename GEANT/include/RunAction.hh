//
// ********************************************************************
// ********************************************************************
//

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

class RunAction : public G4UserRunAction
{
public:
  RunAction();
  virtual ~RunAction();

  void BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);
    
  void fillPerEvent(G4double, G4double, G4double); 

private:
  G4double sumEcal, sum2Ecal;
  G4double sumHcal, sum2Hcal;
  G4double sumZero, sum2Zero;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
	
#endif

