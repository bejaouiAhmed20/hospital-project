#include "../include/patient.h"
#include "../include/ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Create a new patient list
PatientList* createPatientList() {
    PatientList* list = (PatientList*)malloc(sizeof(PatientList));
    if (list == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la liste de patients\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->count = 0;
    return list;
}

// Free memory allocated for patient list
void freePatientList(PatientList* list) {
    if (list == NULL) return;
    
    Patient* current = list->head;
    while (current != NULL) {
        Patient* next = current->next;
        free(current);
        current = next;
    }
    
    free(list);
}

// Create a new patient
Patient* createPatient(const char* id, const char* firstName, const char* lastName, 
                      const char* phone, const char* address, struct tm dob) {
    Patient* patient = (Patient*)malloc(sizeof(Patient));
    if (patient == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le patient\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(patient->patientId, id, MAX_ID_LENGTH - 1);
    patient->patientId[MAX_ID_LENGTH - 1] = '\0';
    
    strncpy(patient->firstName, firstName, MAX_NAME_LENGTH - 1);
    patient->firstName[MAX_NAME_LENGTH - 1] = '\0';
    
    strncpy(patient->lastName, lastName, MAX_NAME_LENGTH - 1);
    patient->lastName[MAX_NAME_LENGTH - 1] = '\0';
    
    strncpy(patient->phoneNumber, phone, MAX_PHONE_LENGTH - 1);
    patient->phoneNumber[MAX_PHONE_LENGTH - 1] = '\0';
    
    strncpy(patient->address, address, MAX_ADDRESS_LENGTH - 1);
    patient->address[MAX_ADDRESS_LENGTH - 1] = '\0';
    
    // Initialize diagnosis and treatment as empty
    patient->diagnosis[0] = '\0';
    patient->treatment[0] = '\0';
    
    // Copy date of birth
    patient->dateOfBirth = dob;
    
    patient->next = NULL;
    
    return patient;
}

// Add a patient to the list
void addPatient(PatientList* list, Patient* patient) {
    if (list == NULL || patient == NULL) return;
    
    // Add at the beginning of the list for simplicity
    patient->next = list->head;
    list->head = patient;
    list->count++;
}

// Find a patient by ID
Patient* findPatientById(PatientList* list, const char* id) {
    if (list == NULL || id == NULL) return NULL;
    
    Patient* current = list->head;
    while (current != NULL) {
        if (strcmp(current->patientId, id) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL; // Patient not found
}

// Remove a patient from the list
int removePatient(PatientList* list, const char* id) {
    if (list == NULL || id == NULL || list->head == NULL) return 0;
    
    // If the patient to remove is the head
    if (strcmp(list->head->patientId, id) == 0) {
        Patient* temp = list->head;
        list->head = list->head->next;
        free(temp);
        list->count--;
        return 1;
    }
    
    // Search for the patient in the rest of the list
    Patient* current = list->head;
    while (current->next != NULL) {
        if (strcmp(current->next->patientId, id) == 0) {
            Patient* temp = current->next;
            current->next = current->next->next;
            free(temp);
            list->count--;
            return 1;
        }
        current = current->next;
    }
    
    return 0; // Patient not found
}

// Update patient information
void updatePatientInfo(Patient* patient, const char* firstName, const char* lastName, 
                      const char* phone, const char* address) {
    if (patient == NULL) return;
    
    if (firstName != NULL) {
        strncpy(patient->firstName, firstName, MAX_NAME_LENGTH - 1);
        patient->firstName[MAX_NAME_LENGTH - 1] = '\0';
    }
    
    if (lastName != NULL) {
        strncpy(patient->lastName, lastName, MAX_NAME_LENGTH - 1);
        patient->lastName[MAX_NAME_LENGTH - 1] = '\0';
    }
    
    if (phone != NULL) {
        strncpy(patient->phoneNumber, phone, MAX_PHONE_LENGTH - 1);
        patient->phoneNumber[MAX_PHONE_LENGTH - 1] = '\0';
    }
    
    if (address != NULL) {
        strncpy(patient->address, address, MAX_ADDRESS_LENGTH - 1);
        patient->address[MAX_ADDRESS_LENGTH - 1] = '\0';
    }
}

// Assign diagnosis and treatment to a patient
void assignDiagnosis(Patient* patient, const char* diagnosis, const char* treatment) {
    if (patient == NULL) return;
    
    if (diagnosis != NULL) {
        strncpy(patient->diagnosis, diagnosis, MAX_DIAGNOSIS_LENGTH - 1);
        patient->diagnosis[MAX_DIAGNOSIS_LENGTH - 1] = '\0';
    }
    
    if (treatment != NULL) {
        strncpy(patient->treatment, treatment, MAX_TREATMENT_LENGTH - 1);
        patient->treatment[MAX_TREATMENT_LENGTH - 1] = '\0';
    }
}

// Display patient information
void displayPatient(Patient* patient) {
    if (patient == NULL) return;
    
    printf("ID: %s\n", patient->patientId);
    printf("Nom: %s %s\n", patient->firstName, patient->lastName);
    printf("Téléphone: %s\n", patient->phoneNumber);
    printf("Adresse: %s\n", patient->address);
    printf("Date de naissance: %02d/%02d/%04d\n", 
           patient->dateOfBirth.tm_mday, 
           patient->dateOfBirth.tm_mon + 1, 
           patient->dateOfBirth.tm_year + 1900);
    
    if (patient->diagnosis[0] != '\0') {
        printf("Diagnostic: %s\n", patient->diagnosis);
        printf("Traitement: %s\n", patient->treatment);
    } else {
        printf("Aucun diagnostic ou traitement assigné.\n");
    }
}

// Display all patients
void displayAllPatients(PatientList* list) {
    if (list == NULL || list->head == NULL) {
        printf("Aucun patient enregistré.\n");
        return;
    }
    
    printf("Liste des patients (%d):\n", list->count);
    printf("--------------------------------------------------\n");
    
    Patient* current = list->head;
    int count = 1;
    while (current != NULL) {
        printf("%d. %s %s (ID: %s)\n", 
               count++, current->firstName, current->lastName, current->patientId);
        current = current->next;
    }
    printf("--------------------------------------------------\n");
}

// Patient management menu
void patientMenu(PatientList* patients) {
    int choice = 0;
    do {
        clearScreen();
        printf("=== GESTION DES PATIENTS ===\n\n");
        printf("1. Ajouter un patient\n");
        printf("2. Rechercher un patient\n");
        printf("3. Modifier un patient\n");
        printf("4. Supprimer un patient\n");
        printf("5. Assigner un diagnostic et traitement\n");
        printf("6. Afficher tous les patients\n");
        printf("7. Retour au menu principal\n");
        
        printf("\nVotre choix: ");
        if (scanf("%d", &choice) != 1) {
            choice = 0; // Invalid input
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
        switch (choice) {
            case 1: {
                // Add a new patient
                char id[MAX_ID_LENGTH];
                char firstName[MAX_NAME_LENGTH];
                char lastName[MAX_NAME_LENGTH];
                char phone[MAX_PHONE_LENGTH];
                char address[MAX_ADDRESS_LENGTH];
                int day, month, year;
                
                printf("ID du patient: ");
                readString(id, MAX_ID_LENGTH);
                
                // Check if ID already exists
                if (findPatientById(patients, id) != NULL) {
                    printf("Un patient avec cet ID existe déjà.\n");
                    waitForKeypress();
                    break;
                }
                
                printf("Prénom: ");
                readString(firstName, MAX_NAME_LENGTH);
                
                printf("Nom: ");
                readString(lastName, MAX_NAME_LENGTH);
                
                printf("Téléphone: ");
                readString(phone, MAX_PHONE_LENGTH);
                
                printf("Adresse: ");
                readString(address, MAX_ADDRESS_LENGTH);
                
                printf("Date de naissance (jour): ");
                scanf("%d", &day);
                clearInputBuffer();
                
                printf("Date de naissance (mois): ");
                scanf("%d", &month);
                clearInputBuffer();
                
                printf("Date de naissance (année): ");
                scanf("%d", &year);
                clearInputBuffer();
                
                struct tm dob = {0};
                dob.tm_mday = day;
                dob.tm_mon = month - 1; // tm_mon is 0-11
                dob.tm_year = year - 1900; // tm_year is years since 1900
                
                Patient* newPatient = createPatient(id, firstName, lastName, phone, address, dob);
                addPatient(patients, newPatient);
                
                printf("Patient ajouté avec succès!\n");
                waitForKeypress();
                break;
            }
            case 2: {
                // Search for a patient
                char id[MAX_ID_LENGTH];
                printf("Entrez l'ID du patient: ");
                readString(id, MAX_ID_LENGTH);
                
                Patient* patient = findPatientById(patients, id);
                if (patient != NULL) {
                    displayPatient(patient);
                } else {
                    printf("Patient non trouvé.\n");
                }
                waitForKeypress();
                break;
            }
            case 3: {
                // Update patient information
                char id[MAX_ID_LENGTH];
                printf("Entrez l'ID du patient à modifier: ");
                readString(id, MAX_ID_LENGTH);
                
                Patient* patient = findPatientById(patients, id);
                if (patient != NULL) {
                    char firstName[MAX_NAME_LENGTH];
                    char lastName[MAX_NAME_LENGTH];
                    char phone[MAX_PHONE_LENGTH];
                    char address[MAX_ADDRESS_LENGTH];
                    
                    printf("Nouveau prénom (laissez vide pour ne pas modifier): ");
                    readString(firstName, MAX_NAME_LENGTH);
                    
                    printf("Nouveau nom (laissez vide pour ne pas modifier): ");
                    readString(lastName, MAX_NAME_LENGTH);
                    
                    printf("Nouveau téléphone (laissez vide pour ne pas modifier): ");
                    readString(phone, MAX_PHONE_LENGTH);
                    
                    printf("Nouvelle adresse (laissez vide pour ne pas modifier): ");
                    readString(address, MAX_ADDRESS_LENGTH);
                    
                    updatePatientInfo(patient, 
                                     firstName[0] != '\0' ? firstName : NULL,
                                     lastName[0] != '\0' ? lastName : NULL,
                                     phone[0] != '\0' ? phone : NULL,
                                     address[0] != '\0' ? address : NULL);
                    
                    printf("Informations du patient mises à jour avec succès!\n");
                } else {
                    printf("Patient non trouvé.\n");
                }
                waitForKeypress();
                break;
            }
            case 4: {
                // Delete a patient
                char id[MAX_ID_LENGTH];
                printf("Entrez l'ID du patient à supprimer: ");
                readString(id, MAX_ID_LENGTH);
                
                if (removePatient(patients, id)) {
                    printf("Patient supprimé avec succès!\n");
                } else {
                    printf("Patient non trouvé.\n");
                }
                waitForKeypress();
                break;
            }
            case 5: {
                // Assign diagnosis and treatment
                char id[MAX_ID_LENGTH];
                printf("Entrez l'ID du patient: ");
                readString(id, MAX_ID_LENGTH);
                
                Patient* patient = findPatientById(patients, id);
                if (patient != NULL) {
                    char diagnosis[MAX_DIAGNOSIS_LENGTH];
                    char treatment[MAX_TREATMENT_LENGTH];
                    
                    printf("Diagnostic: ");
                    readString(diagnosis, MAX_DIAGNOSIS_LENGTH);
                    
                    printf("Traitement: ");
                    readString(treatment, MAX_TREATMENT_LENGTH);
                    
                    assignDiagnosis(patient, diagnosis, treatment);
                    
                    printf("Diagnostic et traitement assignés avec succès!\n");
                } else {
                    printf("Patient non trouvé.\n");
                }
                waitForKeypress();
                break;
            }
            case 6: {
                // Display all patients
                displayAllPatients(patients);
                waitForKeypress();
                break;
            }
            case 7:
                // Return to main menu
                return;
                
            default:
                printf("Option invalide. Veuillez réessayer.\n");
                waitForKeypress();
        }
    } while (1); // Changed from choice != 7 to always loop until explicit return
}