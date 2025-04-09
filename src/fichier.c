// Update the file paths to use the correct location
#include "../include/fichier.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// File paths - change from relative to absolute paths
#define PATIENTS_FILE "c:\\Users\\bejao\\Desktop\\haroun project\\data\\patients.txt"
#define MEDECINS_FILE "c:\\Users\\bejao\\Desktop\\haroun project\\data\\medecins.txt"
#define RENDEZ_VOUS_FILE "c:\\Users\\bejao\\Desktop\\haroun project\\data\\rendez_vous.txt"

// Save patients to file
void savePatients(PatientList* patients) {
    if (patients == NULL) return;
    
    FILE* file = fopen(PATIENTS_FILE, "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", PATIENTS_FILE);
        return;
    }
    
    Patient* current = patients->head;
    while (current != NULL) {
        // Format: ID|FirstName|LastName|Phone|Address|DOB_Day|DOB_Month|DOB_Year|Diagnosis|Treatment
        fprintf(file, "%s|%s|%s|%s|%s|%d|%d|%d|%s|%s\n",
                current->patientId,
                current->firstName,
                current->lastName,
                current->phoneNumber,
                current->address,
                current->dateOfBirth.tm_mday,
                current->dateOfBirth.tm_mon + 1,  // tm_mon is 0-11
                current->dateOfBirth.tm_year + 1900,  // tm_year is years since 1900
                current->diagnosis,
                current->treatment);
        
        current = current->next;
    }
    
    fclose(file);
}

// Save doctors to file
void saveMedecins(MedecinList* medecins) {
    if (medecins == NULL) return;
    
    FILE* file = fopen(MEDECINS_FILE, "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", MEDECINS_FILE);
        return;
    }
    
    Medecin* current = medecins->head;
    while (current != NULL) {
        // Format: ID|FirstName|LastName|Phone|SpecialityEnum|SpecialityStr
        fprintf(file, "%s|%s|%s|%s|%d|%s\n",
                current->medecinId,
                current->firstName,
                current->lastName,
                current->phoneNumber,
                current->speciality,
                current->specialityStr);
        
        current = current->next;
    }
    
    fclose(file);
}

// Save appointments to file
void saveRendezVous(RendezVousList* rendezVous) {
    if (rendezVous == NULL) return;
    
    FILE* file = fopen(RENDEZ_VOUS_FILE, "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", RENDEZ_VOUS_FILE);
        return;
    }
    
    RendezVous* current = rendezVous->head;
    while (current != NULL) {
        // Format: ID|PatientID|MedecinID|Day|Month|Year|Hour|Minute|Notes
        fprintf(file, "%s|%s|%s|%d|%d|%d|%d|%d|%s\n",
                current->rendezVousId,
                current->patientId,
                current->medecinId,
                current->dateTime.tm_mday,
                current->dateTime.tm_mon + 1,  // tm_mon is 0-11
                current->dateTime.tm_year + 1900,  // tm_year is years since 1900
                current->dateTime.tm_hour,
                current->dateTime.tm_min,
                current->notes);
        
        current = current->next;
    }
    
    fclose(file);
}

// Save all data to files
void saveAllData(PatientList* patients, MedecinList* medecins, RendezVousList* rendezVous) {
    savePatients(patients);
    saveMedecins(medecins);
    saveRendezVous(rendezVous);
}

// Load patients from file
void loadPatients(PatientList* patients) {
    if (patients == NULL) return;
    
    FILE* file = fopen(PATIENTS_FILE, "r");
    if (file == NULL) {
        // File might not exist yet, which is fine
        return;
    }
    
    char line[MAX_NAME_LENGTH + MAX_ID_LENGTH + MAX_PHONE_LENGTH + 
              MAX_ADDRESS_LENGTH + MAX_DIAGNOSIS_LENGTH + MAX_TREATMENT_LENGTH + 50];
    
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        // Parse the line
        char id[MAX_ID_LENGTH];
        char firstName[MAX_NAME_LENGTH];
        char lastName[MAX_NAME_LENGTH];
        char phone[MAX_PHONE_LENGTH];
        char address[MAX_ADDRESS_LENGTH];
        int day, month, year;
        char diagnosis[MAX_DIAGNOSIS_LENGTH];
        char treatment[MAX_TREATMENT_LENGTH];
        
        // Format: ID|FirstName|LastName|Phone|Address|DOB_Day|DOB_Month|DOB_Year|Diagnosis|Treatment
        int result = sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d|%[^|]|%[^\n]",
                           id, firstName, lastName, phone, address, &day, &month, &year, diagnosis, treatment);
        
        if (result >= 8) {  // At least the basic info is present
            struct tm dob = {0};
            dob.tm_mday = day;
            dob.tm_mon = month - 1;  // tm_mon is 0-11
            dob.tm_year = year - 1900;  // tm_year is years since 1900
            
            Patient* patient = createPatient(id, firstName, lastName, phone, address, dob);
            
            // Add diagnosis and treatment if present
            if (result >= 10 && diagnosis[0] != '\0') {
                assignDiagnosis(patient, diagnosis, treatment);
            }
            
            addPatient(patients, patient);
        }
    }
    
    fclose(file);
}

// Load doctors from file
void loadMedecins(MedecinList* medecins) {
    if (medecins == NULL) return;
    
    FILE* file = fopen(MEDECINS_FILE, "r");
    if (file == NULL) {
        // File might not exist yet, which is fine
        return;
    }
    
    char line[MAX_NAME_LENGTH + MAX_ID_LENGTH + MAX_PHONE_LENGTH + MAX_SPECIALITY_LENGTH + 50];
    
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        // Parse the line
        char id[MAX_ID_LENGTH];
        char firstName[MAX_NAME_LENGTH];
        char lastName[MAX_NAME_LENGTH];
        char phone[MAX_PHONE_LENGTH];
        int specialityEnum;
        char specialityStr[MAX_SPECIALITY_LENGTH];
        
        // Format: ID|FirstName|LastName|Phone|SpecialityEnum|SpecialityStr
        int result = sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%[^\n]",
                           id, firstName, lastName, phone, &specialityEnum, specialityStr);
        
        if (result >= 5) {  // At least the basic info is present
            Speciality speciality = (Speciality)specialityEnum;
            
            Medecin* medecin = createMedecin(id, firstName, lastName, phone, speciality, 
                                           result >= 6 ? specialityStr : NULL);
            
            addMedecin(medecins, medecin);
        }
    }
    
    fclose(file);
}

// Load appointments from file
void loadRendezVous(RendezVousList* rendezVous) {
    if (rendezVous == NULL) return;
    
    FILE* file = fopen(RENDEZ_VOUS_FILE, "r");
    if (file == NULL) {
        // File might not exist yet, which is fine
        return;
    }
    
    char line[MAX_ID_LENGTH * 3 + MAX_NOTES_LENGTH + 50];
    
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        
        // Parse the line
        char id[MAX_ID_LENGTH];
        char patientId[MAX_ID_LENGTH];
        char medecinId[MAX_ID_LENGTH];
        int day, month, year, hour, minute;
        char notes[MAX_NOTES_LENGTH];
        
        // Format: ID|PatientID|MedecinID|Day|Month|Year|Hour|Minute|Notes
        int result = sscanf(line, "%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%d|%[^\n]",
                           id, patientId, medecinId, &day, &month, &year, &hour, &minute, notes);
        
        if (result >= 8) {  // At least the basic info is present
            struct tm dateTime = {0};
            dateTime.tm_mday = day;
            dateTime.tm_mon = month - 1;  // tm_mon is 0-11
            dateTime.tm_year = year - 1900;  // tm_year is years since 1900
            dateTime.tm_hour = hour;
            dateTime.tm_min = minute;
            
            RendezVous* rdv = createRendezVous(id, patientId, medecinId, dateTime, 
                                             result >= 9 ? notes : NULL);
            
            addRendezVous(rendezVous, rdv);
        }
    }
    
    fclose(file);
}

// Load all data from files
void loadAllData(PatientList* patients, MedecinList* medecins, RendezVousList* rendezVous) {
    loadPatients(patients);
    loadMedecins(medecins);
    loadRendezVous(rendezVous);
}