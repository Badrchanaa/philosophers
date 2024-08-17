# include <semaphore.h>
# include <sys/wait.h>
# include <stdio.h>
# include <fcntl.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>

struct s_tst
{
	sem_t	*sem_test;
};

sem_t	*open_semaphore(const char *name, int val)
{
	sem_t	*sem;

	sem = sem_open(name, O_CREAT | O_EXCL, 0600, val);
	if (sem == SEM_FAILED)
	{
		sem_unlink(name);
		sem = sem_open(name, O_CREAT | O_EXCL, 0600, val);
	}
	return (sem);
}


void	*thread_main(void	*p_tst)
{
	struct s_tst *tst;
	sem_t	*sem_test;
	tst = p_tst;

	sem_test = sem_open("/test_sem3", 0, 0, 0);
	if (sem_test == SEM_FAILED)
	{
		printf("sem opem failed\n");
		return (NULL);
	}
	usleep(5000);
	sem_close(sem_test);
	exit(1);
	return (NULL);
}

int	main(int ac, char **av)
{
	struct s_tst tst;
	sem_t	*sem_test;
	pthread_t thread;

	pthread_create(&thread, NULL, thread_main, &tst);
	pthread_detach(thread);
	tst.sem_test = open_semaphore("/test_sem3", 1);
	while (1)
	{
		sem_wait(tst.sem_test);
		sem_post(tst.sem_test);
		usleep(1000);
	}
	//sem_close(tst.sem_test);
	return (0);
}