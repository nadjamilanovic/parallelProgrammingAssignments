CC = gcc

CFLAGS = -g -O3 -fstrict-aliasing -ftree-vectorize -fopenmp-simd \
         -march=native -mtune=native -mprefer-vector-width=256 \
         -fopt-info-vec-optimized -fopt-info-vec-missed -lm

all: timestep_opt1 timestep_opt2 timestep_opt3

timestep_opt1: main.o timestep_opt1.o timer.o
	$(CC) $(CFLAGS) -o $@ $^

timestep_opt2: main.o timestep_opt2.o timer.o
	$(CC) $(CFLAGS) -o $@ $^

CFLAGS_OPT3 = $(CFLAGS) -fno-trapping-math -fno-math-errno
timestep_opt3: main.o timestep_opt3.o timer.o
	$(CC) $(CFLAGS_OPT3) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

timestep_opt1.o: timestep_opt1.c
	$(CC) $(CFLAGS) -c $<

timestep_opt2.o: timestep_opt2.c
	$(CC) $(CFLAGS) -c $<

timestep_opt3.o: timestep_opt3.c
	$(CC) $(CFLAGS_OPT3) -c $<

timer.o: timer.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o timestep_opt1 timestep_opt2 timestep_opt3
