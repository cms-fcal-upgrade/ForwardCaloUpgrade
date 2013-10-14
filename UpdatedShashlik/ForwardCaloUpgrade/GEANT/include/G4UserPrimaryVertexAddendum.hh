/////////////////////////////////////////
//
//  G4UserPrimaryVertexAddendum.hh
//  
//  Andrew Ackert
//  Florida State University
//
//  9/22/13
//
//
//
// The purpose of this library is to construct
// a concrete header that will utilize the 
// abstract header G4VUserPrimaryVertexInformation.hh
// that is in the source code of Geant4
//
/////////////////////////////////////////////

#ifndef G4UserPrimaryVertexAddendum_h
#define G4UserPrimaryVertexAddendum_h 1

#include "globals.hh"
#include "G4VUserPrimaryVertexInformation.hh"
#include "G4PrimaryVertex.hh"

class G4VUserPrimaryVertexInformation;

class G4UserPrimaryVertexAddendum : public G4VUserPrimaryVertexInformation
{
  
public:
  
  
  G4UserPrimaryVertexAddendum(){;}
  ~G4UserPrimaryVertexAddendum(){;}

private:

  G4int barcode;

public:


  inline void SetBarcode( G4int bcode )
  { barcode = bcode ;}
  inline G4int GetBarcode()
  {return barcode;}

  virtual void Print() const{
    G4cout << "print doesnt do anything yet" << G4endl;
  }
};

#endif

  
