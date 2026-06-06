/**
 * @file filters.h
 * @brief Modulo per il filtraggio delle richieste di assistenza.
 *
 * Fornisce le funzioni per selezionare sottoinsiemi di richieste di assistenza
 * basandosi su criteri specifici quali lo stato del ticket, il livello di priorità
 * o il nome del cliente richiedente.
 */

#ifndef FILTERS_H
#define FILTERS_H
#include"ware.h"

/**
 * @brief Filtra le richieste di assistenza in base al loro stato attuale.
 * @param arr Puntatore all'array di richieste originale (non modificato).
 * @param status Lo stato in base al quale effettuare la selezione (es. OPEN, CLOSED).
 * @return Un nuovo oggetto AssistanceRequestArray contenente solo le richieste corrispondenti, 
 * o NULL in caso di errore o parametri non validi.
 */
const AssistanceRequestArray *filter_by_status(const AssistanceRequestArray * const arr, RequestStatus status);

/**
 * @brief Filtra le richieste di assistenza in base al loro livello di priorità.
 * @param arr Puntatore all'array di richieste originale (non modificato).
 * @param priority Il livello di urgenza desiderato (es. LOW, HIGH).
 * @return Un nuovo oggetto AssistanceRequestArray contenente solo le richieste corrispondenti, 
 * o NULL in caso di errore o parametri non validi.
 */
const AssistanceRequestArray *filter_by_priority(const AssistanceRequestArray * const arr, PriorityLevel priority);

/**
 * @brief Filtra le richieste di assistenza in base al nome esatto del cliente.
 * @param arr Puntatore all'array di richieste originale (non modificato).
 * @param customer_name Stringa contenente il nome del cliente da rintracciare.
 * @return Un nuovo oggetto AssistanceRequestArray contenente solo le richieste corrispondenti, 
 * o NULL in caso di errore o parametri non validi.
 */
const AssistanceRequestArray *filter_by_customer_name(const AssistanceRequestArray * const arr, char *customer_name);

#endif //FILTERS_H