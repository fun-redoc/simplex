#ifndef __VEC__
#define __VEC__

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

#ifndef CAT
  #define CAT(a,b) CAT2(a,b)
  #define CAT2(a,b) a##b
#endif

#ifndef LEN
  #define LEN(a) ((sizeof a)/(sizeof *a))
#endif
#ifndef MIN
  #define MIN(a,b) (a<b?a:b)
#endif
#ifndef MAX
  #define MAX(a,b) (a>=b?a:b)
#endif

void vec_set(double *xs, int dims, ...);
double vecLen(int dims, double *xs);
bool vecAllPos(int dims, double *xs);
double failDouble(const char* msg);
double scalarProdIter(int dims, double *as, double *bs);

#define VEC_CHK_DIMS_AND_EXIT(v1,v2)\
  if(v1._dims != v2._dims)\
  {\
    fprintf(stderr, "ERROR: Dimensions not equal.\n");\
    exit(1);\
  }

#define VEC_TYPE_NAME(dims) CAT(TVec,dims)
#define VEC_TYPE(dims)\
  typedef struct{int _dims;double _xs[dims];} CAT(TVec,dims);
#define VEC(dims, varname) CAT(TVec,dims) varname; varname._dims = dims;
#define VEC_SET(dims, varname,...)\
  VEC(dims,varname)\
  vec_set(varname._xs,dims,__VA_ARGS__);
#define VEC_CPY(res, v1)\
  VEC_CHK_DIMS_AND_EXIT(res,v1)\
  memcpy(res._xs, v1._xs, sizeof(double)*v1._dims);
#define VEC_ADD(res, v1, v2)\
  VEC_CHK_DIMS_AND_EXIT(res,v1)\
  VEC_CHK_DIMS_AND_EXIT(res,v2)\
  for(int i=0; i < res._dims; i++)\
  {\
    res._xs[i] = v1._xs[i] + v2._xs[i];\
  }
#define VEC_SUB(res, v1, v2)\
  VEC_CHK_DIMS_AND_EXIT(res,v1)\
  VEC_CHK_DIMS_AND_EXIT(res,v2)\
  for(int i=0; i < res._dims; i++)\
  {\
    res._xs[i] = v1._xs[i] - v2._xs[i];\
  }
#define VEC_MUL(res, v1, v2)\
  VEC_CHK_DIMS_AND_EXIT(res,v1)\
  VEC_CHK_DIMS_AND_EXIT(res,v2)\
  for(int i=0; i < res._dims; i++)\
  {\
    res._xs[i] = v1._xs[i] * v2._xs[i];\
  }
#define VEC_DIV(res, v1, v2)\
  VEC_CHK_DIMS_AND_EXIT(res,v1)\
  VEC_CHK_DIMS_AND_EXIT(res,v2)\
  for(int i=0; i < res._dims; i++)\
  {\
    res._xs[i] = v1._xs[i] / v2._xs[i];\
  }
#define VEC_SCALE(res, v1, s)\
  VEC_CHK_DIMS_AND_EXIT(res,v1)\
  for(int i=0; i < res._dims; i++)\
  {\
    res._xs[i] = v1._xs[i] * s;\
  }
#define VEC_EQL(b, v1, v2)\
  VEC_CHK_DIMS_AND_EXIT(v1,v2)\
  b=true;\
  for(int i=0; i < v1._dims && b; i++)\
  {\
    b &= v1._xs[i] == v2._xs[i];\
  }
#define VEC_LEN(v) vecLen(v._dims, v._xs)
#define VEC_NORM(res, v)\
  VEC_SCALE(res, v, (1/VEC_LEN(v)));

#define VEC_ALL_POS(v)\
  vecAllPos(v._dims, v._xs)

#define DBL_RAND ((double)(rand()))/((double)RAND_MAX)

#define VEC_RANDOM(v)\
  for(int i=0; i<v._dims; i++)\
  {\
    v._xs[i] = DBL_RAND;\
  }

#define VEC_SCALAR_PROD(v1,v2)\
  v1._dims==v2._dims?scalarProdIter(v1._dims,v1._xs, v2._xs):failDouble("Dim mismatch.\n")
#define VEC_PRINT(stream, v)\
  fprintf(stream,"(");\
  for(int i=0; i<(v._dims-1);i++) fprintf(stream,"%.2f,",v._xs[i]);\
  fprintf(stream,"%.2f)\n", v._xs[v._dims-1]); 

#endif
