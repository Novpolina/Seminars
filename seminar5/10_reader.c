#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd;
    size_t size;
    char resstring[28];
    char name[] = "aaa.fifo";

    // Открываем FIFO для чтения
    if ((fd = open(name, O_RDONLY)) < 0) {
        printf("Can't open FIFO for reading\n");
        exit(-1);
    }

    // Читаем сообщение из FIFO
    size = read(fd, resstring, 28);
    if (size < 0) {
        printf("Can't read string from FIFO\n");
        exit(-1);
    }

    // Выводим прочитанную строку
    printf("Reader exit, resstring: %s\n", resstring);

    // Закрываем FIFO
    close(fd);
    return 0;
}