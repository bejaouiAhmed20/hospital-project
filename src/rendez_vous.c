#include "../include/rendez_vous.h"
#include "../include/ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Create a new appointment list
RendezVousList* createRendezVousList() {
    RendezVousList* list = (RendezVousList*)malloc(sizeof(RendezVousList));
    if (list == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la liste de rendez-vous\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->count = 0;
    return list;
}

// Free memory allocated for appointment list
void freeRendezVousList(RendezVousList* list) {
    if (list == NULL) return;
    
    RendezVous* current = list->head;
    while (current != NULL) {
        RendezVous* next = current->next;
        free(current);
        current = next;
    }
    
    free(list);
}

// Create a new appointment
RendezVous* createRendezVous(const char* id, const char* patientId, const char* medecinId, 
                           struct tm dateTime, const char* notes) {
    RendezVous* rendezVous = (RendezVous*)malloc(sizeof(RendezVous));
    if (rendezVous == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le rendez-vous\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(rendezVous->rendezVousId, id, MAX_ID_LENGTH - 1);
    rendezVous->rendezVousId[MAX_ID_LENGTH - 1] = '\0';
    
    strncpy(rendezVous->patientId, patientId, MAX_ID_LENGTH - 1);
    rendezVous->patientId[MAX_ID_LENGTH - 1] = '\0';
    
    strncpy(rendezVous->medecinId, medecinId, MAX_ID_LENGTH - 1);
    rendezVous->medecinId[MAX_ID_LENGTH - 1] = '\0';
    
    rendezVous->dateTime = dateTime;
    
    if (notes != NULL) {
        strncpy(rendezVous->notes, notes, MAX_NOTES_LENGTH - 1);
        rendezVous->notes[MAX_NOTES_LENGTH - 1] = '\0';
    } else {
        rendezVous->notes[0] = '\0';
    }
    
    rendezVous->next = NULL;
    
    return rendezVous;
}

// Add an appointment to the list
void addRendezVous(RendezVousList* list, RendezVous* rendezVous) {
    if (list == NULL || rendezVous == NULL) return;
    
    // Add at the beginning of the list for simplicity
    rendezVous->next = list->head;
    list->head = rendezVous;
    list->count++;
}

// Find an appointment by ID
RendezVous* findRendezVousById(RendezVousList* list, const char* id) {
    if (list == NULL || id == NULL) return NULL;
    
    RendezVous* current = list->head;
    while (current != NULL) {
        if (strcmp(current->rendezVousId, id) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL; // Appointment not found
}

// Remove an appointment from the list
int removeRendezVous(RendezVousList* list, const char* id) {
    if (list == NULL || id == NULL || list->head == NULL) return 0;
    
    // If the appointment to remove is the head
    if (strcmp(list->head->rendezVousId, id) == 0) {
        RendezVous* temp = list->head;
        list->head = list->head->next;
        free(temp);
        list->count--;
        return 1;
    }
    
    // Search for the appointment in the rest of the list
    RendezVous* current = list->head;
    while (current->next != NULL) {
        if (strcmp(current->next->rendezVousId, id) == 0) {
            RendezVous* temp = current->next;
            current->next = current->next->next;
            free(temp);
            list->count--;
            return 1;
        }
        current = current->next;
    }
    
    return 0; // Appointment not found
}

// Update appointment notes
void updateRendezVousNotes(RendezVous* rendezVous, const char* notes) {
    if (rendezVous == NULL || notes == NULL) return;
    
    strncpy(rendezVous->notes, notes, MAX_NOTES_LENGTH - 1);
    rendezVous->notes[MAX_NOTES_LENGTH - 1] = '\0';
}

// Display appointment information
void displayRendezVous(RendezVous* rendezVous, PatientList* patients, MedecinList* medecins) {
    if (rendezVous == NULL) return;
    
    printf("ID: %s\n", rendezVous->rendezVousId);
    
    // Find and display patient information
    Patient* patient = findPatientById(patients, rendezVous->patientId);
    if (patient != NULL) {
        printf("Patient: %s %s (ID: %s)\n", 
               patient->firstName, patient->lastName, patient->patientId);
    } else {
        printf("Patient: ID %s (non trouvé)\n", rendezVous->patientId);
    }
    
    // Find and display doctor information
    Medecin* medecin = findMedecinById(medecins, rendezVous->medecinId);
    if (medecin != NULL) {
        printf("Médecin: %s %s (ID: %s) - %s\n", 
               medecin->firstName, medecin->lastName, 
               medecin->medecinId, medecin->specialityStr);
    } else {
        printf("Médecin: ID %s (non trouvé)\n", rendezVous->medecinId);
    }
    
    // Display date and time
    printf("Date et heure: %02d/%02d/%04d à %02d:%02d\n", 
           rendezVous->dateTime.tm_mday, 
           rendezVous->dateTime.tm_mon + 1, 
           rendezVous->dateTime.tm_year + 1900,
           rendezVous->dateTime.tm_hour,
           rendezVous->dateTime.tm_min);
    
    // Display notes if any
    if (rendezVous->notes[0] != '\0') {
        printf("Notes: %s\n", rendezVous->notes);
    } else {
        printf("Aucune note.\n");
    }
}

// Display all appointments
void displayAllRendezVous(RendezVousList* list, PatientList* patients, MedecinList* medecins) {
    if (list == NULL || list->head == NULL) {
        printf("Aucun rendez-vous enregistré.\n");
        return;
    }
    
    printf("Liste des rendez-vous (%d):\n", list->count);
    printf("--------------------------------------------------\n");
    
    RendezVous* current = list->head;
    int count = 1;
    while (current != NULL) {
        // Find patient and doctor names
        char patientName[MAX_NAME_LENGTH * 2 + 1] = "Patient non trouvé";
        char medecinName[MAX_NAME_LENGTH * 2 + 1] = "Médecin non trouvé";
        
        Patient* patient = findPatientById(patients, current->patientId);
        if (patient != NULL) {
            snprintf(patientName, sizeof(patientName), "%s %s", 
                    patient->firstName, patient->lastName);
        }
        
        Medecin* medecin = findMedecinById(medecins, current->medecinId);
        if (medecin != NULL) {
            snprintf(medecinName, sizeof(medecinName), "%s %s", 
                    medecin->firstName, medecin->lastName);
        }
        
        printf("%d. %02d/%02d/%04d à %02d:%02d - %s avec %s (ID: %s)\n", 
               count++, 
               current->dateTime.tm_mday, 
               current->dateTime.tm_mon + 1, 
               current->dateTime.tm_year + 1900,
               current->dateTime.tm_hour,
               current->dateTime.tm_min,
               patientName, medecinName, current->rendezVousId);
        
        current = current->next;
    }
    printf("--------------------------------------------------\n");
}

// Find appointments by patient ID
RendezVous** findRendezVousByPatientId(RendezVousList* list, const char* patientId, int* count) {
    if (list == NULL || patientId == NULL || count == NULL) return NULL;
    
    // First, count matching appointments
    *count = 0;
    RendezVous* current = list->head;
    while (current != NULL) {
        if (strcmp(current->patientId, patientId) == 0) {
            (*count)++;
        }
        current = current->next;
    }
    
    if (*count == 0) return NULL;
    
    // Allocate array for results
    RendezVous** results = (RendezVous**)malloc(sizeof(RendezVous*) * (*count));
    if (results == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les résultats de recherche\n");
        exit(EXIT_FAILURE);
    }
    
    // Fill array with matching appointments
    current = list->head;
    int index = 0;
    while (current != NULL) {
        if (strcmp(current->patientId, patientId) == 0) {
            results[index++] = current;
        }
        current = current->next;
    }
    
    return results;
}

// Find appointments by doctor ID
RendezVous** findRendezVousByMedecinId(RendezVousList* list, const char* medecinId, int* count) {
    if (list == NULL || medecinId == NULL || count == NULL) return NULL;
    
    // First, count matching appointments
    *count = 0;
    RendezVous* current = list->head;
    while (current != NULL) {
        if (strcmp(current->medecinId, medecinId) == 0) {
            (*count)++;
        }
        current = current->next;
    }
    
    if (*count == 0) return NULL;
    
    // Allocate array for results
    RendezVous** results = (RendezVous**)malloc(sizeof(RendezVous*) * (*count));
    if (results == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour les résultats de recherche\n");
        exit(EXIT_FAILURE);
    }
    
    // Fill array with matching appointments
    current = list->head;
    int index = 0;
    while (current != NULL) {
        if (strcmp(current->medecinId, medecinId) == 0) {
            results[index++] = current;
        }
        current = current->next;
    }
    
    return results;
}

// Check if a date and time is available for a doctor
int isDateTimeAvailable(RendezVousList* list, const char* medecinId, struct tm dateTime) {
    if (list == NULL || medecinId == NULL) return 1; // Assume available if invalid input
    
    RendezVous* current = list->head;
    while (current != NULL) {
        if (strcmp(current->medecinId, medecinId) == 0) {
            // Check if date and time match (within the same hour)
            if (current->dateTime.tm_year == dateTime.tm_year &&
                current->dateTime.tm_mon == dateTime.tm_mon &&
                current->dateTime.tm_mday == dateTime.tm_mday &&
                current->dateTime.tm_hour == dateTime.tm_hour) {
                return 0; // Not available
            }
        }
        current = current->next;
    }
    
    return 1; // Available
}

// Appointment management menu
void rendezVousMenu(RendezVousList* rendezVous, PatientList* patients, MedecinList* medecins) {
    int choice = 0;
    do {
        clearScreen();
        printf("===== GESTION DES RENDEZ-VOUS =====\n");
        printf("1. Ajouter un rendez-vous\n");
        printf("2. Rechercher un rendez-vous\n");
        printf("3. Modifier les notes d'un rendez-vous\n");
        printf("4. Supprimer un rendez-vous\n");
        printf("5. Afficher tous les rendez-vous\n");
        printf("6. Afficher les rendez-vous d'un patient\n");
        printf("7. Afficher les rendez-vous d'un médecin\n");
        printf("8. Retour au menu principal\n");
        printf("Votre choix: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch (choice) {
            case 1: {
                // Add a new appointment
                char id[MAX_ID_LENGTH];
                char patientId[MAX_ID_LENGTH];
                char medecinId[MAX_ID_LENGTH];
                struct tm dateTime = {0};
                char notes[MAX_NOTES_LENGTH];
                
                printf("ID du rendez-vous: ");
                readString(id, MAX_ID_LENGTH);
                
                // Check if ID already exists
                if (findRendezVousById(rendezVous, id) != NULL) {
                    printf("Un rendez-vous avec cet ID existe déjà.\n");
                    waitForKeypress();
                    break;
                }
                
                // Display all patients and get patient ID
                displayAllPatients(patients);
                printf("ID du patient: ");
                readString(patientId, MAX_ID_LENGTH);
                
                // Check if patient exists
                if (findPatientById(patients, patientId) == NULL) {
                    printf("Patient non trouvé.\n");
                    waitForKeypress();
                    break;
                }
                
                // Display all doctors and get doctor ID
                displayAllMedecins(medecins);
                printf("ID du médecin: ");
                readString(medecinId, MAX_ID_LENGTH);
                
                // Check if doctor exists
                Medecin* medecin = findMedecinById(medecins, medecinId);
                if (medecin == NULL) {
                    printf("Médecin non trouvé.\n");
                    waitForKeypress();
                    break;
                }
                
                // Find and display appointments
                int count = 0;
                RendezVous** medecinRendezVous = findRendezVousByMedecinId(rendezVous, medecinId, &count);
                
                if (count > 0) {
                    printf("Rendez-vous pour %s %s (ID: %s):\n", 
                           medecin->firstName, medecin->lastName, medecin->medecinId);
                    printf("--------------------------------------------------\n");
                    
                    for (int i = 0; i < count; i++) {
                        // Find patient name
                        char patientName[MAX_NAME_LENGTH * 2 + 1] = "Patient non trouvé";
                        
                        Patient* patient = findPatientById(patients, medecinRendezVous[i]->patientId);
                        if (patient != NULL) {
                            snprintf(patientName, sizeof(patientName), "%s %s", 
                                    patient->firstName, patient->lastName);
                        }
                        
                        printf("%d. %02d/%02d/%04d à %02d:%02d - %s (ID: %s)\n", 
                               i + 1, 
                               medecinRendezVous[i]->dateTime.tm_mday, 
                               medecinRendezVous[i]->dateTime.tm_mon + 1, 
                               medecinRendezVous[i]->dateTime.tm_year + 1900,
                               medecinRendezVous[i]->dateTime.tm_hour,
                               medecinRendezVous[i]->dateTime.tm_min,
                               patientName, medecinRendezVous[i]->rendezVousId);
                    }
                    printf("--------------------------------------------------\n");
                    
                    free(medecinRendezVous);
                } else {
                    printf("Aucun rendez-vous trouvé pour ce médecin.\n");
                }
                
                waitForKeypress();
                break;
            }
            case 8:
                // Return to main menu
                break;
            default:
                printf("Option invalide. Veuillez réessayer.\n");
                waitForKeypress();
        }
    } while (choice != 8);
}