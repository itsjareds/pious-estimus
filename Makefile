CC=gcc
CFLAGS=

MPICC=mpicc
MPIFLAGS=

TAR=tar

BINS=intro_mpi pi_serial pi_mpi
ARCHIVE=LICENSE README.md intro_mpi.c pi_serial.c pi_mpi.c Makefile pi.pbs

all: $(BINS)

intro_mpi: intro_mpi.c
	$(MPICC) $(MPIFLAGS) -o $@ $<

pi_serial: pi_serial.c
	$(CC) $(CFLAGS) -o $@ $< -lm

pi_mpi: pi_mpi.c
	$(MPICC) $(MPIFLAGS) -o $@ $< -lm

%: %.c
	$(CC) $(CFLAGS) -o $@ $< 

archive: $(ARCHIVE)
	tar -czf asg2.tar.gz $(ARCHIVE)

clean:
	rm -f $(BINS)

