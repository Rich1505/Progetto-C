#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cli.h"
#include "file_manager.h"
#include "sort.h"
#include "search.h"
#include "filters.h"
#include "reports.h"

static void print_menu(void);
static int read_int(const char *prompt);
static float read_float(const char *prompt);
static void read_string(const char *prompt, char *buffer, int size);
static int has_digit(const char *string);
static AssistanceRequest *read_existing_request(AssistanceRequestArray *list);

static void show_error_message(const char *message);
static void show_success_message(const char *message);

static DeviceType read_device_type(void);
static PriorityLevel read_priority_level(void);
static RequestStatus read_status(void);

static AssistanceRequest *find_request_by_code(AssistanceRequestArray *list, int code);

static void show_all_requests(AssistanceRequestArray *list);
static void insert_request_cli(AssistanceRequestArray *list);
static void update_status_cli(AssistanceRequestArray *list);
static void update_estimated_cost_cli(AssistanceRequestArray *list);
static void update_final_cost_cli(AssistanceRequestArray *list);
static void search_request_cli(AssistanceRequestArray *list);
static void update_description_cli(AssistanceRequestArray *list);
static void filter_by_status_cli(AssistanceRequestArray *list);
static void filter_by_priority_cli(AssistanceRequestArray *list);
static void filter_by_customer_name_cli(AssistanceRequestArray *list);
static void general_report_cli(AssistanceRequestArray *list);
static void operative_report_cli(AssistanceRequestArray *list);

void run_main_menu(AssistanceRequestArray *list) 
{
    int choice;

       if (list == NULL) {
            show_error_message("Lista richieste non valida.");
            return;
        }

    do
    {
        print_menu();
        choice = read_int("Seleziona un'opzione: ");

        switch (choice)
        {
            case 1:
                show_all_requests(list);
                break;
            case 2:
                insert_request_cli(list);
                break;
            case 3 :
                search_request_cli(list);
                break;
            case 4:
                update_status_cli(list);
                break;
            case 5:
                update_estimated_cost_cli(list);
                break;
            case 6:
                update_final_cost_cli(list);
                break;
            case 7:
                update_description_cli(list);
                break;
            case 8:
                sort_by_customer_name(list);
                show_success_message("Ordinamento per nome completato.");
                break;
            case 9:
                sort_by_estimated_cost(list);
                show_success_message("Ordinamento per costo stimato completato.");
                break;
            case 10: 
                if (write_in_memory(list) == 0)
                show_success_message("Salvataggio completato.");
                else
                show_error_message("Errore durante il salvataggio.");
                break;
            case 11:
                filter_by_status_cli(list);
                break;
            case 12:
                filter_by_priority_cli(list);
                break;
            case 13:
                filter_by_customer_name_cli(list);
                break;
            case 14:
                general_report_cli(list);
                break;
            case 15:
                operative_report_cli(list);
                break;
            case 0:
                printf("Uscita in corso...\n");
                break;
            default:
                show_error_message("Opzione non valida. Inserisci un numero valido del menu.");
        }
    } while (choice != 0);

}

void show_error_message(const char *message)
{
    printf("[ERRORE] %s\n", message);
}

void show_success_message(const char *message)
{
    printf("[OK] %s\n", message);
}

void print_menu(void)
{
    printf("\n====== MENU PRINCIPALE ======\n");
    printf("1. Visualizza tutte le richieste\n");
    printf("2. Inserisci nuova richiesta\n");
    printf("3. Cerca richiesta per codice\n");
    printf("4. Aggiorna stato richiesta\n");
    printf("5. Aggiorna costo stimato\n");
    printf("6. Aggiorna costo finale\n");
    printf("7. Aggiorna descrizione richiesta\n");
    printf("8. Ordina per nome cliente\n");
    printf("9. Ordina per costo stimato\n");
    printf("10. Salva su file\n");
    printf("11. Filtra per stato\n");
    printf("12. Filtra per priorita'\n");
    printf("13. Filtra per nome cliente\n");
    printf("14. Report generale\n");
    printf("15. Report operativo\n");
    printf("0. Esci\n");
    printf("\n=============================\n");
}

static int read_int(const char *prompt)
{
    int value;
    char row[100];
    char extra;

    while (1) {
        printf("%s", prompt);
        fgets(row, sizeof(row), stdin);

        if (sscanf(row, "%d %c", &value, &extra) == 1) {
            return value;
        }

        show_error_message("Inserisci un numero intero valido.");
    }
}
    
static float read_float(const char *prompt)
{
    float value;
    char row[100];
    char extra;

    while (1) {
        printf("%s", prompt);
        fgets(row, sizeof(row), stdin);

        if (sscanf(row, "%f %c", &value, &extra) == 1) {
            return value;
        }

        show_error_message("Inserisci un numero valido.");
    }
}
    
static void read_string(const char *prompt, char *buffer, int size)
{
    while (1) {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) > 0) {
            return;
        }

        show_error_message("Il campo non puo' essere vuoto.");
    }
}

static int has_digit(const char *string)
{
    int i = 0;

    while (string[i] != '\0') {
        if (isdigit((unsigned char)string[i])) {
            return 1;
        }
        i++;
    }

    return 0;
}

static DeviceType read_device_type(void) 
{
    int choice;

    while (1)
    {
        printf("Tipo dispositivo:\n");
        printf("1. Smartphone\n");
        printf("2. Tablet\n");
        printf("3. Laptop\n");
        printf("4. Desktop\n");
        printf("5. Printer\n");

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

static PriorityLevel read_priority_level(void) 
{
    int choice;
    while (1)
    {
        printf("Priorita:\n");
        printf("1. Low\n");
        printf("2. Medium\n");
        printf("3. High\n");

        choice = read_int("Scelta: ");

        switch (choice)
        {
        case 1: return PRIORITY_LOW;
        case 2: return PRIORITY_MEDIUM;
        case 3: return PRIORITY_HIGH;
        default: show_error_message("Priorita non valida. Reinserisci la scelta.");
        }
    }
}

static RequestStatus read_status(void) 
{
    int choice;
    while (1)
    {
        printf("Stato richiesta:\n");
        printf("1. Open\n");
        printf("2. In progress\n");
        printf("3. Closed\n");
        printf("4. Canceled\n");

        choice = read_int("Scelta: ");

        switch (choice)
        {
        case 1: return STATUS_OPEN;
        case 2: return STATUS_IN_PROGRESS;
        case 3: return STATUS_CLOSED;
        case 4: return STATUS_CANCELED;
        default: show_error_message("Stato non valido. Reinserisci la scelta.");
        }
    }
}

static AssistanceRequest *find_request_by_code(AssistanceRequestArray *list, int code)
{
    AssistanceRequest **array;
    int size;

    if (list == NULL)
        return NULL;

    array = get_assistance_request_array_ptr(list);
    size = get_assistance_request_array_size(list);

    if (array == NULL || size < 0)
        return NULL;

    for (int i = 0; i < size; i++) {
        if (get_request_code(array[i]) == code) {
            return array[i];
        }
    }

    return NULL;
}

static AssistanceRequest *read_existing_request(AssistanceRequestArray *list) 
{
    int code;
    AssistanceRequest *request;

    while (1) {
        code = read_int("Codice richiesta: ");
        
        if (code == -1) {
            return NULL;
        }

        request = search_by_request_code(list, code);

        if (request != NULL) {
            return request;
        }

        show_error_message("Richiesta non trovata. Inserisci un codice valido.");
    }
}

static void show_all_requests(AssistanceRequestArray *list)
{
    AssistanceRequest **array;
    int size;

    array = get_assistance_request_array_ptr(list);
    size = get_assistance_request_array_size(list);

    if (array == NULL || size <= 0) {
        printf("Nessuna richiesta presente.\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        print_request(array[i]);
    }
}

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
        }

        AssistanceRequest *request_by_code = search_by_request_code(list, code);
        if(request_by_code != NULL)
        {
            show_error_message("Il codice inserito e' gia' utilizzato da un'altra richiesta");
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
        int day = read_int("Giorno: ");
        int month = read_int("Mese: ");
        int year = read_int("Anno: ");

        date = create_date(day, month, year);

        if (date.day == -1)
        {
            show_error_message("Data non valida.");
        }
    } while (date.day == -1);

    new_request = create_assistance_request(
        code,
        customer_name,
        type,
        description,
        priority,
        STATUS_OPEN,
        estimated_cost,
        0.0f,
        date
    );

    if (new_request == NULL) {
        show_error_message("Creazione richiesta fallita.");
        return;
    }

    if (insert_assistance_request(list, new_request) == 0)
        show_success_message("Richiesta inserita correttamente.");
    else {
        free_assistance_request(new_request);
        show_error_message("Inserimento fallito.");
    }
}

static void search_request_cli(AssistanceRequestArray *list)
{
    int code;
    AssistanceRequest *request;

    code = read_int("Codice richiesta da cercare: ");

    request = search_by_request_code(list, code);

    if (request == NULL)
    {
        show_error_message("Richiesta non trovata.");
        return;
    }

    print_request(request);
}

static void update_status_cli(AssistanceRequestArray *list)
{
    RequestStatus newStatus;
    AssistanceRequest *request;
    int result = -1;

    request = read_existing_request(list);

    if (request == NULL)
    {
        show_error_message("Operazione annullata.");
        return;
    }

    newStatus = read_status();

    switch (newStatus)
    {
    case STATUS_OPEN:
        result = set_request_status_opened(request);
        break;
    case STATUS_IN_PROGRESS:
        result = set_request_status_in_progress(request);
        break;
    case STATUS_CLOSED:
        result = set_request_status_closed(request);
        break;
    case STATUS_CANCELED:
        result = set_request_status_canceled(request);
        break;
    default:
        result = -1;
    }

    if (result == 0)
        show_success_message("Stato aggiornato.");
    else
        show_error_message("Aggiornamento stato fallito.");
}

static void update_estimated_cost_cli(AssistanceRequestArray *list)
{
    float cost;
    AssistanceRequest *request;

    request = read_existing_request(list);

    if (request == NULL) {
        show_error_message("Operazione annullata.");
        return;
    }
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

static void update_final_cost_cli(AssistanceRequestArray *list)
{
    float cost;
    AssistanceRequest *request;

    request = read_existing_request(list);

    if (request == NULL) {
        show_error_message("Richiesta non trovata.");
        return;
    }

    cost = read_float("Nuovo costo finale: ");

    if (set_final_cost(request, cost) == 0)
        show_success_message("Costo finale aggiornato.");
}

static void update_description_cli(AssistanceRequestArray *list)
{
    AssistanceRequest *request;
    char description[MAX_DESCRIPTION];

    request = read_existing_request(list);

    if (request == NULL)
    {
        show_error_message("Operazione annullata.");
        return;
    }

    read_string("Nuova descrizione: ",description,MAX_DESCRIPTION);

    if (set_description(request, description) == 0)
        show_success_message("Descrizione aggiornata.");
    else
        show_error_message("Aggiornamento descrizione fallito.");
}

/* Definiamo un tipo puntatore a funzione per astrarre e generalizzare la sorgente dei dati filtrati */
typedef const AssistanceRequestArray *(*FilterProvider)(AssistanceRequestArray *list);

/**
 * @brief Incapsula e centralizza la visualizzazione e la deallocazione dei sotto-array filtrati.
 * @param list Elenco globale delle richieste.
 * @param provider Call-back incaricata di erogare la specifica variante di filtro.
 * @param empty_message Testo da stampare sul terminale qualora nessun record soddisfi i criteri.
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
    size = get_assistance_request_array_size(filtered);

    if (size <= 0)
    {
        printf("%s\n", empty_message);
    }
    else
    {
        for (int i = 0; i < size; i++)
        {
            print_request(array[i]);
        }
    }

    /* Rilascio "shallow" (superficiale) dell'oggetto array temporaneo per salvaguardare le entità reali presenti in lista */
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