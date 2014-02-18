TSplineFit* BCF91A_Abs(Double_t factor = 1.0, Bool_t todraw = kFALSE,Bool_t infile = kFALSE,Bool_t firstinfile = kFALSE)
//
// Arguments:
//
//  todraw       : true if the fit is to be drawn
//  infile       : true if the fit is to be put in file SplineFitDB.rdb
//  firstinfile  : if true, [BE CAREFUL: RISK OF LOOSING FILE SplineFitDB.rdb ]
//                 delete all fits present in file SplineFitDB.rdb and place
//                 this fit as the first fit in a new file SplineFitDB.rdb
//                 default false !!!
// All defaults for arguments correspond to the case where one calls this CINT
//macro from within a Litrani CINT macro to define a needed fit, instead of trying
//to find it in the database file SplineFitDB.rdb using TSplineFit::FindFit()
//
//  AbsLengthWLS_Fibre
//
//  Absorption length (or diffusion length,as you like) of Plastic Fibre with wavelength shifting
//
//  Source : David Wahl
//
{
  Int_t k1;
  Int_t k2 = -100;
  k1 = TClassTable::GetID("TSplineFit");
  if (k1<0) k2 = gSystem.Load("libSplineFit");
  const Int_t M = 42;
  const Int_t m = 1;
  Int_t i;
  TSplineFit *WLSLength;
  Double_t x[M]= {350.58,356.83,371.47,373.59,377.00,386.03,392.38,396.52,401.39,402.04,406.92,412.47,416.06,419.44,423.74,427.09,428.57,429.31,434.11,441.06,447.26,448.72,450.84,451.49,452.98,453.64,454.30,455.64,456.31,459.86,463.99,472.47,475.36,480.33,485.41,490.40,495.29,500.28,505.38,510.37,515.46,520.43 };
  Double_t y[M]= {0.28,0.28,0.26,0.25,0.24,0.21,0.19,0.16,0.13,0.13,0.14,0.11,0.08,0.05,0.02,0.05,0.08,0.10,0.13,0.10,0.08,0.07,0.08,0.11,0.13,0.16,0.19,0.21,0.24,0.27,0.30,2.69,3.49,3.99,5.00,11.6,21.6,33.1,175.0,393.0,617.0,794.0 };
  if (factor != 1.0) {
    cout << "=====================================" << endl;
    cout << "Updating spectrum with factor: ";
    cout << factor << endl;
    cout << "=====================================" << endl;
    for (int i = 0; i < M; i++)
      y[i] = factor*y[i];
  }
  WLSLength = new TSplineFit("BCF91A_Abs","Absorption Length for BCF91A | Fibre with WLS",20, M, x, y, 300.0, 800.0);
  WLSLength = new TSplineFit("AbsLengthWLS_BCF91A","Absorption Length for BCF91A | Fibre with WLS",20, M, x, y, 300.0, 800.0);
  WLSLength->SetSource("Saint-Gobain Documentation");
  WLSLength->SetSource("Saint-Gobain Documentation");
  WLSLength->SetMacro("BCF91A_Abs.C");
  WLSLength->SetXLabel("Wavelength [nm]");
  WLSLength->SetVLabel("Absorption length with WLS [cm]");
  if (todraw) {
    WLSLength->SetDefaultLabels();
    WLSLength->DrawFit();
    WLSLength->Print();
  }
  if (infile) {
    if (firstinfile) WLSLength->UpdateFile(kTRUE);
    else             WLSLength->UpdateFile(kFALSE);
  }
  printf("BCF91A_Abs Loaded!!!\n");
  return WLSLength;
}
