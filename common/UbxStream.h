// UbxStream.h
#ifndef UbxStream_H
#define UbxStream_H

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>

struct satellite // GNSS sat raw data
{
	std::string svid;
	double C1C; // code
	double L1C; // phase
	float D1C; // doppler
	int S1C; // snr
	int L1CSlip; // slip flag
};

struct constellation // GNSS constellation raw data
{
	bool isAvailable = false;
	int numSats;
	int available[32];
	//std::vector<int> available;
	satellite sats[32];
};

struct Obs // GNSS observations raw data
{
	std::string receiverID;
	double sow;
	int week;
	constellation consts[7];
};

class UbxStream
{
  std::ifstream *fileStream;
  int streamType, nsat, prn=0, i, tstat, lockt, halfc, numFiles=0, indFile=0;
  
  double cp1, pr1;
  unsigned char chr, cl, id, qi, sys, flag, lock1, lli;
  unsigned char *p;
  unsigned short len;
  unsigned char buf[5000];
  void getStream();
  
public:
  std::string portName;
  Obs obs;
  
  // Default constructor
  UbxStream();
  
  // Conversion functions
  unsigned short U2(int offset);//int offset, unsigned short &val
  unsigned int U4(int offset);
  int I4(int offset);
  double I8(int offset);
  float R4(int offset);
  double R8(int offset);
  bool checkSum();
  
  int getNextEpoch();
  void setFile(std::string fileName);
  
};

#endif