
#include "HFDataFormat.hh"

#include <iostream>

HFDataFormat::HFDataFormat():m_file(NULL),m_event(NULL)
,m_wavelength(NULL),m_energy(NULL),m_na(NULL)
{ }

HFDataFormat::HFDataFormat(const std::string &fileName):
m_wavelength(NULL),m_energy(NULL),m_na(NULL)
{ 
  SetFileName(fileName);
}

HFDataFormat::~HFDataFormat()
{ }




// fill from stacking
void HFDataFormat::fillStackingAction(const StackingStruct &st) 
{
  m_wavelength->push_back(st.wavelength);
  m_energy->push_back(st.energy);
  m_na->push_back(st.na);
}


void HFDataFormat::store()
{
  m_event->Fill();

  clearStacking();
}



void HFDataFormat::fileDump()
{
  m_file->cd();
  m_event->Write();
  m_file->Close();
}


void HFDataFormat::generateTrees()
{

  m_event = new TTree("event","event");

  // generate event tree
  m_event->Branch("wavelength",&m_wavelength);
  m_event->Branch("energy",&m_energy);
  m_event->Branch("na",&m_na);

}

void HFDataFormat::SetFileName(const G4String &fileName)
{

  m_file = new TFile(fileName.c_str(),"RECREATE");
  generateTrees();

  std::cout << "HF Created " << fileName << std::endl;

}


void HFDataFormat::clearStacking()
{
  m_wavelength->clear();
  m_energy->clear();
  m_na->clear();
}
