
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>

#include "benchmark.h"

#define N 1000
/**
 * \brief Écris le temps `time` en correspondance avec `Nfiles`
 *
 * L'`overhead` est d'abord retiré de `time`
 * En cas d'erreur, il `exit` avec `EXIT_FAILURE`
 *
 * \param rec le `recorder` dans lequel écrire, il est supposé non-`NULL`
 * \param repertoire le nom du repertoire parcouru. 
 * \param Nfiles l'abscisse
 * \param t le temps à écrire en ordonnée
 */
void benchmark_readdir(timer *t, char *repertoire, recorder *rec, int Nfiles)
{
	DIR *rep=NULL;
	rep=opendir(repertoire);

	if(rep==NULL)
	{
		fprintf(stderr, "Erreur ouverture dossier source");
		exit(1);
	}

	struct dirent* fichierLu=NULL;  
    start_timer(t);
	while((fichierLu = readdir(rep)) != NULL)
	{
		/*if (strcmp(fichierLu->d_name, ".")!=0 && strcmp(fichierLu->d_name, "..")!=0)
		{
			
		}*/
	}			

    write_record_n(rec, Nfiles, stop_timer(t), /*Nfiles+2*/ 1);

	if(closedir(rep) == -1)
	{
		fprintf(stderr, "Erreur fermeture dosssier source");
		exit(-1);
	}

}

char* stringConcat(char* s1, char* s2)
 { 
	 int size = strlen(s1)+strlen(s2);
	 char* s = (char*)malloc(sizeof(char)*(size+1));
	 
	 int j,k, l =0;
	 for(j=0; j<strlen(s1); ++j)
	 {
		 s[j] = s1[j];
	 }
	 for(k=j; k<size+1; ++k)
	 {
		 s[k] = s2[l];
		 ++l;
	 }
	 
	 return s;
 }

void rm(char* repertoire)
{
	DIR *rep=NULL;
	rep=opendir(repertoire);
	if(rep==NULL)
	{
		fprintf(stderr, "Erreur ouverture dossier source");
		exit(1);
	}

	struct dirent* fichierLu=NULL;  
	while((fichierLu = readdir(rep)) != NULL)
	{
		if (strcmp(fichierLu->d_name, ".")!=0 && strcmp(fichierLu->d_name, "..")!=0)
		{
			char *s;
			s = stringConcat("dir/",fichierLu->d_name);
			remove(s);
			free(s);
		}
	}			

	if(closedir(rep) == -1)
	{
		fprintf(stderr, "Erreur fermeture dosssier source");
		exit(-1);
	}
	
	rmdir(repertoire);

}

int main (int argc, char *argv[]) {
  	  
  timer *t = timer_alloc();

  recorder *readdir_rec = recorder_alloc("readdir.csv");
  
  mkdir("dir", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  char str[10];
  char* s;
  char* s1;
  int filename;
  for(filename = 1; filename <= N; ++filename)
  { 

	  sprintf(str,"%d",filename);
	  s = stringConcat("dir/test",str);
	  s1 = stringConcat(s,".txt");
	  //printf("%s\n",s1);
	  FILE *file = fopen(s1, "w");
	  if (file == NULL) 
	  {
		  perror("fopen");
		  exit(EXIT_FAILURE);
	  }
	  
	  //fprintf(file, "testtesttesttesttestBENCHMARKREADDDIRMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM");
      fclose(file);
	  free(s);
      free(s1);
      
      update_overhead();   
      benchmark_readdir(t, "dir", readdir_rec, filename);
	}
  rm("dir");
  recorder_free(readdir_rec);

  timer_free(t);
  return EXIT_SUCCESS;
}
