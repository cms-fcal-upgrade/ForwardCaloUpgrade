
#include "HFDataFormat.hh"

#include <iostream>

HFDataFormat::HFDataFormat(const std::string &fileName)
{ 
  _storeOpticalInfo = true;
  _storeParticleInfo = true;
  _storeGeneratorInfo = true; 
  _storePMTInfo = true;
  _messenger = new HFDataFormatMessenger(this);
  SetFileName(fileName);
}

HFDataFormat::~HFDataFormat()
{ 
  delete _messenger;
}




// fill from stacking
void HFDataFormat::fillStackingAction(const StackingStruct &st) 
{
  if (_storeOpticalInfo){
    m_opt_wavelength.push_back(st.wavelength);
    m_opt_energy.push_back(st.energy);
    m_opt_na.push_back(st.na);
    m_opt_fx.push_back(st.x);
    m_opt_fy.push_back(st.y);
    m_opt_fz.push_back(st.z);
    m_opt_t.push_back(st.t);
  }
}

void HFDataFormat::fillStackingAction(const StackingStruct &st, const ROType t) 
{
  if (_storeOpticalInfo){
    if ( t == fCherenkov ) {
      m_opt_wavelength.push_back(st.wavelength);
      m_opt_energy.push_back(st.energy);
      m_opt_na.push_back(st.na);
      m_opt_fx.push_back(st.x);
      m_opt_fy.push_back(st.y);
      m_opt_fz.push_back(st.z);
      m_opt_t.push_back(st.t);
    } else {
      m_scin_wavelength.push_back(st.wavelength);
      m_scin_energy.push_back(st.energy);
      m_scin_na.push_back(st.na);
      m_scin_fx.push_back(st.x);
      m_scin_fy.push_back(st.y);
      m_scin_fz.push_back(st.z);
      m_scin_t.push_back(st.t);
    }
  }
}

// fill particle
void HFDataFormat::fillParticle(const ParticleStruct &pt)
{
  if (_storeParticleInfo){
    m_part_pdgId.push_back(pt.pdgID);
    m_part_px.push_back(pt.px);
    m_part_py.push_back(pt.py);
    m_part_pz.push_back(pt.pz);
    m_part_x.push_back(pt.x);
    m_part_y.push_back(pt.y);
    m_part_z.push_back(pt.z);
    m_part_e.push_back(pt.e);
  }
}

void HFDataFormat::fillGenerator(const GeneratorStruct &gn)
{
  if (_storeGeneratorInfo){
    m_gen_x.push_back(gn.x);
    m_gen_y.push_back(gn.y);
  }
}

void HFDataFormat::fillSteppingAction(const SteppingStruct &st)
{
  if ( _storePMTInfo ) {
    m_pmt_x.push_back(st.x);
    m_pmt_y.push_back(st.y);
    m_pmt_z.push_back(st.z);
    m_pmt_t.push_back(st.t);
    m_pmt_wavelength.push_back(st.lambda);
    m_pmt_polX.push_back(st.polX);
    m_pmt_polY.push_back(st.polY);
  }
}

void HFDataFormat::fillSteppingAction(const SteppingStruct &st, const ROType t)
{
  if ( _storePMTInfo ) {
    if ( t == fScintillation ) {
      m_pmtScin_x.push_back(st.x);
      m_pmtScin_y.push_back(st.y);
      m_pmtScin_z.push_back(st.z);
      m_pmtScin_t.push_back(st.t);
      m_pmtScin_wavelength.push_back(st.lambda);
      m_pmtScin_polX.push_back(st.polX);
      m_pmtScin_polY.push_back(st.polY);
    } else {
      m_pmt_x.push_back(st.x);
      m_pmt_y.push_back(st.y);
      m_pmt_z.push_back(st.z);
      m_pmt_t.push_back(st.t);
      m_pmt_wavelength.push_back(st.lambda);
      m_pmt_polX.push_back(st.polX);
      m_pmt_polY.push_back(st.polY);
    }
  }
}

void HFDataFormat::store()
{
  m_event->Fill();

  clearStacking();
  clearPMT();
  clearParticle();
  clearGenerator();
}



void HFDataFormat::fileDump()
{
  m_file->cd();
  m_event->Write();
  m_file->Close();
}


void HFDataFormat::generateTrees()
{

  // generate event tree
  if (_storeOpticalInfo){
    m_event->Branch("opt_wavelength",&m_opt_wavelength);
    m_event->Branch("opt_energy",&m_opt_energy);
    m_event->Branch("opt_na",&m_opt_na);
    m_event->Branch("opt_fx",&m_opt_fx);
    m_event->Branch("opt_fy",&m_opt_fy);
    m_event->Branch("opt_fz",&m_opt_fz);
    m_event->Branch("opt_t",&m_opt_t);

    m_event->Branch("scin_wavelength",&m_scin_wavelength);
    m_event->Branch("scin_energy",&m_scin_energy);
    m_event->Branch("scin_na",&m_scin_na);
    m_event->Branch("scin_fx",&m_scin_fx);
    m_event->Branch("scin_fy",&m_scin_fy);
    m_event->Branch("scin_fz",&m_scin_fz);
    m_event->Branch("scin_t",&m_scin_t);
  }

  if (_storeParticleInfo){
    m_event->Branch("part_pdgId",&m_part_pdgId);
    m_event->Branch("part_px",&m_part_px);
    m_event->Branch("part_py",&m_part_py);
    m_event->Branch("part_pz",&m_part_pz);
    m_event->Branch("part_x",&m_part_x);
    m_event->Branch("part_y",&m_part_y);
    m_event->Branch("part_z",&m_part_z);
    m_event->Branch("part_e",&m_part_e);
  }
  
  if (_storeGeneratorInfo){
    m_event->Branch("gen_x",&m_gen_x);
    m_event->Branch("gen_y",&m_gen_y);
  }

  if ( _storePMTInfo ) {
    m_event->Branch("pmt_t",&m_pmt_t);
    m_event->Branch("pmt_wavelength",&m_pmt_wavelength);
    m_event->Branch("pmt_polX",&m_pmt_polX);
    m_event->Branch("pmt_polY",&m_pmt_polY);
    m_event->Branch("pmt_x",&m_pmt_x);
    m_event->Branch("pmt_y",&m_pmt_y);
    m_event->Branch("pmt_z",&m_pmt_z);

    m_event->Branch("pmtScin_t",&m_pmtScin_t);
    m_event->Branch("pmtScin_wavelength",&m_pmtScin_wavelength);
    m_event->Branch("pmtScin_polX",&m_pmtScin_polX);
    m_event->Branch("pmtScin_polY",&m_pmtScin_polY);
    m_event->Branch("pmtScin_x",&m_pmtScin_x);
    m_event->Branch("pmtScin_y",&m_pmtScin_y);
    m_event->Branch("pmtScin_z",&m_pmtScin_z);
  }

}

void HFDataFormat::SetFileName(const G4String &fileName)
{

  m_file = new TFile(fileName.c_str(),"RECREATE");
  
  m_event = new TTree("event","event");

  std::cout << "HF Created " << fileName << std::endl;

}


void HFDataFormat::clearStacking()
{
  if (_storeOpticalInfo){
    m_opt_wavelength.clear();
    m_opt_energy.clear();
    m_opt_na.clear();
    m_opt_fx.clear();
    m_opt_fy.clear();
    m_opt_fz.clear();
    m_opt_t.clear();

    m_scin_wavelength.clear();
    m_scin_energy.clear();
    m_scin_na.clear();
    m_scin_fx.clear();
    m_scin_fy.clear();
    m_scin_fz.clear();
    m_scin_t.clear();
  }
}

void HFDataFormat::clearPMT()
{
  if ( _storePMTInfo ) {
    m_pmt_x.clear();
    m_pmt_y.clear();
    m_pmt_z.clear();
    m_pmt_t.clear();
    m_pmt_wavelength.clear();
    m_pmt_polX.clear();
    m_pmt_polY.clear();

    m_pmtScin_x.clear();
    m_pmtScin_y.clear();
    m_pmtScin_z.clear();
    m_pmtScin_t.clear();
    m_pmtScin_wavelength.clear();
    m_pmtScin_polX.clear();
    m_pmtScin_polY.clear();
  }
}

void HFDataFormat::clearParticle()
{
  if (_storeParticleInfo){
    m_part_pdgId.clear();
    m_part_px.clear();
    m_part_py.clear();
    m_part_pz.clear();
    m_part_x.clear();
    m_part_y.clear();
    m_part_z.clear();
    m_part_e.clear();
  }
}

void HFDataFormat::clearGenerator()
{
  if (_storeGeneratorInfo){
    m_gen_x.clear();
    m_gen_y.clear();
  }
}

