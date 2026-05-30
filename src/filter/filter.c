#include "filter.h"
#include "ware.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int (*Comparator)(AssistanceRequest *ass, RequestStatus status, PriorityLevel priority, char *customer_name);

int check_status(AssistanceRequest *ass, RequestStatus status, PriorityLevel priority, char *customer_name)
{
    return get_request_status(ass) == status;
}

int check_priority(AssistanceRequest *ass, RequestStatus status, PriorityLevel priority, char *customer_name)
{
    return get_priority_level(ass) == priority;
}

int check_customer_name(AssistanceRequest *ass, RequestStatus status, PriorityLevel priority, char *customer_name)
{
    return strcmp(get_customer_name(ass), customer_name) == 0;
}

const AssistanceRequestArray *filter(const AssistanceRequestArray *const arr, RequestStatus status, PriorityLevel priority, char *customer_name, Comparator comp)
{
    if (arr == NULL)
    {
        fprintf(stderr, "Errore filter: puntatore NULL\n");
        return NULL;
    }

    int size = get_assistance_request_array_size(arr);
    AssistanceRequest **original_array = get_assistance_request_array_ptr(arr);

    AssistanceRequest **filtered_array = malloc(size * sizeof(AssistanceRequest *));
    if (filtered_array == NULL)
    {
        fprintf(stderr, "Errore filter: allocazione memoria non riuscita\n");
        return NULL;
    }

    int filtered_array_size = 0;

    for (int i = 0; i < size; i++)
    {
        if (comp(original_array[i], status, priority, customer_name))
        {
            filtered_array[filtered_array_size++] = original_array[i];
        }
    }

    AssistanceRequest **temp = realloc(filtered_array, filtered_array_size * sizeof(AssistanceRequest *));
    if (temp == NULL)
    {
        fprintf(stderr, "Errore filter: riallocazione fallita\n");
        free(filtered_array);
        return NULL;
    }
    filtered_array = temp;

    AssistanceRequestArray *filtered_array_object = create_assistance_request_array(filtered_array, filtered_array_size, filtered_array_size);

    return filtered_array_object;
}

const AssistanceRequestArray *filter_by_priority(const AssistanceRequestArray *const arr, PriorityLevel priority)
{
    return filter(arr, 0, priority, NULL, check_priority);
}

const AssistanceRequestArray *filter_by_status(const AssistanceRequestArray *const arr, RequestStatus status)
{
    return filter(arr, status, 0, NULL, check_status);
}

const AssistanceRequestArray *filter_by_customer_name(const AssistanceRequestArray *const arr, char *customer_name)
{
    return filter(arr, 0, 0, customer_name, check_customer_name);
}
