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
  void AddHcal(G4double de) { EnergyHcal += de; };       

  void AddHcalRange(G4double dl, G4int lay_hcal) {
                                 RangeHcal += dl;
                                 RangeHcalLay[lay_hcal] +=dl; };

  void AddEcalRange(G4double dl, G4int lay_ecal) {
                                 RangeEcal += dl;
                                 RangeEcalLay[lay_ecal] +=dl; };

  void fillEcalStep(G4double dEstep, G4int Lbin, G4int Rbin) {
                                     dEdL[Lbin] += dEstep; 
                                     dEdR[Rbin] += dEstep; };

  void fillHcalStep(G4double dEstep, G4int Lbin, G4int Rbin) {
                                     dEdLHcal[Lbin] += dEstep;
                                     dEdRHcal[Rbin] += dEstep; };

private:

   RunAction*  runAct;
   G4double    EnergyEcal, EnergyHcal, EnergyZero;
   G4double    RangeHcal,  RangeEcal;
   G4int       aSize;
   G4double    dEdL[500], dEdR[500];
   G4double    dEdLHcal[20], dEdRHcal[500];
   G4double    RangeHcalLay[20], RangeEcalLay[500];
   G4int       printModulo;                     

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
