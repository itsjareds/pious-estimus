CC=gcc
CFLAGS=

MPICC=mpicc
MPIFLAGS=

BINS=intro_mpi pi_serial

all: $(BINS)

intro_mpi: intro_mpi.c
	$(MPICC) $(MPIFLAGS) -o $@ $<

pi_serial: pi_serial.c
	$(CC) $(CFLAGS) -o $@ $< -lm

pi_mpi: pi_mpi.c
	$(MPICC) $(MPIFLAGS) -o $@ $< -lm

%: %.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(BINS)

