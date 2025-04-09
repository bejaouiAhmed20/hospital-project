#ifndef MEDECIN_H
#define MEDECIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define maximum string lengths
#define MAX_NAME_LENGTH 50
#define MAX_ID_LENGTH 20
#define MAX_PHONE_LENGTH 15
#define MAX_SPECIALITY_LENGTH 50

// Enumeration for doctor specialities
typedef enum {
    GENERAL_MEDICINE,
    CARDIOLOGY,
    NEUROLOGY,
    PEDIATRICS,
    ORTHOPEDICS,
    DERMATOLOGY,
    OPHTHALMOLOGY,
    OTHER
} Speciality;

// Doctor structure
typedef struct Medecin {
    char medecinId[MAX_ID_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char phoneNumber[MAX_PHONE_LENGTH];
    Speciality speciality;
    char specialityStr[MAX_SPECIALITY_LENGTH]; // For custom specialities
    struct Medecin* next;
} Medecin;

// Doctor list structure (linked list)
typedef struct {
    Medecin* head;
    int count;
} MedecinList;

// Function prototypes
MedecinList* createMedecinList();
void freeMedecinList(MedecinList* list);
Medecin* createMedecin(const char* id, const char* firstName, const char* lastName, 
                      const char* phone, Speciality speciality, const char* specialityStr);
void addMedecin(MedecinList* list, Medecin* medecin);
Medecin* findMedecinById(MedecinList* list, const char* id);
int removeMedecin(MedecinList* list, const char* id);
void updateMedecinInfo(Medecin* medecin, const char* firstName, const char* lastName, 
                      const char* phone, Speciality speciality, const char* specialityStr);
void medecinMenu(MedecinList* medecins);
void displayMedecin(Medecin* medecin);
void displayAllMedecins(MedecinList* list);
const char* specialityToString(Speciality speciality);
Speciality stringToSpeciality(const char* str);

#endif // MEDECIN_H