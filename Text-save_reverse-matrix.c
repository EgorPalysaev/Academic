// #include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
// #include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WORDS 128
#define MAX_WORD_LENGTH 63

enum ERR { OK = 0, ERROR = 1 };

size_t my_strlen(const char *string){
    for (size_t i = 0; ; i++) if (string[i] == '\0') return i;
    return 0;
}

char *my_strchr(const char* string, char ch){
    for (size_t i = 0; i < my_strlen(string); i++){
	if (string[i] == ch) return (char *)&string[i];
    }
    return NULL;
}

void *my_memcpy(void *destination, const void *source, size_t length){
    unsigned char *dest = destination;
    const unsigned char *src = source;
    for(size_t i = 0; i < length; i++){
	dest[i] = src[i];
    }
    return destination;
}

int my_isspace(const char *pointer){
    if (*pointer == 32 || *pointer == '\n' || *pointer == '\t' || *pointer == '\f' || *pointer == '\v' || *pointer == '\r'
	|| *pointer == ',' || *pointer == '.' || *pointer == '!' || *pointer == '?') return *pointer;
    return 0;
}

static int count_digits(char *string, int length){
    int count = 0;
    for (int i = 0; i < length; i++){
	if ('0' <= string[i] && string[i] <= '9') count++;
    }
    return count;
}
static bool split_words(const char *input, char words[MAX_WORDS][MAX_WORD_LENGTH + 1], int *word_count, int digits_in_words[MAX_WORDS]) {
    const unsigned char *p = (const unsigned char *)input;
    int count = 0;

    while (*p != '\0') {
        while (*p != '\0' && my_isspace((const char *)p)) {
	    p++;
        }

        if (*p == '\0') {
            break;
        }

        const unsigned char *start = p;
        while (*p != '\0' && !my_isspace((const char *)p)) {
            p++;
        }

        size_t length = (size_t)(p - start) * sizeof (char);

        if (length > MAX_WORD_LENGTH) {
            fprintf(stderr, "Error: a word exceeds %d characters.\n",
                    MAX_WORD_LENGTH);
            return false;
        }

        if (count >= MAX_WORDS) {
            fprintf(stderr, "Error: more than %d words.\n", MAX_WORDS);
            return false;
        }

        my_memcpy(words[count], start, length);
	digits_in_words[count] = count_digits(words[count], length);
        words[count][length] = '\0';
        count++;
    }

    *word_count = count;
    return true;
}

int main(void) {
    START:
    printf("[A]rray, [M]atrix, [Q]uit\n");
    printf("> ");
    char input = getchar();
    while(getchar() != '\n')
	;
    switch (input){
	case 'A': case 'a':
	    char input[MAX_WORDS * (MAX_WORD_LENGTH+1)];
	    char words[MAX_WORDS][MAX_WORD_LENGTH + 1];
	    int digits_in_words[MAX_WORDS];
	    int word_count = 0;

	    printf("Enter text: ");

	    if (fgets(input, sizeof input, stdin) == NULL) {
		fprintf(stderr, "Error: could not read input.\n");
		return ERROR;
	    }

	    if (my_strchr(input, '\n') == NULL) {
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF);
		fprintf(stderr, "Error: input line is incorrect.\n");
		return ERROR;
	    }

	    if (!split_words(input, words, &word_count, digits_in_words)) {
		return ERROR;
	    }

	    double max = 0.0;
	    char *max_digit = NULL;
	    for (int i = 0; i < word_count; i++) {
		printf("%d.)Word: %s. Has %d digits and percentage of digits is %2.2f\n",(i+1), words[i], digits_in_words[i], (double)digits_in_words[i]/(double)my_strlen(words[i]) * 100);
		if (max < (double)digits_in_words[i]/(double)my_strlen(words[i])){
		    max = (double)digits_in_words[i]/(double)my_strlen(words[i]);
		    max_digit = words[i];
		}
	    }
	    if (max_digit != NULL) printf("Word that has maximum percentage of digits: %s\n", max_digit);
	    else printf("Nothing to analyze\n");
	    goto START;
	case 'M': case 'm':
	    srand((unsigned)time(NULL));
	    int n, sum = 0;
	    printf("Enter a period: ");
	    scanf("%d", &n);
	    int **period = (int**)malloc(n * sizeof(int *));
	    for (int i = 0; i < n; i++){
		printf("\n");
		*(period+i) = (int *)malloc(n * sizeof(int));
		for (int j = 0; j < n; j++){
		    *(*(period + i) + j) = rand() % 100;
		    printf("%-3d ", *(*(period + i) + j));
		}
	    }
	    printf("\n>----------------------------------------------<");
	    for (int i = 0; i < n; i++){
		if (n % 2 == 1){
		    if (i == n/2){
			sum += *(*(period + i) + i); 
			continue;
		    }
		}
		int temp = *(*(period + i) + i);
		*(*(period + i) + i) = *(*(period + i) + (n-1-i)); 
		*(*(period + i) + (n-1-i)) = temp;
		sum += *(*(period + i) + i) + *(*(period + i) + (n-1-i));
	    }
	    for (int i = 0; i < n; i++){
		printf("\n");
		for (int j = 0; j < n; j++){
		    printf("%-3d ", *(*(period + i) + j));
		}
	    }
	    printf("\nSum is %d\n", sum);
	    goto START;
	case 'Q': case 'q': default: printf("Quit\n");
    }
    return OK;
}
    
