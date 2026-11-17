/* 1. Complete date
 * 2. Complete data deletion
 * 3. Complete directory creation
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <limits.h>


int main(int argc, char **argv) {
    if (argc > 3 || argc == 1) {
	printf("\n=-------------=\n=    ╱|、     =\n=   (˚ˎₒ7     =\n=   |、˜〵    =\n=   じしˍ,)ノ =\n=-------------=\n\n");
	printf("Error: Invalid argument\n");
	printf("Enter: [./app.exe --help]\n");
	exit(1);
    }
    if (strcmp(argv[1], "--help") == 0) {
	printf("-l List all files in directory.  [./app.exe -l <path>]\n");
	printf("-d Delete file.                  [./app.exe -d <path/filename>]\n");
	printf("-a Add file.                     [./app.exe -a <path/filename>]\n");
	printf("-r Open file.                    [./app.exe -r <path/filename>]\n");
	printf("-w Write file.                   [./app.exe -w <path/filename>]\n");
    } else if (strcmp(argv[1], "-l") == 0 && argc == 3) {
	DIR *dir = opendir(argv[2]);
	if (dir == NULL) {
	    perror("opendir");
	    exit(1);
	}
	char path[PATH_MAX];
	struct dirent *entry = NULL;
	FILE *file = NULL;
	size_t file_size;
	while ((entry = readdir(dir)) != NULL) {
	    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
	    snprintf(path, sizeof path, "%s/%s", argv[2], entry->d_name);
	    file = fopen(path, "rb"); 
	    if (file == NULL) {
		perror("fopen");
		continue;
	    }

	    if (fseek(file, 0, SEEK_END) != 0) {
		perror("fseek");
		fclose(file);
		continue;
	    }

	    if (ftell(file) == -1) {
		perror("ftell");
		fclose(file);
		continue;
	    }
	    file_size = ftell(file);
	    rewind(file);
	    fclose(file);
	    printf("%-30s\t[%zu].bytes\n", entry->d_name, file_size);
	}
	closedir(dir);
	exit(0);
	
    } else if (strcmp(argv[1], "-d") == 0 && argc == 3) {
	if (remove(argv[2]) != 0) {
	    perror("remove");
	    exit(1);
	}
	printf("File was deleted\n");
	exit(0);

    } else if (strcmp(argv[1], "-a") == 0 && argc == 3) {
	FILE *file = fopen(argv[2], "a+"); 
	if (file == NULL) {
	    perror("fopen");
	    exit(1);
	}
	printf("File was created\n");
	fclose(file);
	exit(0);
	
    } else if (strcmp(argv[1], "-r") == 0 && argc == 3) {
	FILE *file = fopen(argv[2], "r+"); 
	if (file == NULL) {
	    perror("fopen");
	    exit(1);
	}

	if (fseek(file, 0, SEEK_END) != 0) {
	    perror("fseek");
	    fclose(file);
	    exit(1);
	}

	if (ftell(file) == -1) {
	    perror("ftell");
	    fclose(file);
	    exit(1);
	}
	size_t file_size = ftell(file);
	printf("File size [%zu].bytes\n", file_size);
	char *buffer = (char *)calloc(file_size + 1, sizeof(char));
	rewind(file);

	if (file_size != fread(buffer, 1, file_size, file)) {
	    if (ferror(file)) {
		fclose(file);
		free(buffer);
		exit(1);
		}
	}
	buffer[file_size] = '\0';
	printf("File:\n");
	for (size_t i = 0; i < file_size; i++) {
	    printf("%c", buffer[i]);
	}

	printf("\n");
	fclose(file);
	free(buffer);
	exit(0);
    
    } else if (strcmp(argv[1], "-w") == 0 && argc == 3) {
	FILE *file = fopen(argv[2], "a+");
	if (file == NULL) {
	    perror("fopen");
	    exit(1);
	}

	printf("Enter data: \n");
	char ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {
	    if (!fwrite(&ch, 1, 1, file)) {
		if (ferror(file)) {
		    perror("fwrite");
		    fclose(file);
		    exit(1);
		}
	    }
	}

	fclose(file);
	exit(0);

    } else {
	printf("\n=-------------=\n=    ╱|、     =\n=   (˚ˎₒ7     =\n=   |、˜〵    =\n=   じしˍ,)ノ =\n=-------------=\n\n");
	printf("Error: Invalid argument\n");
	printf("Enter: [./app.exe --help]\n");
	exit(1);
    }

    return 0;
}
