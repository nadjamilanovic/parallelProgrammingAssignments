

#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "sum_algorithms.h"

#define ORDERS_OF_MAGNITUDE 1.0e9

void cpu_timer_start(struct timespec *tstart_cpu) {
   clock_gettime(CLOCK_MONOTONIC, tstart_cpu);
}

double cpu_timer_stop(struct timespec tstart_cpu) {
   struct timespec tstop_cpu, tresult;

   clock_gettime(CLOCK_MONOTONIC, &tstop_cpu);

   tresult.tv_sec = tstop_cpu.tv_sec - tstart_cpu.tv_sec;
   tresult.tv_nsec = tstop_cpu.tv_nsec - tstart_cpu.tv_nsec;

   double result = (double)tresult.tv_sec + (double)tresult.tv_nsec * 1.0e-9;

   return result;
}

static void ensure_csv_header(FILE *fp)
{
   fseek(fp, 0, SEEK_END);
   long pos = ftell(fp);
   if (pos == 0) {
      fprintf(fp, "method,pow_of_two,ncells,accurate_sum,test_sum,diff,relative_diff,runtime_seconds\n");
      fflush(fp);
   }
}

int main(int argc, char *argv[])
{
   printf("========================================\n\n");

   FILE *csv = fopen("results.csv", "a+");
   if (csv == NULL) {
      fprintf(stderr, "Warning: cannot open results.csv for writing. Will continue printing to terminal only.\n");
   } else {
      ensure_csv_header(csv);
   }

   for (int pow_of_two = 10; pow_of_two <= 27; pow_of_two++) {
      long ncells = (long)pow(2.0, (double)pow_of_two);
      long ncellsdiv2 = ncells / 2;

      printf("========================================\n");
      printf("TESTING WITH 2^%d = %ld elements\n", pow_of_two, ncells);
      printf("========================================\n");

      double high_value = 1.0e-1;
      double low_value  = 1.0e-1 / ORDERS_OF_MAGNITUDE;

      double accurate_sum = (double)ncellsdiv2 * high_value +
                            (double)ncellsdiv2 * low_value;

      double *energy = (double *)malloc(ncells * sizeof(double));

      if (energy == NULL) {
         fprintf(stderr, "Error: Failed to allocate memory for %ld elements\n", ncells);
         if (csv) fclose(csv);
         return 1;
      }

      printf("Initializing array (Leblanc problem: high values first, then low values)\n");
      for (long i = 0; i < ncells; i++){
         energy[i] = (i < ncellsdiv2) ? high_value : low_value;
      }

      printf("Expected accurate sum: %-17.16lg\n\n", accurate_sum);

      struct timespec cpu_timer;
      double cpu_time;
      double test_sum;
      double diff, rel;

      cpu_timer_start(&cpu_timer);
      test_sum = do_sum(energy, ncells);
      cpu_time = cpu_timer_stop(cpu_timer);
      diff = test_sum - accurate_sum;
      rel = diff / accurate_sum;
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf   Standard double sum\n",
             accurate_sum, test_sum, diff, rel, cpu_time);
      if (csv) fprintf(csv, "standard_double,%d,%ld,%.17g,%.17g,%.17g,%.17g,%.9g\n",
                       pow_of_two, ncells, accurate_sum, test_sum, diff, rel, cpu_time);

      cpu_timer_start(&cpu_timer);
      test_sum = do_sum_longdouble(energy, ncells);
      cpu_time = cpu_timer_stop(cpu_timer);
      diff = test_sum - accurate_sum;
      rel = diff / accurate_sum;
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf   Long double accumulator\n",
             accurate_sum, test_sum, diff, rel, cpu_time);
      if (csv) fprintf(csv, "long_double_acc,%d,%ld,%.17g,%.17g,%.17g,%.17g,%.9g\n",
                       pow_of_two, ncells, accurate_sum, test_sum, diff, rel, cpu_time);

      cpu_timer_start(&cpu_timer);
      test_sum = do_sum_pairwise(energy, ncells);
      cpu_time = cpu_timer_stop(cpu_timer);
      diff = test_sum - accurate_sum;
      rel = diff / accurate_sum;
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf   Pairwise summation\n",
             accurate_sum, test_sum, diff, rel, cpu_time);
      if (csv) fprintf(csv, "pairwise,%d,%ld,%.17g,%.17g,%.17g,%.17g,%.9g\n",
                       pow_of_two, ncells, accurate_sum, test_sum, diff, rel, cpu_time);

      cpu_timer_start(&cpu_timer);
      test_sum = do_sum_kahan(energy, ncells);
      cpu_time = cpu_timer_stop(cpu_timer);
      diff = test_sum - accurate_sum;
      rel = diff / accurate_sum;
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf   Kahan summation\n",
             accurate_sum, test_sum, diff, rel, cpu_time);
      if (csv) fprintf(csv, "kahan,%d,%ld,%.17g,%.17g,%.17g,%.17g,%.9g\n",
                       pow_of_two, ncells, accurate_sum, test_sum, diff, rel, cpu_time);

      cpu_timer_start(&cpu_timer);
      test_sum = do_sum_neumaier(energy, ncells);
      cpu_time = cpu_timer_stop(cpu_timer);
      diff = test_sum - accurate_sum;
      rel = diff / accurate_sum;
      printf("  accurate sum %-17.16lg sum %-17.16lg diff %10.4lg relative diff %10.4lg runtime %lf   Neumaier (Knuth) summation\n",
             accurate_sum, test_sum, diff, rel, cpu_time);
      if (csv) fprintf(csv, "neumaier,%d,%ld,%.17g,%.17g,%.17g,%.17g,%.9g\n",
                       pow_of_two, ncells, accurate_sum, test_sum, diff, rel, cpu_time);

      free(energy);
      printf("\n");
   }

   if (csv) fclose(csv);

   printf("========================================\n");
   printf("All tests complete!\n");
   printf("========================================\n");

   return 0;
}
