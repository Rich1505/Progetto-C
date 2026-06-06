/**
 * @file search.h
 * @brief Modulo per la ricerca e il filtraggio delle richieste di assistenza.
 *
 * Fornisce le funzioni necessarie a rintracciare specifiche richieste all'interno
 * dei contenitori dinamici sfruttando i codici identificativi univoci.
 */

#ifndef SEARCH_H
#define SEARCH_H
#include"ware.h"

/**
 * @brief Cerca una richiesta di assistenza all'interno dell'array tramite il suo codice identificativo.
 * * Estrae l'array di puntatori sottostante e delega la ricerca lineare a una funzione interna.
 * * @param arr Puntatore all'array contenitore all'interno del quale effettuare la ricerca.
 * @param request_code Codice identificativo univoco da trovare.
 * @return Puntatore alla richiesta trovata, o NULL se il codice non esiste o i parametri non sono validi.
 */
AssistanceRequest *search_by_request_code(AssistanceRequestArray *arr, int request_code);

#endif //SEARCH_H