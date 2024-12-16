#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
   int     fd;
   size_t  size;
   char    string[] = "Hello, world!";

   if((fd = open("./output/file1.txt", O_WRONLY | O_CREAT, 0666)) < 0){
     printf("Can\'t open file\n");
     exit(-1);
   }

   size = write(fd, string, 14);

   if(size != 14){
     printf("Can\'t write all string\n");
     exit(-1);
   }

   if(close(fd) < 0){
     printf("Can\'t close file\n");
   }

   return 0;
}

//Права доступа созданного файла отличаются от прав доступа, указанных в параметрах системного вызова open(),
//потому что при создании файла реально устанавливаемые права доступа получаются из стандартной
//комбинации параметра mode и маски создания файлов текущего процесса umask, а именно –
//они равны mode & ~umask.