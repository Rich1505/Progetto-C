#include "file_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LINE_LENGTH 1024

/**
 * @brief Dealloca l'array di puntatori locale in caso di errore durante il parsing.
 * @param arr Array di puntatori a richieste parzialmente allocato.
 * @param size Numero di elementi attualmente allocati nell'array.
 */
void free_array(AssistanceRequest **arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

/**
 * @brief Gestisce la chiusura del file e la pulizia della memoria a seguito di un errore di lettura.
 * @param arr Array di puntatori da liberare.
 * @param size Elementi inseriti fino al momento dell'errore.
 * @param fp Puntatore al file di testo aperto.
 */
void manage_error(AssistanceRequest **arr, int size, FILE *fp)
{
    free_array(arr, size);
    fclose(fp);
}

AssistanceRequestArray *read_from_memory()
{
    FILE *fp = fopen("requests.txt", "r");

    if (fp == NULL)
    {
        printf("File 'requests.txt' non trovato. Creazione in corso...\n");
        
        fp = fopen("requests.txt", "w");
        if (fp == NULL)
        {
            fprintf(stderr, "Errore fatale: impossibile creare il file di memoria.\n");
            return NULL;
        }
        
        fclose(fp);
        
        fp = fopen("requests.txt", "r");
        if (fp == NULL)
        {
            return NULL; 
        }
    }

    int capacity = 10;
    int size = 0;

    AssistanceRequest **array = malloc(capacity * sizeof(AssistanceRequest *));

    char line[LINE_LENGTH];

    /* Lettura riga per riga del file di testo fino alla fine del documento */
    while (fgets(line, LINE_LENGTH, fp))
    {
        if (size == capacity)
        {
            capacity *= 2;

            AssistanceRequest **tmp =
                realloc(array, capacity * sizeof(AssistanceRequest *));

            if (tmp == NULL)
            {
                fprintf(stderr, "Errore allocazione memoria\n");
                manage_error(array, size, fp);
                return NULL;
            }

            array = tmp;
        }

        line[strcspn(line, "\n")] = '\0';

        char *token;

        token = strtok(line, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        int request_code = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        char customer_name[MAX_CUSTOMER_NAME];
        strncpy(customer_name, token, MAX_CUSTOMER_NAME - 1);
        customer_name[MAX_CUSTOMER_NAME - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        DeviceType device_type;

        /* Conversione della stringa testuale nell'enumerato DeviceType corrispondente */
        if (strcmp(token, "SMARTPHONE") == 0)
            device_type = DEVICE_SMARTPHONE;
        else if (strcmp(token, "TABLET") == 0)
            device_type = DEVICE_TABLET;
        else if (strcmp(token, "LAPTOP") == 0)
            device_type = DEVICE_LAPTOP;
        else if (strcmp(token, "DESKTOP") == 0)
            device_type = DEVICE_DESKTOP;
        else if (strcmp(token, "PRINTER") == 0)
            device_type = DEVICE_PRINTER;
        else
        {
            fprintf(stderr, "Inserire un tipo di dispostivo valido\n");
            manage_error(array, size, fp);
            return NULL;
        }

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        char description[MAX_DESCRIPTION];
        strncpy(description, token, MAX_DESCRIPTION - 1);
        description[MAX_DESCRIPTION - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        PriorityLevel priority_level;

        int pr = atoi(token);

        if (pr == 0)
            priority_level = PRIORITY_LOW;
        else if (pr == 1)
            priority_level = PRIORITY_MEDIUM;
        else if (pr == 2)
            priority_level = PRIORITY_HIGH;
        else
        {
            fprintf(stderr, "Inserire una priorità valida\n");
            manage_error(array, size, fp);
            return NULL;
        }

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        RequestStatus request_status;

        int st = atoi(token);

        if (st == 0)
            request_status = STATUS_OPEN;
        else if (st == 1)
            request_status = STATUS_IN_PROGRESS;
        else if (st == 2)
            request_status = STATUS_CLOSED;
        else
        {
            fprintf(stderr, "Inserire uno stato valido\n");
            manage_error(array, size, fp);
            return NULL;
        }

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        float estimated_cost = atof(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        float final_cost = atof(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        int day = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        int month = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            fprintf(stderr, "Errore durante la tokenizzazione del file\n");
            return NULL;
        }
        int year = atoi(token);

        Date opening_date = create_date(day, month, year);
        if (opening_date.day == -1)
        {
            manage_error(array, size, fp);
            return NULL;
        }

        AssistanceRequest *assistance_request = create_assistance_request(
            request_code,
            customer_name,
            device_type,
            description,
            priority_level,
            request_status,
            estimated_cost,
            final_cost,
            opening_date);

        if (assistance_request == NULL)
        {
            fprintf(stderr, "Errore allocazione memoria\n");
            manage_error(array, size, fp);
            return NULL;
        }

        array[size++] = assistance_request;
    }

    AssistanceRequestArray *assistance_request_array = create_assistance_request_array(array, size, capacity);
    if(assistance_request_array == NULL)
    {
        manage_error(array, size, fp);
        return NULL;
    }

    fclose(fp);

    return assistance_request_array;
}

int write_in_memory(AssistanceRequestArray *assistance_request_array)
{
    if (assistance_request_array == NULL)
    {
        fprintf(stderr, "Errore write_in_memory: puntatore NULL\n");
        return -1;
    }

    FILE *fp = fopen("requests.txt", "w");

    if (fp == NULL)
    {
        fprintf(stderr, "Errore apertura file in scrittura\n");
        return -1;
    }

    int size = get_assistance_request_array_size(assistance_request_array);
    AssistanceRequest **array = get_assistance_request_array_ptr(assistance_request_array);

    for (int i = 0; i < size; i++)
    {
        if (array[i] == NULL)
        {
            fprintf(stderr, "Errore: rilevato puntatore NULL all'indice %d dell'array\n", i);
            fclose(fp);
            return -1;
        }

        DeviceType device_type = get_device_type(array[i]);
        const char *device_str;

        /* Mappatura dell'enumerato DeviceType nella corrispondente stringa per il file di testo */
        switch (device_type)
        {
        case DEVICE_SMARTPHONE:
            device_str = "SMARTPHONE";
            break;
        case DEVICE_TABLET:
            device_str = "TABLET";
            break;
        case DEVICE_LAPTOP:
            device_str = "LAPTOP";
            break;
        case DEVICE_DESKTOP:
            device_str = "DESKTOP";
            break;
        case DEVICE_PRINTER:
            device_str = "PRINTER";
            break;
        default:
            fprintf(stderr, "Errore: Tipo dispositivo non valido all'indice %d\n", i);
            fclose(fp);
            return -1;
        }

        Date opening_date = get_opening_date(array[i]);

        int written = fprintf(fp, "%d;%s;%s;%s;%d;%d;%.2f;%.2f;%d;%d;%d\n",
                              get_request_code(array[i]),
                              get_customer_name(array[i]),
                              device_str,
                              get_description(array[i]),
                              (int)get_priority_level(array[i]),
                              (int)get_request_status(array[i]),
                              get_estimated_cost(array[i]),
                              get_final_cost(array[i]),
                              opening_date.day,
                              opening_date.month,
                              opening_date.year);
        
        //negativo in caso di errore nella scrittura
        if (written < 0)
        {
            fprintf(stderr, "Errore durante la scrittura su file all'indice %d\n", i);
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);

    return 0;
}