

#ifndef SUM_ALGORITHMS_H
#define SUM_ALGORITHMS_H


double do_sum(double* restrict var, long ncells);


double do_sum_longdouble(double* restrict var, long ncells);


double do_sum_pairwise(double* restrict var, long ncells);


double do_sum_kahan(double* restrict var, long ncells);


double do_sum_neumaier(double* restrict var, long ncells);

#endif 
