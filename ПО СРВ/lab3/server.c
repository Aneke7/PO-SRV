// server.c
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>

// Функция для сдвига строки влево на n символов
void shiftLeft(char *str, int n) {
    int len = strlen(str);
    if (len == 0 || n <= 0) {
        // Если строка пустая или n <= 0, ничего не делаем
        return;
    }

    // Обработка случая, когда n больше длины строки
    n = n % len;

    // Создаем временную строку для хранения результата
    char temp[20];
    strncpy(temp, str + n, len - n);         // Копируем оставшуюся часть строки
    strncpy(temp + (len - n), str, n);       // Копируем первые n символов в конец
    temp[len] = '\0';                        // Добавляем завершающий ноль

    // Копируем результат обратно в исходную строку
    strcpy(str, temp);
}

void server(void)
{
    int rcvid;                 // Указывает кому надо ответить
    int chid;                  // Идентификатор канала
    char message[512];         //

    printf("Server start working \n");

    chid = ChannelCreate(0);   // Создание Канала
    printf("Channel id: %d \n", chid);
    printf("Pid: %d \n", getpid());
    // Воплощается вечно - для сервера это нормально
    while (1)
    {
        // Получить и вывести сообщение

        rcvid = MsgReceive(chid, message, sizeof(message), NULL);
        printf("Poluchili soobsheniye, rcvid %X \n", rcvid);
        printf("Soobsheniye : \"%s\". \n", message);

        // Подготовить ответ
        // Сдвигаем строку влево на 2 символа, чтобы восстановить исходную строку
        shiftLeft(message, 2);

        printf("Vosstanovlennaya stroka: \"%s\"\n", message);

        // Отправляем восстановленную строку клиенту
        MsgReply(rcvid, EOK, message, strlen(message) + 1);
        printf("Otpravleno clienty: \"%s\"\n", message);
    }
}

int main(void)
{
    printf("Prog server \n");
    server();
    sleep(5);
    return 1;
}