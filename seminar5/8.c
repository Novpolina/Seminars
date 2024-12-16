#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define BUFFER_SIZE 30
long get_pipe_size() {
    FILE *fp;
    long pipe_size;

    // Выполняем команду getconf PIPE_BUF и получаем результат
    fp = popen("getconf PIPE_BUF /", "r");
    if (fp == NULL) {
        perror("Failed to run command");
        exit(1);
    }

    // Читаем результат
    if (fscanf(fp, "%ld", &pipe_size) != 1) {
        perror("Failed to read pipe size");
        pclose(fp);
        exit(1);
    }
    
    pclose(fp);
    return pipe_size;
}

int main() {
    
    long pipe_size = get_pipe_size();
    printf("Размер pipe: %ld байт\n", pipe_size);

}