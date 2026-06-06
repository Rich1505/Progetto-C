#include <stdio.h>
#include <float.h>
#include "ware.h"

void print_stats_by_device_type(const AssistanceRequestArray *list)
{
    if (list == NULL) {
        fprintf(stderr, "[ERRORE] Lista richieste non valida.\n");
        return;
    }

    AssistanceRequest **array = get_assistance_request_array_ptr(list);
    int size = get_assistance_request_array_size(list);

    if (array == NULL || size <= 0) {
        printf("\n--- Statistiche per Tipologia Dispositivo ---\n");
        printf("Nessuna richiesta presente per calcolare le statistiche.\n");
        return;
    }

    // Array di supporto per accumulare i dati (0: Smartphone, 1: Tablet, 2: Laptop, 3: Desktop, 4: Printer)
    // Usiamo 5 come dimensione basandoci sul device_to_string presente in ware.c
    int count[5] = {0};
    double total_cost[5] = {0.0};
    float max_cost[5] = {-1.0f}; // Inizializzato a -1 per identificare se non ci sono costi registrati

    for (int i = 0; i < size; i++) {
        AssistanceRequest *req = array[i];
        if (req == NULL) continue;

        DeviceType type = get_device_type(req);
        
        // Mappatura sicura del tipo enum all'indice dell'array di supporto
        int idx = -1;
        switch (type) {
            case DEVICE_SMARTPHONE: idx = 0; break;
            case DEVICE_TABLET:     idx = 1; break;
            case DEVICE_LAPTOP:     idx = 2; break;
            case DEVICE_DESKTOP:    idx = 3; break;
            case DEVICE_PRINTER:    idx = 4; break;
            default: continue; // Salta tipologie di errore o non riconosciute
        }

        // Consideriamo il costo finale se la richiesta è chiusa, altrimenti il costo stimato
        float current_cost = (get_request_status(req) == STATUS_CLOSED) ? get_final_cost(req) : get_estimated_cost(req);

        count[idx]++;
        total_cost[idx] += current_cost;
        if (current_cost > max_cost[idx]) {
            max_cost[idx] = current_cost;
        }
    }

    const char *labels[] = {"SMARTPHONE", "TABLET", "LAPTOP", "DESKTOP", "PRINTER"};

    printf("\n======================================================\n");
    printf("       STATISTICHE PER TIPOLOGIA DI DISPOSITIVO       \n");
    printf("======================================================\n");
    printf("%-15s | %-10s | %-12s | %-12s\n", "Dispositivo", "N. Rich.", "Costo Medio", "Costo Max");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < 5; i++) {
        if (count[i] > 0) {
            printf("%-15s | %-10d | %-12.2f | %-12.2f\n", 
                   labels[i], count[i], total_cost[i] / count[i], max_cost[i]);
        } else {
            printf("%-15s | %-10d | %-12s | %-12s\n", 
                   labels[i], 0, "N/D", "N/D");
        }
    }
    printf("======================================================\n");
}

void print_stats_by_priority(const AssistanceRequestArray *list)
{
    if (list == NULL) {
        fprintf(stderr, "[ERRORE] Lista richieste non valida.\n");
        return;
    }

    AssistanceRequest **array = get_assistance_request_array_ptr(list);
    int size = get_assistance_request_array_size(list);

    if (array == NULL || size <= 0) {
        printf("\n--- Statistiche per Priorità ---\n");
        printf("Nessuna richiesta presente per calcolare le statistiche.\n");
        return;
    }

    // Array di supporto (0: LOW, 1: MEDIUM, 2: HIGH)
    int count[3] = {0};
    double total_cost[3] = {0.0};
    float max_cost[3] = {-1.0f};

    for (int i = 0; i < size; i++) {
        AssistanceRequest *req = array[i];
        if (req == NULL) continue;

        PriorityLevel priority = get_priority_level(req);
        int idx = -1;
        switch (priority) {
            case PRIORITY_LOW:    idx = 0; break;
            case PRIORITY_MEDIUM: idx = 1; break;
            case PRIORITY_HIGH:   idx = 2; break;
            default: continue;
        }

        float current_cost = (get_request_status(req) == STATUS_CLOSED) ? get_final_cost(req) : get_estimated_cost(req);

        count[idx]++;
        total_cost[idx] += current_cost;
        if (current_cost > max_cost[idx]) {
            max_cost[idx] = current_cost;
        }
    }

    const char *labels[] = {"LOW", "MEDIUM", "HIGH"};

    printf("\n======================================================\n");
    printf("              STATISTICHE PER PRIORITA'               \n");
    printf("======================================================\n");
    printf("%-15s | %-10s | %-12s | %-12s\n", "Priorita'", "N. Rich.", "Costo Medio", "Costo Max");
    printf("------------------------------------------------------\n");

    for (int i = 0; i < 3; i++) {
        if (count[i] > 0) {
            printf("%-15s | %-10d | %-12.2f | %-12.2f\n", 
                   labels[i], count[i], total_cost[i] / count[i], max_cost[i]);
        } else {
            printf("%-15s | %-10d | %-12s | %-12s\n", 
                   labels[i], 0, "N/D", "N/D");
        }
    }
    printf("======================================================\n");
}