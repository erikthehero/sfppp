#include <stdio.h>
#include <string>
#include <vector>

#include "UbxStream.h"
#include "Segments.h"

int main( int argc, char* argv[] ) {
  // variables
  std::string fileName("tmp");
  std::string cmdArg("tmp");
	
  // parse cmdline
  if (argc < 2) {
    printf("Not enough input arguments: %d / 1, see -h\n", argc-2);
	return 0;
  } else {
    for (int i = 0; i < argc; i++) {
	  if (i + 1 != argc) {
	    cmdArg = argv[i]; 
        if ( cmdArg.compare("-f") != 1) {
		  fileName.assign(argv[i + 1]);
	    }
	  }
	}
	printf( "running: %s\n", fileName.c_str() );
	//return 0;
  }
  
  Segments segments;
  int cutSize = 1200;
	
  // files
  //std::string fileName("C:/Users/Erik/Documents/sfppp/cpp/main/shortbaseline/exampledata/m8t119470035.ubx");
  
  // allocate and parse
  segments.ubxStr->setFile(fileName);
  segments.parse();
  segments.removeOrbitSignal();
  segments.compsdD1C();
  segments.getStatistics("output/sdResults.txt", "output/sdD1C.txt", cutSize);
  
  return 0;
  
}








