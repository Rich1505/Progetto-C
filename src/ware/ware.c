#include "ware.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int max_days_per_month[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct assistance_request
{
    int request_code;
    char customer_name[MAX_CUSTOMER_NAME];
    DeviceType device_type;
    char description[MAX_DESCRIPTION];
    PriorityLevel priority_level;
    RequestStatus request_status;
    float estimated_cost;
    float final_cost;
    Date opening_date;
};

int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

Date create_date(int day, int month, int year)
{
    if (year < 1900)
    {
        fprintf(stderr, "Inserire un anno valido\n");
        return (Date){-1, -1, -1};
    }

    if (month < 1 || month > 12)
    {
        fprintf(stderr, "Inserire un mese valido\n");
        return (Date){-1, -1, -1};
    }

    if (day > max_days_per_month[month - 1] || day < 1)
    {
        fprintf(stderr, "Inserire un giorno valido\n");
        return (Date){-1, -1, -1};
    }

    if (month == 2)
    {
        if (!is_leap_year(year) && day == 29)
        {
            fprintf(stderr, "Inserire un giorno valido\n");
            return (Date){-1, -1, -1};
        }
    }

    return (Date){day, month, year};
}

AssistanceRequest *create_assistance_request(int request_code, char *customer_name, DeviceType device_type, char *description,
                                             PriorityLevel priority_level, RequestStatus request_status, float estimated_cost, float final_cost, Date opening_date)
{
    if (request_code < 0)
    {
        fprintf(stderr, "Inserire un codice valido\n");
        return NULL;
    }

    if (strlen(customer_name) >= MAX_CUSTOMER_NAME)
    {
        fprintf(stderr, "Il nome inserito per il cliente è troppo lungo, verrà tagliato\n");
    }

    if (strlen(description) >= MAX_DESCRIPTION)
    {
        fprintf(stderr, "La descrizione inserita per la richiesta è troppo lunga, verrà tagliata\n");
    }

    if (estimated_cost < 0.0f)
    {
        fprintf(stderr, "Inserire un costo stimato valido\n");
        return NULL;
    }

    if (final_cost < 0.0f)
    {
        fprintf(stderr, "Inserire un costo finale valido\n");
        return NULL;
    }

    AssistanceRequest *assistance_request = malloc(sizeof(AssistanceRequest));
    if (assistance_request == NULL)
    {
        fprintf(stderr, "Errore allocazione memoria\n");
        return NULL;
    }

    assistance_request->request_code = request_code;
    strncpy(assistance_request->customer_name, customer_name, MAX_CUSTOMER_NAME - 1);
    assistance_request->customer_name[MAX_CUSTOMER_NAME - 1] = '\0';

    assistance_request->device_type = device_type;

    strncpy(assistance_request->description, description, MAX_DESCRIPTION - 1);
    assistance_request->description[MAX_DESCRIPTION - 1] = '\0';

    assistance_request->priority_level = priority_level;
    assistance_request->request_status = request_status;
    assistance_request->estimated_cost = estimated_cost;
    assistance_request->final_cost = final_cost;
    assistance_request->opening_date = opening_date;

    return assistance_request;
}

int get_request_code(AssistanceRequest *request)
{
    return request->request_code;
}

char *get_customer_name(AssistanceRequest *request)
{
    return request->customer_name;
}

DeviceType get_device_type(AssistanceRequest *request)
{
    return request->device_type;
}

char *get_description(AssistanceRequest *request)
{
    return request->description;
}

PriorityLevel get_priority_level(AssistanceRequest *request)
{
    return request->priority_level;
}

RequestStatus get_request_status(AssistanceRequest *request)
{
    return request->request_status;
}

float get_estimated_cost(AssistanceRequest *request)
{
    return request->estimated_cost;
}

float get_final_cost(AssistanceRequest *request)
{
    return request->final_cost;
}

Date get_opening_date(AssistanceRequest *request)
{
    return request->opening_date;
}

int set_request_code(AssistanceRequest *request, int request_code)
{
    if (request_code < 0)
    {
        fprintf(stderr, "Inserire un codice valido\n");
        return -1;
    }

    request->request_code = request_code;
    return 0;
}

int set_customer_name(AssistanceRequest *request, char *customer_name)
{
    if (strlen(customer_name) >= MAX_CUSTOMER_NAME)
    {
        fprintf(stderr, "Nome troppo lungo, verrà tagliato\n");
    }

    strncpy(request->customer_name, customer_name, MAX_CUSTOMER_NAME - 1);
    request->customer_name[MAX_CUSTOMER_NAME - 1] = '\0';

    return 0;
}

int set_device_type(AssistanceRequest *request, DeviceType device_type)
{
    request->device_type = device_type;
    return 0;
}

int set_description(AssistanceRequest *request, char *description)
{
    if (strlen(description) >= MAX_DESCRIPTION)
    {
        fprintf(stderr, "Descrizione troppo lunga, verrà tagliata\n");
    }

    strncpy(request->description, description, MAX_DESCRIPTION - 1);
    request->description[MAX_DESCRIPTION - 1] = '\0';

    return 0;
}

int set_priority_level(AssistanceRequest *request, PriorityLevel priority_level)
{
    request->priority_level = priority_level;
    return 0;
}

int set_request_status(AssistanceRequest *request, RequestStatus request_status)
{
    request->request_status = request_status;
    return 0;
}

int set_estimated_cost(AssistanceRequest *request, float estimated_cost)
{
    if (estimated_cost < 0.0f)
    {
        fprintf(stderr, "Costo stimato non valido\n");
        return -1;
    }

    request->estimated_cost = estimated_cost;
    return 0;
}

int set_final_cost(AssistanceRequest *request, float final_cost)
{
    if (final_cost < 0.0f)
    {
        fprintf(stderr, "Costo finale non valido\n");
        return -1;
    }

    request->final_cost = final_cost;
    return 0;
}

int set_opening_date(AssistanceRequest *request, Date opening_date)
{
    request->opening_date = opening_date;
    return 0;
}