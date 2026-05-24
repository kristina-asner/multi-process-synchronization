#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

union semun {
     int val;
     struct semid_ds *buf;
     unsigned short *array;
     struct seminfo *__buf;
    };

int main(int argc, char *argv[]) {
    int max_num = atoi(argv[1]);
    int num_of_procs = atoi(argv[2]);

    int semid = semget(IPC_PRIVATE, num_of_procs, IPC_CREAT | 0666);

    union semun semarg;
    semarg.array = (unsigned short *) malloc(num_of_procs * sizeof(unsigned short));  
   
    for (int i = 0; i < num_of_procs; i++) {
         semarg.array[i] = num_of_procs - 1 - i;
    }  
    semctl(semid, 0, SETALL, semarg);  

    struct sembuf op;
    op.sem_flg = 0;

    for (int k = 0; k < num_of_procs; k++) {
       
        if (fork() == 0) {
            int current = k + 1;
           
            while (current <= max_num) {
               
                op.sem_num = k;
                op.sem_op = -(num_of_procs - 1);
                semop(semid, &op, 1);
               
                printf("%d\n", current);
                fflush(stdout);
               
               
                for (int j = 0; j < num_of_procs; j++) {
                    if (k != j) {
                        op.sem_num = j;
                        op.sem_op = 1;
                        semop(semid, &op, 1);
                    }
                }
               
                current += num_of_procs;
            }
            exit(0);
        }
    }
    int stat;
    int childprocess = num_of_procs;
    while (childprocess > 0) {
        wait(&stat);
        childprocess--;
    }

    semctl(semid, 0, IPC_RMID);
    return 0;
}