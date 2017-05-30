#include "UbxStream.h"

// GNSS 		Signal 	gnssId 	numWords 	period
//GPS L1 C/A 0 10 6s
//SBAS L1 C/A 1 8 1s
//Galileo E1-B 2 8 2s
//BeiDou B1 D1 3 10 6s
//BeiDou B1 D2 3 10 0.6s
//IMES Short 4 1 -
//IMES Medium 4 2 -
//IMES Position 1 4 3 -
//IMES Position 2 4 4 -
//QZSS L1 C/A 5 10 6s
//QZSS L1 SAIF 5 8 1s
//GLONASS L1 OF 6

UbxStream::UbxStream() {
  //portNames = port; 
  //fileStream = new std::ifstream(port, std::ios::in|std::ios::binary);
  printf("Constructing UbxStream\n");
  fileStream = new std::ifstream();
}

unsigned short UbxStream::U2(int offset) {
      unsigned short u;
      memcpy(&u,p+offset,2);
      return u;  
}

unsigned int UbxStream::U4(int offset) {
  unsigned int u;
  memcpy(&u,p+offset,4);
  return u;
}

int UbxStream::I4(int offset) {
  int u;
  memcpy(&u,p+offset,4);
  return u;
}

double UbxStream::I8(int offset) {
  p += offset;
  return I4(4) * 4294967296.0+U4(0);
}

float UbxStream::R4(int offset) {
  float r;
  memcpy(&r,p+offset,4); return r;
}

double UbxStream::R8(int offset) {
  double r;
  memcpy(&r,p+offset,8); return r;
}

void UbxStream::setFile(std::string fileName) {
  portName = fileName;
  //numFiles = portNames.size();
  //printf("%d files names set to ubxStream\n", numFiles);
  return;
}

int UbxStream::getNextEpoch() {
	
  if ( !(fileStream->is_open()) ) { // no open file
    printf( "UbxStream: FileStream is not open, opening %s\n", portName.c_str() );
	// open first file
	
	fileStream->open( portName, std::ios::in|std::ios::binary );
	indFile++;
    numFiles--;
	
  } else if ( fileStream->eof() ) { // reached end of file
	printf("Reached end of file\n");
	fileStream->close();
	return 1;
  }
  
  // read next epoch from file
  getStream();
  
  return 0; // success
}

void UbxStream::getStream() {
  unsigned char cka=0,ckb=0;
  
  while (!fileStream->eof()) {

    fileStream->read(reinterpret_cast<char*>(&chr), 1);

    if (chr == 0xB5) {
	  fileStream->read(reinterpret_cast<char*>(&chr), 1);
	  if (chr == 0x62) {
		fileStream->read(reinterpret_cast<char*>(&buf), 4);
		cl = buf[0]; id = buf[1]; p = buf; len = U2(2);
		cka = 0; ckb = 0;
	    for (int i=0; i<4; i++) { // cs part 1
		  cka+=buf[i]; ckb+=cka;
		}
		
		// Read payload and compute checksum
		fileStream->read(reinterpret_cast<char*>(&buf), len+2);
		
		for (int i=0; i<len; i++) { // cs part 2
		  cka+=buf[i]; ckb+=cka;
		}
	    
		if (cka!=buf[len] || ckb!=buf[len+1]) {
		  //printf("%d %d %d %d\n", cka, buf[len], ckb, buf[len+1]);
		  for (int k=0; k<7; k++) {
			obs.consts[k].numSats = 0;
			//obs.consts[k].available.clear();
			obs.consts[k].isAvailable = false;
		  }
		  return;
		}	
		
		if (cl == 0x02 && id == 0x15) {
		  // new epoch, clear satellite vectors
		  for (int k=0; k<7; k++) {
			obs.consts[k].numSats = 0;
			//obs.consts[k].available.clear();
			obs.consts[k].isAvailable = false;
		  }
	      obs.sow = R8(0);
          obs.week = U2(8);
		  nsat = buf[11];
		  
		  //printf("%d %d\n", len, 16+32*nsat);
		  if (len<16+32*nsat) {
            printf("Warning: raw len < 16+32*nsat\n");
            return;
		  }
		  
		  for (i=0,p+=16;i<nsat;++i,p+=32) {
			  
			sys = *(p+20);
			prn = *(p+21);
			tstat = *(p+30); // tracking status
			pr1=tstat&1?R8(0):0.0;
			cp1=tstat&2?R8(8):0.0;
			
            if (cp1==-0.5) cp1=0.0; // invalid phase
			
			if (sys >= 7) {
			  printf("Warning: unknown system\n");
			  return;
			}
			
			obs.consts[sys].isAvailable = true;
			obs.consts[sys].numSats++; // increment sat counter
			obs.consts[sys].available[obs.consts[sys].numSats-1] = prn;

			//obs.consts[sys].available.resize(obs.consts[sys].available.size() + 1);
			//obs.consts[sys].available[obs.consts[sys].available.size()] = prn;
			
			//insert(obs.consts[sys].available.end(),tmp);
			obs.consts[sys].sats[prn-1].C1C = pr1;
			//printf("close33\n");
			obs.consts[sys].sats[prn-1].L1C = cp1;
			//printf("close44\n");
			obs.consts[sys].sats[prn-1].D1C = R4(16);
			//printf("close55\n");
			obs.consts[sys].sats[prn-1].S1C = *(p+26);// * 4; // rtklib multiplies CN0 by 4
			//printf("close66\n");
			obs.consts[sys].sats[prn-1].L1CSlip = tstat&4?0:2;
			//printf("close77\n");
			// TODO: add additional cycle slip detection
		  }
		  //printf("Obtained next epoch, returning\n");
		  return;
		}
	  }
	}
  }
}