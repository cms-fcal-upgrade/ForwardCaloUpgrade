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

    void FillHisto(G4double, G4double, G4double, G4double);
    void FillHistoLayer(G4double*);
    void FillHistoRange(G4double*);
    void FillHistoCell(G4double*);

  private:

    TH1D*  ph_ecal;
    TH1D*  ph_hcal;
    TH1D*  ph_zero;
  
    TTree*    tree_tot;
    TTree*    tree_vec;
    TTree*    tree_ran;
    TTree*    tree_cel;

    G4double  e_ecal, e_hcal, e_zero, e_rang;
    G4double  e_vec[17], r_vec[17], e_cell[1700];

    G4String  fileName ;
};

#endif
//===============================================================
