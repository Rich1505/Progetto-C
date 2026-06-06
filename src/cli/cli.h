/**
 * @file cli.h
 * @brief Interfaccia a riga di comando (CLI) per l'interazione con l'utente.
 *
 * Questo modulo definisce i punti di ingresso per mostrare i menu testuali,
 * acquisire e convalidare l'input da tastiera e coordinare le chiamate verso
 * la logica di business dell'applicazione.
 */

#ifndef CLI_H
#define CLI_H
#include"ware.h"

/**
 * @brief Avvia il ciclo principale dell'interfaccia a riga di comando.
 * * Gestisce il menu principale e reindirizza l'utente verso le specifiche
 * funzionalità del programma in base alla scelta immessa. Al termine delle 
 * operazioni, garantisce la corretta transizione di uscita.
 * * @param list Puntatore alla lista principale delle richieste da manipolare.
 */
void run_main_menu(AssistanceRequestArray *list);

#endif //CLI_H