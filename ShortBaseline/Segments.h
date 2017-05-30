// Segments.h
#ifndef Segments_H
#define Segments_H

#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <functional>
#include <algorithm>
#include <numeric>

#include "UbxStream.h"
#include "PolyFit.h"

struct Seg
{
	int prn;
	std::vector<int> week;
	std::vector<double> sow;
	std::vector<double> C1C; // code
	std::vector<double> L1C; // phase
	std::vector<float> D1C; // doppler
	std::vector<int> S1C; // snr
	
	// data after orbit computation
	std::vector<double> sdTime;
	std::vector<double> yfit;
	std::vector<double> sdObservations; // single difference observations
	
	std::vector<double> sdD1CS1C;
	std::vector<double> sdD1C; // D1C - (( L1C_t-1 + L1C_t+1) / 2*dt )
};

struct Segs
{
  int t; // 1 = single, 2 = double
  std::map<int,int> activePrns; // map of active elements prn(key):place in activeSegs(value)
  std::vector<Seg> fSegs; // finished segments
  std::vector<Seg> aSegs; // active segments
};

class Segments
{
  double maxGap = 3.0; // minutes
  int maxSegSize = 1200; // epochs (120 seconds at 10 Hz)
  double c = 299792458.0; // speed of light
  double f0 = 10.23e6; // center frequency
  double freq1 = 154*f0; // frequency on L1
  double lam1 = c/freq1; // wavelength on L1
  
  void clearSeg(int ind);

public:
  // Default constructor
  Segments();
  
  // public instances
  Segs segs;
  UbxStream *ubxStr;
  PolyFit pf;
 
  
  void parse();
  void removeOrbitSignal();
  void compsdD1C();
  void getStatistics(std::string fn1, std::string fn2, int cutSize);

  
};

#endif 
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  