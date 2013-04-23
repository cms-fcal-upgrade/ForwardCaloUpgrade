#ifndef HEPMC_G4_ASCII_READER_H
#define HEPMC_G4_ASCII_READER_H


#include "G4VPrimaryGenerator.hh"
#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"

class HepMCG4AsciiReaderMessenger;

class HepMCG4AsciiReader : public G4VPrimaryGenerator {
protected:
  G4String filename;
  HepMC::IO_GenEvent* asciiInput;

  G4int verbose;
  HepMCG4AsciiReaderMessenger* messenger;

  // Note that the life of HepMC event object must be handled by users.
  // In the default implementation, a current HepMC event will be
  // deleted at GeneratePrimaryVertex() in the next event.
  HepMC::GenEvent* hepmcEvent; // (care for single event case only)

  HepMC::GenEvent* GenerateHepMCEvent();

  // We  have to take care for the position of primaries because
  // primary vertices outside the world voulme give rise to G4Execption.
  // If the default implementation is not adequate, an alternative 
  // can be implemented in your own class.
  G4bool CheckVertexInsideWorld(const G4ThreeVector& pos) const;

  // service method for conversion from HepMC::GenEvent to G4Event
  void HepMC2G4(const HepMC::GenEvent* hepmcevt, G4Event* g4event);

public:
  HepMCG4AsciiReader();
  ~HepMCG4AsciiReader();

  // set/get methods
  void SetFileName(G4String name);
  G4String GetFileName() const;

  void SetVerboseLevel(G4int i);
  G4int GetVerboseLevel() const; 

  // methods...
  void Initialize();

  // The default behavior is that a single HepMC event generated by
  // GenerateHepMCEvent() will be converted to G4Event through HepMC2G4().
  void GeneratePrimaryVertex(G4Event* anEvent);


};

// ====================================================================
// inline functions
// ====================================================================

inline void HepMCG4AsciiReader::SetFileName(G4String name)
{
  filename= name;
}

inline G4String HepMCG4AsciiReader::GetFileName() const
{
  return filename;
}

inline void HepMCG4AsciiReader::SetVerboseLevel(G4int i)
{
  verbose= i;
}

inline G4int HepMCG4AsciiReader::GetVerboseLevel() const
{
  return verbose;
}

#endif
