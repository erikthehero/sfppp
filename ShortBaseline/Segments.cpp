#include "Segments.h"

Segments::Segments() {
  ubxStr = new UbxStream();
}

void Segments::clearSeg(int ind) {
  segs.aSegs[ind].week.clear();
  segs.aSegs[ind].sow.clear();
  segs.aSegs[ind].C1C.clear();
  segs.aSegs[ind].L1C.clear();
  segs.aSegs[ind].D1C.clear();
  segs.aSegs[ind].S1C.clear();
}

void Segments::parse() {
  int cnt = 0;
  int prn;
  int count = 0;
  int ind;
  double prevSow = -9999;
  double dt;
  
  while (cnt == 0) {
    cnt = ubxStr->getNextEpoch();
	// check data of system 0(GPS) and 2(GALILEO + 150)
	if (ubxStr->obs.consts[0].isAvailable) {
	  for (int i=0; i<ubxStr->obs.consts[0].numSats; i++) { // loop available prns
	    prn = ubxStr->obs.consts[0].available[i];
	    if (ubxStr->obs.consts[0].sats[prn-1].L1C == 0.0) {
		  //printf("Heoa1\n");
		  continue;
		}
		if (segs.activePrns.count( prn ) > 0 ) { // prn contained in active segments
		  ind = segs.activePrns.at( prn ); // indice of seg in aSegs
		  /*if ( segs.aSegs[ind].week.size() == maxSegSize ) { // size aSeg > maxSegSize
			segs.fSegs.push_back(segs.aSegs[ind]);
			clearSeg(ind); // clear seg
			// fill with new data
			segs.aSegs[ind].week.push_back( ubxStr->obs.week );
		    segs.aSegs[ind].sow.push_back( ubxStr->obs.sow );
		    segs.aSegs[ind].C1C.push_back(ubxStr->obs.consts[0].sats[prn-1].C1C);
		    segs.aSegs[ind].L1C.push_back(ubxStr->obs.consts[0].sats[prn-1].L1C);
		    segs.aSegs[ind].D1C.push_back(ubxStr->obs.consts[0].sats[prn-1].D1C);
		    segs.aSegs[ind].S1C.push_back(ubxStr->obs.consts[0].sats[prn-1].S1C);
			continue;
		  } else */if (ubxStr->obs.consts[0].sats[prn-1].L1CSlip > 0 ) { // cycle slip
		    if (segs.aSegs[ind].week.size() > 1) {
		      segs.fSegs.push_back(segs.aSegs[ind]);
			}
			clearSeg(ind);
			// fill with new data
			segs.aSegs[ind].week.push_back( ubxStr->obs.week );
		    segs.aSegs[ind].sow.push_back( ubxStr->obs.sow );
		    segs.aSegs[ind].C1C.push_back(ubxStr->obs.consts[0].sats[prn-1].C1C);
		    segs.aSegs[ind].L1C.push_back(ubxStr->obs.consts[0].sats[prn-1].L1C);
		    segs.aSegs[ind].D1C.push_back(ubxStr->obs.consts[0].sats[prn-1].D1C);
		    segs.aSegs[ind].S1C.push_back(ubxStr->obs.consts[0].sats[prn-1].S1C);
		  } else if ( ubxStr->obs.sow - segs.aSegs[ind].sow[segs.aSegs[ind].sow.size()-1] > maxGap*60 ) { // gap > maxGap 
		    if (segs.aSegs[ind].week.size() > 1) {
		      segs.fSegs.push_back(segs.aSegs[ind]);
			}
			clearSeg(ind);
			segs.aSegs[ind].week.push_back( ubxStr->obs.week );
		    segs.aSegs[ind].sow.push_back( ubxStr->obs.sow );
		    segs.aSegs[ind].C1C.push_back(ubxStr->obs.consts[0].sats[prn-1].C1C);
		    segs.aSegs[ind].L1C.push_back(ubxStr->obs.consts[0].sats[prn-1].L1C);
		    segs.aSegs[ind].D1C.push_back(ubxStr->obs.consts[0].sats[prn-1].D1C);
		    segs.aSegs[ind].S1C.push_back(ubxStr->obs.consts[0].sats[prn-1].S1C);		
		  } else { // add epoch to aSeg
			segs.aSegs[ind].week.push_back( ubxStr->obs.week );
		    segs.aSegs[ind].sow.push_back( ubxStr->obs.sow );
		    segs.aSegs[ind].C1C.push_back(ubxStr->obs.consts[0].sats[prn-1].C1C);
		    segs.aSegs[ind].L1C.push_back(ubxStr->obs.consts[0].sats[prn-1].L1C);
		    segs.aSegs[ind].D1C.push_back(ubxStr->obs.consts[0].sats[prn-1].D1C);
		    segs.aSegs[ind].S1C.push_back(ubxStr->obs.consts[0].sats[prn-1].S1C);			  
		  }
		} else { // add new satellite to segment
		  Seg tmpSeg;
		  tmpSeg.prn = prn;
		  tmpSeg.week.push_back( ubxStr->obs.week );
		  tmpSeg.sow.push_back( ubxStr->obs.sow );
		  tmpSeg.C1C.push_back(ubxStr->obs.consts[0].sats[prn-1].C1C);
		  tmpSeg.L1C.push_back(ubxStr->obs.consts[0].sats[prn-1].L1C);
		  tmpSeg.D1C.push_back(ubxStr->obs.consts[0].sats[prn-1].D1C);
		  tmpSeg.S1C.push_back(ubxStr->obs.consts[0].sats[prn-1].S1C);

	      segs.aSegs.push_back(tmpSeg);
		  segs.activePrns.emplace( prn, segs.aSegs.size()-1 );
		}
	  }
	}
	
	if (ubxStr->obs.consts[2].isAvailable) {
	  for (int i=0; i<ubxStr->obs.consts[2].numSats; i++) { // loop available prns
	    prn = ubxStr->obs.consts[2].available[i];
	    if (ubxStr->obs.consts[2].sats[prn-1].L1C == 0.0) {
		  //printf("Heoa1\n");
		  continue;
		}
		if (segs.activePrns.count( prn+150 ) > 0 ) { // prn contained in active segments
		  ind = segs.activePrns.at( prn+150 ); // indice of seg in aSegs
		  /*if ( segs.aSegs[ind].week.size() == maxSegSize ) { // size aSeg > maxSegSize
			segs.fSegs.push_back(segs.aSegs[ind]);
			clearSeg(ind); // clear seg
			// fill with new data
			segs.aSegs[ind].week.push_back( ubxStr->obs.week );
		    segs.aSegs[ind].sow.push_back( ubxStr->obs.sow );
		    segs.aSegs[ind].C1C.push_back(ubxStr->obs.consts[2].sats[prn-1].C1C);
		    segs.aSegs[ind].L1C.push_back(ubxStr->obs.consts[2].sats[prn-1].L1C);
		    segs.aSegs[ind].D1C.push_back(ubxStr->obs.consts[2].sats[prn-1].D1C);
		    segs.aSegs[ind].S1C.push_back(ubxStr->obs.consts[2].sats[prn-1].S1C);
			continue;
		  } else */if (ubxStr->obs.consts[2].sats[prn-1].L1CSlip > 0 ) { // cycle slip
		    if (segs.aSegs[ind].week.size() > 1) {
		      segs.fSegs.push_back(segs.aSegs[ind]);
			}
			clearSeg(ind);
			// fill with new data
			segs.aSegs[ind].week.push_back( ubxStr->obs.week );
		    segs.aSegs[ind].sow.push_back( ubxStr->obs.sow );
		    segs.aSegs[ind].C1C.push_back(ubxStr->obs.consts[2].sats[prn-1].C1C);
		    segs.aSegs[ind].L1C.push_back(ubxStr->obs.consts[2].sats[prn-1].L1C);
		    segs.aSegs[ind].D1C.push_back(ubxStr->obs.consts[2].sats[prn-1].D1C);
		    segs.aSegs[ind].S1C.push_back(ubxStr->obs.consts[2].sats[prn-1].S1C);
		  } else if ( ubxStr->obs.sow - segs.aSegs[ind].sow[segs.aSegs[ind].sow.size()-1] > maxGap*60 ) { // gap > maxGap 
		    if (segs.aSegs[ind].week.size() > 1) {
		      segs.fSegs.push_back(segs.aSegs[ind]);
			}
			clearSeg(ind);
			segs.aSegs[ind].week.push_back( ubxStr->obs.week );
		    segs.aSegs[ind].sow.push_back( ubxStr->obs.sow );
		    segs.aSegs[ind].C1C.push_back(ubxStr->obs.consts[2].sats[prn-1].C1C);
		    segs.aSegs[ind].L1C.push_back(ubxStr->obs.consts[2].sats[prn-1].L1C);
		    segs.aSegs[ind].D1C.push_back(ubxStr->obs.consts[2].sats[prn-1].D1C);
		    segs.aSegs[ind].S1C.push_back(ubxStr->obs.consts[2].sats[prn-1].S1C);		
		  } else { // add epoch to aSeg
			segs.aSegs[ind].week.push_back( ubxStr->obs.week );
		    segs.aSegs[ind].sow.push_back( ubxStr->obs.sow );
		    segs.aSegs[ind].C1C.push_back(ubxStr->obs.consts[2].sats[prn-1].C1C);
		    segs.aSegs[ind].L1C.push_back(ubxStr->obs.consts[2].sats[prn-1].L1C);
		    segs.aSegs[ind].D1C.push_back(ubxStr->obs.consts[2].sats[prn-1].D1C);
		    segs.aSegs[ind].S1C.push_back(ubxStr->obs.consts[2].sats[prn-1].S1C);			  
		  }
		} else { // add new satellite to segment
		  Seg tmpSeg;
		  tmpSeg.prn = prn+150;
		  tmpSeg.week.push_back( ubxStr->obs.week );
		  tmpSeg.sow.push_back( ubxStr->obs.sow );
		  tmpSeg.C1C.push_back(ubxStr->obs.consts[2].sats[prn-1].C1C);
		  tmpSeg.L1C.push_back(ubxStr->obs.consts[2].sats[prn-1].L1C);
		  tmpSeg.D1C.push_back(ubxStr->obs.consts[2].sats[prn-1].D1C);
		  tmpSeg.S1C.push_back(ubxStr->obs.consts[2].sats[prn-1].S1C);

	      segs.aSegs.push_back(tmpSeg);
		  segs.activePrns.emplace( prn+150, segs.aSegs.size()-1 );
		}
	  }
	}
	count ++;
  }
  // add aSegs to fSegs
  for ( int i=0; i<segs.aSegs.size(); i++ ) {
	if (segs.aSegs[i].week.size() > 1) {
	  segs.fSegs.push_back(segs.aSegs[i]);
    }
  }
  
  printf("Read epochs %d\n", count);
  printf("Found total of %d segments\n", segs.fSegs.size());
  return;
}

// Remove orbit signal through polynomial
void Segments::removeOrbitSignal(){
  printf("Segments::removing orbit signal\n");
  int degree;
  int segLength;
  std::vector<double> sdObservations;
  std::vector<double> coefs; // polynomial coefficients

  for (int i=0; i<segs.fSegs.size(); i++) {
	segLength = segs.fSegs[i].week.size();
	if (segLength < 20) { // way too small
	  continue;
	}
	
	// set degree and resize coefs
	degree=std::round( std::log( (segLength+1)/10 ) );
	coefs.resize(degree+1, 0.0);

	if (degree < 1)
	  degree = 1;
  
    if (degree > 8)
	  degree = 8;

    // create single difference observations
    for (int j=0; j<segLength; j++) {
	  segs.fSegs[i].sdTime.push_back( 4 * 
									 ( segs.fSegs[i].sow[j] - segs.fSegs[i].sow[std::round( (1+segLength)/2 )] ) /
									 ( segs.fSegs[i].sow[segLength-1] - segs.fSegs[i].sow[0] ));
	  sdObservations.push_back(segs.fSegs[i].C1C[j] - segs.fSegs[i].L1C[j]*lam1);
	}

	// compute polynomial
    pf.getCoefs(segs.fSegs[i].sdTime, sdObservations, degree, coefs);
	
	// remove orbit signal
	segs.fSegs[i].yfit = pf.evalCoefs(segs.fSegs[i].sdTime, coefs);
	
	for (int j=0; j<segLength; j++) {
	  //segs.fSegs[i].sdTime.push_back( 4*(t(1:end)-t(round((1+length(t))/2)))./(t(length(t))-t(1)) );
	  segs.fSegs[i].sdObservations.push_back( sdObservations[j]-segs.fSegs[i].yfit[j] );
	}
	
	// clear sdObservations and coefs
	sdObservations.clear();
	coefs.clear();
  }

  return;
}

void Segments::compsdD1C() {
  double tdL1C, dt;
  
  for (int i=0; i<segs.fSegs.size(); i++) {
	for (int j=1; j<segs.fSegs[i].week.size()-1; j++) {
	  dt = segs.fSegs[i].sow[j+1] - segs.fSegs[i].sow[j-1];
	  if (std::round( dt*10) == 2 ) {
		tdL1C =  (segs.fSegs[i].L1C[j-1]*lam1 - segs.fSegs[i].L1C[j+1]*lam1) / dt;
		segs.fSegs[i].sdD1CS1C.push_back(segs.fSegs[i].S1C[j]);
		segs.fSegs[i].sdD1C.push_back(segs.fSegs[i].D1C[j]*lam1-tdL1C);
      }
	}
  }

  return;
}

// Compute statistics over segments and write to file
void Segments::getStatistics(std::string fn1, std::string fn2, int cutSize) {
  FILE *file1, *file2;
  int file_exists, segSize, lowerBound, upperBound, numSubSegs;
  double sum, m, meanS1C, accum, stdev;
  std::vector<int>::iterator iterLower, iterUpper;
  
  // does file1 exist
  file1=fopen(fn1.c_str(),"r");
  if (file1==NULL) file_exists=0;
  else {file_exists=1; fclose(file1);}
    
  if (file_exists==1) {
    file1=fopen(fn1.c_str(),"a");
  } else {
    file1=fopen(fn1.c_str(),"w");
  }
  
  // does file2 exist
  file2=fopen(fn2.c_str(),"r");
  if (file2==NULL) file_exists=0;
  else {file_exists=1; fclose(file2);}
    
  if (file_exists==1) {
    file2=fopen(fn2.c_str(),"a");
  } else {
    file2=fopen(fn2.c_str(),"w");
  }
  
  for (int i=0; i<segs.fSegs.size(); i++ ) {
	  
	segSize = segs.fSegs[i].week.size();
	if ( segSize < cutSize ) {
	  continue;
	}
	
	numSubSegs = (int)(segSize/1200);
	
	// divide segments and compute statistics
	//printf( "subsegs in seg: %d %d\n", numSubSegs, segSize);
	for (int j=0; j<numSubSegs; j++) {
	  lowerBound = j * 1200;
	  upperBound = (j+1) * 1200 - 1;
	  
	  // SD CODE
	  // mean S1C
	  sum = std::accumulate( segs.fSegs[i].S1C.begin()+lowerBound, segs.fSegs[i].S1C.begin()+upperBound, 0.0 );
	  meanS1C = sum/cutSize;
	  
	  // mean
	  sum = std::accumulate( segs.fSegs[i].sdObservations.begin()+lowerBound, segs.fSegs[i].sdObservations.begin()+upperBound, 0.0 );
	  m =  sum / 1200.0;
	  
	  // std
	  accum = 0.0;
	  std::for_each (segs.fSegs[i].sdObservations.begin()+lowerBound, segs.fSegs[i].sdObservations.begin()+upperBound, [&](const double d) {
        accum += (d - m) * (d - m);
	  });
	  stdev = std::sqrt(accum / (segs.fSegs[i].sdObservations.size()-1));
	  // to file
	  fprintf(file1, "%d %f %f\n", segs.fSegs[i].prn, meanS1C, stdev);
	  
	  // SD DOPPLER
	  if ( upperBound > segs.fSegs[i].sdD1C.size() ) {
		continue;
	  }
	  
	  // mean S1C
	  sum = std::accumulate( segs.fSegs[i].sdD1CS1C.begin()+lowerBound, segs.fSegs[i].sdD1CS1C.begin()+upperBound, 0.0 );
	  meanS1C = sum/cutSize;
	  
	  // mean
	  sum = std::accumulate( segs.fSegs[i].sdD1C.begin()+lowerBound, segs.fSegs[i].sdD1C.begin()+upperBound, 0.0 );
	  m =  sum / 1200.0;
	  
	  // std
	  accum = 0.0;
	  std::for_each (segs.fSegs[i].sdD1C.begin()+lowerBound, segs.fSegs[i].sdD1C.begin()+upperBound, [&](const double d) {
        accum += (d - m) * (d - m);
	  });
	  stdev = std::sqrt(accum / (segs.fSegs[i].sdD1C.size()-1));
	  // to file
	  fprintf(file2, "%d %f %f\n", segs.fSegs[i].prn, meanS1C, stdev);
	  
	  

	}
  }
  
  if (file1!=NULL) {
    fclose(file1);
  }  
  
  return;
}








