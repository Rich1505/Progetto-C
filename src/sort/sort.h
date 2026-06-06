/**
 * @file sort.h
 * @brief Modulo per l'ordinamento delle richieste di assistenza.
 *
 * Fornisce le funzioni necessarie a ordinare le richieste contenute
 * nell'array dinamico basandosi su criteri specifici, quali il costo stimato
 * o il nome del cliente.
 */

#ifndef SORT_H
#define SORT_H
#include "ware.h"

/**
 * @brief Ordina l'array delle richieste di assistenza in base al costo stimato in modo crescente.
 * * Estrae l'array di puntatori sottostante e applica l'algoritmo di ordinamento Merge Sort
 * utilizzando il comparatore numerico dedicato.
 * * @param arr Puntatore all'array contenitore da ordinare in-place.
 */
void sort_by_estimated_cost(AssistanceRequestArray *arr);

/**
 * @brief Ordina l'array delle richieste di assistenza in base al nome del cliente in ordine alfabetico.
 * * Estrae l'array di puntatori sottostante e applica l'algoritmo di ordinamento Merge Sort
 * utilizzando il comparatore lessicografico dedicato.
 * * @param arr Puntatore all'array contenitore da ordinare in-place.
 */
void sort_by_customer_name(AssistanceRequestArray *arr);

#endif //SORT_H