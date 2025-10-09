student@itcenter-lab128:~/Desktop/assignment-1$ make valgrind
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./memory_demo
==7671== Memcheck, a memory error detector
==7671== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==7671== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==7671== Command: ./memory_demo
==7671== 
==7671== Invalid write of size 4
==7671==    at 0x1091C6: main (assignment1.c:8)
==7671==  Address 0x4a9e068 is 0 bytes after a block of size 40 alloc'd
==7671==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==7671==    by 0x109185: main (assignment1.c:5)
==7671== 
==7671== Conditional jump or move depends on uninitialised value(s)
==7671==    at 0x1091F4: main (assignment1.c:11)
==7671==  Uninitialised value was created by a stack allocation
==7671==    at 0x109169: main (assignment1.c:3)
==7671== 
==7671== Invalid read of size 4
==7671==    at 0x1091EF: main (assignment1.c:11)
==7671==  Address 0x4a9e068 is 0 bytes after a block of size 40 alloc'd
==7671==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==7671==    by 0x109185: main (assignment1.c:5)
==7671== 
==7671== 
==7671== HEAP SUMMARY:
==7671==     in use at exit: 40 bytes in 1 blocks
==7671==   total heap usage: 1 allocs, 0 frees, 40 bytes allocated
==7671== 
==7671== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1
==7671==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==7671==    by 0x109185: main (assignment1.c:5)
==7671== 
==7671== LEAK SUMMARY:
==7671==    definitely lost: 40 bytes in 1 blocks
==7671==    indirectly lost: 0 bytes in 0 blocks
==7671==      possibly lost: 0 bytes in 0 blocks
==7671==    still reachable: 0 bytes in 0 blocks
==7671==         suppressed: 0 bytes in 0 blocks
==7671== 
==7671== For lists of detected and suppressed errors, rerun with: -s
==7671== ERROR SUMMARY: 14 errors from 4 contexts (suppressed: 0 from 0)

Code explanation:

//int ipos=-1, ival=0;
Before, variables were just declared but they were not initialized. The program could not later use ipos or ival because the value was udefined. Valgrind reported: ==7671== Conditional jump or move depends on uninitialised value(s).

//for (int i = 0; i<10; i++){
        iarray[i] = i; 
    }
My program tried to write outside the allocated memory area. The malloc(10*sizeof(int)) created space for 10 integers but before code was i<=10 which means that loop went from 0 to 10 (including 10), which is 11 elements. So i fixed it by making the last iteration be 10 and cannot write to memory that does not belong to me. Valgrind reported: ==7671== Invalid write of size 4

//for (int i = 0; i<10; i++){
        if (ival == iarray[i]) 
        ipos = i;
    }
Same problem as above, program tried to read from memory that it shouldnt. It reads the element iarray[10] which doesnt exist, since 10 should be last iteration. Valgrind reported: ==7671== Invalid read of size 4

//free(iarray); 
    return 0;
I allocated memory with malloc(), but never freed it. When program ended that memory was still looked like it was in use because it doesnt automatically release them. So i added free() to return that memory to the system. Valgrind reported: ==7671== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1

student@itcenter-lab128:~/Desktop/assignment-1$ make valgrind
gcc -Wall -Wextra -g -std=c99 -o memory_demo assignment1.c
assignment1.c: In function ‘main’:
assignment1.c:23:20: warning: format ‘%d’ expects a matching ‘int’ argument [-Wformat=]
   23 |     printf("Value %d found at position %d\n");//
      |                   ~^
      |                    |
      |                    int
assignment1.c:23:41: warning: format ‘%d’ expects a matching ‘int’ argument [-Wformat=]
   23 |     printf("Value %d found at position %d\n");//
      |                                        ~^
      |                                         |
      |                                         int
assignment1.c:5:9: warning: variable ‘ipos’ set but not used [-Wunused-but-set-variable]
    5 |     int ipos=-1, ival=0; //
      |         ^~~~
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./memory_demo
==9766== Memcheck, a memory error detector
==9766== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==9766== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==9766== Command: ./memory_demo
==9766== 
Value -16778216 found at position 36
==9766== 
==9766== HEAP SUMMARY:
==9766==     in use at exit: 0 bytes in 0 blocks
==9766==   total heap usage: 2 allocs, 2 frees, 1,064 bytes allocated
==9766== 
==9766== All heap blocks were freed -- no leaks are possible
==9766== 
==9766== For lists of detected and suppressed errors, rerun with: -s
==9766== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)