# Sistema di Gestione Richieste di Assistenza

Questo progetto è un'applicazione console scritta in linguaggio C progettata per gestire un flusso di lavoro di assistenza tecnica per dispositivi elettronici (smartphone, tablet, PC, ecc.). 

Il sistema permette di creare, visualizzare, cercare e ordinare richieste di assistenza, garantendo la persistenza dei dati tramite file di testo.

## Funzionalità Principali

*   **Gestione Richieste**: Creazione di nuove richieste con codice univoco, nome cliente, tipo dispositivo, descrizione e priorità.
*   **Aggiornamento**: Possibilità di modificare lo stato della riparazione, il costo stimato, il costo finale e la descrizione.
*   **Ricerca e Ordinamento**: Ricerca rapida per codice richiesta e ordinamento della lista per nome cliente o per costo stimato.
*   **Persistenza**: Salvataggio automatico e caricamento dei dati dal file `requests.txt`.
*   **Interfaccia CLI**: Menu testuale intuitivo per l'interazione con l'utente.

## Struttura del Progetto

Il codice è organizzato in modo modulare:
*   `src/ware/`: Gestione delle strutture dati core (`AssistanceRequest`).
*   `src/cli/`: Gestione dell'interfaccia utente a riga di comando.
*   `src/file_manager/`: Logica di lettura/scrittura su file.
*   `src/sort/`: Algoritmi di ordinamento (Merge Sort).
*   `src/search/` e `src/filter/`: Funzioni di ricerca e filtraggio.

## Prerequisiti

Per compilare il progetto è necessario avere installato:
*   Un compilatore C (**GCC**, **Clang** o **MSVC**).
*   **CMake** (versione 3.10 o superiore).

*   **Windows**: È consigliato l'uso di MinGW-w64 o MSYS2.
*   **Linux/macOS**: GCC è solitamente disponibile tramite il gestore pacchetti (es. `apt install build-essential`).

## Compilazione

Il progetto utilizza **CMake**. Segui questi passaggi dalla cartella principale per compilare:

```bash
# 1. Crea una cartella per i file di build
mkdir build
cd build

# 2. Genera i file di configurazione
cmake ..

# 3. Compila il progetto
cmake --build .
```

*Nota: Assicurati di includere `src/main.c` (o il file che contiene la funzione `main`) e tutti i file `.c` necessari.*

## Esecuzione

Una volta compilato, puoi avviare il programma con:

```bash
./gestione_assistenza
```

## Note sul File di Memoria
Al primo avvio, se il file `requests.txt` non esiste, il programma lo creerà automaticamente. I dati vengono salvati nel file ogni volta che viene selezionata l'opzione "Salva su file" dal menu principale.

---
*Sviluppato come progetto di gestione magazzino e assistenza in C.*