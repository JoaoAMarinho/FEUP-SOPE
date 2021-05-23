#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_ITER 100

int static inline get_random_num(int min, int max) {
    return rand() % max + min;
}

void read_sequential(FILE* file) {
    char str[NUM_ITER];

    for (int i = 0; i < NUM_ITER; ++i) {
        fread(str + i, sizeof(char), 1, file);
        fprintf(stdout, "File Pointer Position: %ld bytes.\n", ftell(file));
    }
    printf("%s", str);
}

void read_random(FILE* file) {
    char str[NUM_ITER];

    for (int i = 0; i < NUM_ITER; ++i) {
        fseek(file, get_random_num(1, 28), SEEK_SET);
        fread(str + i, sizeof(char), 1, file);
        
        fprintf(stdout, "File Pointer Position: %ld bytes.\n", ftell(file));
    }
    printf("%s", str);
}

#define NUM_ARGS 2

int main(int argc, char * argv[]) {
    if (argc != NUM_ARGS) {
        fprintf(stderr, "Usage: ex5 <file>\n");
        return 1;
    }

    srand(time(NULL));

    FILE* file;
    if ((file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Impossible to open the file - %s", argv[1]);
        return 1;
    }

    fprintf(stdout, "===== SEQUENTIAL =====\n\n");
    read_sequential(file);


    fprintf(stdout, "\n\n===== RANDOM =====\n\n");
    read_random(file);

    fclose(file);
    return 0;
}