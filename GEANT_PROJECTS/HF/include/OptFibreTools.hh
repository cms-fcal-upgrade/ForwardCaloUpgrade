#ifndef OptFibreTools_hh
#define OptFibreTools_hh   

//////////////////////////////////////////////////////////
//  OptFibreTools
//  Some helper structs and functions to calculate track length
// and arrival times of optical photons in fibres.
//////////////////////////////////////////////////////////

#include "TVector3.h"


struct Fiber{
  double   lengthTotal;
  double   refrIndCore;   
  double   refrIndClad;   
  double   refrIndAir;    
  double   refrIndDet;    
  double   attenuation;
  TVector3 position; 
  TVector3 direction;
  double   radius;
};

struct Photon{
  TVector3 position;
  TVector3 direction;
  double   dist;         // relative from 0 to 1 (0 = photodetector)
};

struct Travel{
  int      nmax;
  double   prob[10];
  double   time[10];
  double   costh;
  double   cosg;
};


double fresnelReflection(double th, double R);


Travel GetTimeAndProbability(Photon ph, Fiber fib);

#endif
