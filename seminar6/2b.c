#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int shmid;
    key_t key;
    char *str;

    // Генерация того же самого ключа
    key = ftok("2a.c", 65);
    if (key < 0) {
        perror("Error generating key");
        exit(1);
    }

    // Получение существующего сегмента общей памяти
    shmid = shmget(key, 1024, 0666);
    if (shmid < 0) {
        perror("Error accessing shared memory");
        exit(1);
    }

    // Подключение к сегменту общей памяти
    str = (char *)shmat(shmid, NULL, 0);
    if (str == (char *)(-1)) {
        perror("Error attaching shared memory");
        exit(1);
    }

    // Вывод текста из сегмента на экран
    printf("Text read from shared memory: %s\n", str);

    // Отключение от сегмента и удаление
    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);  // Удаление сегмента из системы

    return 0;
}