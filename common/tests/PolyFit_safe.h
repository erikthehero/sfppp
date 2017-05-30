// PolyFit.h
#ifndef PolyFit_H
#define PolyFit_H

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
#include <fstream>

class PolyFit
{

public:
  
  // Default constructor
  PolyFit();
  
  // Calculate polynomial of ndim
  void getCoefs(std::vector<double> &Xs, std::vector<double> &Ys, int degree, std::vector<double> &coefs);
  // Evaluate polynomial
  std::vector<double> evalCoefs(std::vector<double> &Xs, std::vector<double> &coefs);
  
};







#endif