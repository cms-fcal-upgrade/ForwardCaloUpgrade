
#include "HFEventAction.hh"
#include "G4Event.hh"

HFEventAction::HFEventAction(HFDataFormat *df)
:m_df(df)
{ }

HFEventAction::~HFEventAction() { }

void HFEventAction::BeginOfEventAction(const G4Event *ev)
{ 
  G4cout << "### Event " << ev->GetEventID() << G4endl; 

}


void HFEventAction::EndOfEventAction(const G4Event *ev)
{ 

  m_df->store();
}

