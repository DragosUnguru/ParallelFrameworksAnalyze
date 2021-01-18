                    # Unguru Dragos-Gabriel
                            # 343C1

            # BRIEF INTRODUCTION
    
    The presented project represents my approach at implementing
the Burrows-Wheeler Encoding algorithm alongside the Run Length Encoding
algorithm. The use of these two is fairly common as the BW algorithm rearranges
a string (in a lossless way) to a form where the repeating characters are near
one another. This makes the Run Length Encoding much more efficient.

    To test easier and validate the correct output of the programs,
redirect the output to a text file and run it against the sequential
implementation.

    This implementation encodes each line individually. We'll further observe
how the length of the line impacts both the effectiveness of this encoding and
the performance.
    To understand why the line length impacts the performance so drastically, we have
to understand that in order to implement the Burrows-Wheeler Encoding algorithm we have to

    1. Generate all rotations of the said line;
    2. !!! Sort all these rotations in alphanumeric order (I've used a quicksort approach);
    3. Take the last character of every sorted rotation in order to build the resulted encoded line.

            # HOW TO RUN
    
    Open the Makefile and using the '-D' directive, specify the input file. For running
    on the 'very_long_words.in' file, add the line: -Dvery_long_words.in
    in the compilation line of the desired implementation

    Run 'make' to build all the executables
    Run:
        'make run_seq' -> runs the sequential implementation
        'make run_pth' -> runs the Pthreads implementation
        'make run_omp' -> runs the OpenMP implementation
        'make run_mpi' -> runs the MPI implementation
        'make run_hyb' -> runs the hybrid implementation

    !!! Important note:
        To both test and avoid the big output of the program, run redirecting the output
    to a file. (eg.: make run_pth > pth.out)
        To test the correctness of the implementations, use the 'diff' utilitary against the
    output of the sequential implementation, ignoring the different name of the executable
    and MPI's additional beggining messages.

            # DETAILS
        
    Every implementation has a different approach at parallelizing the algorithm.
I've decided this way not only to explore more points of view and to find the best way,
but also highlighting the limitations of each and every paradigm used implementation-wise.

    Note: The following execution times are for an input file containing very large lines
(up to 20.000 bytes on a line), favoring the parallel quicksort implementation.

    -> SEQUENTIAL
    Avg. time: 3m12.535s
    
    -> PTHREADS
    Avg. time: 0m51.470s
    For the Pthreads implementation, i've chosen the simplest approach. That is
to generate threads that split the work line-wise. Not only this is the most efficient way
to parallelize the program, but it also is the easiest on the programmer.

    -> OPENMP
    Avg. time: 0m46.899s
    The OpenMP utilitary is the easiest on the programmer. In this implementation, not only
did it make life easier to parallelize encoding tasks, but also to parallelize the QuickSort
algorithm. This is the only implementation that has the sorting part parallelized. It also
parallelizes data management parts like freeing allocated memory as it was too easy not to.

    To ensure that we don't deal with any implicit barriers or waiting/blocking mechanisms,
the recursive call to quicksort method is done using the 'parallel for' directive.
    Only this part is bringing real performance gains but only for very long words on lines.
This is because if we have small lines, there won't be too many rotations to sort, therefore,
the overhead of spawning multiple threads is overcome by the fact that there isn't much to sort.
    This also brings into play a large number of threads as every thread is recursively spawning
another two threads, and so on.
    This implementation has the biggest potential with the correct and desired input.

    - MPI
    Avg. time: 0m50.567s
    The primary target here is to not have any idle processes, as the MPI library calls
are usually blocking. Therefore, the approach here is to not flood any process while leaving
others with no work.
    
    To obtain this, this approach implements the BOSS-WORKER paradigm, where the MASTER process
assigns work to all other WORKER threads. To ensure the target described above, the master process
sends work to do to all worker threads (symultaneously) and right after it expects the results before
sending work again to all workers, without flooding any other buffers, processes or staying blocked
in any call.

    The big downside is that MPI's framework comes with the disadvantage that everything must be sent,
there is no shared memory between processes. This comes with a huge performance drawback as there
is a significant overhead in sending and receiving data.

    - HYBRID
    Avg. time: 0m51.206s
    The hybrid method cobines the MPI implementation, giving us the boss-worker paradigm to work on
and the efficient splitting of tasks and the OpenMP implementation, the most promosing one, as it
parallelizes the execution of the work the best, down to the Quicksort algorithm.


#           VERY LONG LINES: long_words.in
                            18.500 characters per line
                            93 lines

-> sequential: 158.748

-> PTHREADS
    1thread:    159.375
    2threads:   88.977
    4threads:   54.084
    8threads:   45.570

-> OPENMP
    1thread:    160.993
    2threads:   82.255
    4threads:   49.732
    8threads:   34.342

-> MPI
    1thread:    409.452
    2threads:   180.397
    4threads:   70.754
    8threads:   40.335

-> HYBRID
    1thread:    389.589
    2threads:   170.802
    4threads:   67.043
    8threads:   41.645


#           SHORT LINES, MANY LINES: input.txt
                            44 characters per line
                            999600 lines

-> sequential: 140.722

-> PTHREADS
    1thread:    31.232      ?????????????????????????
    2threads:   20.796      ?????????????????????????
    4threads:   21.744      ?????????????????????????
    8threads:   26.789      ?????????????????????????

-> OPENMP
    1thread:    177.465
    2threads:   77.586
    4threads:   42.080
    8threads:   41.596

-> MPI
    1thread:    -
    2threads:   32.800      28.692s
    4threads:   22.839      18.935s
    8threads:   19.134      17.051s

-> HYBRID
    1thread:    -
    2threads:   87.811
    4threads:   
    8threads:   