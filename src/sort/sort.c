#include"sort.h"
#include <string.h>
#include<stdio.h>
#include<stdlib.h>

/* I nostri comparatori restituiranno un valore < 0 se il primo elemento viene prima del secondo, 0 se sono uguali e > 0 se il secondo elemento viene prima del primo */
typedef int (*Comparator)(AssistanceRequest*, AssistanceRequest*);

static int compare_by_estimated_cost(AssistanceRequest *a, AssistanceRequest *b);

static int compare_by_customer_name(AssistanceRequest *a, AssistanceRequest *b);

/**
 * @brief Unisce due sotto-array ordinati in un unico blocco coeso e ordinato.
 * @param arr Array di puntatori complessivo.
 * @param l Indice sinistro del primo sotto-array.
 * @param m Indice mediano che separa i due sotto-array.
 * @param r Indice destro del secondo sotto-array.
 * @param comp Puntatore alla funzione di comparazione da applicare.
 * @return 0 in caso di successo, -1 se l'allocazione temporanea fallisce.
 */
static int merge(AssistanceRequest **arr, int l, int m, int r, Comparator comp);

/**
 * @brief Funzione ricorsiva interna che implementa l'algoritmo Merge Sort.
 * @param arr Array di puntatori da ordinare.
 * @param l Indice iniziale della porzione corrente.
 * @param r Indice finale della porzione corrente.
 * @param comp Puntatore alla funzione di comparazione.
 * @return 0 in caso di successo, -1 se si verifica un errore a cascata nelle chiamate interne.
 */
int merge_sort(AssistanceRequest **arr, int l, int r, Comparator comp);

void sort_by_estimated_cost(AssistanceRequestArray *arr)
{
    if(arr == NULL)
    {
        fprintf(stderr, "Errore sort_by_estimated_cost: puntatore NULL\n");
        return;
    }

    int size = get_assistance_request_array_size(arr);
    if (size <= 1) return;

    merge_sort(get_assistance_request_array_ptr(arr), 0, get_assistance_request_array_size(arr) - 1, compare_by_estimated_cost);
}

void sort_by_customer_name(AssistanceRequestArray *arr)
{
    if(arr == NULL)
    {
        fprintf(stderr, "Errore sort_by_customer_name: puntatore NULL\n");
        return;
    }

    int size = get_assistance_request_array_size(arr);
    if (size <= 1) return;

    merge_sort(get_assistance_request_array_ptr(arr), 0, get_assistance_request_array_size(arr) - 1, compare_by_customer_name);
}

int compare_by_estimated_cost(AssistanceRequest *a, AssistanceRequest *b)
{
    float costo_a = get_estimated_cost(a);
    float costo_b = get_estimated_cost(b);
    
    if (costo_a < costo_b) return -1;
    if (costo_a > costo_b) return 1;
    return 0;
}

int compare_by_customer_name(AssistanceRequest *a, AssistanceRequest *b)
{
    return strcmp(get_customer_name(a), get_customer_name(b));
}

static int merge(AssistanceRequest **arr, int l, int m, int r, Comparator comp)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* Allocazione dinamica dei buffer ausiliari temporanei per contenere le due metà */
    AssistanceRequest **L = (AssistanceRequest **)malloc(n1 * sizeof(AssistanceRequest *));
    AssistanceRequest **R = (AssistanceRequest **)malloc(n2 * sizeof(AssistanceRequest *));
    
    if(L == NULL || R == NULL)
    {
        if (L != NULL) free(L);
        
        if (R != NULL) free(R);

        fprintf(stderr, "Errore merge: allocazione memoria fallita\n");
        return -1;
    }
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;

    /* Fusione dei due buffer intermedi all'interno dell'array principale in base all'esito del comparatore */
    while (i < n1 && j < n2) {
        if (comp(L[i], R[j]) <= 0) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    /* Rilascio dei blocchi temporanei allocati all'inizio della procedura */
    free(L);
    free(R);

    return 0;
}

static int merge_sort(AssistanceRequest **arr, int l, int r, Comparator comp)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        if(merge_sort(arr, l, m, comp) == -1)
            return -1;

        if(merge_sort(arr, m + 1, r, comp) == -1)
            return -1;

        if(merge(arr, l, m, r, comp) == -1)
            return -1;
    }

    return 0;
}