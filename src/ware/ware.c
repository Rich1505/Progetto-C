#include "ware.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<stdbool.h>

static const int max_days_per_month[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * @struct assistance_request
 * @brief Rappresenta una singola richiesta di assistenza tecnica (Ticket).
 * * Contiene tutte le informazioni anagrafiche, tecniche, operative ed economiche
 * relative a un intervento di riparazione su un dispositivo.
 */
struct assistance_request
{
    int request_code;                       /**< Codice identificativo univoco e positivo della richiesta */
    char customer_name[MAX_CUSTOMER_NAME];  /**< Stringa contenente il nome e cognome del cliente */
    DeviceType device_type;                 /**< Tipologia di dispositivo da riparare (es. SMARTPHONE, LAPTOP) */
    char description[MAX_DESCRIPTION];      /**< Testo descrittivo del guasto o del problema riscontrato */
    PriorityLevel priority_level;           /**< Livello di urgenza assegnato all'intervento (es. LOW, HIGH) */
    RequestStatus request_status;           /**< Stato di avanzamento corrente del ticket (es. OPEN, CLOSED) */
    float estimated_cost;                   /**< Preventivo di spesa iniziale calcolato in fase di accettazione */
    float final_cost;                       /**< Costo effettivo consuntivato al termine della lavorazione */
    Date opening_date;                      /**< Data di inserimento della richiesta nel sistema */
};

/**
 * @struct assistance_request_array
 * @brief Contenitore dinamico (Array di puntatori) per la gestione delle richieste.
 * * Implementa una struttura dati ad allocazione dinamica che permette di raggruppare 
 * ed elaborare le richieste di assistenza, ottimizzando l'occupazione in memoria.
 */
struct assistance_request_array
{
    AssistanceRequest **array; /**< Array di puntatori a strutture AssistanceRequest allocati nell'Heap */
    int size;                  /**< Numero di elementi (richieste) attualmente presenti nel contenitore */
    int capacity;              /**< Spazio massimo di memoria allocato prima di una necessaria riallocazione */
};

/**
 * @brief Verifica se un anno è bisestile secondo il calendario gregoriano.
 * @param year Anno da verificare.
 * @return 1 se l'anno è bisestile, 0 altrimenti.
 */
static int is_leap_year(int year);

/**
 * @brief Valida la correttezza formale di una data sul calendario, considerando gli anni bisestili.
 * @param day Giorno.
 * @param month Mese.
 * @param year Anno.
 * @return true se la data esiste ed è valida, false in caso contrario con segnalazione su stderr.
 */
static bool check_date(int day, int month, int year);

static const char *device_to_string(DeviceType type);

static const char *priority_to_string(PriorityLevel level);

static const char *status_to_string(RequestStatus status);

static int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static bool check_date(int day, int month, int year)
{
    if (year < 1900)
    {
        fprintf(stderr, "Inserire un anno valido\n");
        return false;
    }

    if (month < 1 || month > 12)
    {
        fprintf(stderr, "Inserire un mese valido\n");
        return false;
    }

    if (day > max_days_per_month[month - 1] || day < 1)
    {
        fprintf(stderr, "Inserire un giorno valido\n");
        return false;
    }

    if (month == 2)
    {
        if (!is_leap_year(year) && day == 29)
        {
            fprintf(stderr, "Inserire un giorno valido\n");
            return false;
        }
    }

    return true;
}

Date create_date(int day, int month, int year)
{
    if(!check_date(day,month,year))
    {
        return (Date){-1, -1, -1};
    }

    return (Date){day, month, year};
}

AssistanceRequest *create_assistance_request(int request_code, const char *customer_name, DeviceType device_type, const char *description,
                                             PriorityLevel priority_level, RequestStatus request_status, float estimated_cost, float final_cost, Date opening_date)
{
    if (request_code < 0)
    {
        fprintf(stderr, "Inserire un codice valido\n");
        return NULL;
    }

    if(customer_name == NULL)
    {
        fprintf(stderr, "Inserire un nome valido\n");
        return NULL;
    }

    if(description == NULL)
    {
        fprintf(stderr, "Inserire una descrizione valida\n");
        return NULL;
    }

    if (strlen(customer_name) >= MAX_CUSTOMER_NAME)
    {
        fprintf(stderr, "Il nome inserito per il cliente è troppo lungo, verrà tagliato\n");
    }

    if (strlen(description) >= MAX_DESCRIPTION)
    {
        fprintf(stderr, "La descrizione inserita per la richiesta è troppo lunga, verrà tagliata\n");
    }

    if (estimated_cost < 0.0f)
    {
        fprintf(stderr, "Inserire un costo stimato valido\n");
        return NULL;
    }

    if (final_cost < 0.0f)
    {
        fprintf(stderr, "Inserire un costo finale valido\n");
        return NULL;
    }

    if(!check_date(opening_date.day, opening_date.month, opening_date.year))
    {
        fprintf(stderr, "Inserire una data valida\n");
        return NULL;
    }

    AssistanceRequest *assistance_request = malloc(sizeof(AssistanceRequest));
    if (assistance_request == NULL)
    {
        fprintf(stderr, "Errore allocazione memoria\n");
        return NULL;
    }

    assistance_request->request_code = request_code;
    strncpy(assistance_request->customer_name, customer_name, MAX_CUSTOMER_NAME - 1);
    assistance_request->customer_name[MAX_CUSTOMER_NAME - 1] = '\0';

    assistance_request->device_type = device_type;

    strncpy(assistance_request->description, description, MAX_DESCRIPTION - 1);
    assistance_request->description[MAX_DESCRIPTION - 1] = '\0';

    assistance_request->priority_level = priority_level;
    assistance_request->request_status = request_status;
    assistance_request->estimated_cost = estimated_cost;
    assistance_request->final_cost = final_cost;
    assistance_request->opening_date = opening_date;

    return assistance_request;
}

int insert_assistance_request(AssistanceRequestArray *assistance_request_array, AssistanceRequest *assistance_request)
{
    if(assistance_request_array == NULL || assistance_request == NULL)
    {
        fprintf(stderr, "Errore insert_assistance_request: puntatore NULL\n");
        return -1;
    }

    if(assistance_request_array->size == assistance_request_array->capacity)
    {
        int new_capacity = (assistance_request_array->capacity == 0) ? 4 : assistance_request_array->capacity * 2;
        AssistanceRequest **tmp = realloc(assistance_request_array->array, new_capacity * sizeof(AssistanceRequest *));
        if(tmp == NULL)
        {
            fprintf(stderr, "Errore insert_assistance_request: Allocazione memoria non riuscita\n");
            return -1;
        }

        assistance_request_array->array = tmp;
        assistance_request_array->capacity = new_capacity;
    }

    assistance_request_array->array[assistance_request_array->size] = assistance_request;
    assistance_request_array->size = assistance_request_array->size + 1;
    return 0;
}

AssistanceRequest *copy_assistance_request(const AssistanceRequest *src)
{
    if (src == NULL)
    {
        fprintf(stderr, "Errore copy_assistance_request: puntatore sorgente NULL\n");
        return NULL;
    }

    return create_assistance_request(
        get_request_code(src),
        get_customer_name(src),
        get_device_type(src),
        get_description(src),
        get_priority_level(src),
        get_request_status(src),
        get_estimated_cost(src),
        get_final_cost(src),
        get_opening_date(src)
    );
}

AssistanceRequestArray *create_assistance_request_array(AssistanceRequest **array, int size, int capacity)
{
    if(array == NULL)
    {
        fprintf(stderr, "Errore create_assistance_request_array: puntatore array NULL\n");
        return NULL;
    }

    if(capacity < size)
    {
        fprintf(stderr, "Errore create_assistance_request_array: size non può essere maggiore di capacity\n");
        return NULL;
    }

    AssistanceRequestArray *assistance_request_array = malloc(sizeof(AssistanceRequestArray));
    if(assistance_request_array == NULL)
    {
        fprintf(stderr, "Errore create_assistance_request_array: Errore allocazione memoria\n");
        return NULL;
    }
    assistance_request_array->array = array;
    assistance_request_array->size = size;
    assistance_request_array->capacity = capacity;

    return assistance_request_array;
}

AssistanceRequestArray *create_empty_request_array()
{
    AssistanceRequest **arr = malloc(sizeof (AssistanceRequest *) * 2);
    if(arr == NULL)
    {
        return NULL;
    }
    arr[0] = NULL;
    arr[1] = NULL;

    AssistanceRequestArray *assistance_request_array = create_assistance_request_array(arr, 0, 2);
    if(assistance_request_array == NULL)
    {
        free(arr);
        return NULL;
    }

    return assistance_request_array;
}

AssistanceRequestArray *copy_assistance_request_array(const AssistanceRequestArray *src)
{
    if(src == NULL)
    {
        fprintf(stderr, "Errore copy_assistance_request_array: puntatore NULL\n");
        return NULL;
    }

    int capacity = get_assistance_request_array_capacity(src);
    int size = get_assistance_request_array_size(src);
    AssistanceRequest **original_arr = get_assistance_request_array_ptr(src);

    AssistanceRequest **new_arr = malloc(capacity * sizeof(AssistanceRequest *));
    if(new_arr == NULL)
    {
        fprintf(stderr, "Errore copy_assistance_request_array: Allocazione memoria non riuscita\n");
        return NULL;
    }

    for(int i = 0;i < size;i++)
    {
        new_arr[i] = copy_assistance_request(original_arr[i]);
        if (new_arr[i] == NULL)
        {
            fprintf(stderr, "Errore copy_assistance_request_array: copia elemento %d fallita\n", i);
            // Libera solo gli elementi già copiati con successo (0..i-1)
            for (int j = 0; j < i; j++)
            {
                free_assistance_request(new_arr[j]);
            }
            free(new_arr);
            return NULL;
        }
    }

    AssistanceRequestArray *dest = malloc(sizeof(AssistanceRequestArray));
    if(dest == NULL)
    {
        fprintf(stderr, "Errore copy_assistance_request_array: Allocazione memoria non riuscita\n");
        for (int i = 0; i < size; i++) free_assistance_request(new_arr[i]);
        free(new_arr);
        return NULL;
    }

    dest->array = new_arr;
    dest->size = size;
    dest->capacity = capacity;
    
    return dest;
}

void free_assistance_request(AssistanceRequest *request)
{
    if (request == NULL)
    {
        return;
    }

    free(request);
}

void free_assistance_request_array(AssistanceRequestArray *arr)
{
    if (arr == NULL)
    {
        return;
    }

    if (arr->array != NULL)
    {
        for (int i = 0; i < arr->size; i++)
        {
            free_assistance_request(arr->array[i]);
        }

        free(arr->array);
    }

    free(arr);
}

void free_assistance_request_array_shallow(AssistanceRequestArray *arr)
{
    if (arr == NULL)
    {
        return;
    }

    if (arr->array != NULL)
    {
        free(arr->array);
    }

    free(arr);
}

// GETTER

int get_request_code(const AssistanceRequest *request)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore get_request_code: puntatore richiesta NULL\n");
        return -1;
    }
    return request->request_code;
}

const char *get_customer_name(const AssistanceRequest *request)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore get_customer_name: puntatore richiesta NULL\n");
        return NULL;
    }
    return request->customer_name;
}

DeviceType get_device_type(const AssistanceRequest *request)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore get_device_type: puntatore richiesta NULL\n");
        return DEVICE_ERROR; 
    }
    return request->device_type;
}

const char *get_description(const AssistanceRequest *request)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore get_description: puntatore richiesta NULL\n");
        return NULL;
    }
    return request->description;
}

PriorityLevel get_priority_level(const AssistanceRequest *request)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore get_priority_level: puntatore richiesta NULL\n");
        return PRIORITY_ERROR;
    }
    return request->priority_level;
}

RequestStatus get_request_status(const AssistanceRequest *request)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore get_request_status: puntatore richiesta NULL\n");
        return STATUS_ERROR;
    }
    return request->request_status;
}

float get_estimated_cost(const AssistanceRequest *request)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore get_estimated_cost: puntatore richiesta NULL\n");
        return -1.0f;
    }
    return request->estimated_cost;
}

float get_final_cost(const AssistanceRequest *request)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore get_final_cost: puntatore richiesta NULL\n");
        return -1.0f;
    }
    return request->final_cost;
}

Date get_opening_date(const AssistanceRequest *request)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore get_opening_date: puntatore richiesta NULL\n");
        return (Date){-1, -1, -1};
    }
    return request->opening_date;
}

AssistanceRequest **get_assistance_request_array_ptr(const AssistanceRequestArray *assistance_request_array)
{
    if(assistance_request_array == NULL)
    {
        fprintf(stderr, "Errore get_assistance_request_array_ptr: Puntatore array NULL\n");
        return NULL;
    }

    return assistance_request_array->array;
}

int get_assistance_request_array_size(const AssistanceRequestArray *assistance_request_array)
{
    if(assistance_request_array == NULL)
    {
        fprintf(stderr, "Errore get_assistance_request_array_size: Puntatore array NULL\n");
        return -1;
    }

    return assistance_request_array->size;
}

int get_assistance_request_array_capacity(const AssistanceRequestArray *assistance_request_array)
{
    if(assistance_request_array == NULL)
    {
        fprintf(stderr, "Errore get_assistance_request_array_capacity: Puntatore array NULL\n");
        return -1;
    }

    return assistance_request_array->capacity;
}

// SETTER

int set_description(AssistanceRequest *request, const char *description)
{
    if (request == NULL || description == NULL)
    {
        fprintf(stderr, "Errore set_description: uno o più parametri NULL\n");
        return -1;
    }
    if (strlen(description) >= MAX_DESCRIPTION)
    {
        fprintf(stderr, "Descrizione troppo lunga, verrà tagliata\n");
    }

    strncpy(request->description, description, MAX_DESCRIPTION - 1);
    request->description[MAX_DESCRIPTION - 1] = '\0';

    return 0;
}

int set_request_status_opened(AssistanceRequest *request)
{
    if(request == NULL)
    {
        fprintf(stderr, "Errore set_request_status: puntatore richiesta NULL\n");
        return -1;
    }

    request->request_status = STATUS_OPEN;
    return 0;
}

int set_request_status_in_progress(AssistanceRequest *request)
{
    if(request == NULL)
    {
        fprintf(stderr, "Errore set_request_status: puntatore richiesta NULL\n");
        return -1;
    }

    request->request_status = STATUS_IN_PROGRESS;
    return 0;
}

int set_request_status_closed(AssistanceRequest *request)
{
    if(request == NULL)
    {
        fprintf(stderr, "Errore set_request_status: puntatore richiesta NULL\n");
        return -1;
    }

    request->request_status = STATUS_CLOSED;
    return 0;
}

int set_request_status_cancelled(AssistanceRequest *request)
{
    if(request == NULL)
    {
        fprintf(stderr, "Errore set_request_status: puntatore richiesta NULL\n");
        return -1;
    }

    request->request_status = STATUS_CANCELLED;
    return 0;
}

int set_estimated_cost(AssistanceRequest *request, float estimated_cost)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore set_estimated_cost: puntatore richiesta NULL\n");
        return -1;
    }
    if (estimated_cost < 0.0f)
    {
        fprintf(stderr, "Costo stimato non valido\n");
        return -1;
    }

    request->estimated_cost = estimated_cost;
    return 0;
}

int set_final_cost(AssistanceRequest *request, float final_cost)
{
    if (request == NULL)
    {
        fprintf(stderr, "Errore set_final_cost: puntatore richiesta NULL\n");
        return -1;
    }
    if (final_cost < 0.0f)
    {
        fprintf(stderr, "Costo finale non valido\n");
        return -1;
    }

    if(request->request_status != STATUS_CLOSED)
    {
        fprintf(stderr, "Non puoi modificare il costo finale di una richiesta non completata!\n");
        return -1;
    }

    request->final_cost = final_cost;
    return 0;
}

static const char *device_to_string(DeviceType type) 
{
    switch (type) {
        case DEVICE_SMARTPHONE: return "SMARTPHONE";
        case DEVICE_TABLET:     return "TABLET";
        case DEVICE_LAPTOP:     return "LAPTOP";
        case DEVICE_DESKTOP:    return "DESKTOP";
        case DEVICE_PRINTER:    return "PRINTER";
        default:                return "ERRORE";
    }
}

static const char *priority_to_string(PriorityLevel level) 
{
    switch (level) {
        case PRIORITY_LOW:    return "LOW";
        case PRIORITY_MEDIUM: return "MEDIUM";
        case PRIORITY_HIGH:   return "HIGH";
        default:              return "ERRORE";
    }
}

static const char *status_to_string(RequestStatus status) 
{
    switch (status) {
        case STATUS_OPEN:        return "OPEN";
        case STATUS_IN_PROGRESS: return "IN_PROGRESS";
        case STATUS_CLOSED:      return "CLOSED";
        case STATUS_CANCELLED:    return "CANCELED";
        default:                 return "ERRORE";
    }
}

void print_request(const AssistanceRequest *request)
{
    Date data;

    if (request == NULL) {
        fprintf(stderr, "Errore print_request: puntatore richiesta NULL\n");
        return;
    }

    data = get_opening_date(request);

    printf("--------------------------------------------\n");
    printf("Codice: %d\n", get_request_code(request));
    printf("Cliente: %s\n", get_customer_name(request));
    printf("Dispositivo: %s\n", device_to_string(get_device_type(request)));
    printf("Descrizione: %s\n", get_description(request));
    printf("Priorita: %s\n", priority_to_string(get_priority_level(request)));
    printf("Stato: %s\n", status_to_string(get_request_status(request)));
    printf("Costo stimato: %.2f\n", get_estimated_cost(request));
    printf("Costo finale: %.2f\n", get_final_cost(request));
    printf("Data apertura: %02d/%02d/%04d\n",data.day, data.month, data.year);
    printf("--------------------------------------------\n");
}