#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define MEM_NAME "ourmemory"

int threads(void);
void *thread_1(void *);
void *thread_2(void *);
int process_1(void);
int process_2(void);

// Структура разделяемой памяти
typedef struct {
    sem_t sem;
    int value;
} sh_mem_t;

pthread_mutex_t mut;          // Мьютекс
sh_mem_t *sm;                 // Указатель на разделяемую память

int main(void)
{
    int md;                   // Дескриптор разделяемой памяти
    int childpid;             // Идентификатор дочернего процесса

    /* ИНИЦИАЛИЗАЦИЯ */

    // На случай не удаленного блока памяти
    shm_unlink(MEM_NAME);

    // Создадим разделяемую память
    md = shm_open(MEM_NAME, O_RDWR | O_CREAT, 0777);
    if (md == -1) {
        perror("shm_open()");
        return -1;
    }

    // Установка размера
    if (ftruncate(md, sizeof(*sm)) == -1) {
        perror("ftruncate()");
        return -1;
    }

    // Разметка объекта памяти
    sm = (sh_mem_t*) mmap(NULL, sizeof(*sm), PROT_READ | PROT_WRITE, MAP_SHARED, md, 0);
    if (sm == MAP_FAILED) {
        perror("mmap()");
        return -1;
    }

    // Инициализация семафора
    if (sem_init(&(sm->sem), 1, 1) == -1) {
        perror("sem_init()");
        exit(EXIT_FAILURE);
    }

    // Записываем начальное значение в разделяемую память
    sm->value = 0;

    /* ИСПОЛНЕНИЕ */

    // Создание дочернего процесса
    childpid = fork();
    if (childpid == -1) {
        perror("fork()");
    }
    if (childpid) {
        // Родительский процесс
        if (process_1()) {
            perror("process_1()");
            exit(EXIT_FAILURE);
        }
        // Дождемся завершения дочерних процессов
        wait(NULL);
    } else {
        // Дочерний процесс
        if (process_2()) {
            perror("process_2()");
            exit(EXIT_FAILURE);
        }
        // Поработаем с синхронизацией потоков
        if (threads()) {
            perror("threads()");
            exit(EXIT_FAILURE);
        }
    }

    /* ЗАВЕРШЕНИЕ */

    // Закроем разделяемую область
    close(md);

    // Удалим её
    shm_unlink(MEM_NAME);

    // Уничтожение мьютекса
    pthread_mutex_destroy(&mut);

    return EXIT_SUCCESS;
}

int process_1(void)
{
    // Блокируем память при помощи семафора
    sem_wait(&(sm->sem));
    printf("First process.\n(1) Previous value: %i\n", sm->value);

    // Работа с разделяемой памятью
    sm->value = 10;
    sm->value += 2;
    sm->value *= 4;
    sm->value -= 15;
    printf("(1) Result: %i\n", sm->value);

    // Разблокируем
    sem_post(&(sm->sem));

    return 0;
}

int process_2(void)
{
    // Берем ключ
    sem_wait(&(sm->sem));
    printf("Second process.\n(2) Previous value: %i\n", sm->value);

    // Работа с разделяемой памятью
    sm->value = 7;
    sm->value += 5;
    sm->value *= 2;
    sm->value -= 1;
    printf("(2) Result: %i\n", sm->value);

    // Возвращаем ключ
    sem_post(&(sm->sem));

    return 0;
}

// Функция создает два потока и мьютекс для их синхронизации
int threads(void)
{
    pthread_t thid_1, thid_2; // Идентификаторы потоков

    // Инициализация мьютекса
    if (pthread_mutex_init(&mut, NULL) == -1) {
        perror("pthread_mutex_init()");
        return -1;
    }

    // Создание потоков
    if (pthread_create(&thid_1, NULL, &thread_1, NULL) == -1 ||
        pthread_create(&thid_2, NULL, &thread_2, NULL) == -1) {
        perror("pthread_create()");
        return -1;
    }

    // Дождемся их завершения
    pthread_join(thid_1, NULL);
    pthread_join(thid_2, NULL);

    return 0;
}

// Два потока. Синхронизируется между собой при помощи мьютекса.
void *thread_1(void *arg)
{
    // Захват мьютекса
    pthread_mutex_lock(&mut);

    // Выполнение
    printf("First lock\n");
    sleep(2);
    printf("First unlock\n");

    // Освобождение
    pthread_mutex_unlock(&mut);
}

void *thread_2(void *arg)
{
    // Захват мьютекса
    pthread_mutex_lock(&mut);

    // Выполнение
    printf("Second lock\n");
    sleep(2);
    printf("Second unlock\n");

    // Освобождение
    pthread_mutex_unlock(&mut);
}