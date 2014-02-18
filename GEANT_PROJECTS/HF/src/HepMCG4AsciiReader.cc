//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// ====================================================================
//
//   HepMCG4AsciiReader.cc
//   $Id: HepMCG4AsciiReader.cc,v 1.2 2013/04/23 17:44:07 heli Exp $
//
// ====================================================================
#include "HepMCG4AsciiReader.hh"
#include "HepMCG4AsciiReaderMessenger.hh"

#include "G4RunManager.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4Event.hh"
#include "G4LorentzVector.hh"
#include "G4TransportationManager.hh"

#include <iostream>
#include <fstream>

////////////////////////////////////////
HepMCG4AsciiReader::HepMCG4AsciiReader()
  :  filename("xxx.dat"), verbose(0),firstEvent(0)
////////////////////////////////////////
{
  asciiInput= new HepMC::IO_GenEvent(filename, std::ios::in);
  asciiInput->use_input_units( HepMC::Units::GEV, HepMC::Units::MM );
  messenger= new HepMCG4AsciiReaderMessenger(this);
  hepmcEvent = NULL;
}

/////////////////////////////////////////
HepMCG4AsciiReader::~HepMCG4AsciiReader()
/////////////////////////////////////////
{
  delete asciiInput;
  delete messenger;
  delete hepmcEvent;
}

/////////////////////////////////////
void HepMCG4AsciiReader::Initialize()
/////////////////////////////////////
{
  delete asciiInput;

  asciiInput= new HepMC::IO_GenEvent(filename, std::ios::in);
  asciiInput->use_input_units( HepMC::Units::GEV, HepMC::Units::MM );

  GotoFirstEvent();
}


////////////////////////////////////////////////
void HepMCG4AsciiReader::GotoFirstEvent()
////////////////////////////////////////////////
{
  for ( G4int i=0; i != firstEvent; i++ ) {
    HepMC::GenEvent* evt= asciiInput->read_next_event();
  }
}


/////////////////////////////////////////////////////////
G4bool HepMCG4AsciiReader::CheckVertexInsideWorld
                         (const G4ThreeVector& pos) const
/////////////////////////////////////////////////////////
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()
                                                 -> GetNavigatorForTracking();

  G4VPhysicalVolume* world= navigator-> GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()-> GetSolid();
  EInside qinside= solid-> Inside(pos);

  if( qinside != kInside) return false;
  else return true;
}

/////////////////////////////////////////////////////////
HepMC::GenEvent* HepMCG4AsciiReader::GenerateHepMCEvent()
/////////////////////////////////////////////////////////
{
  HepMC::GenEvent* evt= asciiInput->read_next_event();
  if(!evt) return evt; // no more event

  if(verbose>0) evt-> print();
    
  return evt;
}

//////////////////////////////////////////////////////////////
void HepMCG4AsciiReader::GeneratePrimaryVertex(G4Event* anEvent)
//////////////////////////////////////////////////////////////
{

  // delete previous event object
  if (hepmcEvent) {
    delete hepmcEvent;
  }

  // generate next event
  hepmcEvent= GenerateHepMCEvent();
  if(! hepmcEvent) {
    G4cout << "HepMCG4AsciiReader: no generated particles. run terminated..."
           << G4endl;
    G4RunManager::GetRunManager()-> AbortRun();
    return;
  }
  HepMC2G4(hepmcEvent, anEvent);
}


////////////////////////////////////////////////////////////////
void HepMCG4AsciiReader::HepMC2G4(const HepMC::GenEvent* hepmcevt,
                                G4Event* g4event)
////////////////////////////////////////////////////////////////
{
  for(HepMC::GenEvent::vertex_const_iterator vitr= hepmcevt->vertices_begin();
      vitr != hepmcevt->vertices_end(); ++vitr ) { // loop for vertex ...

    // real vertex?
    G4bool qvtx=false;
    for (HepMC::GenVertex::particle_iterator
           pitr= (*vitr)->particles_begin(HepMC::children);
         pitr != (*vitr)->particles_end(HepMC::children); ++pitr) {

      if (!(*pitr)->end_vertex() && (*pitr)->status()==1) {
        qvtx=true;
        break;
      }
    }
    if (!qvtx) continue;

    // check world boundary
    HepMC::FourVector pos= (*vitr)-> position();
    G4LorentzVector xvtx(pos.x(), pos.y(), pos.z(), pos.t());
    if (! CheckVertexInsideWorld(xvtx.vect()*mm)) continue;

    // create G4PrimaryVertex and associated G4PrimaryParticles
    G4PrimaryVertex* g4vtx=
      new G4PrimaryVertex(xvtx.x()*mm, xvtx.y()*mm, xvtx.z()*mm,
                          xvtx.t()*mm/c_light);

    for (HepMC::GenVertex::particle_iterator
           vpitr= (*vitr)->particles_begin(HepMC::children);
         vpitr != (*vitr)->particles_end(HepMC::children); ++vpitr) {

      if( (*vpitr)->status() != 1 ) continue;

      G4int pdgcode= (*vpitr)-> pdg_id();
      pos= (*vpitr)-> momentum();
      if( pos.pz() < 0.0 ) continue; // take only jet along z+ axis 
      G4LorentzVector p(pos.px(), pos.py(), pos.pz(), pos.e());
      G4PrimaryParticle* g4prim=
        new G4PrimaryParticle(pdgcode, p.x()*GeV, p.y()*GeV, p.z()*GeV);

      g4vtx-> SetPrimary(g4prim);
    }
    g4event-> AddPrimaryVertex(g4vtx);
  }
}

