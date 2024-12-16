#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
   int     fd;
   size_t  size = 0;
   char   string[256] = {};

   //(void)umask(0);

   if((fd = open("./output/file3.txt", O_RDWR | O_CREAT, 0666)) < 0){
     printf("Can\'t open file\n");
     exit(-1);
   }
   size = read(fd, string, 256);

   if(size < 0){
     printf("Can\'t read from file\n");
     exit(-1);
   }
   printf("%s", string);
   

   if(close(fd) < 0){
     printf("Can\'t close file\n");
   }

   return 0;
}