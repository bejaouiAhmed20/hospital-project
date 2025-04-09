#ifndef RENDEZ_VOUS_H
#define RENDEZ_VOUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "patient.h"
#include "medecin.h"

// Define maximum string lengths
#define MAX_ID_LENGTH 20
#define MAX_NOTES_LENGTH 500

// Appointment structure
typedef struct RendezVous {
    char rendezVousId[MAX_ID_LENGTH];
    char patientId[MAX_ID_LENGTH];
    char medecinId[MAX_ID_LENGTH];
    struct tm dateTime;
    char notes[MAX_NOTES_LENGTH];
    struct RendezVous* next;
} RendezVous;

// Appointment list structure (linked list)
typedef struct {
    RendezVous* head;
    int count;
} RendezVousList;

// Function prototypes
RendezVousList* createRendezVousList();
void freeRendezVousList(RendezVousList* list);
RendezVous* createRendezVous(const char* id, const char* patientId, const char* medecinId, 
                           struct tm dateTime, const char* notes);
void addRendezVous(RendezVousList* list, RendezVous* rendezVous);
RendezVous* findRendezVousById(RendezVousList* list, const char* id);
int removeRendezVous(RendezVousList* list, const char* id);
void updateRendezVousNotes(RendezVous* rendezVous, const char* notes);
void rendezVousMenu(RendezVousList* rendezVous, PatientList* patients, MedecinList* medecins);
void displayRendezVous(RendezVous* rendezVous, PatientList* patients, MedecinList* medecins);
void displayAllRendezVous(RendezVousList* list, PatientList* patients, MedecinList* medecins);
RendezVous** findRendezVousByPatientId(RendezVousList* list, const char* patientId, int* count);
RendezVous** findRendezVousByMedecinId(RendezVousList* list, const char* medecinId, int* count);
int isDateTimeAvailable(RendezVousList* list, const char* medecinId, struct tm dateTime);

#endif // RENDEZ_VOUS_H