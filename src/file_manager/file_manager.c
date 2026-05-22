#include "file_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LINE_LENGTH 1024

void free_array(AssistanceRequest **arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

void manage_error(AssistanceRequest **arr, int size, FILE *fp)
{
    free_array(arr, size);
    fclose(fp);
}

AssistanceRequest **read_from_memory(int *out_size)
{
    FILE *fp = fopen("requests.txt", "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Errore apertura file\n");
        return NULL;
    }

    int capacity = 10;
    int size = 0;

    AssistanceRequest **array = malloc(capacity * sizeof(AssistanceRequest *));

    char line[1024];

    while (fgets(line, sizeof(line), fp))
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
            return NULL;
        }
        int request_code = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            return NULL;
        }
        char customer_name[MAX_CUSTOMER_NAME];
        strncpy(customer_name, token, MAX_CUSTOMER_NAME - 1);
        customer_name[MAX_CUSTOMER_NAME - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            return NULL;
        }
        DeviceType device_type;

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
            return NULL;
        }
        char description[MAX_DESCRIPTION];
        strncpy(description, token, MAX_DESCRIPTION - 1);
        description[MAX_DESCRIPTION - 1] = '\0';

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
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
            return NULL;
        }
        float estimated_cost = atof(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            return NULL;
        }
        float final_cost = atof(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            return NULL;
        }
        int day = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
            return NULL;
        }
        int month = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL)
        {
            manage_error(array, size, fp);
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

    fclose(fp);

    *out_size = size;
    return array;
}