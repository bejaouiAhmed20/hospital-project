#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/patient.h"
#include "../include/medecin.h"
#include "../include/rendez_vous.h"
#include "../include/fichier.h"
#include "../include/ui.h"

int main() {
    // Set console encoding for proper display of accented characters
    setConsoleEncoding();
    
    // Initialize data structures
    PatientList* patients = createPatientList();
    MedecinList* medecins = createMedecinList();
    RendezVousList* rendezVous = createRendezVousList();
    
    // Load data from files - REMOVED
    // loadAllData(patients, medecins, rendezVous);
    
    // Display welcome message
    displayWelcomeMessage();
    
    int choice = 0;
    do {
        // Display main menu and get user choice
        choice = displayMainMenu();
        
        // Process user choice
        switch (choice) {
            case 1: // Patient management
                patientMenu(patients);
                break;
            case 2: // Doctor management
                medecinMenu(medecins);
                break;
            case 3: // Appointment management
                rendezVousMenu(rendezVous, patients, medecins);
                break;
            case 4: // Exit (previously Save data)
                printf("Au revoir!\n");
                break;
            default:
                printf("Option invalide. Veuillez réessayer.\n");
                waitForKeypress();
        }
    } while (choice != 4); // Changed from 5 to 4
    
    // Free allocated memory
    freePatientList(patients);
    freeMedecinList(medecins);
    freeRendezVousList(rendezVous);
    
    return 0;
}