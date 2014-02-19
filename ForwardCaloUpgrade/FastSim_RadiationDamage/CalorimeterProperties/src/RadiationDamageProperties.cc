// Class containing radiation damage calculations

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FastSimulation/CalorimeterProperties/interface/RadiationDamageProperties.h"
#include <math.h>

#include <iostream>

void RadiationDamageProperties::setLumi(const edm::ParameterSet& rad)
{
  isDamaged_ = rad.getParameter<bool>("isDamaged");
  lumiTotal_ = rad.getParameter<double>("lumiTotal");
  lumiInstant_ = rad.getParameter<double>("lumiInstant");
}

// Calculate dose at 500/fb
double RadiationDamageProperties::doseProfile(double eta) const
{
  double x = fabs(eta);
  double dose_profile = (x < 1.489) ? exp(-4.11065 + 0.258478*x) : exp(-13.5112 + 7.913860*x - 0.998649*x*x);

  return dose_profile;
}

// electromagnetic damages to relative light yield
double RadiationDamageProperties::relLY_EM(double eta) const
{
  double dose_rate = 5.0*doseProfile(eta)*lumiInstant_/1.e+34;
  double relLY_EM = 1.0 - 0.316*(1.32*dose_rate/1.9)/(0.054 + 1.32*dose_rate/1.9);

  return relLY_EM;
}

// hadronic damages to relative light yield
double RadiationDamageProperties::relLY_Had(double eta, double L) const
{
  double flux = doseProfile(eta)*2.7e+13/500.0*lumiTotal_;
  if(flux < 1.e-5) flux = 1.e-5;

  double mu_ind = 2.08e-13*pow(flux, 1.0049);
  double relLY_Had = exp(-1.*mu_ind*L);

  return relLY_Had;
}

// total damage to relative light yield
double RadiationDamageProperties::relativeLightYield(double eta, double L) const
{
  double rel_LY = (isDamaged_ == true) ? relLY_Had(eta, L)*relLY_EM(eta) : 1.;
  if(rel_LY > 1.) rel_LY = 1.;
  else if(rel_LY < 1.e-300) rel_LY = 1.e-300;
if(L < 0.219 || L > 0.231) std::cout << "RadiationDamageProperties::relativeLightYield( " << eta << " , " << L << " )" << std::endl;
  return rel_LY;
}



//____________________________________________________________
double RadiationDamageProperties::InducedAbsorptionEM(double lumi, double eta) const
{
  double mu_max  = 2.0;
  double logL = 5.0*doseProfile(eta)*lumi/1e+34;
  if(lumi<1e-6) logL = 1e-6;
  logL = log10(logL);
  double muEM = mu_max/(1.0 + exp(-2.2095*(logL-8.95476e-01))); 
  return muEM;
}

//_____________________________________________________
double RadiationDamageProperties::LightCollectionEfficiency(double z, double mu) const
{
  double f = 0;
  if(z<=0) return f;
  if(z>=0.22) return f;
  
  double e0 =  6.91563e-02;
  double e1 =  1.64406e+00;
  double e2 =  6.42509e-01;
  double E =  e0/(1+exp(e1*(log10(mu)-e2)));

  double d0 =  3.85334e-01;
  double d1 = -1.04647e-02;
  double D = d0*exp(d1*mu);

  double c0 =  3.77629e-01;
  double c1 = -3.23755e-01;
  double c2 =  1.50247e+00;
  double c3 =  3.03278e-01;
  double C =  -1 + c0*exp(c1*mu)*(1+c2*exp(c3*mu));

  double b0 = -3.33575e-01;
  double b1 =  4.44856e-01;
  double b2 =  1.91766e+00;
  double b3 =  2.69423e+00;
  double b4 =  1.06905e+00;
  double B =  (1/mu)*(b0 + b1*log10(mu) + b2*pow(log10(mu),2) 
                      + b3*pow(log10(mu),3) + b4*pow(log10(mu),4));

  double a0 = 7.18248e-02; 
  double a1 = 1.89016e+00;
  double a2 = 2.15651e-02;
  double a3 = 2.30786e-02;
  double A =  exp(B*mu*0.015)*(a0/(exp(a1*(log10(mu)+a2))+1)+a3);

  double R = 0.01*D*( 4/(0.222+E)/(0.222+E) - 1/((0.22-z)*(z+E)) );
  f =  A * exp(-B*mu*(0.22-z)) * (1+C*exp(R));
  
  return f;
}

//_________________________________________________________
double RadiationDamageProperties::InducedAbsorptionHadronic(double lumi, double eta) const
{
  double fluence = doseProfile(eta) * 2.7e+13/500.0 * lumi;
  double mu = 2.08E-13 * pow( fluence, 1.0049);
  return mu;
}


//_________________________________________________________
double RadiationDamageProperties::AgingVPT(double lumi, double eta) const
{
  double Q = 0.26e-3 * doseProfile(eta) * lumi;
  double result = 0.772+0.228*(3.94304e-01*exp(-Q/5.99232e-04)+(1-3.94304e-01)*exp(-Q/1.58243e-02));
  return result;
}


//_____________________________________________________________
double RadiationDamageProperties::DegradationMeanEM50GeV(double mu) const
{
  double retval = 1.0;
  double x = mu;
  if( x<1e-4   ) return retval;
  if( x>=200.0 ) x=200.0;  // parameterization is not valid for large mu

  double par[11] = {  1.00000e+01,
                     -4.41441e-01, 7.08607e-02, -3.75572e-01, -3.60410e-01, 1.30130e-01,
                     -4.72350e-01, 3.36315e-01, -1.19872e-01,  1.99574e-02,-1.22910e-03  };


  double alpha = par[0];

  double f1 = par[1]*x + par[2]*x*x;  
  double u = log(x);
  double f2 = par[10];
  for(int i=9; i>=3; i--) f2 = par[i] + f2*u;

  retval = f1/(1.0+exp(alpha*u)) + f2/(1.0+exp(-alpha*u));
  retval = exp(retval);
  return retval;
  

}




//_____________________________________________________________
double RadiationDamageProperties::DegradationNonLinearityEM50GeV(double mu, double ene) const
{
  if(ene<=1e-3) return 0.0;

  double x = mu;
  if( mu<=0.06  ) x=0.06;
  if( mu>=150.0 ) x=150.0;

  double par[9] = { 5.17712e-03, 1.97597e-02, 3.36596e-02, 2.84505e-02, 1.38480e-02,
                    1.11498e-02, 7.73634e-03, -1.30767e-03, -2.20628e-03 };

  double u = log10(x);
  double slope = par[8];
  for(int i=7; i>=0; i--) slope = par[i] + slope*u;

  double retval = 1.0 + slope*log10(ene/50.0);
  if(retval<=0.0) retval = 0.0;
  return retval;

}


//_____________________________________________________________
double RadiationDamageProperties::ResolutionConstantTermEM50GeV(double mu) const
{

  double x = mu;
  if( mu<=0.01  ) x=0.01;
  if( mu>=200.0 ) x=200.0;

  double par[10] = {-6.21503e+00,  1.59759e+00, -4.75221e-02, -3.90299e-02, 3.97269e-03,
                     2.29574e-03, -1.05280e-04, -9.60963e-05, -1.29594e-06, 1.70850e-06};

  double u = log(x);
  double f = par[9];
  for(int i=8; i>=0; i--) f = par[i] + f*u;
  return exp(f);

}

//______________________________________________________________________________
double RadiationDamageProperties::relativeLYLoss(double eta) const
{
  double rel_LY = 1.0;
  if( isDamaged_ ){
    double absEta = fabs(eta);
    double muEM = InducedAbsorptionEM(lumiInstant_, absEta);
    double muHD = InducedAbsorptionHadronic(lumiTotal_, absEta);
    double vpt = 1.0;
    if(absEta>=1.489) vpt = AgingVPT(lumiTotal_, absEta);
    rel_LY = DegradationMeanEM50GeV(muEM+muHD) * vpt;
    if(rel_LY > 1.) rel_LY = 1.;
    else if(rel_LY < 1.e-300) rel_LY = 1.e-300;

  }
  return rel_LY;
}
 
//______________________________________________________________________________
double RadiationDamageProperties::nonlinearityLYLoss(double eta, double e) const
{
  double result = 1.0;
  if( isDamaged_ ){
    double absEta = fabs(eta);
    double muEM = InducedAbsorptionEM(lumiInstant_, absEta);
    double muHD = InducedAbsorptionHadronic(lumiTotal_, absEta);
    result = DegradationNonLinearityEM50GeV(muEM+muHD, e);
  }
  return result;
}

//______________________________________________________________________________
double RadiationDamageProperties::constantTermLYLoss(double eta) const
{
  double result = 0.0;
  if( isDamaged_ ){
    double absEta = fabs(eta);
    double muEM = InducedAbsorptionEM(lumiInstant_, absEta);
    double muHD = InducedAbsorptionHadronic(lumiTotal_, absEta);
    result = ResolutionConstantTermEM50GeV(muEM+muHD);
  }
  return result;
}

