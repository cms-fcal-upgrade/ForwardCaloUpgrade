
#include "HFDataFormat.hh"

#include <iostream>

HFDataFormat::HFDataFormat():m_file(NULL),m_event(NULL)
,m_opt_wavelength(NULL),m_opt_energy(NULL),m_opt_na(NULL)
,m_opt_fx(NULL),m_opt_fy(NULL)
,m_part_pdgId(NULL),m_part_px(NULL),m_part_py(NULL)
,m_part_pz(NULL),m_part_x(NULL),m_part_y(NULL),m_part_z(NULL)
,m_part_e(NULL)
{ }

HFDataFormat::HFDataFormat(const std::string &fileName):
m_opt_wavelength(NULL),m_opt_energy(NULL),m_opt_na(NULL)
,m_opt_fx(NULL),m_opt_fy(NULL)
,m_part_pdgId(NULL),m_part_px(NULL),m_part_py(NULL)
,m_part_pz(NULL),m_part_x(NULL),m_part_y(NULL),m_part_z(NULL)
,m_part_e(NULL)
{ 
  SetFileName(fileName);
}

HFDataFormat::~HFDataFormat()
{ }




// fill from stacking
void HFDataFormat::fillStackingAction(const StackingStruct &st) 
{
  m_opt_wavelength->push_back(st.wavelength);
  m_opt_energy->push_back(st.energy);
  m_opt_na->push_back(st.na);
  m_opt_fx->push_back(st.x);
  m_opt_fy->push_back(st.y);
}

// fill particle
void HFDataFormat::fillParticle(const ParticleStruct &pt)
{
  m_part_pdgId->push_back(pt.pdgID);

  m_part_px->push_back(pt.px);
  m_part_py->push_back(pt.py);
  m_part_pz->push_back(pt.pz);

  m_part_x->push_back(pt.x);
  m_part_y->push_back(pt.y);
  m_part_z->push_back(pt.z);

  m_part_e->push_back(pt.e);
}


void HFDataFormat::store()
{
  m_event->Fill();

  clearStacking();
  clearParticle();
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
  m_event->Branch("opt_wavelength",&m_opt_wavelength);
  m_event->Branch("opt_energy",&m_opt_energy);
  m_event->Branch("opt_na",&m_opt_na);
  m_event->Branch("opt_fx",&m_opt_fx);
  m_event->Branch("opt_fy",&m_opt_fy);

  m_event->Branch("part_pdgId",&m_part_pdgId);
  m_event->Branch("part_px",&m_part_px);
  m_event->Branch("part_py",&m_part_py);
  m_event->Branch("part_pz",&m_part_pz);
  m_event->Branch("part_x",&m_part_x);
  m_event->Branch("part_y",&m_part_y);
  m_event->Branch("part_z",&m_part_z);
  m_event->Branch("part_e",&m_part_e);

}

void HFDataFormat::SetFileName(const G4String &fileName)
{

  m_file = new TFile(fileName.c_str(),"RECREATE");
  generateTrees();

  std::cout << "HF Created " << fileName << std::endl;

}


void HFDataFormat::clearStacking()
{
  m_opt_wavelength->clear();
  m_opt_energy->clear();
  m_opt_na->clear();
  m_opt_fx->clear();
  m_opt_fy->clear();
}

void HFDataFormat::clearParticle()
{
  m_part_pdgId->clear();
  m_part_px->clear();
  m_part_py->clear();
  m_part_pz->clear();
  m_part_x->clear();
  m_part_y->clear();
  m_part_z->clear();
  m_part_e->clear();
}
