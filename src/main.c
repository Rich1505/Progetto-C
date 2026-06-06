/**
 * @file main.c
 * @brief Punto di ingresso principale dell'applicazione di assistenza tecnica.
 *
 * Inizializza il sistema caricando lo stato del database locale dalla memoria
 * secondaria e cede il controllo del flusso all'interfaccia utente (CLI).
 * Al termine del ciclo di esecuzione della CLI, il programma rilascia le risorse
 * e termina la sua esecuzione.
 */

#include "file_manager.h"
#include "ware.h"
#include "sort.h"
#include <stdlib.h>
#include <stdio.h>
#include "search.h"
#include "cli.h"
#include "filters.h"

int main(void)
{
    AssistanceRequestArray *assistance_request_array = read_from_memory();

    run_main_menu(assistance_request_array);

    free_assistance_request_array(assistance_request_array);

    return 0;
}