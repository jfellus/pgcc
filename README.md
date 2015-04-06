#####################################################
# PGCC : A Promethe-to-GCC Neural Networks Compiler #
#####################################################

Version 1.0 by J.Fellus [apr. 2015]


## BUILDING

Just type 'make'


## RUNNING

pgcc <some_script_file> <output_directory>

A C++ Project is generated that fully realizes the given script.



## FEATURES

- PGCC generates handsome and efficient C++ code out of the Neural Net graphs
- PGCC allows nested networks (macroscripts in Promethe©) without cries  (see demo)
- PGCC takes care of parallel execution for you by infering async/sync/sequential links (see demo)
- PGCC automatically puts semaphores for links specified as synchronous. 
- All generated C++ code has maximal inlining opportunities, 
- PGCC takes benefit of every GCC functionalities and features (easy debugging, errors and warning reporting in 
   a readable fashion, ...)
- With PGCC, you explicitly know how many threads will be created during the whole execution (just take a look at main.cpp)



## CAVEATS

- Lacks timescales (but coming soon !)
- Lacks syntax converter from old promethe nets and scripts (coming soon !)
- Lacks graphical editor (but Coeos++ is just 'round the corner...)



## BUGS

No known bug

