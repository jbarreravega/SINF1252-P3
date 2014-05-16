//author :  Juan Sebastian Barrera Vega (94471000)
//          Olivier Boegaerts (44481100)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <limits.h>

#include "benchmark.h"

#define MAX_SIZE  0x100000 // 1MB

#define MIN(a,b) ((a) < (b) ? (a) : (b))

int main (int argc, char *argv[])
{
	timer *t = timer_alloc();
	recorder *writev_rec = recorder_alloc("writev.csv");
	recorder *write_rec = recorder_alloc("write.csv");

	int size;
	int fildes = open("tmp.dat", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	ssize_t len;
	off_t lsk;
	if(fildes == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	char *s = (char *) malloc((MAX_SIZE + 1) * sizeof(char));
	//char *s2 = (char *) malloc((MAX_SIZE + 1) *10* sizeof(char));
	memset(s, 0, MAX_SIZE + 1);

	const int numVec;

	struct iovec* iovectors = malloc(10 * sizeof (struct iovec));
	if(iovectors == NULL)
	{
		perror("malloc");
		return EXIT_FAILURE;
	}
	int i;
	for (i=0; i<10;i++)
	{
		iovectors[i].iov_base=(void *) s;//creation de 10 buffers
	}


	for (size = 1; size <= MAX_SIZE; size *=2 )
	{

	int j;
	for (j=0; j<10;j++)
	{
		iovectors[j].iov_len=size;//les buffers a ecrire sont de taille size
	}
		start_timer(t);
		int k;
		len = writev(fildes, iovectors, 10);
		write_record(writev_rec, size, stop_timer(t));//on ecrit les 10 buffers en une fois
		if (len == -1)
		{
			perror("writev");
			return EXIT_FAILURE;
		}

		start_timer(t);
		int h;
		for (h=0; h<10; h++)
		{
			//lsk = lseek(fildes,0,SEEK_SET);
			len = write(fildes, (void *) s, size);//on ecrit 10 fois un buffer de taille size
		}
		write_record(write_rec, size, stop_timer(t));
		if (lsk < 0)
		{
			perror("lseek");
			return EXIT_FAILURE;
		}
		if (len == -1)
		{
			perror("write");
			return EXIT_FAILURE;
		}

	}
	free (iovectors);
	int err = close(fildes);
	if(err == -1)
	{
		perror("close");
		exit(EXIT_FAILURE);
	}
	free(s);
	recorder_free(writev_rec);
	recorder_free(write_rec);
	timer_free(t);

	return EXIT_SUCCESS;
}
