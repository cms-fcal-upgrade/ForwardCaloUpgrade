#include "G4Timer.hh"
#include "HFEventAction.hh"
#include "G4Event.hh"

HFEventAction::HFEventAction(HFDataFormat *df)
:m_df(df)
{ timer = new G4Timer; }

HFEventAction::~HFEventAction() { delete timer; }

void HFEventAction::BeginOfEventAction(const G4Event *ev)
{ 
  timer->Start();
  G4cout << "### Event " << ev->GetEventID() << G4endl;
  
}


void HFEventAction::EndOfEventAction(const G4Event *ev)
{ 
  timer->Stop();
  G4cout << "  End of Event --> timer : "<< *timer << G4endl;
  m_df->store();
}

