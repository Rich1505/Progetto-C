/**
 * @file ware.h
 * @brief Gestione delle richieste di assistenza tecnica per dispositivi informatici.
 *
 * Questo modulo definisce le strutture dati e le funzioni necessarie per gestire 
 * un sistema di ticket di assistenza, inclusi i dettagli del cliente, del dispositivo, 
 * i costi, la priorità e lo stato di avanzamento della richiesta.
 */

#ifndef WARE_H
#define WARE_H

#define MAX_CUSTOMER_NAME 50
#define MAX_DESCRIPTION 200
#define WARE_INVALID_REQUEST_CODE -1

/**
 * @typedef AssistanceRequest
 * @brief Tipo opaco che rappresenta una singola richiesta di assistenza tecnica.
 *
 * Questo tipo incapsula l'intera anagrafica e lo stato operativo di un ticket.
 * Essendo un tipo opaco, i suoi campi interni non sono accessibili direttamente;
 * per interagire con l'oggetto è necessario utilizzare le funzioni di interfaccia
 * (getter e setter) fornite dal modulo.
 */
typedef struct assistance_request AssistanceRequest;

/**
 * @typedef AssistanceRequestArray
 * @brief Tipo opaco che rappresenta il contenitore dinamico delle richieste di assistenza.
 *
 * Questa struttura funge da "database" in memoria, raggruppando tutti i ticket attivi.
 * Trattandosi di un tipo opaco, la gestione della memoria, il ridimensionamento
 * e l'accesso ai dati interni sono completamente incapsulati e gestiti mediante
 * le funzioni dedicate del modulo (es. inserimento, rimozione, ordinamento).
 */
typedef struct assistance_request_array AssistanceRequestArray;

/**
 * @typedef Date
 * @struct date
 * @brief Rappresenta una data nel formato standard giorno/mese/anno.
 * * Utilizzata per tracciare la cronologia degli eventi del sistema, come
 * il momento dell'apertura o della presa in carico di un ticket.
 */
typedef struct date
{
    int day;   /**< Giorno del mese (valore compreso tra 1 e 31) */
    int month; /**< Mese dell'anno (valore compreso tra 1 e 12) */
    int year;  /**< Anno a quattro cifre (es. 2026) */
} Date;

typedef enum
{
    DEVICE_ERROR = -1,
    DEVICE_SMARTPHONE = 0,
    DEVICE_TABLET,
    DEVICE_LAPTOP,
    DEVICE_DESKTOP,
    DEVICE_PRINTER
} DeviceType;

typedef enum
{
    PRIORITY_ERROR = -1,
    PRIORITY_LOW = 0,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH
} PriorityLevel;

typedef enum
{
    STATUS_ERROR = -1,
    STATUS_OPEN = 0,
    STATUS_IN_PROGRESS,
    STATUS_CLOSED,
    STATUS_CANCELED
} RequestStatus;

/**
 * @brief Inizializza e convalida una struttura data.
 * @param day Giorno del mese.
 * @param month Mese dell'anno.
 * @param year Anno.
 * @return La struttura Date compilata se valida, altrimenti una struttura con valori fittizi (-1).
 */
Date create_date(int day, int month, int year);

/**
 * @brief Alloca e inizializza una nuova richiesta di assistenza.
 * @param request_code Codice identificativo univoco della richiesta.
 * @param customer_name Nome del cliente richiedente.
 * @param device_type Tipologia di dispositivo guasto.
 * @param description Descrizione del problema riscontrato.
 * @param priority_level Livello di urgenza della richiesta.
 * @param request_status Stato iniziale del ticket.
 * @param estimated_cost Preventivo del costo di riparazione.
 * @param final_cost Costo effettivo a consuntivo.
 * @param opening_date Data di apertura della richiesta.
 * @return Puntatore alla nuova richiesta allocata, o NULL in caso di parametri non validi o errore di memoria.
 */
AssistanceRequest *create_assistance_request(int request_code, const char *customer_name, DeviceType device_type, const char *description,
                                             PriorityLevel priority_level, RequestStatus request_status, float estimated_cost, float final_cost, Date opening_date);

/**
 * @brief Alloca un contenitore dinamico (array) per le richieste di assistenza.
 * @param array Puntatore al blocco di memoria contenente i puntatori alle richieste.
 * @param size Numero attuale di elementi inseriti.
 * @param capacity Capacità massima allocata per il blocco di memoria.
 * @return Puntatore alla struttura array allocata, o NULL in caso di parametri non conformi.
 */
AssistanceRequestArray *create_assistance_request_array(AssistanceRequest **array, int size, int capacity);

/**
 * @brief Inserisce una richiesta nell'array dinamico, gestendo il ridimensionamento automatico.
 * @param assistance_request_array Puntatore all'array contenitore.
 * @param assistance_request Puntatore alla richiesta da inserire.
 * @return 0 in caso di successo, -1 in caso di errore di allocazione o parametri NULL.
 */
int insert_assistance_request(AssistanceRequestArray *assistance_request_array, AssistanceRequest *assistance_request);

/**
 * @brief Crea una copia profonda (deep copy) di un intero array di richieste.
 * @param src Puntatore all'array sorgente da duplicare.
 * @return Puntatore al nuovo array duplicato con elementi indipendenti, o NULL in caso di errore.
 */
AssistanceRequestArray *copy_assistance_request_array(const AssistanceRequestArray *src);

/**
 * @brief Crea una copia profonda (deep copy) di una singola richiesta di assistenza.
 * @param src Puntatore alla richiesta sorgente da duplicare.
 * @return Puntatore alla nuova richiesta duplicata in memoria, o NULL se la sorgente è non valida.
 */
AssistanceRequest *copy_assistance_request(const AssistanceRequest *src);

/**
 * @brief Libera la memoria occupata da una singola richiesta di assistenza.
 * @param request Puntatore alla richiesta da deallocare.
 */
void free_assistance_request(AssistanceRequest *request);

/**
 * @brief Dealloca l'intero array e distrugge tutte le richieste in esso contenute (rimozione profonda).
 * @param arr Puntatore all'array da liberare.
 */
void free_assistance_request_array(AssistanceRequestArray *arr);

/**
 * @brief Dealloca la struttura dell'array senza liberare le singole richieste in esso puntate (rimozione superficiale).
 * @param arr Puntatore all'array da liberare.
 */
void free_assistance_request_array_shallow(AssistanceRequestArray *arr);

/**
 * @brief Stampa a terminale i dettagli completi di una singola richiesta di assistenza.
 * * Formatta in modo strutturato e leggibile tutti i campi informativi dell'entità 
 * passata come argomento (codice, cliente, dispositivo, descrizione, priorità, stato, costi e data).
 * * @param request Puntatore costante all'istanza della richiesta da stampare.
 */
void print_request(const AssistanceRequest *request);

// GETTER
int get_request_code(const AssistanceRequest *request);

const char *get_customer_name(const AssistanceRequest *request);

DeviceType get_device_type(const AssistanceRequest *request);

const char *get_description(const AssistanceRequest *request);

PriorityLevel get_priority_level(const AssistanceRequest *request);

RequestStatus get_request_status(const AssistanceRequest *request);

float get_estimated_cost(const AssistanceRequest *request);

float get_final_cost(const AssistanceRequest *request);

Date get_opening_date(const AssistanceRequest *request);

AssistanceRequest **get_assistance_request_array_ptr(const AssistanceRequestArray *assistance_request_array);

int get_assistance_request_array_size(const AssistanceRequestArray *assistance_request_array);

int get_assistance_request_array_capacity(const AssistanceRequestArray *assistance_request_array);

// SETTER
int set_description(AssistanceRequest *request, const char *description);

int set_request_status_opened(AssistanceRequest *request);

int set_request_status_in_progress(AssistanceRequest *request);

int set_request_status_closed(AssistanceRequest *request);

int set_request_status_canceled(AssistanceRequest *request);

int set_estimated_cost(AssistanceRequest *request, float estimated_cost);

/**
 * @brief Imposta il costo finale, consentito solo se il ticket è nello stato chiuso.
 * @param request Puntatore alla richiesta di assistenza.
 * @param final_cost Importo consuntivo della riparazione.
 * @return 0 in caso di successo, -1 se il ticket non è chiuso o i parametri sono errati.
 */
int set_final_cost(AssistanceRequest *request, float final_cost);

#endif // WARE_H