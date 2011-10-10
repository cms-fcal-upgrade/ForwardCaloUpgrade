//
// ====================================================================

#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

// ====================================================================

class TH1D;
class TH2D;
class TTree;

class HistoManager 
{
private:
  static HistoManager* myanalysis;

  public:

    HistoManager();
   ~HistoManager();

    static HistoManager* GetPointer();

    void Book();
    void Clear();
    void Save();

    void FillHisto(G4double, G4double, G4double);
    void FillHistoLayer(G4double*);

  private:

    TH1D*  ph_ecal;
    TH1D*  ph_hcal;
    TH1D*  ph_zero;
  
    TTree*    tree_tot;
    TTree*    tree_vec;

    G4double  e_ecal, e_hcal, e_zero;
    G4double  e_vec[17];
    G4String  fileName ;
};

#endif
//===============================================================
