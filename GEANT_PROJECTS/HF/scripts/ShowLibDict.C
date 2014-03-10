#include <vector>

#include <TLorentzVector.h>

#ifdef __MAKECINT__

#pragma link C++ class vector<float >+;

#pragma link C++ class vector<vector<int> >+;

#pragma link C++ class vector<vector<TLorentzVector> >+;  //or any other vectors you want to use

#endif
