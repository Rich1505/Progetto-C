#include "reports.h"
#include <stdio.h>
#include "sort.h"
#include <stdlib.h>

#define GENERAL_REPORT_FILENAME "general_report.txt"
#define OPERATIVE_REPORT_FILENAME "operative_report.txt"

static void print_request_list_to_file(const AssistanceRequestArray *list, FILE *fp);
static void print_request_to_file(const AssistanceRequest *request, FILE *fp);

void general_requests_report(const AssistanceRequestArray *list)
{
    if (list == NULL)
    {
        fprintf(stderr, "Errore general_requests_report: puntatore NULL\n");
        return;
    }

    FILE *fp = fopen(GENERAL_REPORT_FILENAME, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Errore general_requests_report: impossibile aprire il file '%s'\n", GENERAL_REPORT_FILENAME);
        return;
    }

    int size = get_assistance_request_array_size(list);
    AssistanceRequest **arr = get_assistance_request_array_ptr(list);

    int open_status_requests_number = 0;
    int in_progress_status_requests_number = 0;
    int closed_status_requests_number = 0;
    int cancelled_status_requests_number = 0;

    int low_priority_requests_number = 0;
    int medium_priority_requests_number = 0;
    int high_priority_requests_number = 0;

    float estimated_cost_sum = 0.0f;
    float final_cost_sum = 0.0f;

    /* Singolo ciclo di scansione lineare per accumulare contatori e statistiche finanziarie */
    for (int i = 0; i < size; i++)
    {
        switch (get_request_status(arr[i]))
        {
        case STATUS_OPEN:
            open_status_requests_number++;
            break;
        case STATUS_IN_PROGRESS:
            in_progress_status_requests_number++;
            break;
        case STATUS_CLOSED:
            closed_status_requests_number++;
            final_cost_sum += get_final_cost(arr[i]);
            break;
        case STATUS_CANCELLED:
            cancelled_status_requests_number++;
            break;
        default:
            break;
        }

        switch (get_priority_level(arr[i]))
        {
        case PRIORITY_HIGH:
            high_priority_requests_number++;
            break;
        case PRIORITY_MEDIUM:
            medium_priority_requests_number++;
            break;
        case PRIORITY_LOW:
            low_priority_requests_number++;
            break;
        default:
            break;
        }

        estimated_cost_sum += get_estimated_cost(arr[i]);
    }

    float estimated_cost_average = -1.0f;
    float final_cost_average = -1.0f;

    if (size > 0)
    {
        estimated_cost_average = estimated_cost_sum / size;
    }

    if (closed_status_requests_number > 0)
    {
        final_cost_average = final_cost_sum / closed_status_requests_number;
    }

    AssistanceRequestArray *sorted_array = copy_assistance_request_array(list);
    if (sorted_array == NULL)
    {
        fprintf(stderr, "Errore general_requests_report: copia array fallita\n");
        fclose(fp);
        return;
    }
    sort_by_estimated_cost(sorted_array);

    fprintf(fp, "==================================================\n");
    fprintf(fp, "            REPORT GENERALE RICHIESTE             \n");
    fprintf(fp, "==================================================\n");

    fprintf(fp, "i. Numero totale di richieste: %d\n\n", size);

    fprintf(fp, "ii. Richieste per STATO:\n");
    fprintf(fp, "    - Aperte (OPEN):              %d\n", open_status_requests_number);
    fprintf(fp, "    - In Corso (IN PROGRESS):     %d\n", in_progress_status_requests_number);
    fprintf(fp, "    - Chiuse (CLOSED):            %d\n", closed_status_requests_number);
    fprintf(fp, "    - Cancellate (CANCELED):      %d\n\n", cancelled_status_requests_number);

    fprintf(fp, "iii. Richieste per PRIORITA':\n");
    fprintf(fp, "     - Alta (HIGH):    %d\n", high_priority_requests_number);
    fprintf(fp, "     - Media (MEDIUM): %d\n", medium_priority_requests_number);
    fprintf(fp, "     - Bassa (LOW):    %d\n\n", low_priority_requests_number);

    fprintf(fp, "iv. Costo medio stimato: ");
    if (estimated_cost_average >= 0)
        fprintf(fp, "%.2f\n", estimated_cost_average);
    else
        fprintf(fp, "N/D (Nessuna richiesta presente)\n");

    fprintf(fp, "v. Costo medio finale (Richieste Chiuse): ");
    if (final_cost_average >= 0)
        fprintf(fp, "%.2f\n", final_cost_average);
    else
        fprintf(fp, "N/D (Nessuna richiesta completata)\n");

    fprintf(fp, "\nvi. Elenco richieste (ordinate per costo stimato):\n");

    AssistanceRequest **sorted_array_ptr = get_assistance_request_array_ptr(sorted_array);
    for (int i = 0; i < size; i++)
    {
        print_request_to_file(sorted_array_ptr[i], fp);
    }

    fprintf(fp, "==================================================\n");

    free_assistance_request_array(sorted_array);
    fclose(fp);

    printf("[OK] Report generale salvato in '%s'\n", GENERAL_REPORT_FILENAME);
}

void operative_requests_report(const AssistanceRequestArray *list)
{
    if (list == NULL)
    {
        fprintf(stderr, "Errore operative_requests_report: puntatore NULL\n");
        return;
    }

    FILE *fp = fopen(OPERATIVE_REPORT_FILENAME, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Errore operative_requests_report: impossibile aprire il file '%s'\n", OPERATIVE_REPORT_FILENAME);
        return;
    }

    int size = get_assistance_request_array_size(list);
    AssistanceRequest **arr = get_assistance_request_array_ptr(list);

    int device_smartphone_requests_number = 0;
    int device_tablet_requests_number = 0;
    int device_laptop_requests_number = 0;
    int device_desktop_requests_number = 0;
    int device_printer_requests_number = 0;

    AssistanceRequestArray *high_priority_open_requests = NULL;
    AssistanceRequestArray *in_progress_requests = NULL;
    AssistanceRequestArray *closed_requests = NULL;
    AssistanceRequestArray *cancelled_requests = NULL;

    high_priority_open_requests = create_empty_request_array();
    if (high_priority_open_requests == NULL)
    {
        fclose(fp);
        return;
    }

    in_progress_requests = create_empty_request_array();
    if (in_progress_requests == NULL)
    {
        free_assistance_request_array(high_priority_open_requests);
        fclose(fp);
        return;
    }

    closed_requests = create_empty_request_array();
    if (closed_requests == NULL)
    {
        free_assistance_request_array(high_priority_open_requests);
        free_assistance_request_array(in_progress_requests);
        fclose(fp);
        return;
    }

    cancelled_requests = create_empty_request_array();
    if (cancelled_requests == NULL)
    {
        free_assistance_request_array(high_priority_open_requests);
        free_assistance_request_array(in_progress_requests);
        free_assistance_request_array(closed_requests);
        fclose(fp);
        return;
    }

    for (int i = 0; i < size; i++)
    {
        switch (get_request_status(arr[i]))
        {
        case STATUS_OPEN:
            if (get_priority_level(arr[i]) == PRIORITY_HIGH)
            {
                insert_assistance_request(high_priority_open_requests, arr[i]);
            }
            break;
        case STATUS_IN_PROGRESS:
            insert_assistance_request(in_progress_requests, arr[i]);
            break;
        case STATUS_CLOSED:
            insert_assistance_request(closed_requests, arr[i]);
            break;
        case STATUS_CANCELLED:
            insert_assistance_request(cancelled_requests, arr[i]);
            break;
        default:
            break;
        }

        switch (get_device_type(arr[i]))
        {
        case DEVICE_SMARTPHONE:
            device_smartphone_requests_number++;
            break;
        case DEVICE_TABLET:
            device_tablet_requests_number++;
            break;
        case DEVICE_LAPTOP:
            device_laptop_requests_number++;
            break;
        case DEVICE_DESKTOP:
            device_desktop_requests_number++;
            break;
        case DEVICE_PRINTER:
            device_printer_requests_number++;
            break;
        default:
            break;
        }
    }

    fprintf(fp, "==================================================\n");
    fprintf(fp, "            REPORT OPERATIVO RICHIESTE            \n");
    fprintf(fp, "==================================================\n");

    fprintf(fp, "\ni. Richieste ad ALTA PRIORITA' ancora APERTE:\n");
    if (get_assistance_request_array_size(high_priority_open_requests) == 0)
        fprintf(fp, "Nessuna richiesta\n");
    else
        print_request_list_to_file(high_priority_open_requests, fp);

    fprintf(fp, "\nii. Richieste IN LAVORAZIONE (IN PROGRESS):\n");
    if (get_assistance_request_array_size(in_progress_requests) == 0)
        fprintf(fp, "Nessuna richiesta\n");
    else
        print_request_list_to_file(in_progress_requests, fp);

    fprintf(fp, "\niii. Richieste COMPLETATE (CLOSED):\n");
    if (get_assistance_request_array_size(closed_requests) == 0)
        fprintf(fp, "Nessuna richiesta\n");
    else
        print_request_list_to_file(closed_requests, fp);

    fprintf(fp, "\niv. Richieste ANNULLATE (CANCELED):\n");
    if (get_assistance_request_array_size(cancelled_requests) == 0)
        fprintf(fp, "Nessuna richiesta\n");
    else
        print_request_list_to_file(cancelled_requests, fp);

    fprintf(fp, "\nv. Riepilogo per TIPOLOGIA DI DISPOSITIVO:\n");
    fprintf(fp, "   - Smartphone: %d\n", device_smartphone_requests_number);
    fprintf(fp, "   - Tablet:     %d\n", device_tablet_requests_number);
    fprintf(fp, "   - Laptop:     %d\n", device_laptop_requests_number);
    fprintf(fp, "   - Desktop:    %d\n", device_desktop_requests_number);
    fprintf(fp, "   - Stampanti:  %d\n", device_printer_requests_number);
    fprintf(fp, "==================================================\n");

    free_assistance_request_array(high_priority_open_requests);
    free_assistance_request_array(in_progress_requests);
    free_assistance_request_array(closed_requests);
    free_assistance_request_array(cancelled_requests);

    fclose(fp);

    printf("[OK] Report operativo salvato in '%s'\n", OPERATIVE_REPORT_FILENAME);
}

static void print_request_list_to_file(const AssistanceRequestArray *list, FILE *fp)
{
    if (list == NULL)
    {
        fprintf(stderr, "Errore print_request_list_to_file: puntatore NULL\n");
        return;
    }

    int size = get_assistance_request_array_size(list);
    AssistanceRequest **arr = get_assistance_request_array_ptr(list);

    for (int i = 0; i < size; i++)
    {
        print_request_to_file(arr[i], fp);
    }
}

static void print_request_to_file(const AssistanceRequest *request, FILE *fp)
{
    if (request == NULL || fp == NULL)
    {
        fprintf(stderr, "Errore print_request_to_file: parametro NULL\n");
        return;
    }

    Date data = get_opening_date(request);

    /* Tabelle di conversione locali, speculari a quelle statiche in ware.c */
    const char *device_str[] = {"SMARTPHONE", "TABLET", "LAPTOP", "DESKTOP", "PRINTER"};
    const char *priority_str[] = {"LOW", "MEDIUM", "HIGH"};
    const char *status_str[] = {"OPEN", "IN_PROGRESS", "CLOSED", "CANCELED"};

    DeviceType dev = get_device_type(request);
    PriorityLevel pri = get_priority_level(request);
    RequestStatus st = get_request_status(request);

    fprintf(fp, "--------------------------------------------\n");
    fprintf(fp, "Codice:        %d\n", get_request_code(request));
    fprintf(fp, "Cliente:       %s\n", get_customer_name(request));
    fprintf(fp, "Dispositivo:   %s\n", (dev >= 0 && dev < 5) ? device_str[dev] : "ERRORE");
    fprintf(fp, "Descrizione:   %s\n", get_description(request));
    fprintf(fp, "Priorita':     %s\n", (pri >= 0 && pri < 3) ? priority_str[pri] : "ERRORE");
    fprintf(fp, "Stato:         %s\n", (st >= 0 && st < 4) ? status_str[st] : "ERRORE");
    fprintf(fp, "Costo stimato: %.2f\n", get_estimated_cost(request));
    fprintf(fp, "Costo finale:  %.2f\n", get_final_cost(request));
    fprintf(fp, "Data apertura: %02d/%02d/%04d\n", data.day, data.month, data.year);
    fprintf(fp, "--------------------------------------------\n");
}