#include <stdio.h>
#include <unistd.h>

int main() { 
    // O print formata o número, já o write passa os bytes do número que dps não consegue ser lido.
    //Usar pipe de hexdump para ver os binary hidden.
    int i = 1234567;
    write(STDOUT_FILENO, &i, sizeof(i));
    printf("\n%d", i);
    return 0;
}