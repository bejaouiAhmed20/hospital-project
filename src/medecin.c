#include "../include/medecin.h"
#include "../include/ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a new doctor list
MedecinList* createMedecinList() {
    MedecinList* list = (MedecinList*)malloc(sizeof(MedecinList));
    if (list == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour la liste de médecins\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->count = 0;
    return list;
}

// Free memory allocated for doctor list
void freeMedecinList(MedecinList* list) {
    if (list == NULL) return;
    
    Medecin* current = list->head;
    while (current != NULL) {
        Medecin* next = current->next;
        free(current);
        current = next;
    }
    
    free(list);
}

// Convert speciality enum to string
const char* specialityToString(Speciality speciality) {
    switch (speciality) {
        case GENERAL_MEDICINE: return "Médecine Générale";
        case CARDIOLOGY: return "Cardiologie";
        case NEUROLOGY: return "Neurologie";
        case PEDIATRICS: return "Pédiatrie";
        case ORTHOPEDICS: return "Orthopédie";
        case DERMATOLOGY: return "Dermatologie";
        case OPHTHALMOLOGY: return "Ophtalmologie";
        case OTHER: return "Autre";
        default: return "Inconnue";
    }
}

// Convert string to speciality enum
Speciality stringToSpeciality(const char* str) {
    if (strcmp(str, "Médecine Générale") == 0) return GENERAL_MEDICINE;
    if (strcmp(str, "Cardiologie") == 0) return CARDIOLOGY;
    if (strcmp(str, "Neurologie") == 0) return NEUROLOGY;
    if (strcmp(str, "Pédiatrie") == 0) return PEDIATRICS;
    if (strcmp(str, "Orthopédie") == 0) return ORTHOPEDICS;
    if (strcmp(str, "Dermatologie") == 0) return DERMATOLOGY;
    if (strcmp(str, "Ophtalmologie") == 0) return OPHTHALMOLOGY;
    return OTHER;
}

// Create a new doctor
Medecin* createMedecin(const char* id, const char* firstName, const char* lastName, 
                      const char* phone, Speciality speciality, const char* specialityStr) {
    Medecin* medecin = (Medecin*)malloc(sizeof(Medecin));
    if (medecin == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le médecin\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(medecin->medecinId, id, MAX_ID_LENGTH - 1);
    medecin->medecinId[MAX_ID_LENGTH - 1] = '\0';
    
    strncpy(medecin->firstName, firstName, MAX_NAME_LENGTH - 1);
    medecin->firstName[MAX_NAME_LENGTH - 1] = '\0';
    
    strncpy(medecin->lastName, lastName, MAX_NAME_LENGTH - 1);
    medecin->lastName[MAX_NAME_LENGTH - 1] = '\0';
    
    strncpy(medecin->phoneNumber, phone, MAX_PHONE_LENGTH - 1);
    medecin->phoneNumber[MAX_PHONE_LENGTH - 1] = '\0';
    
    medecin->speciality = speciality;
    
    if (speciality == OTHER && specialityStr != NULL) {
        strncpy(medecin->specialityStr, specialityStr, MAX_SPECIALITY_LENGTH - 1);
        medecin->specialityStr[MAX_SPECIALITY_LENGTH - 1] = '\0';
    } else {
        strncpy(medecin->specialityStr, specialityToString(speciality), MAX_SPECIALITY_LENGTH - 1);
        medecin->specialityStr[MAX_SPECIALITY_LENGTH - 1] = '\0';
    }
    
    medecin->next = NULL;
    
    return medecin;
}

// Add a doctor to the list
void addMedecin(MedecinList* list, Medecin* medecin) {
    if (list == NULL || medecin == NULL) return;
    
    // Add at the beginning of the list for simplicity
    medecin->next = list->head;
    list->head = medecin;
    list->count++;
}

// Find a doctor by ID
Medecin* findMedecinById(MedecinList* list, const char* id) {
    if (list == NULL || id == NULL) return NULL;
    
    Medecin* current = list->head;
    while (current != NULL) {
        if (strcmp(current->medecinId, id) == 0) {
            return current;
        }
        current = current->next;
    }
    
    return NULL; // Doctor not found
}

// Remove a doctor from the list
int removeMedecin(MedecinList* list, const char* id) {
    if (list == NULL || id == NULL || list->head == NULL) return 0;
    
    // If the doctor to remove is the head
    if (strcmp(list->head->medecinId, id) == 0) {
        Medecin* temp = list->head;
        list->head = list->head->next;
        free(temp);
        list->count--;
        return 1;
    }
    
    // Search for the doctor in the rest of the list
    Medecin* current = list->head;
    while (current->next != NULL) {
        if (strcmp(current->next->medecinId, id) == 0) {
            Medecin* temp = current->next;
            current->next = current->next->next;
            free(temp);
            list->count--;
            return 1;
        }
        current = current->next;
    }
    
    return 0; // Doctor not found
}

// Update doctor information
void updateMedecinInfo(Medecin* medecin, const char* firstName, const char* lastName, 
                      const char* phone, Speciality speciality, const char* specialityStr) {
    if (medecin == NULL) return;
    
    if (firstName != NULL) {
        strncpy(medecin->firstName, firstName, MAX_NAME_LENGTH - 1);
        medecin->firstName[MAX_NAME_LENGTH - 1] = '\0';
    }
    
    if (lastName != NULL) {
        strncpy(medecin->lastName, lastName, MAX_NAME_LENGTH - 1);
        medecin->lastName[MAX_NAME_LENGTH - 1] = '\0';
    }
    
    if (phone != NULL) {
        strncpy(medecin->phoneNumber, phone, MAX_PHONE_LENGTH - 1);
        medecin->phoneNumber[MAX_PHONE_LENGTH - 1] = '\0';
    }
    
    if (speciality != medecin->speciality) {
        medecin->speciality = speciality;
        
        if (speciality == OTHER && specialityStr != NULL) {
            strncpy(medecin->specialityStr, specialityStr, MAX_SPECIALITY_LENGTH - 1);
            medecin->specialityStr[MAX_SPECIALITY_LENGTH - 1] = '\0';
        } else {
            strncpy(medecin->specialityStr, specialityToString(speciality), MAX_SPECIALITY_LENGTH - 1);
            medecin->specialityStr[MAX_SPECIALITY_LENGTH - 1] = '\0';
        }
    }
}

// Display doctor information
void displayMedecin(Medecin* medecin) {
    if (medecin == NULL) return;
    
    printf("ID: %s\n", medecin->medecinId);
    printf("Nom: %s %s\n", medecin->firstName, medecin->lastName);
    printf("Téléphone: %s\n", medecin->phoneNumber);
    printf("Spécialité: %s\n", medecin->specialityStr);
}

// Display all doctors
void displayAllMedecins(MedecinList* list) {
    if (list == NULL || list->head == NULL) {
        printf("Aucun médecin enregistré.\n");
        return;
    }
    
    printf("Liste des médecins (%d):\n", list->count);
    printf("--------------------------------------------------\n");
    
    Medecin* current = list->head;
    int count = 1;
    while (current != NULL) {
        printf("%d. %s %s (ID: %s) - %s\n", 
               count++, current->firstName, current->lastName, 
               current->medecinId, current->specialityStr);
        current = current->next;
    }
    printf("--------------------------------------------------\n");
}

// Doctor management menu
void medecinMenu(MedecinList* medecins) {
    int choice = 0;
    do {
        clearScreen();
        printf("===== GESTION DES MÉDECINS =====\n");
        printf("1. Ajouter un médecin\n");
        printf("2. Rechercher un médecin\n");
        printf("3. Modifier un médecin\n");
        printf("4. Supprimer un médecin\n");
        printf("5. Afficher tous les médecins\n");
        printf("6. Retour au menu principal\n");
        printf("Votre choix: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch (choice) {
            case 1: {
                // Add a new doctor
                char id[MAX_ID_LENGTH];
                char firstName[MAX_NAME_LENGTH];
                char lastName[MAX_NAME_LENGTH];
                char phone[MAX_PHONE_LENGTH];
                int specialityChoice;
                char specialityStr[MAX_SPECIALITY_LENGTH] = "";
                
                printf("ID du médecin: ");
                readString(id, MAX_ID_LENGTH);
                
                // Check if ID already exists
                if (findMedecinById(medecins, id) != NULL) {
                    printf("Un médecin avec cet ID existe déjà.\n");
                    waitForKeypress();
                    break;
                }
                
                printf("Prénom: ");
                readString(firstName, MAX_NAME_LENGTH);
                
                printf("Nom: ");
                readString(lastName, MAX_NAME_LENGTH);
                
                printf("Téléphone: ");
                readString(phone, MAX_PHONE_LENGTH);
                
                printf("Spécialité:\n");
                printf("1. Médecine Générale\n");
                printf("2. Cardiologie\n");
                printf("3. Neurologie\n");
                printf("4. Pédiatrie\n");
                printf("5. Orthopédie\n");
                printf("6. Dermatologie\n");
                printf("7. Ophtalmologie\n");
                printf("8. Autre\n");
                printf("Votre choix: ");
                scanf("%d", &specialityChoice);
                clearInputBuffer();
                
                Speciality speciality;
                switch (specialityChoice) {
                    case 1: speciality = GENERAL_MEDICINE; break;
                    case 2: speciality = CARDIOLOGY; break;
                    case 3: speciality = NEUROLOGY; break;
                    case 4: speciality = PEDIATRICS; break;
                    case 5: speciality = ORTHOPEDICS; break;
                    case 6: speciality = DERMATOLOGY; break;
                    case 7: speciality = OPHTHALMOLOGY; break;
                    case 8: 
                        speciality = OTHER;
                        printf("Précisez la spécialité: ");
                        readString(specialityStr, MAX_SPECIALITY_LENGTH);
                        break;
                    default: 
                        speciality = GENERAL_MEDICINE;
                        printf("Choix invalide, Médecine Générale sera utilisée par défaut.\n");
                }
                
                Medecin* newMedecin = createMedecin(id, firstName, lastName, phone, speciality, specialityStr);
                addMedecin(medecins, newMedecin);
                
                printf("Médecin ajouté avec succès!\n");
                waitForKeypress();
                break;
            }
            case 2: {
                // Search for a doctor
                char id[MAX_ID_LENGTH];
                printf("Entrez l'ID du médecin: ");
                readString(id, MAX_ID_LENGTH);
                
                Medecin* medecin = findMedecinById(medecins, id);
                if (medecin != NULL) {
                    displayMedecin(medecin);
                } else {
                    printf("Médecin non trouvé.\n");
                }
                waitForKeypress();
                break;
            }
            case 3: {
                // Update doctor information
                char id[MAX_ID_LENGTH];
                printf("Entrez l'ID du médecin à modifier: ");
                readString(id, MAX_ID_LENGTH);
                
                Medecin* medecin = findMedecinById(medecins, id);
                if (medecin != NULL) {
                    char firstName[MAX_NAME_LENGTH];
                    char lastName[MAX_NAME_LENGTH];
                    char phone[MAX_PHONE_LENGTH];
                    int specialityChoice;
                    char specialityStr[MAX_SPECIALITY_LENGTH] = "";
                    
                    printf("Nouveau prénom (laissez vide pour ne pas modifier): ");
                    readString(firstName, MAX_NAME_LENGTH);
                    
                    printf("Nouveau nom (laissez vide pour ne pas modifier): ");
                    readString(lastName, MAX_NAME_LENGTH);
                    
                    printf("Nouveau téléphone (laissez vide pour ne pas modifier): ");
                    readString(phone, MAX_PHONE_LENGTH);
                    
                    printf("Nouvelle spécialité (0 pour ne pas modifier):\n");
                    printf("1. Médecine Générale\n");
                    printf("2. Cardiologie\n");
                    printf("3. Neurologie\n");
                    printf("4. Pédiatrie\n");
                    printf("5. Orthopédie\n");
                    printf("6. Dermatologie\n");
                    printf("7. Ophtalmologie\n");
                    printf("8. Autre\n");
                    printf("Votre choix: ");
                    scanf("%d", &specialityChoice);
                    clearInputBuffer();
                    
                    Speciality speciality = medecin->speciality;
                    if (specialityChoice > 0 && specialityChoice <= 8) {
                        switch (specialityChoice) {
                            case 1: speciality = GENERAL_MEDICINE; break;
                            case 2: speciality = CARDIOLOGY; break;
                            case 3: speciality = NEUROLOGY; break;
                            case 4: speciality = PEDIATRICS; break;
                            case 5: speciality = ORTHOPEDICS; break;
                            case 6: speciality = DERMATOLOGY; break;
                            case 7: speciality = OPHTHALMOLOGY; break;
                            case 8: 
                                speciality = OTHER;
                                printf("Précisez la spécialité: ");
                                readString(specialityStr, MAX_SPECIALITY_LENGTH);
                                break;
                        }
                    }
                    
                    updateMedecinInfo(medecin, 
                                     firstName[0] != '\0' ? firstName : NULL,
                                     lastName[0] != '\0' ? lastName : NULL,
                                     phone[0] != '\0' ? phone : NULL,
                                     speciality,
                                     specialityStr[0] != '\0' ? specialityStr : NULL);
                    
                    printf("Informations du médecin mises à jour avec succès!\n");
                } else {
                    printf("Médecin non trouvé.\n");
                }
                waitForKeypress();
                break;
            }
            case 4: {
                // Delete a doctor
                char id[MAX_ID_LENGTH];
                printf("Entrez l'ID du médecin à supprimer: ");
                readString(id, MAX_ID_LENGTH);
                
                if (removeMedecin(medecins, id)) {
                    printf("Médecin supprimé avec succès!\n");
                } else {
                    printf("Médecin non trouvé.\n");
                }
                waitForKeypress();
                break;
            }
            case 5: {
                // Display all doctors
                displayAllMedecins(medecins);
                waitForKeypress();
                break;
            }
            case 6:
                // Return to main menu
                break;
            default:
                printf("Option invalide. Veuillez réessayer.\n");
                waitForKeypress();
        }
    } while (choice != 6);
}