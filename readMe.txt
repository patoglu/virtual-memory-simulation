For Part3:

IMPORTANT NOTE: Before running Part3 please run the command: chmod +x ./sortArrays (Because it uses system command.)


This program is finds the best frame size for 4 sort algorithms.
FOR GITHUB:
	
	-In order to understand project purpose and sample usage check CSE 312 Spring 2020 Final Exam Project_Part3_Updated (1).pdf
	-In order to see implementation check Report.pdf


You can build the program with typing make.

you can run the program as:


./best_frame_size FIFO

./best_frame_size SC

./best_frame_size NRU

./best_frame_size LRU

the program requires ./sortArrays executable in order to run it's own(it's present in directory). It's makefile is not

provided because i couldn't keep two makefiles under same directory.



You can try with higher inputs via changing program text.(2d commands array)


For Part 2:




The code unfortunately have a lot duplicate areas i couldn't have enough time
to edit it.

Makefile is provided. The code doesn't have any memory leaks.


Also my page table entry is named with page_table_entry_NRU in my code. That page table struct 
is universal for all page replacement algorithms not just for NRU. It's just the variable name.
