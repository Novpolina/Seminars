#include <pthread.h>  
#include <stdio.h>    
#include <stdlib.h>   

int a = 0; // Глобальная переменная, которая будет изменяться потоками

// Функция, которая будет выполняться в новом потоке
void *mythread(void *dummy) {
    /*
     *  Параметр void *dummy используется лишь для совместимости типов 
     *  и не будет использован в этой функции. Аналогично, возвращаемое 
     *  значение функции имеет тип void *, но не будет использовано в этой программе.
     */

    pthread_t mythid; // Идентификатор создаваемого потока
    mythid = pthread_self(); // Получаем идентификатор текущего потока

    a = a + 1; // Увеличиваем глобальную переменную a
    // Выводим идентификатор потока и значение a
    printf("Thread %u, Calculation result = %d\n", mythid, a);

    return NULL; // Возвращаем NULL, так как результат не важен
}

int main() {
    pthread_t thid, mythid; // Идентификаторы потоков
    int result; // Переменная для хранения результата создания потока

    // Создаем новый поток, который выполнит функцию mythread
    result = pthread_create(&thid, (pthread_attr_t *)NULL, mythread, NULL);

    // Проверяем, успешно ли создан поток
    if(result != 0) {
        printf("Error on thread create, return value = %d\n", result);
        exit(-1); // Завершаем программу в случае ошибки
    }

    // Выводим идентификатор созданного потока
    printf("Thread created, thid = %u\n", thid);

    mythid = pthread_self(); // Получаем идентификатор текущего потока (основного)

    a = a + 1; // Увеличиваем глобальную переменную a в основном потоке
    // Выводим идентификатор основного потока и значение a
    printf("Thread %u, Calculation result = %d\n", mythid, a);

    // Ожидаем завершения потока thid перед продолжением исполнения программы
    pthread_join(thid, (void **)NULL);

    return 0; // Завершение программы
}

/*Пояснения к работе программы:

Глобальная переменная a: Эта переменная объявлена вне функций и может быть изменена как основным, 
так и созданным потоком, что делает её общей для них.

Функция mythread: Это функция, которая будет выполняться в новом потоке. В ней также используется глобальная 
переменная a. Поток увеличивает значение a и выводит его на экран вместе с идентификатором потока.

Создание потока: В функции main создается новый поток с помощью pthread_create, который начинает выполнение 
функции mythread. Если создание потока не удалось, программа будет завершена с соответствующим сообщением об ошибке.

Основной поток: После создания потока основная программа (основной поток) также увеличивает значение 
переменной a и выводит его на экран.

Синхронизация с помощью pthread_join: Основной поток ожидает завершения созданного потока (thid) 
перед тем, как завершить свою работу. Это обеспечивает корректность работы программы, позволяя убедиться, 
что все потоки завершили свою работу до выхода из main.

Команды вывода: Каждая из пременных mytid и thid будет выводить значения, так что мы увидим, как они работают параллельно. */