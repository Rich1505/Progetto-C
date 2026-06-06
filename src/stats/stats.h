/**
 * @file stats.h
 * @brief Interfaccia del modulo per il calcolo delle statistiche aggregate.
 *
 * Questo modulo si occupa dell'analisi quantitativa e finanziaria dei dati
 * memorizzati nel sistema. Fornisce report di sintesi focalizzati sul calcolo
 * di metriche quali il volume di richieste, i costi medi e i picchi di spesa,
 * aggregati per categorie specifiche.
 */
#ifndef STATS_H
#define STATS_H
#include"ware.h"

/**
 * @brief Calcola e stampa le statistiche per tipologia di dispositivo (Punto 10).
 * Mostra il numero di richieste, il costo medio e il costo finale più alto.
 */
void print_stats_by_priority(AssistanceRequestArray *list);

/**
 * @brief Calcola e stampa le statistiche per livello di priorità (Punto 12).
 */
void print_stats_by_device_type(AssistanceRequestArray *list);

#endif //STATS_H