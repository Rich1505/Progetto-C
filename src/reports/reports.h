/**
 * @file reports.h
 * @brief Modulo per la generazione di reportistica e statistiche sulle richieste.
 *
 * Fornisce le funzioni necessarie a elaborare i dati aggregati del sistema,
 * permettendo la produzione di resoconti generali sui costi, sulla distribuzione
 * dei carichi di lavoro e sullo stato operativo dei ticket di assistenza.
 */

#ifndef REPORTS_H
#define REPORTS_H
#include "ware.h"

/**
 * @brief Genera e stampa su file un report statistico generale di tutte le richieste in memoria.
 * * Analizza l'intero dataset per calcolare metriche aggregate quali il fatturato totale
 * (stimato e finale) e la distribuzione percentuale per stato e livello di priorità.
 * * @param list Puntatore costante all'array contenitore delle richieste (sorgente dati in sola lettura).
 */
void general_requests_report(const AssistanceRequestArray *list);

/**
 * @brief Genera e stampa su file un report operativo dettagliato focalizzato sui flussi di lavoro.
 * * Suddivide e raggruppa visivamente le richieste in base al loro ciclo di vita attuale
 * e fornisce un riepilogo quantitativo suddiviso per ciascuna tipologia di dispositivo.
 * * @param list Puntatore costante all'array contenitore delle richieste (sorgente dati in sola lettura).
 */
void operative_requests_report(const AssistanceRequestArray *list);

#endif //REPORTS_H