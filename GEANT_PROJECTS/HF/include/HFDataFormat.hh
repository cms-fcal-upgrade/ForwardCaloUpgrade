#ifndef HFDataFormat_hh
#define HFDataFormat_hh

///////////////////////////////////////////////////
// HFDataFormat
// Root ntuple dumping class and helper functions
// to coordinate the output data from the various 
// geant4 user action classes.
//////////////////////////////////////////////////

#include <vector>
#include <string>
#include <iostream>


#include "TFile.h"
#include "TTree.h"

#include "globals.hh"

#include "G4ThreeVector.hh"


///
/// The StackingStruct helps to pass data from the HFStackingAction
/// to the ntuple filling procedure.
struct StackingStruct {
  double wavelength;
  double energy;
  double na;
  double x;
  double y;


  inline StackingStruct(double w, double e, double n,double gx, double gy)
    :wavelength(w),energy(e),na(n)
    ,x(gx),y(gy)
    { }
};


///
/// The ParticleStruct helps to pass data about particles
/// As tracks are created, the HFStackingAction class can record
/// these in an ntuple for studies of the shower development in
/// simulation.
struct ParticleStruct {
  int pdgID;
  double px;
  double py;
  double pz;
  double x;
  double y;
  double z;
  double e;

  inline ParticleStruct(int ID, const G4ThreeVector & mom, const G4ThreeVector & pos,double E)
    :pdgID(ID),px(mom.x()),py(mom.y()),pz(mom.z())
    ,x(pos.x()),y(pos.y()),z(pos.z())
    ,e(E)
    { }

};


class HFDataFormat {

public:

  HFDataFormat();
  // constructor, provide output ROOT file name
  HFDataFormat(const std::string &fileName);

  virtual ~HFDataFormat();


  // fill from SteppingAction
  // fill from StackingAction
  void fillStackingAction(const StackingStruct &);
  // fill from EventAction 

  // fill shower particles when new track is created
  void fillParticle(const ParticleStruct &);
  

  // store event in tree, and clear vectors
  void store();

  // dump file and close   
  void fileDump();

  // set the file name
  void SetFileName(const G4String &fileName);


private:

  // generate trees
  void generateTrees();

  // clear vectors in trees
  // clear stacking vectors
  void clearStacking();
  // clear particle vectors
  void clearParticle();

  // ------------------- member data -------------------
  TFile * m_file;

  TTree * m_event;

  // tree branches
  // event branches
  std::vector<double> * m_opt_wavelength;
  std::vector<double> * m_opt_energy;
  std::vector<double> * m_opt_na;
  std::vector<double> * m_opt_fx;
  std::vector<double> * m_opt_fy;


  // shower particle branches
  std::vector<int> * m_part_pdgId;
  std::vector<double> * m_part_px;
  std::vector<double> * m_part_py;
  std::vector<double> * m_part_pz;
  std::vector<double> * m_part_x;
  std::vector<double> * m_part_y;
  std::vector<double> * m_part_z;
  std::vector<double> * m_part_e;


};


#endif
