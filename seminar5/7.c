#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 30

int main() {
    int fd1[2], fd2[2];  // Два канала для двусторонней связи
    pid_t result;

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        printf("Can't open pipe\n");
        exit(-1);
    }

    result = fork();

    if (result < 0) {
        printf("Can't fork child\n");
        exit(-1);
    } else if (result > 0) {
        /* Родительский процесс */
        char parent_message[] = "Hello from parent!";
        char child_response[BUFFER_SIZE];

        close(fd1[0]);  // Закрываем конец для чтения канала 1
        close(fd2[1]);  // Закрываем конец для записи канала 2

        // Отправляем сообщение дочернему процессу
        write(fd1[1], parent_message, strlen(parent_message) + 1);
        
        // Читаем ответ от дочернего процесса
        read(fd2[0], child_response, BUFFER_SIZE);
        printf("Parent received: %s\n", child_response);

        close(fd1[1]);  // Закрываем конец для записи
        close(fd2[0]);  // Закрываем конец для чтения
        printf("Parent exit\n");

    } else {
        /* Дочерний процесс */
        char resstring[BUFFER_SIZE];
        char child_message[] = "Hello from child!";

        close(fd1[1]);  // Закрываем конец для записи канала 1
        close(fd2[0]);  // Закрываем конец для чтения канала 2

        // Читаем сообщение от родительского процесса
        read(fd1[0], resstring, BUFFER_SIZE);
        printf("Child received: %s\n", resstring);
        
        // Отправляем ответ родительскому процессу
        write(fd2[1], child_message, strlen(child_message) + 1);

        close(fd1[0]);  // Закрываем конец для чтения
        close(fd2[1]);  // Закрываем конец для записи
        printf("Child exit\n");
    }

    return 0;
}