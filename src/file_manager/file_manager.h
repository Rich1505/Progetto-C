/**
 * @file file_manager.h
 * @brief Modulo per la persistenza dei dati su file.
 *
 * Questo modulo si occupa del salvataggio e del caricamento delle richieste
 * di assistenza da un file di testo fungendo da database locale (memoria secondaria)
 * per l'applicazione.
 */

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include"ware.h"

/**
 * @brief Legge le richieste di assistenza dal file di memoria e popola un array dinamico.
 * * Se il file non esiste, tenta di crearlo vuoto. Esegue il parsing riga per riga
 * con validazione dei campi e conversione delle stringhe nei tipi enumerati corrispondenti.
 * * @return Puntatore alla struttura AssistanceRequestArray popolata, o NULL in caso di errore fatale.
 */
AssistanceRequestArray *read_from_memory();

/**
 * @brief Salva l'intero contenuto dell'array di richieste sovrascrivendo il file di memoria.
 * * Converte i campi delle strutture in formato testuale CSV (separato da punti e virgola)
 * e ripristina i valori stringa a partire dagli enumerati (es. DeviceType).
 * * @param array Puntatore all'array contenente i dati da serializzare.
 * @return 0 in caso di successo, -1 in caso di errore di apertura o scrittura del file.
 */
int write_in_memory(AssistanceRequestArray *array);

#endif //FILE_MANAGER_H