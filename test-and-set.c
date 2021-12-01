//Test-and-set
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

int l= 0;

int lock(int* verrou){
    int t=1;
    while(t == 1){
        asm("movl $1 %eax"
        "xchgl   %eax, $0"
        : "=m" (*verrou)
        : "m" (t)
        : "%eax"
        );
    }
}

int unlock(int* verrou){
    int t=0;
    asm(
        "movl    $0, %eax  "   
        "xchgl   %eax, $1"
        :"=m" (*verrou)
        :"m" (t)
        :"%eax"
    );
}

