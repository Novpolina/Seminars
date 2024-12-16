#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int shmid;
    key_t key;
    char *str;

    // Генерация ключа для сегмента общей памяти
    key = ftok("2a.c", 65);  
    if (key < 0) {
        perror("Error generating key");
        exit(1);
    }

    // Создание сегмента общей памяти
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    if (shmid < 0) {
        perror("Error creating shared memory");
        exit(1);
    }

    // Подключение к сегменту общей памяти
    str = (char *)shmat(shmid, NULL, 0);
    if (str == (char *)(-1)) {
        perror("Error attaching shared memory");
        exit(1);
    }

    // Копирование исходного текста в сегмент
    const char *text = "Hello from the creator program!";
    strcpy(str, text);

    printf("Text copied to shared memory: %s\n", str);

    // Отключение от сегмента
    shmdt(str);

    return 0;  
}