#ifndef __LINEAR_SOLVER_H__
#define __LINEAR_SOLVER_H__
#ifdef __DEBUG__
#include <assert.h>
#endif

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h> // for RAND, and rand

#include <maybe.h>

#define LEN(a) ((sizeof a)/(sizeof *a))

typedef struct
{
  double solution;
  int numCoefficients;
  double *coefficients;
} TConstraint;


typedef struct
{
  int numObjectiveFunctionCoefficients;
  double *objectiveFunctionCoefficients;
  int numConstraints;
  TConstraint *constraints;

} TLinearProgram;

typedef struct
{
  int numObjectiveFunctionCoefficients;
  double *values;
  double solution;
} TLinearProgramSolution;

TLinearProgramSolution *solver(TLinearProgram *lp);
    
#endif
