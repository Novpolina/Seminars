#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
   int     *array;
   int     shmid;
   int     new = 1;
   char    pathname[] = "3b.c";
   key_t   key;
   long    i;

   if((key = ftok(pathname,0)) < 0){
     printf("Can\'t generate key\n");
     exit(-1);
   }

   if((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){

      if(errno != EEXIST){
         printf("Can\'t create shared memory\n");
         exit(-1);
      } else {
         if((shmid = shmget(key, 3*sizeof(int), 0)) < 0){
            printf("Can\'t find shared memory\n");
            exit(-1);
	 }
         new = 0;
      }
   }

   if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){
      printf("Can't attach shared memory\n");
      exit(-1);
   }

   if(new){
      array[0] =  0;
      array[1] =  1;
      array[2] =  1;
   } else {
      array[1] += 1;
      for(i=0; i < 200000000L; i++);
      array[2] += 1;
   }

   printf
      ("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
      array[0], array[1], array[2]);

   if(shmdt(array) < 0){
      printf("Can't detach shared memory\n");
      exit(-1);
   }

   return 0;
}

/*Программа 1 (когда она будет запущена во второй раз) увидит, что память уже инициализирована, и выведет на экран увеличенные значения для array[0] (количество запусков программы 1) и array[2] (общее количество запусков)
Программа 2 при дальнейших запусках также продолжит увеличивать свои вращения в array[1] и общее количество в array[2].
В зависимости от порядка запусков и задержки пропуска циклов (из-за использования for(i=0; i<2000000000L; i++);), вы увидите, что количество запусков может увеличиваться, и значения могут изменяться по сравнению с предыдущими запусками — то есть, программы будут делиться данными через общую память. Это позволяет отслеживать, сколько раз каждая программа была запущена, даже если они выполняются параллельно.
Вывод будет выглядеть примерно так: */
