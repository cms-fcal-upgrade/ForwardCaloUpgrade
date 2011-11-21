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
  void AddRange(G4double dl, G4int n_layer) {
                             RangeHcal += dl;
                             rangelay[n_layer] +=dl; };

  void AddCell(G4double de, G4int n_layer, G4double yhit, G4double zhit) 
  {       
      i_cell =  int( sqrt(yhit*yhit+zhit*zhit) / 10. ) + 100*n_layer;
      edepcell[i_cell] +=de;  
  };

private:

   RunAction*  runAct;
   G4double    EnergyEcal, EnergyHcal, EnergyZero;
   G4double    RangeHcal;
   G4double*   p_array;
   G4double*   p_cells;
   G4double*   p_range;
   G4double    edeplay[17], rangelay[17];
   G4double    edepcell[1700];
   G4int       printModulo, n_layer;                     
   G4int       i_cell;                     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
