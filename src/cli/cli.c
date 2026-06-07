#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cli.h"
#include "file_manager.h"
#include "sort.h"
#include "search.h"
#include "filters.h"
#include "reports.h"
#include"stats.h"

/* =========================================================
 *  FORWARD DECLARATIONS — utility di input/output
 * ========================================================= */
static int   read_int(const char *prompt);
static float read_float(const char *prompt);
static void  read_string(const char *prompt, char *buffer, int size);

static void show_error_message(const char *message);
static void show_success_message(const char *message);
static void print_splash_screen(void);

static DeviceType    read_device_type(void);
static PriorityLevel read_priority_level(void);
static RequestStatus read_status(void);

static AssistanceRequest *read_existing_request(AssistanceRequestArray *list);

/* =========================================================
 *  FORWARD DECLARATIONS — menu principali e sottomenù
 * ========================================================= */
static void print_main_menu(void);
static void run_request_management_menu(AssistanceRequestArray *list);
static void run_search_filter_menu(AssistanceRequestArray *list);
static void run_sort_menu(AssistanceRequestArray *list);
static void run_report_menu(AssistanceRequestArray *list);
static void run_stats_menu(AssistanceRequestArray *list);
static void run_save_menu(AssistanceRequestArray *list);

/* =========================================================
 *  FORWARD DECLARATIONS — operazioni di gestione richieste
 * ========================================================= */
static void show_all_requests(AssistanceRequestArray *list);
static void insert_request_cli(AssistanceRequestArray *list);
static void update_request_cli(AssistanceRequestArray *list);
static void cancel_request_cli(AssistanceRequestArray *list);
static void view_request_detail_cli(AssistanceRequestArray *list);

/* =========================================================
 *  FORWARD DECLARATIONS — operazioni di ricerca e filtri
 * ========================================================= */
static void search_request_cli(AssistanceRequestArray *list);
static void filter_by_status_cli(AssistanceRequestArray *list);
static void filter_by_priority_cli(AssistanceRequestArray *list);
static void filter_by_customer_name_cli(AssistanceRequestArray *list);

/* =========================================================
 *  FORWARD DECLARATIONS — operazioni di report
 * ========================================================= */
static void general_report_cli(AssistanceRequestArray *list);
static void operative_report_cli(AssistanceRequestArray *list);

/* =========================================================
 *  FORWARD DECLARATIONS — operazioni di modifica su puntatore diretto
 * ========================================================= */
static void update_status_cli_on(AssistanceRequest *request);
static void update_estimated_cost_cli_on(AssistanceRequest *request);
static void update_final_cost_cli_on(AssistanceRequest *request);
static void update_description_cli_on(AssistanceRequest *request);

/* =========================================================
 *  FORWARD DECLARATIONS — filtri (pattern FilterProvider)
 * ========================================================= */
typedef const AssistanceRequestArray *(*FilterProvider)(AssistanceRequestArray *list);
static void show_filtered(AssistanceRequestArray *list, FilterProvider provider, const char *empty_message);
static const AssistanceRequestArray *filter_status_provider(AssistanceRequestArray *list);
static const AssistanceRequestArray *filter_priority_provider(AssistanceRequestArray *list);
static const AssistanceRequestArray *filter_name_provider(AssistanceRequestArray *list);

/* =========================================================
 *  FORWARD DECLARATIONS — statistiche
 * ========================================================= */
static void device_stats_cli(AssistanceRequestArray *list);
static void priority_stats_cli(AssistanceRequestArray *list);


/* =========================================================
 *  ENTRY POINT — menu principale
 * ========================================================= */

/**
 * @brief Punto di ingresso dell'interfaccia utente.
 *        Mostra la schermata iniziale e gestisce il ciclo del menu principale.
 * @param list Elenco globale delle richieste di assistenza.
 */
void run_main_menu(AssistanceRequestArray *list)
{
    int choice;

    if (list == NULL)
    {
        show_error_message("Lista richieste non valida.");
        return;
    }

    print_splash_screen();

    do
    {
        print_main_menu();
        choice = read_int("Seleziona un'area: ");

        switch (choice)
        {
            case 1: run_request_management_menu(list); break;
            case 2: run_search_filter_menu(list);      break;
            case 3: run_sort_menu(list);               break;
            case 4: run_report_menu(list);             break;
            case 5: run_stats_menu(list);              break;
            case 6: run_save_menu(list);               break;
            case 0: printf("\nArrivederci!\n");        break;
            default:
                show_error_message("Opzione non valida. Inserisci un numero valido del menu.");
        }
    } while (choice != 0);
}


/* =========================================================
 *  SCHERMATA INIZIALE
 * ========================================================= */

/**
 * @brief Stampa la schermata di benvenuto all'avvio del programma.
 */
static void print_splash_screen(void)
{
    printf("\n");
    printf("+----------------------------------------------+\n");
    printf("|     SISTEMA GESTIONE RICHIESTE ASSISTENZA    |\n");
    printf("|              Tecnico  v1.0                   |\n");
    printf("+----------------------------------------------+\n");
    printf("\nBenvenuto! Seleziona un'area dal menu principale.\n\n");
}


/* =========================================================
 *  MENU PRINCIPALE
 * ========================================================= */

/**
 * @brief Stampa il menu principale con le aree funzionali disponibili.
 */
static void print_main_menu(void)
{
    printf("\n+----------------------------------+\n");
    printf("|         MENU PRINCIPALE          |\n");
    printf("+----------------------------------+\n");
    printf("|  1. Gestione Richieste           |\n");
    printf("|  2. Ricerca e Filtri             |\n");
    printf("|  3. Ordinamenti                  |\n");
    printf("|  4. Report                       |\n");
    printf("|  5. Statistiche                  |\n");
    printf("|  6. Salvataggio                  |\n");
    printf("|  0. Esci                         |\n");
    printf("+----------------------------------+\n");
}


/* =========================================================
 *  SOTTOMENU — AREA GESTIONE RICHIESTE
 * ========================================================= */

/**
 * @brief Gestisce il sottomenu dell'area "Gestione Richieste".
 *        Permette di inserire, modificare, annullare e visualizzare richieste.
 * @param list Elenco globale delle richieste.
 */
static void run_request_management_menu(AssistanceRequestArray *list)
{
    int choice;

    do
    {
        printf("\n+----------------------------------+\n");
        printf("|       GESTIONE RICHIESTE         |\n");
        printf("+----------------------------------+\n");
        printf("|  1. Inserisci richiesta          |\n");
        printf("|  2. Modifica richiesta           |\n");
        printf("|  3. Annulla richiesta            |\n");
        printf("|  4. Visualizza tutte le rich.    |\n");
        printf("|  5. Visualizza dettaglio rich.   |\n");
        printf("|  0. Torna al menu principale     |\n");
        printf("+----------------------------------+\n");

        choice = read_int("Seleziona un'opzione: ");

        switch (choice)
        {
            case 1: insert_request_cli(list);      break;
            case 2: update_request_cli(list);      break;
            case 3: cancel_request_cli(list);      break;
            case 4: show_all_requests(list);        break;
            case 5: view_request_detail_cli(list); break;
            case 0: break;
            default: show_error_message("Opzione non valida.");
        }
    } while (choice != 0);
}


/* =========================================================
 *  SOTTOMENU — AREA RICERCA E FILTRI
 * ========================================================= */

/**
 * @brief Gestisce il sottomenu dell'area "Ricerca e Filtri".
 *        Permette di cercare per codice e filtrare per stato, priorita' o cliente.
 * @param list Elenco globale delle richieste.
 */
static void run_search_filter_menu(AssistanceRequestArray *list)
{
    int choice;

    do
    {
        printf("\n+----------------------------------+\n");
        printf("|        RICERCA E FILTRI          |\n");
        printf("+----------------------------------+\n");
        printf("|  1. Cerca per codice             |\n");
        printf("|  2. Filtra per stato             |\n");
        printf("|  3. Filtra per priorita'         |\n");
        printf("|  4. Filtra per nome cliente      |\n");
        printf("|  0. Torna al menu principale     |\n");
        printf("+----------------------------------+\n");

        choice = read_int("Seleziona un'opzione: ");

        switch (choice)
        {
            case 1: search_request_cli(list);          break;
            case 2: filter_by_status_cli(list);        break;
            case 3: filter_by_priority_cli(list);      break;
            case 4: filter_by_customer_name_cli(list); break;
            case 0: break;
            default: show_error_message("Opzione non valida.");
        }
    } while (choice != 0);
}


/* =========================================================
 *  SOTTOMENU — AREA ORDINAMENTI
 * ========================================================= */

/**
 * @brief Gestisce il sottomenu dell'area "Ordinamenti".
 *        Permette di ordinare le richieste per nome cliente o costo stimato.
 * @param list Elenco globale delle richieste.
 */
static void run_sort_menu(AssistanceRequestArray *list)
{
    int choice;

    do
    {
        printf("\n+----------------------------------+\n");
        printf("|          ORDINAMENTI             |\n");
        printf("+----------------------------------+\n");
        printf("|  1. Ordina per nome cliente      |\n");
        printf("|  2. Ordina per costo stimato     |\n");
        printf("|  0. Torna al menu principale     |\n");
        printf("+----------------------------------+\n");

        choice = read_int("Seleziona un'opzione: ");

        switch (choice)
        {
            case 1:
                sort_by_customer_name(list);
                show_success_message("Ordinamento per nome cliente completato.");
                break;
            case 2:
                sort_by_estimated_cost(list);
                show_success_message("Ordinamento per costo stimato completato.");
                break;
            case 0: break;
            default: show_error_message("Opzione non valida.");
        }
    } while (choice != 0);
}


/* =========================================================
 *  SOTTOMENU — AREA REPORT
 * ========================================================= */

/**
 * @brief Gestisce il sottomenu dell'area "Report".
 *        Permette di generare il report generale o quello operativo.
 * @param list Elenco globale delle richieste.
 */
static void run_report_menu(AssistanceRequestArray *list)
{
    int choice;

    do
    {
        printf("\n+----------------------------------+\n");
        printf("|             REPORT               |\n");
        printf("+----------------------------------+\n");
        printf("|  1. Report generale              |\n");
        printf("|  2. Report operativo             |\n");
        printf("|  0. Torna al menu principale     |\n");
        printf("+----------------------------------+\n");

        choice = read_int("Seleziona un'opzione: ");

        switch (choice)
        {
            case 1: general_report_cli(list);   break;
            case 2: operative_report_cli(list); break;
            case 0: break;
            default: show_error_message("Opzione non valida.");
        }
    } while (choice != 0);
}


/* =========================================================
 * SOTTOMENU — AREA STATISTICHE
 * ========================================================= */

/**
 * @brief Gestisce il sottomenu dell'area "Statistiche".
 * Permette di visualizzare dati aggregati per dispositivo e priorità.
 * @param list Elenco globale delle richieste.
 */
static void run_stats_menu(AssistanceRequestArray *list)
{
    int choice;

    do
    {
        printf("\n+----------------------------------+\n");
        printf("|           STATISTICHE            |\n");
        printf("+----------------------------------+\n");
        printf("|  1. Statistiche per dispositivo  |\n");
        printf("|  2. Statistiche per priorita'    |\n");
        printf("|  0. Torna al menu principale     |\n");
        printf("+----------------------------------+\n");

        choice = read_int("Seleziona un'opzione: ");

        switch (choice)
        {
            case 1: device_stats_cli(list);     break;
            case 2: priority_stats_cli(list);    break;
            case 0: break;
            default: show_error_message("Opzione non valida.");
        }
    } while (choice != 0);
}


/* =========================================================
 *  SOTTOMENU — AREA SALVATAGGIO
 * ========================================================= */

/**
 * @brief Gestisce il sottomenu dell'area "Salvataggio".
 *        Permette di salvare i dati su file.
 * @param list Elenco globale delle richieste.
 */
static void run_save_menu(AssistanceRequestArray *list)
{
    int choice;

    do
    {
        printf("\n+----------------------------------+\n");
        printf("|          SALVATAGGIO             |\n");
        printf("+----------------------------------+\n");
        printf("|  1. Salva su file                |\n");
        printf("|  0. Torna al menu principale     |\n");
        printf("+----------------------------------+\n");

        choice = read_int("Seleziona un'opzione: ");

        switch (choice)
        {
            case 1:
                if (write_in_memory(list) == 0)
                    show_success_message("Dati salvati correttamente su file.");
                else
                    show_error_message("Errore durante il salvataggio su file.");
                break;
            case 0: break;
            default: show_error_message("Opzione non valida.");
        }
    } while (choice != 0);
}

/* =========================================================
 *  MESSAGGI
 * ========================================================= */

/**
 * @brief Stampa un messaggio di errore formattato.
 * @param message Testo dell'errore da mostrare.
 */
static void show_error_message(const char *message)
{
    printf("[ERRORE] %s\n", message);
}

/**
 * @brief Stampa un messaggio di conferma/successo formattato.
 * @param message Testo del messaggio di conferma.
 */
static void show_success_message(const char *message)
{
    printf("[OK] %s\n", message);
}


/* =========================================================
 *  UTILITY DI INPUT
 * ========================================================= */

/**
 * @brief Legge un intero da stdin, rifiutando input non validi.
 * @param prompt Testo da mostrare all'utente prima dell'input.
 * @return Intero inserito dall'utente.
 */
static int read_int(const char *prompt)
{
    int value;
    char row[100];
    char extra;

    while (1)
    {
        printf("%s", prompt);
        fgets(row, sizeof(row), stdin);

        if (sscanf(row, "%d %c", &value, &extra) == 1)
        {
            return value;
        }

        show_error_message("Inserisci un numero intero valido.");
    }
}

/**
 * @brief Legge un float da stdin, rifiutando input non validi.
 * @param prompt Testo da mostrare all'utente prima dell'input.
 * @return Float inserito dall'utente.
 */
static float read_float(const char *prompt)
{
    float value;
    char row[100];
    char extra;

    while (1)
    {
        printf("%s", prompt);
        fgets(row, sizeof(row), stdin);

        if (sscanf(row, "%f %c", &value, &extra) == 1)
        {
            return value;
        }

        show_error_message("Inserisci un numero valido.");
    }
}

/**
 * @brief Legge una stringa non vuota da stdin.
 * @param prompt  Testo da mostrare all'utente prima dell'input.
 * @param buffer  Buffer di destinazione.
 * @param size    Dimensione massima del buffer.
 */
static void read_string(const char *prompt, char *buffer, int size)
{
    while (1)
    {
        printf("%s", prompt);
        
        // Se fgets restituisce NULL, si è verificato un errore o EOF
        if (fgets(buffer, size, stdin) == NULL) {
            continue; 
        }

        // Cerchiamo la posizione del carattere di nuova riga
        size_t newline_pos = strcspn(buffer, "\n");

        // Controllo del troncamento:
        // Se non c'è il '\n' E il buffer è pieno fino al limite (size - 1)
        if (buffer[newline_pos] == '\0' && (int)strlen(buffer) == size - 1)
        {
            show_error_message("Attenzione: l'input inserito supera il limite e verra' troncato.");
            
            // Svuotiamo il buffer di input (stdin) fino al newline o a EOF
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                // Ciclo vuoto: consuma i caratteri residui
            }
        }
        else
        {
            // Se il newline è presente, lo sovrascriviamo con il terminatore
            buffer[newline_pos] = '\0';
        }

        // Controllo se la stringa risultante è vuota
        if (strlen(buffer) > 0)
        {
            return;
        }

        show_error_message("Il campo non puo' essere vuoto.");
    }
}

/**
 * @brief Presenta all'utente un menu per scegliere il tipo di dispositivo.
 * @return Il DeviceType selezionato.
 */
static DeviceType read_device_type(void)
{
    int choice;

    while (1)
    {
        printf("Tipo dispositivo:\n");
        printf("  1. Smartphone\n");
        printf("  2. Tablet\n");
        printf("  3. Laptop\n");
        printf("  4. Desktop\n");
        printf("  5. Stampante\n");

        choice = read_int("Scelta: ");

        switch (choice)
        {
            case 1: return DEVICE_SMARTPHONE;
            case 2: return DEVICE_TABLET;
            case 3: return DEVICE_LAPTOP;
            case 4: return DEVICE_DESKTOP;
            case 5: return DEVICE_PRINTER;
            default:
                show_error_message("Tipo dispositivo non valido. Reinserisci la scelta.");
        }
    }
}

/**
 * @brief Presenta all'utente un menu per scegliere il livello di priorita'.
 * @return Il PriorityLevel selezionato.
 */
static PriorityLevel read_priority_level(void)
{
    int choice;

    while (1)
    {
        printf("Priorita':\n");
        printf("  1. Bassa (LOW)\n");
        printf("  2. Media (MEDIUM)\n");
        printf("  3. Alta  (HIGH)\n");

        choice = read_int("Scelta: ");

        switch (choice)
        {
            case 1: return PRIORITY_LOW;
            case 2: return PRIORITY_MEDIUM;
            case 3: return PRIORITY_HIGH;
            default:
                show_error_message("Priorita' non valida. Reinserisci la scelta.");
        }
    }
}

/**
 * @brief Presenta all'utente un menu per scegliere lo stato della richiesta.
 * @return Il RequestStatus selezionato.
 */
static RequestStatus read_status(void)
{
    int choice;

    while (1)
    {
        printf("Stato richiesta:\n");
        printf("  1. Aperta         (OPEN)\n");
        printf("  2. In lavorazione (IN PROGRESS)\n");
        printf("  3. Chiusa         (CLOSED)\n");
        printf("  4. Annullata      (CANCELED)\n");

        choice = read_int("Scelta: ");

        switch (choice)
        {
            case 1: return STATUS_OPEN;
            case 2: return STATUS_IN_PROGRESS;
            case 3: return STATUS_CLOSED;
            case 4: return STATUS_CANCELED;
            default:
                show_error_message("Stato non valido. Reinserisci la scelta.");
        }
    }
}

/**
 * @brief Chiede all'utente un codice e restituisce la richiesta corrispondente.
 *        Riprova finche' il codice e' valido; inserendo -1 annulla l'operazione.
 * @param list Elenco globale delle richieste.
 * @return Puntatore alla richiesta trovata, o NULL se l'utente ha annullato.
 */
static AssistanceRequest *read_existing_request(AssistanceRequestArray *list)
{
    int code;
    AssistanceRequest *request;

    printf("(Inserisci -1 per annullare)\n");

    while (1)
    {
        code = read_int("Codice richiesta: ");

        if (code == -1)
        {
            return NULL;
        }

        request = search_by_request_code(list, code);

        if (request != NULL)
        {
            return request;
        }

        show_error_message("Richiesta non trovata. Inserisci un codice valido o -1 per annullare.");
    }
}


/* =========================================================
 *  OPERAZIONI — GESTIONE RICHIESTE
 * ========================================================= */

/**
 * @brief Mostra tutte le richieste presenti nell'elenco.
 * @param list Elenco globale delle richieste.
 */
static void show_all_requests(AssistanceRequestArray *list)
{
    AssistanceRequest **array;
    int size;

    array = get_assistance_request_array_ptr(list);
    size  = get_assistance_request_array_size(list);

    if (array == NULL || size <= 0)
    {
        printf("Nessuna richiesta presente.\n");
        return;
    }

    printf("\n--- Elenco richieste (%d) ---\n", size);
    for (int i = 0; i < size; i++)
    {
        print_request(array[i]);
    }
}

/**
 * @brief Inserisce una nuova richiesta di assistenza raccogliendo tutti i campi dall'utente.
 * @param list Elenco globale delle richieste.
 */
static void insert_request_cli(AssistanceRequestArray *list)
{
    int code;
    char customer_name[MAX_CUSTOMER_NAME];
    char description[MAX_DESCRIPTION];
    DeviceType type;
    PriorityLevel priority;
    float estimated_cost;
    Date date;
    AssistanceRequest *new_request;

    do
    {
        code = read_int("Codice richiesta: ");
        if (code < 0)
        {
            show_error_message("Il codice deve essere un numero intero positivo.");
            continue;
        }

        if (search_by_request_code(list, code) != NULL)
        {
            show_error_message("Il codice inserito e' gia' utilizzato da un'altra richiesta.");
            code = -1;
        }
    } while (code < 0);

    read_string("Nome cliente: ", customer_name, MAX_CUSTOMER_NAME);
    type = read_device_type();
    read_string("Descrizione: ", description, MAX_DESCRIPTION);
    priority = read_priority_level();

    do
    {
        estimated_cost = read_float("Costo stimato: ");
        if (estimated_cost < 0)
        {
            show_error_message("Il costo stimato deve essere un numero positivo.");
        }
    } while (estimated_cost < 0);

    do
    {
        int day   = read_int("Giorno: ");
        int month = read_int("Mese: ");
        int year  = read_int("Anno: ");

        date = create_date(day, month, year);

        if (date.day == -1)
        {
            show_error_message("Data non valida. Riprova.");
        }
    } while (date.day == -1);

    new_request = create_assistance_request(
        code, customer_name, type, description,
        priority, STATUS_OPEN, estimated_cost, 0.0f, date
    );

    if (new_request == NULL)
    {
        show_error_message("Creazione richiesta fallita.");
        return;
    }

    if (insert_assistance_request(list, new_request) == 0)
    {
        show_success_message("Richiesta inserita correttamente.");
    }
    else
    {
        free_assistance_request(new_request);
        show_error_message("Inserimento fallito.");
    }
}

/**
 * @brief Sottomenu di modifica di una richiesta esistente.
 *        Permette di aggiornare stato, costo stimato, costo finale o descrizione.
 * @param list Elenco globale delle richieste.
 */
static void update_request_cli(AssistanceRequestArray *list)
{
    int choice;
    AssistanceRequest *request;

    request = read_existing_request(list);
    if (request == NULL)
    {
        show_error_message("Operazione annullata.");
        return;
    }

    printf("\n--- Richiesta selezionata ---\n");
    print_request(request);

    do
    {
        printf("\n+----------------------------------+\n");
        printf("|        MODIFICA RICHIESTA        |\n");
        printf("+----------------------------------+\n");
        printf("|  1. Modifica stato               |\n");
        printf("|  2. Modifica costo stimato       |\n");
        printf("|  3. Modifica costo finale        |\n");
        printf("|  4. Modifica descrizione         |\n");
        printf("|  0. Torna indietro               |\n");
        printf("+----------------------------------+\n");

        choice = read_int("Seleziona un'opzione: ");

        switch (choice)
        {
            case 1: update_status_cli_on(request);        break;
            case 2: update_estimated_cost_cli_on(request); break;
            case 3: update_final_cost_cli_on(request);     break;
            case 4: update_description_cli_on(request);    break;
            case 0: break;
            default: show_error_message("Opzione non valida.");
        }
    } while (choice != 0);
}

/**
 * @brief Annulla una richiesta esistente impostando lo stato a CANCELED.
 * @param list Elenco globale delle richieste.
 */
static void cancel_request_cli(AssistanceRequestArray *list)
{
    AssistanceRequest *request;

    request = read_existing_request(list);
    if (request == NULL)
    {
        show_error_message("Operazione annullata.");
        return;
    }

    if (set_request_status_canceled(request) == 0)
    {
        show_success_message("Richiesta annullata correttamente.");
    }
    else
    {
        show_error_message("Impossibile annullare la richiesta.");
    }
}

/**
 * @brief Mostra il dettaglio completo di una singola richiesta cercata per codice.
 * @param list Elenco globale delle richieste.
 */
static void view_request_detail_cli(AssistanceRequestArray *list)
{
    AssistanceRequest *request;

    request = read_existing_request(list);
    if (request == NULL)
    {
        show_error_message("Operazione annullata.");
        return;
    }

    print_request(request);
}


/* =========================================================
 *  OPERAZIONI DI MODIFICA — su puntatore diretto
 *  Usate da update_request_cli per non rileggere il codice
 *  a ogni campo modificato nella stessa sessione.
 * ========================================================= */

/**
 * @brief Aggiorna lo stato di una richiesta gia' individuata.
 * @param request Puntatore alla richiesta da modificare.
 */
static void update_status_cli_on(AssistanceRequest *request)
{
    RequestStatus new_status;
    int result = -1;

    new_status = read_status();

    switch (new_status)
    {
        case STATUS_OPEN:        result = set_request_status_opened(request);      break;
        case STATUS_IN_PROGRESS: result = set_request_status_in_progress(request); break;
        case STATUS_CLOSED:      result = set_request_status_closed(request);      break;
        case STATUS_CANCELED:    result = set_request_status_canceled(request);    break;
        default:                 result = -1;
    }

    if (result == 0)
        show_success_message("Stato aggiornato.");
    else
        show_error_message("Aggiornamento stato fallito.");
}

/**
 * @brief Aggiorna il costo stimato di una richiesta gia' individuata.
 * @param request Puntatore alla richiesta da modificare.
 */
static void update_estimated_cost_cli_on(AssistanceRequest *request)
{
    float cost;

    do
    {
        cost = read_float("Nuovo costo stimato: ");
        if (cost < 0)
        {
            show_error_message("Il costo stimato deve essere un numero positivo.");
        }
    } while (cost < 0);

    if (set_estimated_cost(request, cost) == 0)
        show_success_message("Costo stimato aggiornato.");
    else
        show_error_message("Aggiornamento costo stimato fallito.");
}

/**
 * @brief Aggiorna il costo finale di una richiesta gia' individuata.
 *        Richiede che la richiesta sia nello stato CLOSED.
 * @param request Puntatore alla richiesta da modificare.
 */
static void update_final_cost_cli_on(AssistanceRequest *request)
{
    float cost;

    do
    {
        cost = read_float("Nuovo costo finale: ");
        if (cost < 0)
        {
            show_error_message("Il costo finale deve essere un numero positivo.");
        }
    } while (cost < 0);

    if (set_final_cost(request, cost) == 0)
        show_success_message("Costo finale aggiornato.");
    else
        show_error_message("Aggiornamento costo finale fallito (la richiesta deve essere CLOSED).");
}

/**
 * @brief Aggiorna la descrizione di una richiesta gia' individuata.
 * @param request Puntatore alla richiesta da modificare.
 */
static void update_description_cli_on(AssistanceRequest *request)
{
    char description[MAX_DESCRIPTION];

    read_string("Nuova descrizione: ", description, MAX_DESCRIPTION);

    if (set_description(request, description) == 0)
        show_success_message("Descrizione aggiornata.");
    else
        show_error_message("Aggiornamento descrizione fallito.");
}


/* =========================================================
 *  OPERAZIONI — RICERCA E FILTRI
 * ========================================================= */

/**
 * @brief Cerca e mostra una singola richiesta per codice.
 * @param list Elenco globale delle richieste.
 */
static void search_request_cli(AssistanceRequestArray *list)
{
    int code;
    AssistanceRequest *request;

    code    = read_int("Codice richiesta da cercare: ");
    request = search_by_request_code(list, code);

    if (request == NULL)
    {
        show_error_message("Richiesta non trovata.");
        return;
    }

    print_request(request);
}

/**
 * @brief Incapsula e centralizza la visualizzazione e la deallocazione dei sotto-array filtrati.
 * @param list          Elenco globale delle richieste.
 * @param provider      Callback incaricata di erogare la specifica variante di filtro.
 * @param empty_message Testo da stampare qualora nessun record soddisfi i criteri.
 */
static void show_filtered(AssistanceRequestArray *list, FilterProvider provider, const char *empty_message)
{
    const AssistanceRequestArray *filtered;
    AssistanceRequest **array;
    int size;

    /* Invocazione del puntatore a funzione per generare la lista filtrata specifica */
    filtered = provider(list);

    if (filtered == NULL)
    {
        return;
    }

    array = get_assistance_request_array_ptr(filtered);
    size  = get_assistance_request_array_size(filtered);

    if (size <= 0)
    {
        printf("%s\n", empty_message);
    }
    else
    {
        printf("\n--- Risultati filtro (%d) ---\n", size);
        for (int i = 0; i < size; i++)
        {
            print_request(array[i]);
        }
    }

    /* Rilascio "shallow" dell'oggetto array temporaneo per salvaguardare le entita' reali in lista */
    free_assistance_request_array_shallow((AssistanceRequestArray *)filtered);
}

static const AssistanceRequestArray *filter_status_provider(AssistanceRequestArray *list)
{
    return filter_by_status(list, read_status());
}

static const AssistanceRequestArray *filter_priority_provider(AssistanceRequestArray *list)
{
    return filter_by_priority(list, read_priority_level());
}

static const AssistanceRequestArray *filter_name_provider(AssistanceRequestArray *list)
{
    char customer_name[MAX_CUSTOMER_NAME];
    read_string("Nome cliente: ", customer_name, MAX_CUSTOMER_NAME);
    return filter_by_customer_name(list, customer_name);
}

static void filter_by_status_cli(AssistanceRequestArray *list)
{
    show_filtered(list, filter_status_provider, "Nessuna richiesta trovata con lo stato selezionato.");
}

static void filter_by_priority_cli(AssistanceRequestArray *list)
{
    show_filtered(list, filter_priority_provider, "Nessuna richiesta trovata con la priorita' selezionata.");
}

static void filter_by_customer_name_cli(AssistanceRequestArray *list)
{
    show_filtered(list, filter_name_provider, "Nessuna richiesta trovata per il cliente specificato.");
}


/* =========================================================
 *  OPERAZIONI — REPORT
 * ========================================================= */

/**
 * @brief Mostra il report generale: statistiche aggregate su tutte le richieste.
 * @param list Elenco globale delle richieste.
 */
static void general_report_cli(AssistanceRequestArray *list)
{
    general_requests_report(list);
}

/**
 * @brief Mostra il report operativo: richieste raggruppate per stato e dispositivo.
 * @param list Elenco globale delle richieste.
 */
static void operative_report_cli(AssistanceRequestArray *list)
{
    operative_requests_report(list);
}

/**
 * @brief Mostra le statistiche aggregate per tipo di dispositivo.
 */
static void device_stats_cli(AssistanceRequestArray *list)
{
    print_stats_by_device_type(list);
}

/**
 * @brief Mostra le statistiche aggregate per livello di priorità.
 */
static void priority_stats_cli(AssistanceRequestArray *list)
{
    print_stats_by_priority(list);
}