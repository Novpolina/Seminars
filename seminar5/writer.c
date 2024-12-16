#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd;
    size_t size;
    char *name = "aaa.fifo";

    // Проверяем, существует ли FIFO, и создаем его, если он не существует
    if (mkfifo(name, 0666) < 0) {
            printf("Can't create FIFO\n");
            exit(-1);
    }


    // Открываем FIFO для записи
    if ((fd = open(name, O_WRONLY)) < 0) {
        printf("Can't open FIFO for writing\n");
        exit(-1);
    }

    // Записываем сообщение в FIFO
    size = write(fd, "Hello, world! Hello, polina", 28);
    if (size != 28) {
        printf("Can't write all string to FIFO\n");
        exit(-1);
    }

    // Закрываем FIFO
    close(fd);
    printf("Writer exit\n");

    return 0;
}