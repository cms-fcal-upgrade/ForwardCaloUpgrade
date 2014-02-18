// ParticleGunGen generates a single particle (e, mu, pi, p, n, etc) with
//   a fixed momentum and direction.
// Output is an ASCII HepMC file.

// Input and output files are specified on the command line, e.g. like
// ./ParticleGunGen.exe ElectronGunGen.cmnd HepMCoutElectons.dat > out
// The main program contains no analysis; this is intended to happen later.
// It therefore "never" has to be recompiled to handle different tasks.

// Use Pythia to read input file
#include "Pythia.h"
#include "HepMCInterface.h"

#include "HepMC/GenEvent.h"   
#include "HepMC/IO_GenEvent.h"

// Following line is a deprecated alternative, removed in recent versions.
//#include "HepMC/IO_Ascii.h"
//#include "HepMC/IO_AsciiParticles.h"

// Following line to be used with HepMC 2.04 onwards.
//---------------------------------------------------
#ifdef HEPMC_HAS_UNITS
#include "HepMC/Units.h"
#endif

using namespace Pythia8; 

//==========================================================================

// Simple method to do the filling of partons into the event record.
//-------------------------------------------------------------------
//void fillPartons(int type, double ee, Event& event, ParticleData& pdt, Rndm& rndm) {
void fillParticle(int type, double px, double py, double pz, Event& event, ParticleData& pdt) {

//  cout << " inside fillParticle " << endl;

  // Reset event record to allow for new event.
  //-------------------------------------------
  event.reset();

  // Generate particle with px, py, pz
  //--------------------------
//  cout << " type = " << type << endl;
  double mm = pdt.m0(type);
  double pp = sqrtpos(px*px + py*py + pz*pz);
  double ee = sqrtpos(pp*pp + mm*mm);
//  cout << "  px,py,pz = " << px << " " << py << " " << pz << endl;
  event.append( type, 23, 0, 0, px, py, pz, ee, mm); 

}

// Start main 
//-----------

int main(int argc, char* argv[]) {

  // Check that correct number of command-line arguments
  //-----------------------------------------------------
  if (argc != 3) {
    cerr << " Unexpected number of command-line arguments. \n You are"
         << " expected to provide one input and one output file name. \n"
         << " Program stopped! " << endl;
    return 1;
  }

  // Check that the provided input name corresponds to an existing file.
  //--------------------------------------------------------------------
  ifstream is(argv[1]);  
  if (!is) {
    cerr << " Command-line file " << argv[1] << " was not found. \n"
         << " Program stopped! " << endl;
    return 1;
  }

  // Confirm that external files will be used for input and output.
  //---------------------------------------------------------------
  cout << "\n >>> Settings will be read from file " << argv[1] 
       << " <<< \n >>> HepMC events will be written to file " 
       << argv[2] << " <<< \n" << endl;

// Interface for conversion from Pythia8::Event to HepMC one. 
//------------------------------------------------------------
  HepMC::I_Pythia8 ToHepMC;
//  ToHepMC.set_crash_on_problem();

  // Specify file where HepMC events will be stored.
  //-------------------------------------------------
  HepMC::IO_GenEvent ascii_io(argv[2], std::ios::out);

// Following line is a deprecated alternative, removed in recent versions
// HepMC::IO_Ascii ascii_io("hepmcout32.dat", std::ios::out);
// Line below is an eye-readable one-way output, uncomment the include above
// HepMC::IO_AsciiParticles ascii_io("hepmcout32.dat", std::ios::out);
 
// Generator. 
//-----------
  Pythia pythia;
  Event& event      = pythia.event;
  ParticleData& pdt = pythia.particleData;

// Read in commands from external file.
//-------------------------------------
  pythia.readFile(argv[1]);    

// Extract settings to be used in the main program.
//--------------------------------------------------

// Set number of events to generate and to list.
//----------------------------------------------
  int nEvent = pythia.mode("Main:numberOfEvents");
  int nList  = pythia.mode("Main:numberTolist");
  int nAbort = pythia.mode("Main:timesAllowErrors");

// Set particle type and energy.
//----------------------------
  int type =  pythia.mode("Main:spareMode1");
  double px = pythia.parm("Main:spareParm1");
  double py = pythia.parm("Main:spareParm2");
  double pz = pythia.parm("Main:spareParm3");
//  cout << " type,px,py,pz = " << type << " " << px << " " << py << " " << pz <<endl;

// Key requirement: switch off ProcessLevel, and thereby also PartonLevel.
//------------------------------------------------------------------------
  pythia.readString("ProcessLevel:all = off");

// Also allow resonance decays, with showers in them
//---------------------------------------------------
  pythia.readString("Standalone:allowResDec = on");

// Optionally switch off decays.
//pythia.readString("HadronLevel:Decay = off");

// Switch off automatic event listing in favour of manual.
//--------------------------------------------------------
  pythia.readString("Next:numberShowInfo = 0");
  pythia.readString("Next:numberShowProcess = 0");
  pythia.readString("Next:numberShowEvent = 0"); 

// Initialization.
//----------------
  pythia.init();

//  cout << "Before loop." << endl;

// Begin event loop.
//------------------
  int iAbort = 0; 
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

// Set up parton-level configuration.
//-----------------------------------
//    fillPartons( type, ee, event, pdt, pythia.rndm); 
    fillParticle ( type, px, py, pz, event, pdt); 

// Generate event. 
//----------------
    if (!pythia.next()) {

// If failure because reached end of file then exit event loop.
//-------------------------------------------------------------
      if (pythia.info.atEndOfFile()) {
        cout << " Aborted since reached end of Les Houches Event File\n"; 
        break; 
      }

// First few failures write off as "acceptable" errors, then quit.
//----------------------------------------------------------------
      if (++iAbort < nAbort) continue;
      cout << " Event generation aborted prematurely, owing to error!\n"; 
      break;
    }

// List first few events.
//-----------------------
    if (iEvent < nList) {
      event.list();
    }

// Construct new empty HepMC event. 
//---------------------------------
#ifdef HEPMC_HAS_UNITS

// This form with arguments is only meaningful for HepMC 2.04 onwards, 
// and even then unnecessary if HepMC was built with GeV and mm as units.
//-----------------------------------------------------------------------
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);
#else

// This form is needed for backwards compatibility. 
// In HepMCInterface.cc a conversion from GeV to MeV will be done.

   HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
#endif

// Fill HepMC event, including PDF info.
//--------------------------------------
//    ToHepMC.set_convert_to_mev( false );
    ToHepMC.fill_next_event( pythia, hepmcevt );

// This alternative older method fills event, without PDF info.
// ToHepMC.fill_next_event( pythia.event, hepmcevt );

// Write the HepMC event to file. Done with it.
//---------------------------------------------
    ascii_io << hepmcevt;
    delete hepmcevt;

// End of event loop. Statistics. 
//--------------------------------
  }
//TA  pythia.stat();

// Done.
//------
  return 0;
}
