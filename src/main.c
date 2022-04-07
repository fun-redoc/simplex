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
#include <vec.h>
#include <linear_solver.h>

#ifndef LEN
  #define LEN(a) ((sizeof a)/(sizeof *a))
#endif
#ifndef MIN
  #define MIN(a,b) (a<b?a:b)
#endif
#ifndef MAX
  #define MAX(a,b) (a>=b?a:b)
#endif

void testcase_1(void)
{
  double objectiveFunctionCoefficients[] = {4.0,6.0,2.0};
  double cc1[] = {1,0,2};
  TConstraint c1 = {40, LEN(cc1), cc1};
  double cc2[] = {5,4,1};
  TConstraint c2 = {80, LEN(cc2), cc2};
  double cc3[] = {2,1,5};
  TConstraint c3 = {120, LEN(cc3), cc3};
  TConstraint cs[] = {c1,c2,c3};
  TLinearProgram lp = {LEN(objectiveFunctionCoefficients), objectiveFunctionCoefficients , LEN(cs), cs};
     
  TLinearProgramSolution *lpSolution = solver(&lp);
  if(!lpSolution)
  {
    printf("no solution found\n");
    exit(1);
  }
  else
  {
    printf("Solution: %8.2f = ", lpSolution->solution);
    for(int i=0; i < lpSolution->numObjectiveFunctionCoefficients; i++)
    {
      printf("%.2f * %.2f ", lp.objectiveFunctionCoefficients[i],lpSolution->values[i]);
      if(i < lpSolution->numObjectiveFunctionCoefficients - 1)
      {
        printf("+ ");
      }
    }
    printf("\n");

    // free solution
    free(lpSolution->values);
    free(lpSolution);
  }
}

void testcase_2(void)
{
  double objectiveFunctionCoefficients[] = {2.0,3.0};
  double cc1[] = {4,5};
  TConstraint c1 = {20, LEN(cc1), cc1};
  double cc2[] = {8,3};
  TConstraint c2 = {24, LEN(cc2), cc2};
  TConstraint cs[] = {c1,c2};
  TLinearProgram lp = {LEN(objectiveFunctionCoefficients), objectiveFunctionCoefficients , LEN(cs), cs};
     
  TLinearProgramSolution *lpSolution = solver(&lp);
  if(!lpSolution)
  {
    printf("no solution found\n");
    exit(1);
  }
  else
  {
    printf("Solution: %8.2f = ", lpSolution->solution);
    for(int i=0; i < lpSolution->numObjectiveFunctionCoefficients; i++)
    {
      printf("%.2f * %.2f ", lp.objectiveFunctionCoefficients[i],lpSolution->values[i]);
      if(i < lpSolution->numObjectiveFunctionCoefficients - 1)
      {
        printf("+ ");
      }
    }
    printf("\n");

    // free solution
    free(lpSolution->values);
    free(lpSolution);
  }
}

void testcase_3(void)
{
  double objectiveFunctionCoefficients[] = {10.0,-12.0,5.0};
  double cc1[] = {-1,2,3};
  TConstraint c1 = {15, LEN(cc1), cc1};
  double cc2[] = {4,5,5};
  TConstraint c2 = {20, LEN(cc2), cc2};
  double cc3[] = {3,-4,5};
  TConstraint c3 = {21, LEN(cc3), cc3};
  TConstraint cs[] = {c1,c2,c3};
  TLinearProgram lp = {LEN(objectiveFunctionCoefficients), objectiveFunctionCoefficients , LEN(cs), cs};
     
  TLinearProgramSolution *lpSolution = solver(&lp);
  if(!lpSolution)
  {
    printf("no solution found\n");
    exit(1);
  }
  else
  {
    printf("Solution: %8.2f = ", lpSolution->solution);
    for(int i=0; i < lpSolution->numObjectiveFunctionCoefficients; i++)
    {
      printf("%.2f * %.2f ", lp.objectiveFunctionCoefficients[i],lpSolution->values[i]);
      if(i < lpSolution->numObjectiveFunctionCoefficients - 1)
      {
        printf("+ ");
      }
    }
    printf("\n");

    // free solution
    free(lpSolution->values);
    free(lpSolution);
  }
}

void montecarlo_gradient_testcase4()
{
  VEC_TYPE(4)

  double max_z = -DBL_MAX;
  VEC_SET(4,max,0.0,0.0,0.0,0.0)
  VEC_SET(4,last_max,0.0,0.0,0.0,0.0)
  VEC_SET(4,grad,0.0,0.0,0.0,0.0)

  
  long iterations = 100000;
  srand((unsigned int)time(NULL));

  VEC_SET(4, scale         , 51.0,22.0,22.0,51.0)
  VEC_SET(4, vicinitySmall , 0.9,0.9,0.9,0.9)
  VEC_SET(4, vicinityBig   , 9,9,9,9)
  VEC_SET(4, vicinityGiant , 20,20,20,20)
  VEC_SET(4, destCoefs     , 2.0,-1.0,5.0,-5.0)
  VEC_SET(4, constrCoefs1  , 2.0, 3.0,0.0,-5.0)
  VEC_SET(4, constrCoefs2  , 0.0, 1.0,1.0, 0.0)
  VEC_SET(4, constrCoefs3  , 2.0,-3.0,3.0, 5.0)
  VEC(4, new)
  VEC(4, scaleGrad)
   
  double scaleStep = 1;
  
  for( long i = 0; i < iterations; i++)
  {
    VEC_SCALE(scaleGrad,grad,scaleStep)
    VEC_ADD(new, max, scaleGrad)
    double z = VEC_SCALAR_PROD(destCoefs, new);
    if( VEC_SCALAR_PROD(constrCoefs1, new) <= 12 && 
        VEC_SCALAR_PROD(constrCoefs2, new) <= 22 &&
        VEC_SCALAR_PROD(constrCoefs3, new) <= 51 &&
        VEC_ALL_POS(new)
      )
    {
      if(max_z < z)
      {
        VEC_SUB(grad, new, last_max)
        VEC_NORM(grad,grad)
        last_max = max;
        max = new;
        max_z = z;
      }
      else
      {
        VEC_RANDOM(grad)
        VEC_NORM(grad,grad)
      }
    }
    else
    {
      VEC_RANDOM(grad)
      VEC_NORM(grad,grad)
    }
  }
  printf("\n------- monte carlo result -------\n");
  printf("%.2f = 2 * %.2f - 1 * %.2f + 5 * %.2f - 5 * %.2f\n", max_z, max._xs[0], max._xs[1], max._xs[2], max._xs[3]);
  printf("%.2f, %.2f, %.2f, %.2f\n",VEC_SCALAR_PROD(destCoefs, max),VEC_SCALAR_PROD(constrCoefs1, max),VEC_SCALAR_PROD(constrCoefs2, max),VEC_SCALAR_PROD(constrCoefs3, max));
  assert(VEC_SCALAR_PROD(constrCoefs1, max) <= 12);
  assert(VEC_SCALAR_PROD(constrCoefs2, max) <= 22);
  assert(VEC_SCALAR_PROD(constrCoefs3, max) <= 51);
}


void testcase_4(void)
{
  double objectiveFunctionCoefficients[] = {2.0,-1.0,5.0,-5.0};
  double cc1[] = {2,3,0,-5};
  TConstraint c1 = {12, LEN(cc1), cc1};
  double cc2[] = {0,1,1,0};
  TConstraint c2 = {22, LEN(cc2), cc2};
  double cc3[] = {2,-3,3,5};
  TConstraint c3 = {51, LEN(cc3), cc3};
  TConstraint cs[] = {c1,c2,c3};
  TLinearProgram lp = {LEN(objectiveFunctionCoefficients), objectiveFunctionCoefficients , LEN(cs), cs};
     
  TLinearProgramSolution *lpSolution = solver(&lp);
  if(!lpSolution)
  {
    printf("no solution found\n");
    exit(1);
  }
  else
  {
    printf("Solution: %8.2f = ", lpSolution->solution);
    for(int i=0; i < lpSolution->numObjectiveFunctionCoefficients; i++)
    {
      printf("%.2f * %.2f ", lp.objectiveFunctionCoefficients[i],lpSolution->values[i]);
      if(i < lpSolution->numObjectiveFunctionCoefficients - 1)
      {
        printf("+ ");
      }
    }
    printf("\n");

    // free solution
    free(lpSolution->values);
    free(lpSolution);
  }
}


int main(void)
{
  testcase_1();
  testcase_2();
  testcase_3();
  testcase_4();
}   
