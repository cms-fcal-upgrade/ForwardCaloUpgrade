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
//   $Id: HepMCG4AsciiReader.cc,v 1.1 2012/10/01 15:21:16 andreevv Exp $
//
// ====================================================================
#include "HepMCG4AsciiReader.hh"
#include "HepMCG4AsciiReaderMessenger.hh"

#include <iostream>
#include <fstream>

//#include "HepMCG4Interface.hh"

#include "G4RunManager.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4VPrimaryGenerator.hh"
#include "HepMC/GenEvent.h"

////////////////////////////////////////
HepMCG4AsciiReader::HepMCG4AsciiReader()
  :  filename("xxx.dat"), verbose(0)
////////////////////////////////////////
{
  asciiInput= new HepMC::IO_GenEvent(filename, std::ios::in);
  asciiInput->use_input_units( HepMC::Units::GEV, HepMC::Units::MM );
  messenger= new HepMCG4AsciiReaderMessenger(this);
}

/////////////////////////////////////////
HepMCG4AsciiReader::~HepMCG4AsciiReader()
/////////////////////////////////////////
{
  delete asciiInput;
  delete messenger;
}

/////////////////////////////////////
void HepMCG4AsciiReader::Initialize()
/////////////////////////////////////
{
  delete asciiInput;

  asciiInput= new HepMC::IO_GenEvent(filename, std::ios::in);
  asciiInput->use_input_units( HepMC::Units::GEV, HepMC::Units::MM );
}

/////////////////////////////////////////////////////////
HepMC::GenEvent* HepMCG4AsciiReader::GenerateHepMCEvent()
/////////////////////////////////////////////////////////
{
//  std::cout << "in GenerateHepMCEvent" << std::endl;
  HepMC::GenEvent* evt= asciiInput-> read_next_event();
//  std::cout << "Read an event." << std::endl;
  int ncnt = 0;
  int npos = 0;
  int nneg = 0;
  HepMC::GenEvent::vertex_const_iterator tmpvitr= evt->vertices_begin();
//  std::cout << " didn't crash yet." << std::endl;
//  HepMC::GenEvent::vertex_const_iterator tmpvitr2= evt->vertices_end();
//  if (tmpvitr==tmpvitr2) { std::cout << "wow vertex list is empty" << std::endl;}
//  for(tmpvitr = evt->vertices_begin();
//      tmpvitr != evt->vertices_end(); ++tmpvitr ) { // loop for vertex ...
//  if (tmpvitr==0) std::cout << " no vitr " << std::endl;
//  if (tmpvitr) std::cout << " vitr found " << std::endl;
  for(HepMC::GenEvent::vertex_const_iterator vitr= evt->vertices_begin();
      vitr != evt->vertices_end(); ++vitr ) { // loop for vertex ...
//    std::cout << " vtx found " << std::endl;
    for (HepMC::GenVertex::particle_iterator
             vpitr= (*vitr)->particles_begin(HepMC::children);
           vpitr != (*vitr)->particles_end(HepMC::children); ++vpitr) {
//      std::cout << " particle " << ncnt << " found " << std::endl;
      ncnt++;
      if ((*vpitr)->momentum().pz() < 0) nneg++;
      if ((*vpitr)->momentum().pz() > 0) npos++;
    }
  }
//  std::cout << "  ncnt = " << ncnt << " npos = " << npos << 
//               " nneg = " << nneg << std::endl;

  if(!evt) return 0; // no more event

  if(verbose>0) evt-> print();
    
  return evt;
}

