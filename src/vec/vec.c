#ifdef __DEBUG__
#include <assert.h>
#endif

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h> // for RAND, and rand
#include <float.h>

#include <maybe.h>
#include <linear_solver.h>
#include "vec.h"

void vec_set(double *xs, int dims, ...)
{
  va_list ap;
  va_start(ap, dims);
  for(int i=0; i<dims; i++)
  {
    double x = va_arg(ap, double);
    xs[i]=x;
  }
  va_end(ap);
}

double vecLen(int dims, double *xs)
{
  double res = 0;
  for(int i=0; i < dims; i++)
  {
    res += xs[i]*xs[i];
  }
  return sqrt(res);
}

bool vecAllPos(int dims, double *xs)
{
  bool res = true;
  for(int i=0; i<dims && res; i++)
  {
    res &= (xs[i]>=0.0);
  }
  return res;
}

double failDouble(const char* msg) {
  fprintf(stderr,"%s", msg);
  exit(1);
  return 0.0;
}

double scalarProdIter(int dims, double *as, double *bs)
{
  double res = 0;
   for(int i=0; i<dims; i++)
   {
     res += as[i]*bs[i];
   } 
   return res;
}

void examples(void)
{
  VEC_TYPE(4)

  VEC_SET(4,v1,1.0,2.0,3.0,4.0)
  VEC_SET(4,v2,4.0,3.0,2.0,1.0)
  VEC(4,r)
  VEC(4,r1)
  VEC(4,r2)
  VEC_ADD(r, v1, v2)
  VEC_CPY(r2,r)
  VEC_SCALE(r1,r,2)
  VEC_PRINT(stdout,r)
  VEC_PRINT(stdout,r1)
  VEC_PRINT(stdout,r2)
  bool b;
  VEC_EQL(b, r2, r)
  printf(b ? "true" : "false");
  printf("\n");
  VEC_EQL(b, r2, r1)
  printf(b ? "true" : "false");
  printf("\n");

  double sp = VEC_SCALAR_PROD(v1,v2);
  printf("scalar prord = %.2f\n", sp);

}   
