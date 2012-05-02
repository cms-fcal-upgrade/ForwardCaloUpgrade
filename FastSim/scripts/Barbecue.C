
double dp, da; // thickness in cm
double rho_p, rho_a; // density in g/cm3
double Zp, Za;
double Ap, Aa;
double X0p, X0a; // raditation length (g/cm2)
double Ecp, Eca; // Critical energy in MeV
double RMp, RMa; // Molere radius (cm)
double Lambda_p, Lambda_a; // Radiation length 
double LumiYield_a; // photons / GeV
double LightCollection; // % of light collected
double nonUnif; // non uniformity across the shashlik lengh pf light collected
double stochasticResolution; // stochastic term corresponding to sigmaE/E = a/sqrt(E)

// root -l
// .x ../scripts/Barbecue.C("../python/Detectors/Shashlik_PbLSO.in", "PbLSO")


/*
double dp = 0.8, da = 0.2; // thickness in cm
double rho_p = 11.34, rho_a = 7.4; // density in g/cm3
double Zp = 82., Za = 56.6;
double Ap = 207.2, Aa = 138.2;
double X0p = 6.36, X0a = 8.52; // raditation length (g/cm2)
double Ecp = 7.3, Eca = 10.5; // Critical energy in MeV
double RMp = 1.63, RMa = 2.32; // Molere radius (cm)
double Lambda_p = 18.3, Lambda_a = 21.09; // Radiation length 
double LumiYield_a = 1610E3; // photons / GeV
double LightCollection = 0.01; // % of light collected
double nonUnif = 0.00; // non uniformity across the shashlik lengh pf light collected
double stochasticResolution = 0.136; // stochastic term corresponding to sigmaE/E = a/sqrt(E)
*/

void Barbecue(string sIn, string title){

  string inTxt(sIn);
  ifstream fin(inTxt.c_str());
  
  string s1, s2;

  fin >> s1 >> s2 >> dp;
  fin >> s1 >> s2 >> da;
  fin >> s1 >> s2 >> rho_p;
  fin >> s1 >> s2 >> rho_a;
  fin >> s1 >> s2 >> Zp;
  fin >> s1 >> s2 >> Za;
  fin >> s1 >> s2 >> Ap;
  fin >> s1 >> s2 >> Aa;
  fin >> s1 >> s2 >> X0p;
  fin >> s1 >> s2 >> X0a;
  fin >> s1 >> s2 >> Ecp;
  fin >> s1 >> s2 >> Eca;
  fin >> s1 >> s2 >> RMp;
  fin >> s1 >> s2 >> RMa;
  fin >> s1 >> s2 >> Lambda_p;
  fin >> s1 >> s2 >> Lambda_a;
  fin >> s1 >> s2 >> LumiYield_a;
  fin >> s1 >> s2 >> LightCollection;
  fin >> s1 >> s2 >> nonUnif;
  fin >> s1 >> s2 >> stochasticResolution;

  fin.close();




  double ds = dp+da;
  double rhod_p = dp*rho_p, rhod_a = da*rho_a; // used for weight calculation (g/cm2)
  double rhod_s = (rhod_p + rhod_a);
  double rho_s = rhod_s/ds;


  double wp = rhod_p/rhod_s, wa = rhod_a/rhod_s; // weights
  double ws = wp + wa;

  
  double Zs = wp*Zp + wa*Za;
  double As = wp*Ap + wa*Aa;

  double X0s = 1./(wp/X0p + wa/X0a);
  double X0ls = X0s/rho_s, X0la = X0a/rho_a, X0lp = X0p/rho_p;

  double RMs = 1./(wp/RMp + wa/RMa);

  double Ecs = X0s*(wp/X0p*Ecp + wa/X0a*Eca);

  double Fs = X0s/rhod_s;

  double ehat = 1./(1+0.007*(Zp-Za));

  double Lambda_s = 1./(wp/Lambda_p + wa/Lambda_a);

  double LumiYield_s = LumiYield_a*wa;

  cout << "" << endl;
  cout << "" << endl;
  cout << "Shashlik: " << title.c_str() << endl;
  cout << "----------------------------------------------------------------" << endl;
  cout << "Variable  \t\tAbsorber\tActive\tShashlik " << endl;
  cout << "----------------------------------------------------------------" << endl;
  cout << "d (cm)  \t\t" << dp << "\t\t" << da << "\t" << Form("%.2f",ds) << endl;
  cout << "rho (g/cm3)\t\t" << Form("%.2f",rho_p) << "\t\t" << Form("%.2f",rho_a) << "\t" << Form("%.2f",rho_s) << endl;
  cout << "rhod (g/cm2)\t\t" << Form("%.2f",rhod_p) << "\t\t" <<  Form("%.2f",rhod_a) << "\t" << Form("%.2f",rhod_s) << endl;
  cout << "w (%)     \t\t" <<  Form("%.2f",wp*100) << "\t\t" << Form("%.2f",wa*100) << "\t" <<  Form("%.2f",ws*100) << endl;
  cout << "----------------------------------------------------------------" << endl;
  cout << "Z         \t\t" <<  Form("%.2f",Zp) << "\t\t" << Form("%.2f",Za) << "\t" <<  Form("%.2f",Zs) << endl;
  cout << "A         \t\t" <<  Form("%.2f",Ap) << "\t\t" << Form("%.2f",Aa) << "\t" <<  Form("%.2f",As) << endl;
  cout << "X0 (g/cm2)\t\t" <<  Form("%.2f",X0p) << "\t\t" << Form("%.2f",X0a) << "\t" << Form("%.2f",X0s) << endl;
  cout << "X0 (cm)   \t\t" <<  Form("%.2f",X0lp) << "\t\t" << Form("%.2f",X0la) << "\t" << Form("%.2f",X0ls) << endl;
  cout << "RM (cm)   \t\t" << RMp << "\t\t" << RMa << "\t" << Form("%.2f",RMs) << endl;

  cout << "Ec (MeV)  \t\t" << Ecp << "\t\t" << Eca << "\t" << Form("%.2f",Ecs) << endl;
  cout << "Fs      \t\t" << "" << "\t\t" << "" << "\t" << Form("%.2f",Fs) << endl;
  cout << "ehat    \t\t" << "" << "\t\t" << "" << "\t" << Form("%.2f",ehat) << endl;

  cout << "Lambda (cm)\t\t" <<  Form("%.2f",Lambda_p) << "\t\t" <<  Form("%.2f",Lambda_a) << "\t" <<  Form("%.2f",Lambda_s) << endl;
  cout << "----------------------------------------------------------------" << endl;
  cout << "Lumi Yield (gamma/GeV) \t" << LumiYield_a << "\t"  << "\t" << Form("%.2E",LumiYield_s) << endl;
  cout << "Light coll. (%) \t\t" << "" << "\t"  << "\t" << Form("%.2f",LightCollection*100) << endl;
  cout << "Non Unif. (%) \t\t" << "" << "\t\t"  << "\t" << Form("%.2f",nonUnif) << endl;
  cout << "Stoch. sigma (%)\t" << "" << "\t"  << "\t\t" << Form("%.2f",stochasticResolution*100) << endl;
  cout << "----------------------------------------------------------------" << endl;


  string outTxt("../python/Detectors/Shashlik_"); outTxt = outTxt + "" + title + ".txt";
  ofstream fout(outTxt.c_str());
  
  fout << "Shashlik: " << title.c_str() << endl;
  fout << "----------------------------------------------------------------" << endl;
  fout << "Variable  \t\tAbsorber\tActive\tShashlik " << endl;
  fout << "----------------------------------------------------------------" << endl;
  fout << "d (cm)  \t\t" << dp << "\t\t" << da << "\t" << Form("%.2f",ds) << endl;
  fout << "rho (g/cm3)\t\t" << Form("%.2f",rho_p) << "\t\t" << Form("%.2f",rho_a) << "\t" << Form("%.2f",rho_s) << endl;
  fout << "rhod (g/cm2)\t\t" << Form("%.2f",rhod_p) << "\t\t" <<  Form("%.2f",rhod_a) << "\t" << Form("%.2f",rhod_s) << endl;
  fout << "w (%)     \t\t" <<  Form("%.2f",wp*100) << "\t\t" << Form("%.2f",wa*100) << "\t" <<  Form("%.2f",ws*100) << endl;
  fout << "----------------------------------------------------------------" << endl;
  fout << "Z         \t\t" <<  Form("%.2f",Zp) << "\t\t" << Form("%.2f",Za) << "\t" <<  Form("%.2f",Zs) << endl;
  fout << "A         \t\t" <<  Form("%.2f",Ap) << "\t\t" << Form("%.2f",Aa) << "\t" <<  Form("%.2f",As) << endl;
  fout << "X0 (g/cm2)\t\t" <<  Form("%.2f",X0p) << "\t\t" << Form("%.2f",X0a) << "\t" << Form("%.2f",X0s) << endl;
  fout << "X0 (cm)   \t\t" <<  Form("%.2f",X0lp) << "\t\t" << Form("%.2f",X0la) << "\t" << Form("%.2f",X0ls) << endl;
  fout << "RM (cm)   \t\t" << RMp << "\t\t" << RMa << "\t" << Form("%.2f",RMs) << endl;

  fout << "Ec (MeV)  \t\t" << Ecp << "\t\t" << Eca << "\t" << Form("%.2f",Ecs) << endl;
  fout << "Fs      \t\t" << "" << "\t\t" << "" << "\t" << Form("%.2f",Fs) << endl;
  fout << "ehat    \t\t" << "" << "\t\t" << "" << "\t" << Form("%.2f",ehat) << endl;

  fout << "Lambda (cm)\t\t" <<  Form("%.2f",Lambda_p) << "\t\t" <<  Form("%.2f",Lambda_a) << "\t" <<  Form("%.2f",Lambda_s) << endl;
  fout << "----------------------------------------------------------------" << endl;
  fout << "Lumi Yield (gamma/GeV) \t" << LumiYield_a << "\t"  << "\t" << Form("%.2E",LumiYield_s) << endl;
  fout << "Light coll. (%) \t\t" << "" << "\t"  << "\t" << Form("%.2f",LightCollection*100) << endl;
  fout << "Non Unif. (%) \t\t" << "" << "\t\t"  << "\t" << Form("%.2f",nonUnif) << endl;
  fout << "Stoch. sigma (%)\t" << "" << "\t"  << "\t\t" << Form("%.2f",stochasticResolution*100) << endl;
  fout << "----------------------------------------------------------------" << endl;

  fout.close();









  string outCFF("../python/Shashlik_"); outCFF = outCFF + "" + title + "_cff.py";

  ofstream cffout(outCFF.c_str());

  cffout << "import FWCore.ParameterSet.Config as cms\n" << endl;
  
  cffout << "def myForwardECAL( process ):\n" << endl; 
  
  cffout << "\t#======  Geometrical material properties ========\n" << endl;
    
  cffout << "\t# Light Collection efficiency" << endl;

  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightColl = cms.double(" << Form("%.2f",LightCollection) << ")" << endl;
  cffout << "###    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightColl = cms.double(0.999)\n" << endl;

  cffout << "\t# Light Collection uniformity" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightCollUnif = cms.double(" << Form("%.4f",nonUnif) << ")" << endl;
  cffout << "###    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.lightCollUnif = cms.double(0.00001)\n" << endl;

  cffout << "\t# Photostatistics (photons/GeV) in the homegeneous material." << endl;
  cffout << "\t# estimate the photo-electron fluctuations." << endl;
  cffout << "\t# Be carefull when you sumulate to keep: photoStatistics*lightColl*ParticleEnergy<2^31. Otherwise TRandom3 would refuse to" << endl;
  cffout << "\t## photoStatistics = cms.double(50.E3)," << endl;


  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.photoStatistics = cms.double(" << Form("%.2E",LumiYield_s) << ")" << endl;


  cffout << "\t###    process.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.photoStatistics = cms.double(1.E7)" << endl;
  cffout << "\n\n" << endl;

  cffout << "\t# Thickness of the detector in cm" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.thickness = cms.double(22.0)\n" << endl;



  cffout << "\t    #====== Global parameters of the material ========\n" << endl;
  cffout << "\t# Interaction length in cm provided by A. Ledovskoy" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.interactionLength  = cms.double(" << Form("%.2f",Lambda_s) << ")" << endl;


  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.Aeff = cms.double(" << Form("%.2f",As) << ")" << endl;

  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.Zeff = cms.double(" << Form("%.2f",Zs) << ")" << endl;

  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.rho = cms.double(" << Form("%.2f",rho_s) << ")\n\n" << endl;

  cffout << "\t# Radiation length in g/cm^2" << endl;
  cffout << "\t# Interaction length in cm see Zhu \"Crystal Calorimeters in the Next" << endl;
  cffout << "\t# decade\"" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.radLenIngcm2 = cms.double(" << Form("%.2f",X0s) << ")\n\n" << endl;



  cffout << "\t# ===== Those parameters might be entered by hand" << endl;
  cffout << "\t# or calculated out of the previous ones\n" << endl; 
  
  cffout << "\t# Radiation length in cm. If value set to -1, FastSim uses internally the" << endl;
  cffout << "\t# formula radLenIngcm2/rho" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.radLenIncm = cms.double(-1)" << endl;
  cffout << "\t# Critical energy in GeV. If value set to -1, FastSim uses internally the" << endl;
  cffout << "\t# formula (2.66E-3*(x0*Z/A)^1.1): 8.74E-3 for ECAL EndCap" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.criticalEnergy = cms.double(" << Form("%.4f",Ecs/1000.) << ")" << endl;
  cffout << "\t# Moliere Radius in cm.If value set to -1, FastSim uses internally the" << endl;
  cffout << "\t# formula : Es/criticalEnergy*X0 with Es=sqrt(4*Pi/alphaEM)*me*c^2=0.0212 GeV" << endl;
  cffout << "\t# This value is known to be 2.190 cm for ECAL Endcap, but the formula gives 2.159 cm" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.moliereRadius = cms.double(" << Form("%.2f",RMs) << ")" << endl;
  cffout << "\t" << endl;
  cffout << "\t# Is a homogenious detector?" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.bHom = cms.bool(False)" << endl;
  cffout << "\t" << endl;
  cffout << "\t#====== Parameters for sampling ECAL ========" << endl;
  cffout << "\t" << endl;
  cffout << "\t# Sampling Fraction: Fs = X0eff/(da+dp) where X0eff is the average X0" << endl;
  cffout << "\t# of the active and passive media and da/dp their thicknesses" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.Fs = cms.double(" << Form("%.2f",Fs) << ")" << endl;
  cffout << "\t" << endl;
  cffout << "\t# e/mip for the calorimeter. May be estimated by 1./(1+0.007*(Zp-Za))" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.ehat = cms.double(" << Form("%.2f",ehat) << ")" << endl;
  cffout << "\t" << endl;
  cffout << "\t# a rough estimate of ECAL resolution sigma/E = resE/sqrt(E)" << endl;
  cffout << "\t# it is used to generate Nspots in radial profiles." << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.resE = cms.double(" << Form("%.3f",stochasticResolution) << ")\n" << endl;
  
  cffout << "\t# the width in cm of the active layer" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.da = cms.double(" << Form("%.2f",da) << ")" << endl;
  cffout << "\t" << endl;
  cffout << "\t# the width in cm of the passive layer" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.dp = cms.double(" << Form("%.2f",dp) << ")\n\n\n" << endl;
  cffout << "\t# Activate the LogDebug" << endl;
  cffout << "\tprocess.famosSimHits.Calorimetry.CalorimeterProperties.EndcapCalorimeterProperties.debug = cms.bool(True)" << endl;
  
  cffout.close();

}
