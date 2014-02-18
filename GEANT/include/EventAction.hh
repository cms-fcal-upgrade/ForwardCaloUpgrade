//
// ********************************************************************
// ********************************************************************
//

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "HistoManager.hh"

class RunAction;

// Determination of EventAction-class

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*);
  virtual ~EventAction();

  void  BeginOfEventAction(const G4Event*);
  void    EndOfEventAction(const G4Event*);
    
  void AddEcal(G4double de) { EnergyEcal += de; };
  void AddZero(G4double de) { EnergyZero += de; };

  void AddHcal(G4double de, G4int n_layer) {       
                            EnergyHcal += de; 
                            edeplay[n_layer] +=de; };

private:

   RunAction*  runAct;
   G4double    EnergyEcal, EnergyHcal, EnergyZero;
   G4double*   p_array;
   G4double    edeplay[17];
   G4int       printModulo, n_layer;                     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
