#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

void help(void);
int search(char * dir, char * pattern);
int is_directory(char* path);

int main(int argc, char * argv[]) {
	char* dir;
	char* pattern;

	if (argc < 2 || argc > 3) {
		help();
		exit(EXIT_FAILURE);
	}

	if (argc == 2) {
		dir = ".";
		pattern = argv[1];
	} else {
		dir = argv[1];
		pattern = argv[2];
	}
	//printf("TODO buscar %s en %s\n", pattern, dir);

	int total = search(dir, pattern);
	if (total > 0) {
		printf("Total: %d\n", total);
	}

	exit(EXIT_SUCCESS);
}

void help(void) {
	fprintf(stdout, "Uso: ./main ARCHIV         Busca un archivo en el directorio actual y subdirectorios.\n");
	fprintf(stdout, "     ./main DIR ARCHIVO    Busca un archivo en el directorio DIR y subdirectorios.\n");
}

int search(char * dir, char * pattern) {
	DIR* d;
	struct dirent *ent;
	char* str = malloc((strlen(dir) + 2) * sizeof(char));
	char* name;// = malloc(50);
	char* path = malloc(0);
	int cantidad = 0;
	
	memset(str, 0, (strlen(dir) + 2));

	d = opendir(dir);

	if(d == NULL) {
		return 0;
	}

	//ent = readdir(d);

	str = strcat(str, dir);
	str = strcat(str, "/");
	//printf("str: %s\n", str);

	while((ent = readdir(d)) != NULL) {
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
			continue;
		}
		
		name = malloc(sizeof(ent->d_name));
		name = ent->d_name;
		//printf("-> %s\n", name);
		
		path = malloc(strlen(str)*sizeof(char*) + strlen(name)*sizeof(char*) + 2*sizeof(char));
		strcpy(path, str);
		strcat(path, name);

		//printf("%s\n", path);
		if (strstr(name, pattern)) {
			printf("%s\n", path);
			cantidad += 1;
		}

		if (is_directory(path)) {
			cantidad += search(path, pattern);
		}
		//free(path);
	}

	closedir(d);

	return cantidad;
}

int is_directory(char* path) {
	struct stat stats;
	if (stat(path, &stats) == 0) {
		if (stats.st_mode & S_IFDIR) {
			return 1;	
		} else if (stats.st_mode & S_IFREG) {
			return 0;
		}
	}
	return 1;
}
