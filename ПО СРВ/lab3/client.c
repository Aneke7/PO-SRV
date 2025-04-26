// client.c
#include <stdio.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <string.h>
#include <unistd.h>

// Функция для сдвига строки вправо на n символов
void shiftRight(char *str, int n) {
    int len = strlen(str);
    if (len == 0 || n <= 0) {
        // Если строка пустая или n <= 0, ничего не делаем
        return;
    }

    // Обработка случая, когда n больше длины строки
    n = n % len;

    // Создаем временную строку для хранения результата
    char temp[20];
    strncpy(temp, str + (len - n), n);       // Копируем последние n символов
    strncpy(temp + n, str, len - n);         // Копируем оставшуюся часть строки
    temp[len] = '\0';                        // Добавляем завершающий ноль

    // Копируем результат обратно в исходную строку
    strcpy(str, temp);
}

int main(void)
{
    char smsg[20];     // Буфер для отправляемого сообщения
    char rmsg[200];    // Буфер для получаемого ответа
    int coid;          // Идентификатор соединения
    long serv_pid;     // PID сервера

    printf("Prog client, Vvedite PID servera \n");
    scanf("%ld", &serv_pid);

    printf("Vveli %ld \n", serv_pid);
    coid = ConnectAttach(0, serv_pid, 1, 0, 0);
    printf("Connect res %d, vvedite soobshenie: \n", coid);
    scanf("%s", smsg);
    printf("\nVveli %s \n", smsg);

    // Создаем копию исходной строки для отправки
    char shifted_msg[20];
    strcpy(shifted_msg, smsg);

    // Сдвигаем строку вправо на 2 символа
    shiftRight(shifted_msg, 2);

    printf("Otpravka servery: \"%s\"\n", shifted_msg);

    // Отправляем сдвинутую строку серверу
    if (MsgSend(coid, shifted_msg, strlen(shifted_msg) + 1, rmsg, sizeof(rmsg)) == -1) {
        printf("Error MsgSend \n");
        return 1;
    }

    // Выводим ответ сервера
    printf("Otvet servera: \"%s\"\n", rmsg);
    getchar();
    getchar();

    // Освобождаем ресурсы
    ConnectDetach(coid);

    return 0;
}