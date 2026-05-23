#include"file_manager.h"
#include"ware.h"
#include<stdlib.h>
#include<stdio.h>

int main(void)
{
    FILE *fp = fopen("file.txt", "w");

    if (fp == NULL)
    {
        printf("Errore apertura de manuel file\n");
        return 1;
    }

    fprintf(fp, "Ciao mondo\n");
    fclose(fp);

    int size = 0;
    AssistanceRequest **assistance_request = read_from_memory(&size);

    write_in_memory(assistance_request, size);

    return 0;
}