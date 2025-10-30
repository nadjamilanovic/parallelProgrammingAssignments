

#include <stdlib.h>
#include <math.h>
#include "sum_algorithms.h"

double do_sum(double* restrict var, long ncells)
{
   double sum = 0.0;
   for (long i = 0; i < ncells; i++){
      sum += var[i];
   }
   return sum;
}

double do_sum_longdouble(double* restrict var, long ncells)
{
   long double sum = 0.0L;
   for (long i = 0; i < ncells; i++){
      sum += (long double) var[i];
   }
   return (double) sum;
}

static double pairwise_rec(double* restrict var, long left, long right)
{
   long n = right - left;
   if (n == 0) return 0.0;
   if (n == 1) return var[left];
   long mid = left + n/2;
   double s1 = pairwise_rec(var, left, mid);
   double s2 = pairwise_rec(var, mid, right);
   return s1 + s2;
}

double do_sum_pairwise(double* restrict var, long ncells)
{
   return pairwise_rec(var, 0, ncells);
}

double do_sum_kahan(double* restrict var, long ncells)
{
   double sum = 0.0;
   double c = 0.0; 
   for (long i = 0; i < ncells; i++){
      double y = var[i] - c;
      double t = sum + y;
      c = (t - sum) - y;
      sum = t;
   }
   return sum;
}

double do_sum_neumaier(double* restrict var, long ncells)
{
   double sum = 0.0;
   double c = 0.0;
   for (long i = 0; i < ncells; i++){
      double t = sum + var[i];
      if (fabs(sum) >= fabs(var[i])) {
         c += (sum - t) + var[i];
      } else {
         c += (var[i] - t) + sum;
      }
      sum = t;
   }
   return sum + c;
}
