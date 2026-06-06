#include"search.h"
#include<stdlib.h>
#include<stdio.h>

/**
 * @brief Esegue una ricerca sequenziale (lineare) all'interno di un array di puntatori a richieste.
 * @param arr Array di puntatori a richieste di assistenza.
 * @param size Numero di elementi presenti nell'array.
 * @param request_code Codice identificativo da cercare.
 * @return L'indice dell'elemento se trovato, altrimenti -1.
 */
static int linear_search(AssistanceRequest **arr, int size, int request_code);

AssistanceRequest *search_by_request_code(AssistanceRequestArray *arr, int request_code)
{
    if(arr == NULL)
    {
        fprintf(stderr, "Errore search_by_request_code: puntatore NULL\n");
        return NULL;
    }

    if(request_code < 0)
    {
        return NULL;
    }

    AssistanceRequest **array_ptr = get_assistance_request_array_ptr(arr);
    int size = get_assistance_request_array_size(arr);

    int pos = linear_search(array_ptr, size, request_code);
    if(pos >= 0)
    {
        return array_ptr[pos];
    }

    return NULL;
}

static int linear_search(AssistanceRequest **arr, int size, int request_code) {
    for(int i = 0;i < size; i++)
    {
        if(arr[i] != NULL && get_request_code(arr[i]) == request_code)
        {
            return i;
        }
    }

    return -1;
}