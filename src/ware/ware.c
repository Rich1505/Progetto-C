#include "ware.h"
#include <stdlib.h>
#include <stdio.h>
#include<string.h>

#define MAX_CUSTOMER_NAME 50
#define MAX_DESCRIPTION 200

#define INVALID_REQUEST_CODE -1

int max_days_per_month[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

struct date
{
    int day;
    int month;
    int year;
};

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
    Date date;

    if (year < 1900)
    {
        fprintf(stderr, "Inserire un anno valido\n");
        return (Date){INVALID_REQUEST_CODE};
    }

    if (month < 1 || month > 12)
    {
        fprintf(stderr, "Inserire un mese valido\n");
        return (Date){INVALID_REQUEST_CODE};
    }

    if (day > max_days_per_month[month - 1] || day < 1)
    {
        fprintf(stderr, "Inserire un giorno valido\n");
        return (Date){INVALID_REQUEST_CODE};
    }

    if (month == 2)
    {
        if (!is_leap_year(year) && day == 29)
        {
            fprintf(stderr, "Inserire un giorno valdio\n");
            return (Date){INVALID_REQUEST_CODE};
        }
    }

    return (Date){day,month,year};
}

AssistanceRequest create_assistance_request(int request_code, char *customer_name, DeviceType device_type, char *description,
                                             PriorityLevel priority_level, RequestStatus request_status, float estimated_cost, float final_cost, Date opening_date)
{
    AssistanceRequest assistance_request;

    if(request_code < 0)
    {
        fprintf(stderr, "Inserire un codice valido\n");
        return (AssistanceRequest){INVALID_REQUEST_CODE};
    }

    if(strlen(customer_name) >= MAX_CUSTOMER_NAME)
    {
        fprintf(stderr, "Il nome inserito per il cliente è troppo lungo\n");
    }

    if(strlen(description) >= MAX_DESCRIPTION)
    {
        fprintf(stderr, "La descrizione inserita per la richiesta è troppo lunga\n");
    }

    if(estimated_cost < 0.0f)
    {
        fprintf(stderr, "Inserire un costo stimato valido\n");
        return (AssistanceRequest){INVALID_REQUEST_CODE};
    }

    if(final_cost < 0.0f)
    {
        fprintf(stderr, "Inserire un costo finale valido\n");
        return (AssistanceRequest){INVALID_REQUEST_CODE};
    }


    assistance_request.request_code = request_code;
    strncpy(assistance_request.customer_name, customer_name, MAX_CUSTOMER_NAME - 1);
    assistance_request.device_type = device_type;
    strncpy(assistance_request.description, description, MAX_DESCRIPTION - 1);
    assistance_request.priority_level = priority_level;
    assistance_request.request_status = request_status;
    assistance_request.estimated_cost = estimated_cost;
    assistance_request.final_cost = final_cost;
    assistance_request.opening_date = opening_date;

    return assistance_request;
}