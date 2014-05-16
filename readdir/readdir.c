//author :  Juan Sebastian Barrera Vega (94471000)
//          Olivier Boegaerts (44481100)

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "benchmark.h"

#define MAX_FILES 2048
#define MAX_SIZE 1024// 1KB

int deleteTMP(const char *path, const char *dirName)
{
	errno = 0;
	DIR *dirErase = opendir(dirName);
	if (errno =! ENOENT) return EXIT_FAILURE;
	struct dirent *files;
	while((files=readdir(dirErase)) != NULL)
	{
		if (strcmp(files->d_name, ".") != 0  && strcmp(files->d_name, "..") != 0)
		{
			char filename[14];
			strcpy(filename,path);
			strcat(filename,files->d_name);
			remove(filename);
		}
	}
	if(errno == 0){
			closedir(dirErase);
			rmdir(dirName);
			if (errno !=0){
				perror("unlink");
				exit(EXIT_FAILURE);
			}
			else return EXIT_SUCCESS;
	}
	else{
		perror("readdir");
		exit(EXIT_FAILURE);
	}
}

int createFile(const int fileName, const char *directory, const int nBytes)
{
	int i;
	ssize_t len;
	char filename[14];
	FILE *fd;
	strcpy(filename,directory); // filename = "directory"
	strcat(filename,"/"); //filename = "directory/"
	sprintf(filename+5,"%d",fileName); //filename = "directory/fileName
	fd = fopen(strcat(filename,".dat"),"w+");//open filename = "directory/i.dat"
	fclose(fd);
	int file = open (filename, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
  char *s = (char *) malloc((nBytes+1) * sizeof(char));
  memset(s, 0, nBytes+1);
  len = write(file, (void *) s, nBytes);
  free(s);
  if (len == -1) {
  	perror("write");
  	return EXIT_FAILURE;
  }
  close(file);
	return EXIT_SUCCESS;
}

int main (int argc, char *argv[])  {
	timer *t = timer_alloc();
	recorder *readdirNoSize_rec = recorder_alloc("readdir1.csv");
	recorder *readdirConstSize_rec = recorder_alloc("readdir2.csv");
	recorder *readdirVarSize_rec = recorder_alloc("readdir3.csv");

	struct dirent *file;
	struct dirent *file2;
	struct dirent *file3;
	const char *pathdir1 = "tmp1";
	const char *pathdir2 = "tmp2";
	const char *pathdir3 = "tmp3";
	errno = 0;

	mkdir(pathdir1,S_IRWXU);
	mkdir(pathdir2,S_IRWXU);
	mkdir(pathdir3,S_IRWXU);

	int j;
	int numberFiles = 0;
	for (j = 1 ; j <= MAX_FILES; j *= 2) {

		numberFiles++;
		//Création d'un fichier
		createFile(j, pathdir1, 0);
		createFile(j, pathdir2, MAX_SIZE);
		createFile(j, pathdir3, j);

		//j fichiers de taille 0
		DIR *dirp = opendir(pathdir1);
		if (dirp == NULL) { //Erreur à l'ouverture du dossier
			perror("opendir pathdir1");
			exit(EXIT_FAILURE);
		}
		start_timer(t);
		do { file = readdir(dirp);
		} while (file != NULL);
		//write_record(readdirNoSize_rec, numberFiles, stop_timer(t));
		write_record_n(readdirNoSize_rec, numberFiles, stop_timer(t), numberFiles); //moyenne
		closedir(dirp);

		//j fichiers de même taille (différente de 0)
		dirp = opendir(pathdir2);
		if (dirp == NULL) { //Erreur à l'ouverture du dossier
			perror("opendir pathdir2");
			exit(EXIT_FAILURE);
		}
		start_timer(t);
		do {file = readdir(dirp);
		} while (file != NULL);
		//write_record(readdirConstSize_rec, numberFiles, stop_timer(t));
		write_record_n(readdirConstSize_rec, numberFiles, stop_timer(t), numberFiles);
		closedir(dirp);


		//j fichiers de taille différente
		dirp = opendir(pathdir3);
		if (dirp == NULL) { //Erreur à l'ouverture du dossier
			perror("opendir3 pathdir3");
			exit(EXIT_FAILURE);
		}
		do {file = readdir(dirp);
		} while (file != NULL);
		//write_record(readdirVarSize_rec, numberFiles, stop_timer(t));
		write_record_n(readdirVarSize_rec, numberFiles, stop_timer(t), numberFiles);
		closedir(dirp);

		if (errno !=0) {
			perror("readdir");
			exit(EXIT_FAILURE);
		}
	}

	recorder_free(readdirNoSize_rec);
	recorder_free(readdirConstSize_rec);
	recorder_free(readdirVarSize_rec);
	timer_free(t);

	//Efface les dossiers temporaires
	deleteTMP("tmp1/",pathdir1);
	deleteTMP("tmp2/",pathdir2);
	deleteTMP("tmp3/",pathdir3);

	return EXIT_SUCCESS;
}
