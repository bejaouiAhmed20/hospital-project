#ifndef FICHIER_H
#define FICHIER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "patient.h"
#include "medecin.h"
#include "rendez_vous.h"

// Function prototypes
void savePatients(PatientList* patients);
void saveMedecins(MedecinList* medecins);
void saveRendezVous(RendezVousList* rendezVous);
void saveAllData(PatientList* patients, MedecinList* medecins, RendezVousList* rendezVous);

void loadPatients(PatientList* patients);
void loadMedecins(MedecinList* medecins);
void loadRendezVous(RendezVousList* rendezVous);
void loadAllData(PatientList* patients, MedecinList* medecins, RendezVousList* rendezVous);

#endif // FICHIER_H