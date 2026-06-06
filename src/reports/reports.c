#include"reports.h"
#include<stdio.h>
#include"sort.h"
#include<stdlib.h>

static void print_request_list(const AssistanceRequestArray *list);
static AssistanceRequestArray *empty_request_array();

void general_requests_report(const AssistanceRequestArray *list)
{
    if(list == NULL)
    {
        fprintf(stderr, "Errore general_requests_report: puntatore NULL\n");
        return;
    }

    int size = get_assistance_request_array_size(list);
    AssistanceRequest **arr = get_assistance_request_array_ptr(list);

    int open_status_requests_number = 0;
    int in_progress_status_requests_number = 0;
    int closed_status_requests_number = 0;

    int low_priority_requests_number = 0;
    int medium_priority_requests_number = 0;
    int high_priority_requests_number = 0;

    float estimated_cost_sum = 0.0f;
    float final_cost_sum = 0.0f;

    /* Singolo ciclo di scansione lineare per accumulare contatori e statistiche finanziarie */
    for(int i = 0;i < size;i++)
    {
        switch(get_request_status(arr[i]))
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
        }

        switch(get_priority_level(arr[i]))
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
        }

        estimated_cost_sum += get_estimated_cost(arr[i]);
   
    }

    float estimated_cost_average = -1.0f;
    float final_cost_average = -1.0f;

    if(size > 0)
    {
        estimated_cost_average = estimated_cost_sum / size;
    }

    if(closed_status_requests_number > 0)
    {
        final_cost_average = final_cost_sum / closed_status_requests_number;
    }

    AssistanceRequestArray *sorted_array = copy_assistance_request_array(list);
    sort_by_estimated_cost(sorted_array);


    printf("==================================================\n");
    printf("            REPORT GENERALE RICHIESTE             \n");
    printf("==================================================\n");

    printf("i. Numero totale di richieste: %d\n\n", size);

    printf("ii. Richieste per STATO:\n");
    printf("    - Aperte (OPEN):         %d\n", open_status_requests_number);
    printf("    - In Corso (IN PROGRESS): %d\n", in_progress_status_requests_number);
    printf("    - Chiuse (CLOSED):        %d\n\n", closed_status_requests_number);

    printf("iii. Richieste per PRIORITA':\n");
    printf("     - Alta (HIGH):   %d\n", high_priority_requests_number);
    printf("     - Media (MEDIUM): %d\n", medium_priority_requests_number);
    printf("     - Bassa (LOW):    %d\n\n", low_priority_requests_number);

    printf("iv. Costo medio stimato: ");
    if(estimated_cost_average >= 0) {
        printf("%.2f\n", estimated_cost_average);
    } else {
        printf("N/D (Nessuna richiesta presente)\n");
    }

    printf("v. Costo medio finale (Richieste Chiuse): ");
    if(final_cost_average >= 0) {
        printf("%.2f\n", final_cost_average);
    } else {
        printf("N/D (Nessuna richiesta completata)\n");
    }
    
    for(int i = 0;i < size;i++)
    {
        print_request(arr[i]);
    }

    printf("==================================================\n");

    free_assistance_request_array(sorted_array);
}

AssistanceRequestArray *empty_request_array()
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

void print_request_list(const AssistanceRequestArray *list)
{
    if(list == NULL)
    {
        fprintf(stderr, "Errore print_request_list: puntatore NULL\n");
        return;
    }

    int size = get_assistance_request_array_size(list);
    AssistanceRequest **arr = get_assistance_request_array_ptr(list);
    
    for(int i = 0;i < size;i++)
    {
        print_request(arr[i]);
    }
}

void operative_requests_report(const AssistanceRequestArray *list)
{
    if(list == NULL)
    {
        fprintf(stderr, "Errore operative_requests_report: puntatore NULL\n");
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

    high_priority_open_requests = empty_request_array();
    if(high_priority_open_requests == NULL)
    {
        return;
    }

    in_progress_requests = empty_request_array();
    if(in_progress_requests == NULL)
    {
        free_assistance_request_array(high_priority_open_requests);
        return;
    }

    closed_requests = empty_request_array();
    if(closed_requests == NULL)
    {
        free_assistance_request_array(high_priority_open_requests);
        free_assistance_request_array(in_progress_requests);
        return;
    }

    cancelled_requests = empty_request_array();
    if(cancelled_requests == NULL)
    {
        free_assistance_request_array(high_priority_open_requests);
        free_assistance_request_array(in_progress_requests);
        free_assistance_request_array(closed_requests);
        return;
    }

    for(int i = 0;i < size; i++)
    {
        switch(get_request_status(arr[i]))
        {
            case STATUS_OPEN:
                if(get_priority_level(arr[i]) == PRIORITY_HIGH)
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
            case STATUS_CANCELED:
                insert_assistance_request(cancelled_requests, arr[i]);
                break;
        }

        switch(get_device_type(arr[i]))
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
        }
    }

    printf("==================================================\n");
    printf("            REPORT OPERATIVO RICHIESTE            \n");
    printf("==================================================\n");

    printf("i. Richieste ad ALTA PRIORITA' ancora APERTE:\n");
    if(get_assistance_request_array_size(high_priority_open_requests) == 0)
        printf("Nessuna richiesta\n");
    else
        print_request_list(high_priority_open_requests);
    printf("\n");

    printf("ii. Richieste IN LAVORAZIONE (IN PROGRESS):\n");
    if(get_assistance_request_array_size(in_progress_requests) == 0)
        printf("Nessuna richiesta\n");
    else
        print_request_list(in_progress_requests);
    printf("\n");

    printf("iii. Richieste COMPLETATE (CLOSED):\n");
    if(get_assistance_request_array_size(closed_requests) == 0)
        printf("Nessuna richiesta\n");
    else
        print_request_list(closed_requests);
    printf("\n");

    printf("iv. Richieste ANNULLATE (CANCELED):\n");
    if(get_assistance_request_array_size(cancelled_requests) == 0)
        printf("Nessuna richiesta\n");
    else
        print_request_list(cancelled_requests);
    printf("\n");

    printf("v. Riepilogo per TIPOLOGIA DI DISPOSITIVO:\n");
    printf("   - Smartphone: %d\n", device_smartphone_requests_number);
    printf("   - Tablet:     %d\n", device_tablet_requests_number);
    printf("   - Laptop:     %d\n", device_laptop_requests_number);
    printf("   - Desktop:    %d\n", device_desktop_requests_number);
    printf("   - Stampanti:  %d\n", device_printer_requests_number);
    printf("==================================================\n");

    free_assistance_request_array(high_priority_open_requests);
    free_assistance_request_array(in_progress_requests);
    free_assistance_request_array(closed_requests);
    free_assistance_request_array(cancelled_requests);

}