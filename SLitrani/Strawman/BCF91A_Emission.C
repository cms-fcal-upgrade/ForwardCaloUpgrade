TLitSpectrum* BCF91A_Emission(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
//TLitSpectrum* BCF91A_Emission(Bool_t todraw = kTRUE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
{
//
//
  char c;
  Bool_t ok;
  TLitSpectrum *sp;
  sp = new TLitSpectrum("BCF91A_Emission","Emission Spectrum | BCF91A (Roughly)");
  //  sp->AddOneComponent(480.0,5.0,1.0,12.0,-1.0,0.5,12.0,-1.0,0.44);
  sp->AddOneComponent(494.0,12.0,0.9,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(520.0,25.0,0.85.0,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(565.0,10.0,0.05.0,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->Normalize();
  sp->SetMacro("BCF91A_Absorption.C");
  sp->fComment[0] = "This is a very rough approx to the actual spectrum.";
  if (todraw) {
    if (!gTwoPad) {
      gLit = new TLitGlob(5);
      gLit->BookCanvas();
    }
    
    sp->DrawDecayTimes();
    sp->DrawSpectrum();
    sp->Print();
  }
  if (infile) {
    if (firstinfile) sp->UpdateFile(kTRUE);
    else             sp->UpdateFile(kFALSE);
  }
  return sp;
}
