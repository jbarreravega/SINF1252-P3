/**
	\file writev.c
	\brief Ce programme compare l'utilisation de writev ou de write + lsekk pour ecrire dans un fichier

	Ce programme va donc écrire dans un fichier avec writev d'une part et write + lseek d'autre part, en comparant les temps nécessaires à chacun.	

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
# include <sys/uio.h>
#include <string.h>

#include "benchmark.h"

#define N 100


/*
 * 
 * timer *t = chrono 
 * recorder *write_rec =
 * char *s = chaine de caractères à écrire
 * int write_o = addresse du fichier
 * 
 */

void benchmark_write (timer *t, recorder *write_rec, char *s, int write_o) 
{
	start_timer(t);
	int size_s = strlen(s);
	int write_e = write(write_o,s,size_s);
	if (write_e == -1)
	{
		perror("ecriture");
		exit(EXIT_FAILURE);
	}
		
	//ecriture des résultats dans write.csv 
	write_record_n(write_rec, size_s, stop_timer(t), 1);

}

int main (int argc, char *argv[])  {
	
	int iovcnt;
	char* S_init = "test"; //String d'écriture initiale.
	int size_s = strlen(S_init);
	int size = size_s*N; 
	
	char buffer[size+1];
	buffer[0] = '\0';
	
	struct iovec iov[1];
	char *buf0;
	char *s;
	
	char *buf[N];
	struct iovec iov1[3];
	
	// Déclare un timer, ainsi que 4 recorder qui vont contenir les résultats de l'exécution du programme
	timer *t = timer_alloc();
	recorder *writev_rec = recorder_alloc("writev.csv");
	recorder *writev_rec2 = recorder_alloc("writev2.csv");
 	recorder *write_rec = recorder_alloc("write.csv");
 	recorder *write_rec2 = recorder_alloc("write2.csv");
 	ssize_t bytes_written;

	
	int write_o = open("write.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
	if (write_o == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	int fd = open("writev.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if (fd == -1)
    {
		perror("open");
		exit(EXIT_FAILURE);
	}
	
	// Comparaison du temps pris par write et writev, nous n'utilisons qu'un seul buffer pour les 2 fonctions.
	int i;
	for(i = 1; i <= N; ++i)
	{

		s = strcat(buffer,S_init);
		
		//////////////////////////////// fichier write : une écriture unique de i mots//////////////////////////////////////////////////////
		
		benchmark_write (t,write_rec,s,write_o);
		
       //////////////////////////////////////// fichier writev : utilisation d'un seul buffer pour une écriture de i mots//////////////////
		
		
		buf0 = (char *)realloc(buf0, sizeof(char)*i*size_s);
		int k1 = 0;
		int k;
		for(k=(size_s+1)*(i-1); k < size_s*i; ++k)
		{
			buf0[k] = S_init[k1];
			k1++;
		}
		
		buf0 = (char *)realloc(buf0, (sizeof(char)*(i)*size_s)+sizeof(char));
		buf0[k+1] = '\0';
		
		iov[0].iov_base = buf0;
		iov[0].iov_len = strlen(buf0);
		
		
		iovcnt = sizeof(iov) / sizeof(struct iovec);
		start_timer(t);
		
		bytes_written = writev(fd, iov, iovcnt);
		write_record_n(writev_rec, i*size_s, stop_timer(t), 1);
		if(bytes_written ==-1)
		{
			perror("ecriture");
			exit(EXIT_FAILURE);
		}
	}
	
	int write_f = close(write_o);
	int writev_f = close(fd);
	if (write_f == -1|| fd ==-1)
	{
		perror("fermeture");
		exit(EXIT_FAILURE);
	}
	
	//free(s);
	free(buf0);
	recorder_free(writev_rec);
	recorder_free(write_rec);
		
	// Comparaison du temps pris par write et writev, nous n'utilisons qu'un seul buffer pour write et plusieurs buffer pour writev.
	for(i = 1; i <= N; ++i)
	{

		////////////////////////////////// fichier write : i écritures de un mot////////////////////////////////////////////
	
				
		// ouverture fichier
		int write_o2 = open("write2.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
		if (write_o2 == -1)
		{
			perror("open");
			exit(EXIT_FAILURE);
		}
		
		start_timer(t);
		
		int j;
		for (j=0;j<=i;++j)
		{
			int write_e = write(write_o2,S_init,size_s);
			if (write_e == -1)
			{
				perror("ecriture");
				exit(EXIT_FAILURE);
			}
			write_e = write(write_o2,S_init,size_s);
			if (write_e == -1)
			{
				perror("ecriture");
				exit(EXIT_FAILURE);
			}
			write_e = write(write_o2,S_init,size_s);
			if (write_e == -1)
			{
				perror("ecriture");
				exit(EXIT_FAILURE);
			}
		}
		
		// écriture du résultat dans write2.csv
		write_record_n(write_rec2, 3*i*(size_s), stop_timer(t), 1);
		
		// fermeture du fichier write2.txt
		int write_f1 = close(write_o2);
		if (write_f1 == -1)
		{
		perror("fermeture");
		exit(EXIT_FAILURE);
		}
		
		
		//////////////////////////////////////// fichier writev : utilisation multi buffer pour l'écriture de i mots////////////////////////////////////////////

		int k;
	    for(k=0; k <= 3; ++k)
	    {
			buf[k] = (char *)malloc(sizeof(char)*(size_s+1));
			for(j=0; j <= size_s+1; ++j)
		    {
			   buf[k][j] = S_init[j];
		    }
		    iov1[k].iov_base = buf[k];
		    iov1[k].iov_len = strlen(buf[k]);
		}
		
		iovcnt = sizeof(iov1) / sizeof(struct iovec);
		
		int fd2 = open("writev2.txt", O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
        if (fd2 == -1)
        {
		    perror("open");
		   exit(EXIT_FAILURE);
	     }
		
		start_timer(t);
		for (j=0;j<=i;++j)
		{
		    bytes_written = writev(fd2, iov1, iovcnt);
		    if(bytes_written ==-1)
		    {
			    perror("ecriture");
			    exit(EXIT_FAILURE);
		    }
		}
		
		write_record_n(writev_rec2, 3*i*size_s, stop_timer(t), 1);
		
		if(close(fd2) == -1)
		{
			perror("fermeture");
			exit(EXIT_FAILURE);
		}
	}
	
	remove("write.txt");
	remove("write2.txt");
	remove("writev.txt");
	remove("writev2.txt");
	for(i=0; i <= 3; ++i)
	{
		free(buf[i]);
	}
	
	// liberation de la mémoire
	
  	recorder_free(writev_rec2);
  	recorder_free(write_rec2);
	timer_free(t);

  return EXIT_SUCCESS;
}

