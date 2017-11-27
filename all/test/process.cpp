#include "process.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdlib.h>


#include <sys/types.h>
#include <sys/stat.h>
#include  <fcntl.h>

#include <linux/fs.h>
#include <limits.h>

int test_fork_wait()
{

    int status;
    pid_t pid;
    if (!fork ())
    {
        printf ("Child process pid=%d\n", getpid());
        sleep(1);
        abort();
        exit(EXIT_FAILURE);
        return 1;
    }
    printf ("start waiting...\n");
    pid = wait (&status);
    if (pid == -1)
        perror ("wait");
    printf ("pid=%d\n", pid);
    if (WIFEXITED (status))
        printf ("Нормальное завершение, статус=%d\n",
                WEXITSTATUS (status));
    if (WIFSIGNALED (status))
        printf ("Убит сигналом=%d%s\n",
                WTERMSIG (status), 188,
                WCOREDUMP (status) ? " (dumped core)" : "");
    if (WIFSTOPPED (status))
        printf ("Остановлен сигналом=%d\n",
                WSTOPSIG (status));
    if (WIFCONTINUED (status))
        printf ("Продолжен\n");
    return 0;

}

int test_daemon() {

    pid_t pid;
    int i;
    /* создание нового процесса */
    pid = fork ();
    if (pid == -1)
    {
        printf ("start waiting...\n");
        return -1;
    }
    else if (pid != 0)
    {
        printf ("end parent...\n");
        exit (EXIT_SUCCESS);
    }
    printf ("Child...\n");
    /* создание нового сеанса и группы процессов */
    if (setsid () == -1)
        return -1;
    /* установка в качестве рабочего каталога корневого каталога */
    if (chdir ("/") == -1)
        return -1;
    /* закрытие всех открытых файлов */
    /* NR_OPEN это слишком, но это работает */
    for (i = 0; i < FOPEN_MAX; i++)
        close (i);
    /* перенаправление дескрипторов файла 0,1,2 в /dev/null */

    open ("/dev/null", O_RDWR);
    dup (0);
    dup (0);

    printf ("sleep for 5 sec...\n");
    sleep(5);
    /* stdin */
    /* stdout */
    /* stderror */
    /* всякие действия демона... */
    return 0;


}
