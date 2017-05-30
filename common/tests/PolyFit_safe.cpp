#include "polyfit.h"


PolyFit::PolyFit() {
  printf("Constructing PolyFit\n");
}

std::vector<double> PolyFit::evalCoefs(std::vector<double> &Xs, std::vector<double> &coefs) {
	std::vector<double> out(Xs.size(), 0.0);
	
	for (int i=0; i<Xs.size(); i++) {
	  for (int j=0; j<coefs.size(); j++){
		out[i] = out[i] + coefs[j]*std::pow(Xs[i],j);
	  }
	}
	return out;
}

void PolyFit::getCoefs(std::vector<double> &Xs, std::vector<double> &Ys, int degree, std::vector<double> &coefs) {
//----------------------------------------------------
//
// METHOD:  polyfit
//
// INPUTS:  dependentValues[0..(countOfElements-1)]
//          independentValues[0...(countOfElements-1)]
//          countOfElements
//          order - Order of the polynomial fitting
//
// OUTPUTS: coefficients[0..order] - indexed by term
//               (the (coef*x^3) is coefficients[3])
//
//----------------------------------------------------
/*int polyfit(const double* const dependentValues,
            const double* const independentValues,
            unsigned int        countOfElements,
            unsigned int        order,
            double*             coefficients) 
{*/

	//std::vector<double> coefs(degree+1, 0.0);
    std::vector<double> B(degree+1, 0.0);
    std::vector<double> P(((degree+1) * 2)+1, 0.0);
    std::vector<double> A((degree+1)*2*(degree+1), 0.0);
	//double A[(maxOrder + 1)*2*(maxOrder + 1)] 
	
    /*double B[degree+1] = {0.0f};
    double P[((degree+1) * 2)+1] = {0.0f};
    double A[(degree+1)*2*(degree)] = {0.0f};*/

    double x, y, powx;
	
	int countOfElements = Xs.size();

    unsigned int ii, jj, kk;
	
    if (countOfElements <= degree) { // under determined system
		printf("Warning: elements <= order");
        return;
	}

    // Identify the column vector
    for (ii = 0; ii < countOfElements; ii++) {
      x = Xs[ii];
      y = Ys[ii];
      powx = 1;

      for (jj = 0; jj < (degree + 1); jj++) { 
        B[jj] = B[jj] + (y * powx);
        powx  = powx * x;
      }
    }

    // Initialize the PowX array
    P[0] = countOfElements;
    // Compute the sum of the Powers of X
    for (ii = 0; ii < countOfElements; ii++) {
      x    = Xs[ii];
      powx = Xs[ii];

      for (jj = 1; jj < ((2 * (degree + 1)) + 1); jj++) {
          P[jj] = P[jj] + powx;
          powx  = powx * x;
      }
    }

    // Initialize the reduction matrix
    for (ii = 0; ii < (degree + 1); ii++) {
      for (jj = 0; jj < (degree + 1); jj++) {
          A[(ii * (2 * (degree + 1))) + jj] = P[ii+jj];
      }

      A[(ii*(2 * (degree + 1))) + (ii + (degree + 1))] = 1;
    }

    // Move the Identity matrix portion of the redux matrix
    // to the left side (find the inverse of the left side
    // of the redux matrix
    for (ii = 0; ii < (degree + 1); ii++) {
      x = A[(ii * (2 * (degree + 1))) + ii];
      if (x != 0) {
        for (kk = 0; kk < (2 * (degree + 1)); kk++) {
          A[(ii * (2 * (degree + 1))) + kk] = 
          A[(ii * (2 * (degree + 1))) + kk] / x;
        }

        for (jj = 0; jj < (degree + 1); jj++) {
          if ((jj - ii) != 0) {
            y = A[(jj * (2 * (degree + 1))) + ii];
            for (kk = 0; kk < (2 * (degree + 1)); kk++) {
              A[(jj * (2 * (degree + 1))) + kk] = 
              A[(jj * (2 * (degree + 1))) + kk] -
              y * A[(ii * (2 * (degree + 1))) + kk];
            }
          }
        }
      } else {
        // Cannot work with singular matrices
        return;
      }
    }
	
    // Calculate and Identify the coefficients
    for (ii = 0; ii < (degree + 1); ii++) {
      for (jj = 0; jj < (degree + 1); jj++) {
        x = 0;
        for (kk = 0; kk < (degree + 1); kk++) {
          x = x + (A[(ii * (2 * (degree + 1))) + (kk + (degree + 1))] * B[kk]);
        }
        coefs[ii] = x;
      }
    }

    return;
}

/*
//----------------------------------------------------
//
// METHOD:  polyfit
//
// INPUTS:  dependentValues[0..(countOfElements-1)]
//          independentValues[0...(countOfElements-1)]
//          countOfElements
//          order - Order of the polynomial fitting
//
// OUTPUTS: coefficients[0..order] - indexed by term
//               (the (coef*x^3) is coefficients[3])
//
//----------------------------------------------------
int polyfit(const double* const dependentValues,
            const double* const independentValues,
            unsigned int        countOfElements,
            unsigned int        order,
            double*             coefficients) 
{
    // Declarations...
    // ----------------------------------
    enum {maxOrder = 5};
    
    double B[maxOrder+1] = {0.0f};
    double P[((maxOrder+1) * 2)+1] = {0.0f};
    double A[(maxOrder + 1)*2*(maxOrder + 1)] = {0.0f};

    double x, y, powx;

    unsigned int ii, jj, kk;

    // Verify initial conditions....
    // ----------------------------------

    // This method requires that the countOfElements > 
    // (order+1) 
    if (countOfElements <= order)
        return -1;

    // This method has imposed an arbitrary bound of
    // order <= maxOrder.  Increase maxOrder if necessary.
    if (order > maxOrder)
        return -1;

    // Begin Code...
    // ----------------------------------

    // Identify the column vector
    for (ii = 0; ii < countOfElements; ii++) {
      x = dependentValues[ii];
      y = independentValues[ii];
      powx = 1;

      for (jj = 0; jj < (order + 1); jj++) {
            B[jj] = B[jj] + (y * powx);
            powx  = powx * x;
      }
    }

    // Initialize the PowX array
    P[0] = countOfElements;

    // Compute the sum of the Powers of X
    for (ii = 0; ii < countOfElements; ii++) {
      x    = dependentValues[ii];
      powx = dependentValues[ii];

      for (jj = 1; jj < ((2 * (order + 1)) + 1); jj++) {
          P[jj] = P[jj] + powx;
          powx  = powx * x;
      }
    }

    // Initialize the reduction matrix
    for (ii = 0; ii < (order + 1); ii++) {
      for (jj = 0; jj < (order + 1); jj++) {
          A[(ii * (2 * (order + 1))) + jj] = P[ii+jj];
      }

      A[(ii*(2 * (order + 1))) + (ii + (order + 1))] = 1;
    }

    // Move the Identity matrix portion of the redux matrix
    // to the left side (find the inverse of the left side
    // of the redux matrix
    for (ii = 0; ii < (order + 1); ii++) {
      x = A[(ii * (2 * (order + 1))) + ii];
      if (x != 0) {
        for (kk = 0; kk < (2 * (order + 1)); kk++) {
          A[(ii * (2 * (order + 1))) + kk] = 
          A[(ii * (2 * (order + 1))) + kk] / x;
        }

        for (jj = 0; jj < (order + 1); jj++) {
          if ((jj - ii) != 0) {
            y = A[(jj * (2 * (order + 1))) + ii];
            for (kk = 0; kk < (2 * (order + 1)); kk++) {
              A[(jj * (2 * (order + 1))) + kk] = 
              A[(jj * (2 * (order + 1))) + kk] -
              y * A[(ii * (2 * (order + 1))) + kk];
            }
          }
        }
      } else {
        // Cannot work with singular matrices
        return -1;
      }
    }

    // Calculate and Identify the coefficients
    for (ii = 0; ii < (order + 1); ii++) {
      for (jj = 0; jj < (order + 1); jj++) {
        x = 0;
        for (kk = 0; kk < (order + 1); kk++) {
          x = x + (A[(ii * (2 * (order + 1))) + (kk + (order + 1))] * B[kk]);
        }
        coefficients[ii] = x;
      }
    }
    return 0;
}





*/













