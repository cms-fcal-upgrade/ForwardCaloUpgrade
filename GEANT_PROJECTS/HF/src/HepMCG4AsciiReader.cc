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
  HepMC::GenEvent* evt= asciiInput-> read_next_event();
  if(!evt) return 0; // no more event

  if(verbose>0) evt-> print();
    
  return evt;
}

