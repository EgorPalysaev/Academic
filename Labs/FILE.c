#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

int main(int argc, char **argv) {
    if (argc > 3) {
	printf("Error: Enter ./app.exe --help\n");
	fflush(stderr);
	exit(1);
    }
    if (strcmp(argv[1], "--help") == 0) {
	printf("-l List all files in directory. [./app.exe -l]\n");
	fflush(stdout);
	printf("-d Delete file.                 [./app.exe -d <path/filename>]\n");
	fflush(stdout);
	printf("-a Add file.                    [./app.exe -a <path/filename>]\n");
	fflush(stdout);
	printf("-r Open file.                   [./app.exe -r <path/filename>]\n");
	fflush(stdout);
    } else if (strcmp(argv[1], "-l") == 0 && argc == 2) {
	DIR *dir = opendir(".");
	if (dir == NULL) {
	    perror("Directory");
	    fflush(stderr);
	    exit(1);
	}
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
	    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
	    printf("%s\n", entry->d_name);
	}
	closedir(dir);
	exit(0);
	
    } else if (strcmp(argv[1], "-d") == 0 && argc == 3) {
	if (remove(argv[2]) != 0) {
	    perror("Delete file");
	    fflush(stderr);
	    exit(1);
	}
	printf("File was deleted\n");
	fflush(stdout);
	exit(0);

    } else if (strcmp(argv[1], "-a") == 0 && argc == 3) {
	FILE *file = fopen(argv[2], "a+"); 
	if (file == NULL) {
	    printf("Error while creating\n");
	    fflush(stderr);
	    fclose(file);
	    exit(1);
	}
	printf("File was created\n");
	fflush(stdout);
	fclose(file);
	exit(0);
	
    } else if (strcmp(argv[1], "-r") == 0 && argc == 3) {
	FILE *file = fopen(argv[2], "r+"); 
	if (file == NULL) {
	    printf("Error while opening file\n");
	    fflush(stderr);
	    fclose(file);
	    exit(1);
	}
	fwrite("abcd\naa", 7, 1, file);
	printf("File was opened\n");
	fflush(stdout);
	fclose(file);
	exit(0);
    
    } else {
	printf("Error: Invalid argument\n");
	fflush(stderr);
	printf("Enter ./app.exe --help\n");
	fflush(stderr);
	exit(1);
    }

    return 0;
}
