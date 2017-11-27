#include "select.h"

#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <errno.h>
#include <poll.h>

#define TIMEOUT 5
#define BUF_LEN 1024

int test_select() {
    struct timeval tv;
    fd_set readfds;
    int ret;
    /* Дожидаемся ввода на stdin. */
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    /* Ожидаем не дольше 5 секунд. */
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    /* Хорошо, а теперь блокировка! */
    ret = select (STDIN_FILENO + 1,
                  &readfds,
                  NULL,
                  NULL,
                  &tv);
    if (ret == -1) {
        perror("select");
        return 1;
    } else if (!ret) {
        printf ("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }
    /*
* Готов ли наш файловый дескриптор к считыванию?
* (Должен быть готов, так как это был единственный fd,
* предоставленный нами, а вызов вернулся ненулевым,
* но мы же тут просто развлекаемся.)
*/
    if (FD_ISSET(STDIN_FILENO, &readfds)) {
        char buf[BUF_LEN+1];
        int len;
        /* блокировка гарантированно отсутствует */
        len = read (STDIN_FILENO, buf, BUF_LEN);
        if (len == -1) {
            perror ("read");
            return 1;
        }
        if (len) {
            buf[len] = '\0';
            printf ("read: %s\n", buf);
        }
        return 0;
    }

    fprintf(stderr, "Этого быть не должно!\n");
    return 1;
}


int test_pool(){
    struct pollfd fds[2];
    int ret;
    /* отслеживаем ввод на stdin */
    fds[0].fd = STDIN_FILENO;
    fds[0].events= POLLIN;
    /* отслеживаем возможность записи на stdout (практически всегда true) */
    fds[1].fd = STDOUT_FILENO;
    fds[1].events = POLLOUT;
    /* Все установлено, блокируем! */
    ret= poll(fds, 2, TIMEOUT* 1000);
    if (ret == -1) {
        perror ("poll");
        return 1;
    }
    if (!ret) {
        printf ("%d seconds elapsed.\n", TIMEOUT);
        return 0;
    }
    if (fds[0].revents &POLLIN)
        printf ("stdin is readable\n");
    if (fds[1].revents &POLLOUT)
        printf ("stdout is writable\n");
    return 0;
}
