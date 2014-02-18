
#include "OptFibreTools.hh"

#include "G4PhysicalConstants.hh"

#include <cmath>

double fresnelReflection(double th, double R)
{
  // reflection probablity for unpolarized photons
  //   th - incident angle wrt to normal
  //   R = n1/n2 where 
  //       n1 is refractive index of fiber
  //       n2 is refractive index of outside media
  //  
  double u = 1. - R*R*sin(th)*sin(th);
  if(u<=0.) u=0.;
  u = sqrt(u);
  double Rs = (R*cos(th) - u)/(R*cos(th) + u);
  Rs = Rs * Rs;
  double Rp = (R*u - cos(th))/(R*u + cos(th));
  Rp = Rp * Rp;
  return 0.5*(Rs+Rp);
}


Travel GetTimeAndProbability(Photon ph, Fiber fib)
{

  Travel result;
  for(int i=0; i<10; ++i){
    result.prob[i] = 0.;
    result.time[i] = 1e+9;
  }
  result.nmax = 5;
  result.costh = 0.;
  result.cosg  = 1.;
  
  double cosTheta = fib.direction * ph.direction;
  double theta = acos(cosTheta);
  double absTheta = acos(fabs(cosTheta));

  double probReflectFace  = fresnelReflection(absTheta, fib.refrIndCore/fib.refrIndAir);
  double probReflectRear  = fresnelReflection(absTheta, fib.refrIndCore/fib.refrIndDet);

  // Total internal reflection at the rear, photon will not reach a photo-detector
  if((1.-probReflectRear)<1.e-9) return result;

  // fiber axis
  TVector3 a = fib.position + fib.direction * fib.direction.Dot(ph.position - fib.position);
  // direction of a photon in transverse plane
  TVector3 v = ph.direction - fib.direction * fib.direction.Dot(ph.direction);
  // distance from the photon trajectory and the fiber axis in transverse plane
  TVector3 d = (a - ph.position) - v * (v.Dot(a - ph.position) / v.Mag2());
  // skew angle in transverse plane
  double cosGamma = sqrt( 1. - d.Mag2()/(fib.radius*fib.radius) );  
  // check if there is a total internal reflection off the fiber walls
  if( acos(cosGamma*sin(theta))<asin(fib.refrIndClad/fib.refrIndCore) ) return result;

  result.costh = cosTheta;
  result.cosg  = cosGamma;

  double relDistance = 0.;
  double probReflections = 1.;
  if(cosTheta>0){
    relDistance = ph.dist;
  }else{
    relDistance = (2. - ph.dist);
    probReflections *= probReflectFace;
  }
  for(int i=0; i<result.nmax; i++){    
    double length = relDistance * fib.lengthTotal / fabs(cosTheta);
    result.time[i] = length / c_light * fib.refrIndCore;
    result.prob[i] =  exp( -length / fib.attenuation ) * probReflections * (1.-probReflectRear);
    relDistance += 2.;
    probReflections *= probReflectFace * probReflectRear;
  }

  return result;    
}

