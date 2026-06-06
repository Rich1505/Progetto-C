#include "filters.h"
#include "ware.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Puntatore a funzione per uniformare i criteri di verifica delle singole richieste */
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

/**
 * @brief Funzione interna generica per l'estrazione e il filtraggio dei record.
 * @param arr Array sorgente da esaminare.
 * @param status Criterio di stato opzionale passato al comparatore.
 * @param priority Criterio di priorità opzionale passato al comparatore.
 * @param customer_name Criterio testuale opzionale passato al comparatore.
 * @param comp Funzione di callback (predicato) per valutare la corrispondenza dell'elemento.
 * @return Un nuovo blocco array allocato con i soli elementi validi, o NULL in caso di errore.
 */
const AssistanceRequestArray *filter(const AssistanceRequestArray *const arr, RequestStatus status, PriorityLevel priority, char *customer_name, Comparator comp)
{
    if (arr == NULL)
    {
        fprintf(stderr, "Errore filter: puntatore NULL\n");
        return NULL;
    }

    int size = get_assistance_request_array_size(arr);
    if(size == 0)
    {
        fprintf(stderr, "Nessuna richiesta da filtrare\n");
        return NULL;
    }

    AssistanceRequest **original_array = get_assistance_request_array_ptr(arr);

    /* Allocazione prudenziale iniziale della dimensione massima potenziale del sotto-array */
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

    if(filtered_array_size == 0)
    {
        fprintf(stderr, "Nessuna richiesta trovata\n");
        free(filtered_array);
        return NULL;
    }

    /* Contrazione della memoria allocata per adattarla esattamente al numero di elementi effettivamente trovati */
    AssistanceRequest **temp = realloc(filtered_array, filtered_array_size * sizeof(AssistanceRequest *));
    if (temp == NULL)
    {
        if(size != 0)
        {
            fprintf(stderr, "Errore filter: riallocazione fallita\n");
            free(filtered_array);
        }
        else
            fprintf(stderr, "Nessuna richiesta e' stata trovata\n");

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