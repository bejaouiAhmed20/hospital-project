#include "../include/ui.h"
#ifdef _WIN32
#include <windows.h>
#endif

// Color codes for Windows console
#define COLOR_RESET 7
#define COLOR_BLUE 9
#define COLOR_GREEN 10
#define COLOR_CYAN 11
#define COLOR_RED 12
#define COLOR_MAGENTA 13
#define COLOR_YELLOW 14
#define COLOR_WHITE 15

// Set console text color (Windows)
void setColor(int color) {
    #ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    #endif
}

// Clear the console screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Set console to use UTF-8 encoding (for Windows)
void setConsoleEncoding() {
    #ifdef _WIN32
    // Set console output code page to UTF-8
    SetConsoleOutputCP(65001);
    #endif
}

// Clear the input buffer
void clearInputBuffer() {
    int c;
    // Keep reading characters until we hit a newline or EOF
    while ((c = getchar()) != '\n' && c != EOF);
}

// Wait for a keypress
void waitForKeypress() {
    printf("\nAppuyez sur Entree pour continuer...");
    // The issue is here - we need to make sure we're properly waiting for input
    // and not skipping over it due to leftover characters in the buffer
    int c;
    // First, check if there's already a newline in the buffer
    if ((c = getchar()) != '\n') {
        // If not a newline, clear the rest of the buffer
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

// Read a string from stdin
void readString(char* buffer, int maxLength) {
    if (buffer == NULL || maxLength <= 0) return;
    
    // Make sure we get actual input
    if (fgets(buffer, maxLength, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    
    // Remove newline character if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

// Display the main menu and get user choice
int displayMainMenu() {
    clearScreen();
    
    printf("=== SYSTEME DE GESTION DES PATIENTS - HOPITAL CENTRAL ===\n\n");
    printf("1. Gestion des patients\n");
    printf("2. Gestion des medecins\n");
    printf("3. Gestion des rendez-vous\n");
    printf("4. Quitter\n");
    
    printf("\nVotre choix: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        choice = 0; // Invalid input
    }
    clearInputBuffer();
    
    return choice;
}

// Display welcome message
void displayWelcomeMessage() {
    clearScreen();
    printf("=== SYSTEME DE GESTION DES PATIENTS - HOPITAL CENTRAL ===\n\n");
    printf("Bienvenue dans le systeme de gestion hospitaliere\n");
    waitForKeypress();
}

// Simplified menu header
void displayMenuHeader(const char* title) {
    printf("\n=== %s ===\n\n", title);
}

// Simplified menu option
void displayMenuOption(int number, const char* text) {
    printf("%d. %s\n", number, text);
}

// Simplified menu footer
void displayMenuFooter() {
    printf("\n");
}

// Get menu choice with better error handling
int getMenuChoice() {
    printf("\nVotre choix: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        choice = 0; // Invalid input
    }
    clearInputBuffer();
    
    return choice;
}

// Display error messages
void displayError(const char* message) {
    printf("\nErreur: %s\n", message);
}

// Display success messages
void displaySuccess(const char* message) {
    printf("\nSucces: %s\n", message);
}