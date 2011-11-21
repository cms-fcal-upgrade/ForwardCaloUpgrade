//
// ********************************************************************
// ********************************************************************
//

#include "EventAction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

// Constructor of EventAction-class and
// assignment of pinter runAct(run) <=> runAct=run

EventAction::EventAction(RunAction* run)
:runAct(run),printModulo(20)
{}

EventAction::~EventAction()
{}

// Member function which run at the start of each event

void EventAction::BeginOfEventAction(const G4Event* evt)
{  
  G4int evtNb = evt->GetEventID();

  if (evtNb%printModulo == 0) { 
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
//    CLHEP::HepRandom::showEngineStatus();
  }
 
// initialisation per event

 EnergyEcal = EnergyHcal = EnergyZero = RangeHcal = 0.;
 for (G4int nl=0; nl<17; nl++) {
     edeplay[nl] = 0.;
     rangelay[nl] = 0.;
 }
 for (G4int nc=0; nc<1700; nc++) edepcell[nc] = 0.;

}

// Member function at the end of each event

void EventAction::EndOfEventAction(const G4Event* evt)
{

//accumulates statistic

  runAct->fillPerEvent(EnergyEcal, EnergyHcal, EnergyZero);
  
// fill histos

  HistoManager* myana= HistoManager::GetPointer();
  myana-> FillHisto(EnergyEcal, EnergyHcal, EnergyZero, RangeHcal);

// fill deposited energy for each layer

  p_array = edeplay;        // assign layer's array adress  to pointer
  myana-> FillHistoLayer(p_array);  

// fill range of charge particles for each layer

  p_range = rangelay;        // assign layer's array adress  to pointer
//  myana-> FillHistoRange(p_range);
  myana-> FillHistoRange(rangelay);

// fill deposited energy as radius function for each layers

  p_cells = edepcell;       // assign cell's array adress to pointer
  myana-> FillHistoCell(p_cells);


//print per event (modulo n)

  G4int evtNb = evt->GetEventID();

  if (evtNb%printModulo == 0) {
    G4cout << "---> End of event: " << evtNb << G4endl;	

    G4cout
       << " Total deposited energy: " 
       << "  ECAL = " << std::setw(7)
                     << G4BestUnit(EnergyEcal,"Energy")
       << ", HCAL = " << std::setw(7)
                     << G4BestUnit(EnergyHcal,"Energy")
       << ", ZERO = " << std::setw(7)
                     << G4BestUnit(EnergyZero,"Energy")
       << G4endl;
	  
  }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
