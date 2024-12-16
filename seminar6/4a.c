#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_PROCESSES 2

// Флаги, показывающие, хочет ли процесс войти в критическую секцию
volatile bool want_to_enter[NUM_PROCESSES] = {false, false}; 
// Переменная, указывающая, чей теперь очередь входить
volatile int turn = 0; 

// Функция для входа в критическую секцию
void enter_region(int process) {
    want_to_enter[process] = true; // Устанавливаем флаг, что процесс хочет войти
    turn = 1 - process; // Передаем контроль другому процессу
    // Ждем, пока другой процесс не выйдет из критической секции
    while (want_to_enter[1 - process] && turn == 1 - process);
}

// Функция для выхода из критической секции
void leave_region(int process) {
    want_to_enter[process] = false; // Сбрасываем флаг
}

int main() {
    int *array; // Массив для хранения общих данных
    int shmid; // Идентификатор разделяемой памяти
    int new = 1; // Флаг, указывающий, новый ли процесс
    char pathname[] = "06-3a.c"; // Путь к файлу для генерации ключа
    key_t key;

    // Генерация ключа для разделяемой памяти
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    // Попытка создать разделяемую память
    if ((shmid = shmget(key, 3 * sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            printf("Can't create shared memory\n");
            exit(-1);
        } else {
            // Разделяемая память уже существует, получаем идентификатор
            if ((shmid = shmget(key, 3 * sizeof(int), 0)) < 0) {
                printf("Can't find shared memory\n");
                exit(-1);
            }
            new = 0; // Не новый процесс
        }
    }

    // Присоединение разделяемой памяти
    if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    // Инициализация разделяемой памяти (вне критической секции)
    if (new) {
        array[0] = 1; // Количество запусков первого процесса
        array[1] = 0; // Количество запусков второго процесса
        array[2] = 1; // Общее количество запусков
    } else {
        // Вход в критическую секцию
        enter_region(0);
        array[0] += 1; // Увеличиваем количество запусков первого процесса
        for (long i = 0; i < 2000000000L; i++); // Задержка
        array[2] += 1; // Увеличиваем общее количество запусков
        // Выход из критической секции
        leave_region(0);
    }

    // Вывод результата
    printf("Program 1 was spawned %d times, program 2 - %d times, total - %d times\n", 
           array[0], array[1], array[2]);

    // Отключение от разделяемой памяти
    if (shmdt(array) < 0) {
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    return 0;
}