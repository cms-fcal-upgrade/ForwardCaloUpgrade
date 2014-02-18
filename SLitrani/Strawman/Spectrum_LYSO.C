TLitSpectrum* Spectrum_LYSO(Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
{
//
//  The time components are not accurate. I chose them to get this thing running!
//
  char c;
  Bool_t ok;
  TLitSpectrum *sp;
  sp = new TLitSpectrum("Spectrum_LYSO","Emission Spectrum | LYSO");
  sp->AddOneComponent(       403.439895802,       12.7492678646,       2063.19331493,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(        425.18395412,       10.4384874441,       1308.06849004,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(       436.155159851,        8.2175338303,       244.685482817,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(       452.981065852,       9.83477425888,       538.305924265,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(       386.314691762,       9.13732068716,       957.088321278,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(       442.127712979,         8.045292436,       292.192502725,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(       470.353513305,       10.2669611197,       198.884029154,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(       467.195333502,       29.3611160285,       216.745833206,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(       486.794791357,       15.1565951473,       94.6449700491,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(         610.0124608,       36.8928302369,       35.0290262852,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->AddOneComponent(       512.449212168,       36.0031430044,       232.906821847,12.0,-1.0,0.5,12.0,-1.0,0.5);
  sp->Normalize();
  sp->SetMacro("Spectrum_LYSO.C");
//Up to 10 lines of comments are allowed. We write only 2 here
  sp->fComment[0] = "Data from Ren-Yuan Zhu";
  sp->fComment[1] = "LYSO Emission Spectrum, from file cti-lyso_em_2a.txt";
  if (todraw) {
    if (!gTwoPad) {
      TLit::Get() = new TLit(5);
      TLit::Get()->BookCanvas();
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

