#include <stdio.h>

#include "PolyFit.h"

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
int polyfittest(const double* const dependentValues,
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
}*/

int main() {
  printf("Dit is het begin van PolyFiwadwadt!\n");
  const unsigned int degree = 3;
  const unsigned int countOfElements = 5;
  const double acceptableError = 0.01;
  
    // These inputs should result in the following approximate coefficients:
  //         0.5           2.5           1.0        3.0
  //    y = (0.5 * x^3) + (2.5 * x^2) + (1.0 * x) + 3.0
  double    xData[countOfElements] = {    12.0,
                                            77.8,
                                            44.1,
                                            23.6,
                                           108.2};
  double    yData[countOfElements] = {  1239.00,
                                        250668.38,
                                         47792.19,
                                          7991.13,
                                        662740.98};
										
  double    x1[countOfElements] = {  1.00,
                                        2.0,
                                         3.0,
                                          4.0,
                                        5.0};
  
  PolyFit pf;
  std::vector<double> Xs(5, 0.0);
  std::vector<double> Ys(5, 0.0);
  std::vector<double> X1(5, 0.0);
  std::vector<double> coefs(degree+1, 0.0);
  
  for (int i=0; i<5; i++) {
	Xs[i] = xData[i];
	Ys[i] = yData[i];
	X1[i] = x1[i];
  }
  
  pf.getCoefs(Xs, Ys, degree, coefs);
  std::vector<double> check = pf.evalCoefs(X1, coefs);
  
  for (int i=0; i<check.size(); i++) {
	printf( "%f\n", check[i] );  
  }
  //printf("%d")
  /*double coefficients[degrees + 1]; // resulting array of coefs

  // Perform the polyfit
  result = polyfittest(xData,
                     yData,
                     countOfElements,
                     order,
                     coefficients);
	
	printf("%f %f %f %f\n", coefficients[0], coefficients[1], coefficients[2], coefficients[3]);*/
					 
}