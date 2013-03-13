
#include "HFEventAction.hh"


HFEventAction::HFEventAction(HFDataFormat *df)
:m_df(df)
{ }

HFEventAction::~HFEventAction() { }

void HFEventAction::BeginOfEventAction(const G4Event *ev)
{ }


void HFEventAction::EndOfEventAction(const G4Event *ev)
{ 

  m_df->store();
}

