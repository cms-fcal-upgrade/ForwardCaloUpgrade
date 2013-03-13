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


struct StackingStruct {
  double wavelength;
  double energy;
  double na;

  inline StackingStruct(double w, double e, double n):wavelength(w),energy(e),na(n){ }
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

  // ------------------- member data -------------------
  TFile * m_file;

  TTree * m_event;

  // tree branches
  // event branches
  std::vector<double> * m_wavelength;
  std::vector<double> * m_energy;
  std::vector<double> * m_na;


};


#endif
