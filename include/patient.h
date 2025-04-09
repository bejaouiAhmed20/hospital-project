#ifndef PATIENT_H
#define PATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Define maximum string lengths
#define MAX_NAME_LENGTH 50
#define MAX_ID_LENGTH 20
#define MAX_PHONE_LENGTH 15
#define MAX_ADDRESS_LENGTH 100
#define MAX_DIAGNOSIS_LENGTH 200
#define MAX_TREATMENT_LENGTH 200

// Patient structure
typedef struct Patient {
    char patientId[MAX_ID_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char phoneNumber[MAX_PHONE_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char diagnosis[MAX_DIAGNOSIS_LENGTH];
    char treatment[MAX_TREATMENT_LENGTH];
    struct tm dateOfBirth;
    struct Patient* next;
} Patient;

// Patient list structure (linked list)
typedef struct {
    Patient* head;
    int count;
} PatientList;

// Function prototypes
PatientList* createPatientList();
void freePatientList(PatientList* list);
Patient* createPatient(const char* id, const char* firstName, const char* lastName, 
                      const char* phone, const char* address, struct tm dob);
void addPatient(PatientList* list, Patient* patient);
Patient* findPatientById(PatientList* list, const char* id);
int removePatient(PatientList* list, const char* id);
void updatePatientInfo(Patient* patient, const char* firstName, const char* lastName, 
                      const char* phone, const char* address);
void assignDiagnosis(Patient* patient, const char* diagnosis, const char* treatment);
void patientMenu(PatientList* patients);
void displayPatient(Patient* patient);
void displayAllPatients(PatientList* list);

#endif // PATIENT_H