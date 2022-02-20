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
#include <linear_solver.h>

//#define __TRACE_SOLVER__
#undef __TRACE_SOLVER__

#define LEN(a) ((sizeof a)/(sizeof *a))

//MAYBE_TYPE(int);
//MAYBE_TYPE(double);
//MAYBE_FAIL(int)
//MAYBE_FAIL(double)
//
//typedef struct
//{
//  int row;
//  int col;
//  double val;
//} TTabloCell;
//MAYBE_TYPE(TTabloCell);
//MAYBE_FAIL(TTabloCell)
//
//typedef struct
//{
//  double solution;
//  int numCoefficients;
//  double *coefficients;
//} TConstraint;
//
//
//typedef struct
//{
//  int numObjectiveFunctionCoefficients;
//  double *objectiveFunctionCoefficients;
//  int numConstraints;
//  TConstraint *constraints;
//
//} TLinearProgram;
//
//typedef struct
//{
//  int numObjectiveFunctionCoefficients;
//  double *values;
//  double solution;
//} TLinearProgramSolution;
//
//MAYBE(TTabloCell) maybeMin(MAYBE(TTabloCell) a, TTabloCell b)
//{
//  MAYBE(TTabloCell) res;
//  if(IS_NOTHING(TTabloCell,a)) 
//  {
//    NOTHING(TTabloCell, res)
//  }
//  else 
//  {
//    TTabloCell valOfa = MAYBE_VALUE(TTabloCell,a);
//    if(valOfa.val < b.val)
//    {
//      res = a;
//    }
//    else
//    {
//      INIT_SOME(TTabloCell, someB, b);
//      res = someB;
//    }
//  }
//  return res;
//}
//
//void traceLPTablo(TLinearProgram *lp, double *t, int rows, int cols)
//{
//  const int maxIntPos = 3;
//  const int maxFloatPos = 8;
//  const int maxDecimalPos = 2;
//  const int maxOutLineLen = 1024;
//  char intFmt[32];
//  char fltFmt[32];
//  char strFmt0[32];
//  char strFmt1[32];
//  char outLineFmt[maxOutLineLen];
//  const int maxBuf = 255;
//  char buf[maxBuf];
//
//  int numSlack = lp->numObjectiveFunctionCoefficients;
//  int tabloCols = 1 + lp->numObjectiveFunctionCoefficients + numSlack + 1;
//  int tabloRows = 1 + lp->numConstraints;
//#ifdef __DEBUG__
//  assert(rows == tabloRows);
//  assert(cols == tabloCols);
//#endif
//
//  snprintf(intFmt, sizeof intFmt, "%%0%dd", maxIntPos);
//  snprintf(fltFmt, sizeof fltFmt, "%%%d.%df", maxFloatPos, maxDecimalPos);
//  snprintf(strFmt0, sizeof strFmt0, "%%%ds", maxFloatPos);
//  snprintf(strFmt1, sizeof strFmt1, "%%%ds%%d", maxFloatPos-1);
//
//  //
//  //
//  // Header 
//  memset(outLineFmt, '\0', maxOutLineLen);
//  memset(outLineFmt, '-', (maxFloatPos + 1)*tabloCols+1);
//  printf("%s\n",outLineFmt);
//  memset(outLineFmt, '\0', maxOutLineLen);
//  snprintf(buf, sizeof(buf), strFmt0, "z"); 
//  snprintf(outLineFmt, maxOutLineLen, "|%s", buf);
//  for(int c = 1; c <= lp->numConstraints; c++)
//  {
//    snprintf(buf, sizeof(buf), strFmt1, "x", c ); 
//    snprintf(outLineFmt, maxOutLineLen, "%s|%s", outLineFmt, buf); 
//  }
//  for(int c = 1; c <= numSlack; c++)
//  {
//    snprintf(buf, sizeof(buf), strFmt1, "s", c ); 
//    snprintf(outLineFmt, maxOutLineLen, "%s|%s", outLineFmt, buf); 
//  }
//  snprintf(buf, sizeof(buf), strFmt0, "solution"); 
//  snprintf(outLineFmt, maxOutLineLen, "%s|%s|", outLineFmt, buf); 
//  printf("%s\n",outLineFmt);
//  memset(outLineFmt, '\0', maxOutLineLen);
//  memset(outLineFmt, '-', (maxFloatPos + 1)*tabloCols+1);
//  printf("%s\n",outLineFmt);
//  // Tablo
//  // 1 col Format
//  for(int r = 0; r < tabloRows; r++)
//  {
//    memset(outLineFmt, '\0', maxOutLineLen);
//    for(int c = 0; c < cols; c++)
//    {
//      //snprintf(buf, sizeof(buf), fltFmt, t[r][c]); 
//      snprintf(buf, sizeof(buf), fltFmt, (t + r*sizeof(double))[c]); 
//      snprintf(outLineFmt, maxOutLineLen, "%s|%s", outLineFmt, buf); 
//    }
//    snprintf(outLineFmt, maxOutLineLen, "%s|", outLineFmt); 
//    printf("%s\n",outLineFmt);
//  }
//  memset(outLineFmt, '-', (maxFloatPos + 1)*tabloCols+1);
//  printf("%s\n",outLineFmt);
//}
//
//TLinearProgramSolution *solver(TLinearProgram *lp)
//{
//  int numSlack = lp->numObjectiveFunctionCoefficients;
//  int tabloCols = 1 + lp->numObjectiveFunctionCoefficients + numSlack + 1;
//  int tabloRows = 1 + lp->numConstraints;
//  double tablo[tabloRows][tabloCols];
//  
//  // initalize tablo :
//  // z |  x1 | ... | xn  | s1 | s2 | s3 | ... | sm | sol |
//  // ---------------------------------------------------
//  // 1 | o1  | ... | on  | 0  | 0  | 0  | ... | 0  |  0  |
//  // 0 | c11 | ... | x1n | 1  | 0  | 0  | ... | 0  | sol |
//  // 0 | c21 | ... | x2n | 0  | 1  | 0  | ... | 0  | sol |
//  // 0 | c31 | ... | x3n | 0  | 0  | 1  | ... | 0  | sol |
//  // . | ..  | ... | ... | .. | .. | .. | ... | .. | ... |
//  // 0 | cm1 | ... | xmn | 0  | 0  | 0  | ... | 1  | sol |
//  
//  // 1. initalize Objective Row
//  tablo[0][0] = 1.0;
//  for(int i = 0; i < lp->numObjectiveFunctionCoefficients; i++)
//  {
//    tablo[0][1+i] = -lp->objectiveFunctionCoefficients[i];
//  }
//  for(int i = 0; i < numSlack; i++)
//  {
//    tablo[0][1+lp->numObjectiveFunctionCoefficients+i] = 0.0;
//  }
//  tablo[0][tabloCols-1] = 0;
//
//  // 2. initalize constraint rows
//  for(int r = 1; r < tabloRows; r++)
//  {
//    tablo[r][0] = 0.0;
//#ifdef __DEBUG__ 
////    printf("constraint %d with %d coefficients (should be %d)\n", r-1, lp->constraints[r-1].numCoefficients, lp->numObjectiveFunctionCoefficients);
//    assert(lp->constraints[r-1].numCoefficients == lp->numObjectiveFunctionCoefficients);
//#endif
//    for(int i = 0; i < lp->numObjectiveFunctionCoefficients; i++)
//    {
//      tablo[r][1+i] = lp->constraints[r-1].coefficients[i];
//    }
//    for(int i = 0; i < numSlack; i++)
//    {
//      if((i+1)==r)
//      {
//        tablo[r][1+lp->numObjectiveFunctionCoefficients+i] = 1.0;
//      }
//      else
//      {
//        tablo[0][1+lp->numObjectiveFunctionCoefficients+i] = 0.0;
//      }
//    }
//    tablo[r][tabloCols-1] = lp->constraints[r-1].solution;
//  }
//#if defined(__DEBUG__) && defined(__TRACE_SOLVER__)
//  printf(">> Initial Tablo\n");
//  traceLPTablo(lp, (double *)tablo, tabloRows, tabloCols);
//#endif
//
//  int step = 0;
//  int curRow = 0;
//  double *zrow = tablo[curRow];
//  bool   finished = false;
//  INIT_NOTHING(int,temp);
//  MAYBE(int)    resCoefIdx[lp->numObjectiveFunctionCoefficients];
//  INIT_NOTHING(TTabloCell,enteringVariable);
//  TTabloCell pivotCell;
//
//  // initialize res coef idxes with Nothing
//  for(int i=0; i < lp->numObjectiveFunctionCoefficients; i++)
//  {
//    NOTHING(int,resCoefIdx[i]);
//  }
//
//  // iteration loop
//  while(!finished)
//  {
//
//    // check solution <- all coefficients in zrow are non negative
//    // and find the col with the most negaitve coefficient
//    finished = true;
//    step++;
//    for(int i = 0; i < tabloCols-1; i++) // -1 because last col is solution col
//    {
//      // finished when all zrow coeeficients are non negatigve
//      if(zrow[i] < 0)
//      {
//        finished = false;
//        TTabloCell cell = {curRow, i, zrow[i]};
//        if(IS_NOTHING(TTabloCell,enteringVariable))
//        {
//          SOME(TTabloCell, enteringVariable, cell);
//        }
//        else
//        {
//          enteringVariable = maybeMin(enteringVariable, cell);
//        }
//      }
//    }
//
//    if(!finished)
//    {
//      if(IS_NOTHING(TTabloCell, enteringVariable)) // no solution
//      {
//#ifdef __DEBUG__
//        printf("no entering variable found, no soultion found.\n");
//#endif
//        return NULL;
//      }
//      TTabloCell basicCell = MAYBE_VALUE(TTabloCell, enteringVariable);
//#if defined(__DEBUG__) && defined(__TRACE_SOLVER__)
//      printf("Basic Cell %d val %.2f\n",  basicCell.col, basicCell.val);
//#endif
//      INIT_NOTHING(TTabloCell,leavingVar);
//      double ratio;
//
//      // calculate leaving var / pivot cell, which is the smalles positive ratio of basic
//      // cell value and solution
//      for(int i=0; i < tabloRows; i++)
//      {
//        if(i != basicCell.row)
//        {
//          double nominator = tablo[i][basicCell.col];
//          ratio = nominator == 0 ? 0: tablo[i][tabloCols-1]/nominator;
//          if(ratio > 0) // must be positive
//          {
//            TTabloCell c = {i, basicCell.col, ratio};
//            if(IS_NOTHING(TTabloCell, leavingVar))
//            {
//              SOME(TTabloCell, leavingVar, c);
//            }
//            else
//            {
//              leavingVar = maybeMin(leavingVar, c);
//            }
//          }
//        }
//      }
//      if(!IS_NOTHING(TTabloCell, leavingVar))
//      {
//        // leavingVar contains the pivot cell
//        TTabloCell lv = MAYBE_VALUE(TTabloCell, leavingVar);
//        pivotCell.row = lv.row;
//        pivotCell.col = lv.col;
//        pivotCell.val = tablo[lv.row][lv.col];
//#if defined(__DEBUG__) && defined(__TRACE_SOLVER__)
//        printf("Leaving Var %d %d val %.2f\n",  lv.row, lv.col, lv.val);
//#endif
//
//        // normalize pivot row by pivot value
//        for(int i=0; i < tabloCols; i++)
//        {
//          tablo[pivotCell.row][i] /= pivotCell.val;
//        }
//        // intersect all other rows thus
//        //  subtract tablo[row][pivotCell.col] from each colum value
//        //  multiply by pivot row
//        double calcRow[tabloCols];
//        for(int r=0; r < tabloRows; r++)
//        {
//          if(r != pivotCell.row)
//          {
//            for(int c = 0; c < tabloCols; c++)
//            {
//              calcRow[c] = tablo[r][c] - tablo[r][pivotCell.col]*tablo[pivotCell.row][c]; 
//            }
//            for(int c = 0; c < tabloCols; c++)
//            {
//              tablo[r][c] = calcRow[c];
//            }
//          }
//        }
//      }
//      else
//      {
//#ifdef __DEBUG__
//        printf("no leaving variable found, no solution found.\n");
//#endif
//        return NULL;
//      }
//
//#if defined(__DEBUG__) && defined(__TRACE_SOLVER__)
//      printf(">> Tablo after %d step \n", step);
//      traceLPTablo(lp, (double *)tablo, tabloRows, tabloCols);
//#endif
//    }
//    // check if variable is ready and remember the row
//    if( pivotCell.col > 0 && pivotCell.col <= lp->numObjectiveFunctionCoefficients)
//    {
//      SOME(int,resCoefIdx[pivotCell.row-1],pivotCell.col-1);
//    }
//    NOTHING(TTabloCell, enteringVariable);
//  } // while(!finiehd)
//
//  // collect the solution
//  TLinearProgramSolution *lpSolution = malloc(sizeof(TLinearProgramSolution));
//  lpSolution->values = malloc(sizeof(double)*lp->numObjectiveFunctionCoefficients);
//  lpSolution->numObjectiveFunctionCoefficients = lp->numObjectiveFunctionCoefficients;
//  lpSolution->solution = zrow[tabloCols-1];
//  memset(lpSolution->values, 0.0, sizeof(double)*lp->numObjectiveFunctionCoefficients);
//  for(int r=0; r < lp->numObjectiveFunctionCoefficients; r++)
//  {
//    if(!IS_NOTHING(int, resCoefIdx[r]))
//    {
//      lpSolution->values[MAYBE_VALUE(int,resCoefIdx[r])] = tablo[1+r][tabloCols-1];
//    }
//  }
//#if defined(__DEBUG__) && defined(__TRACE_SOLVER__)
//  printf(">> Solution %8.2f\n", lpSolution->solution);
//  printf(">> Variable Values: ");
//  for(int i=0; i < lpSolution->numObjectiveFunctionCoefficients; i++)
//  {
//    printf("x_%d = %.2f ", i+1,lpSolution->values[i]);
//    if(i < lpSolution->numObjectiveFunctionCoefficients - 1)
//    {
//      printf(", ");
//    }
//  }
//  printf("\n");
//  printf(">> Formula: ");
//  printf("%8.2f = ", lpSolution->solution);
//  for(int i=0; i < lpSolution->numObjectiveFunctionCoefficients; i++)
//  {
//    printf("%.2f * %.2f ", lp->objectiveFunctionCoefficients[i],lpSolution->values[i]);
//    if(i < lpSolution->numObjectiveFunctionCoefficients - 1)
//    {
//      printf("+ ");
//    }
//  }
//  printf("\n");
//#endif
//  return lpSolution;
//}

//MAYBE(int) monadicInc(int i)
//{
//  INIT_SOME(int, res, i+1);
//  return res;
//}
//
//MAYBE(int) monadicDbl(int i)
//{
//  INIT_SOME(int, res, i*2);
//  return res;
//}

int main(void)
{
  //// maybe try
  //MAYBE(int) a;
  //SOME(int, a, 0);
  //MAYBE(int) b = MAYBE_MAP(a, monadicInc);
  //MAYBE(int) c = MAYBE_JOIN(monadicInc, monadicDbl, a);
  //printf("b = %d, c = %d", MAYBE_VALUE(int,b), MAYBE_VALUE(int,c));
  
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
    
