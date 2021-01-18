CC=gcc
MPCC=mpicc
EXE_SEQ_PAR=seq_with_parallel_sort
EXE_SEQ=sequential
EXE_PTH=pthreads
EXE_OMP=openmp
EXE_MPI=mpi
EXE_HYB=hybrid

build: $(EXE_SEQ) $(EXE_PTH) $(EXE_OMP) $(EXE_MPI) $(EXE_HYB)

$(EXE_SEQ): encoder.c encoder.h sorter.c sorter.h sequential.c
	$(CC) sequential.c encoder.c sorter.c -o $@

$(EXE_PTH): encoder.c encoder.h sorter.c sorter.h pthreads.c
	$(CC) pthreads.c encoder.c sorter.c -lm -pthread -o $@

$(EXE_OMP): parallel_encoder.c encoder.h parallel_sorter.c sorter.h openmp.c
	$(CC) openmp.c parallel_encoder.c parallel_sorter.c -fopenmp -o $@

$(EXE_MPI): encoder.c encoder.h sorter.c sorter.h mpi.c
	$(MPCC) -g mpi.c parallel_encoder.c parallel_sorter.c -lm -o $@

$(EXE_HYB): parallel_encoder.c encoder.h parallel_sorter.c sorter.h mpi.c
	$(MPCC) -g mpi.c encoder.c parallel_sorter.c -fopenmp -lm -o $@

run_seq: $(EXE_SEQ)
	./$(EXE_SEQ)

run_pth: $(EXE_PTH)
	./$(EXE_PTH)

run_omp: $(EXE_OMP)
	./$(EXE_OMP)

run_mpi: $(EXE_MPI)
	mpirun -np 8 $(EXE_MPI)

run_hyb: $(EXE_HYB)
	mpirun -np 8 $(EXE_HYB)

clean:
	rm -f $(EXE_SEQ) $(EXE_PTH) $(EXE_OMP) $(EXE_SEQ_PAR) $(EXE_MPI) $(EXE_HYB)