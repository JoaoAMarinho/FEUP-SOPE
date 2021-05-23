#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void memory_test(){
    char * text="2";
    //while (1)
    //{    
        text = malloc(50*sizeof(char));
        text = memset(text,'A',50*sizeof(char));
    //}
    
}


int ex_8(int argc, char**argv){
    int i=0;

    if (argc < 2) {
        fprintf (stderr, "\nUsage: %s <string>\n", argv[0]);
        exit (1);
    }
    printf("Input string: %s\n", argv[1]);
    
    char *buf = NULL;
    buf = (char*)malloc(strlen(argv[1])+1); 
    
    for (; i < strlen(argv[1])+1; i++) {
        buf[i] = toupper(argv[1][i]);
    }
    
    printf("Output string: %s\n", buf);
    free(buf);
    return 0;
}

int ex_9() {
    char buff[15];
    int pass = 0;

    printf("\n Enter the password : \n");
    fgets(buff, 13, stdin);
    printf("%s\n", buff);
    if(strcmp(buff, "thegeekstuff")) {
        printf ("\n Wrong Password. \n");
    }
    else {
        printf ("\n Correct Password.\n");
        pass = 1;
    }

    if(pass) {  // do somelhing priviledged stuff
        printf ("\n Root privileges given to the user.\n");
    }
    return 0;
}

int main(int argc, char *argv[]) {
	
    //4
    //memory_test();

    //7
    //Create a global var, static var unitilaized, memory goes to bss.
    //Initialized goes to data.
    //char * text;
    //text = malloc(50*sizeof(char));
    //printf("%p\n", &globalvar);
    //printf("%p\n", &text);
    //printf("%p\n", &memory_test);
    //free(text);

    //8
    //ex_8(argc, argv);

    //9
    ex_9();
    //a) Se um user colocar uma pass maior do que o buff consegue, supostamente o resto é colocado dentro da pass e obtemos acesso.
    //b) Colocar fgets em vez de gets.

    return 0;
}