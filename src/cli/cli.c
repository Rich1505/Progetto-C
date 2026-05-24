// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include"cli.h"
// #include"ware.h"
// #include"file_manager.h"

// static void print_menu(void);
// static void request_managment(AssistanceRequest *list);
// static void search_filter(AssistanceRequest *list);
// static void sort(AssistanceRequest *list);
// static void statistics(AssistanceRequest *list);
// static void reprt(AssistanceRequest *list);
// static void save_load(AssistanceRequest *list);

// static int read_integer(const char *prompt);
// static void read_string(const char *prompt, char *buffer, size_t size);
// static double read_double(const char *prompt);
// static void wait_for_enter(void);

// static void insert_request(AssistanceRequest *list);
// static void update_cost(AssistanceRequest *list);
// static void edit_request(AssistanceRequest *list);
// static void delete_request(AssistanceRequest *list);
// static void show_request(AssistanceRequest *list);

// void show_startup_screen(void)
// {
//     printf("===========================================================\n");
//     printf("      CENTRO ASSISTENZA TECNICA - GESTIONE RICHIESTE       \n");
//     printf("===========================================================\n");
//     printf("Benvenuto nel sistema di gestione delle richieste di assistenza tecnica!\n");
//     printf("Le funzionalità principali includono:\n");
//     printf("1. Gestione delle richieste: Inserimento, Modifica, Cancellazione, Visualizzazione\n");
//     printf("2. Ricerca e Filtro: Ricerca per codice, nome cliente, tipo di dispositivo, priorità, stato\n");
//     printf("3. Ordinamento: Ordinamento per codice, nome cliente, tipo di dispositivo, priorità, stato\n");
//     printf("4. Statistiche: Numero totale di richieste, richieste per tipo di dispositivo, richieste per priorità, richieste per stato\n");
//     printf("5. Report: Generazione di report dettagliati in formato testo\n");
// }

// void run_main_menu(AssistanceRequest *list)
// {
//     int choice;

//     do
//     {
//         print_menu();
//         choice = read_integer("Seleziona un'opzione: ");

//         switch (choice)
//         {
//             case 1:
//                 request_managment(list);
//                 break;
//             case 2:
//                 search_filter(list);
//                 break;
//             case 3:
//                 sort(list);
//                 break;
//             case 4:
//                 statistics(list);
//                 break;
//             case 5:
//                 reprt(list);
//                 break;
//             case 6:
//                 save_load(list);
//                 break;
//             case 0:
//                 printf("Uscita in corso...\n");
//                 break;
//             default:
//                 show_error_message("Opzione non valida. Inserisci un numero valido del menu.");
//         }
//     } while (choice != 0);
// }

// void print_menu(void)
// {
//     printf("\n====== Menu Principale ======\n");
//     printf("1. Gestione Richieste\n");
//     printf("2. Ricerca e Filtro\n");
//     printf("3. Ordinamento\n");
//     printf("4. Statistiche\n");
//     printf("5. Report\n");
//     printf("6. Salvataggio e Caricamento\n");
//     printf("0. Esci\n");
//     printf("\n=============================\n");
// }

// static void request_managment(AssistanceRequest *list)
// {
//     int choice;

//     do
//     {
//         printf("\n======== Gestione Richieste ======\n");
//         printf("1. Inserisci Nuova Richiesta\n");
//         printf("2. Aggiorna Costo Finale\n");
//         printf("3. Modifica Richiesta\n");
//         printf("4. Elimina Richiesta\n");
//         printf("5. Visualizza Dettagli Richiesta\n");
//         printf("0. Torna al Menu Principale\n");
//         printf("\n==================================\n");
        
//         choice = read_integer("Seleziona un'opzione: ");
        
//         switch (choice)
//         {
//             case 1:
//                 insert_request(list);
//                 break;
//             case 2:
//                 update_cost(list);
//                 break;
//             case 3:
//                 edit_request(list);
//                 break;
//             case 4:
//                 delete_request(list);
//                 break;
//             case 5:
//                 show_request(list);
//                 break;
//             case 0:
//                 printf("Ritorno al menu principale...\n");
//                 break;
//             default:
//                 show_error_message("Opzione non valida. Inserisci un numero valido del menu.");
//         }
//     } while (choice != 0);
// }