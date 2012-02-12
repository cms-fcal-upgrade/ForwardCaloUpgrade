//
// ====================================================================

#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

// ====================================================================

class TH1D;
class TH2D;
class TTree;

 const G4int  nhist = 7; 

class HistoManager 
{
private:
  static HistoManager* myanalysis;

  public:

    HistoManager();
   ~HistoManager();

    static HistoManager* GetPointer();

    void Book(G4double,G4int,G4double,G4int,G4double,G4int,G4double);
    void Clear();
    void Save();

    void FillHisto(G4double, G4double, G4double, G4double, G4double);
    void FillHcalTransShape(G4double*);
    void FillHcalLongShape(G4double*);
    void FillTransShape(G4double*);
    void FillLongShape(G4double*);

  private:

    TH1D*  histo[nhist];

    TTree*    tree_tot;
    TTree*    tree_vec;

//    G4int     hist3, hist4;
    G4int     nhLtot, nhRtot, nhRtotc;
    G4double  dhLbin, dhRbin, dhRbinc;
    G4double  EdepEcalRad, EdepEcalLong;
    G4double  EdepHcalRad, EdepHcalLong;

    G4double  e_ecal, e_hcal, e_zero, r_hcal, r_ecal;
    G4double  e_vec[17];

    G4String  fileName ;
};


#endif
//===============================================================
