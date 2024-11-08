#include <stdio.h> // Standard input/output definitions
#include <stdlib.h> // Standard library definitions (malloc, free, etc.)
#include <string.h> // String handling functions
#include <errno.h> // Error number definitions

// Function declarations
void createFile();
void openFile();
void appendToFile();
void readFile();
void countTextStats();
void displayMenu();
void clearBuffer();
long my_getline(char **lineptr, size_t *n, FILE *stream);

int main() {
    int choice;
    do {
        displayMenu();
        printf("Wybierz opcje: ");
        if (scanf("%d", &choice) != 1) {
            clearBuffer();
            printf("Niepoprawna opcja, sprobuj ponownie. \n");
            continue;
        }
        clearBuffer();

        switch (choice) {
        case 1:
            createFile();
            break;
        case 2:
            openFile();
            break;
        case 3:
            appendToFile();
            break;
        case 4:
            readFile();
            break;
        case 5:
            countTextStats();
            break;
        case 0:
            printf("Koniec programu. \n");
            break;
        default:
            printf("Niepoprawna opcja, sprobuj ponownie. \n");
            break;
        }
    } while (choice != 0);

    return 0;
}

void displayMenu() {
    printf("\n--- MENU ---\n");
    printf("1. Stworz nowy plik\n");
    printf("2. Otworz istniejacy plik\n");
    printf("3. Dopisz tekst do pliku\n");
    printf("4. Wyswietl zawartosc pliku\n");
    printf("5. Policz statystyki tekstu (slowa, znaki)\n");
    printf("0. Wyjscie\n");
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Custom implementation of getline()
long my_getline(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128; // Initial buffer size
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            perror("Memory allocation error");
            return -1;
        }
    }

    char *ptr = *lineptr;
    size_t len = 0;
    int ch;

    while ((ch = fgetc(stream)) != EOF) {
        if (len + 1 >= *n) {
            *n *= 2;
            char *new_ptr = realloc(*lineptr, *n);
            if (new_ptr == NULL) {
                perror("Memory allocation error");
                return -1;
            }
            *lineptr = new_ptr;
            ptr = *lineptr + len;
        }

        *ptr++ = ch;
        len++;

        if (ch == '\n') {
            break;
        }
    }

    if (len == 0) {
        return -1; // No characters read
    }

    *ptr = '\0'; // Null-terminate the buffer
    return len;
}

void createFile() {
    char *fileName = NULL;
    size_t fileNameSize = 0;
    char *text = NULL;
    size_t textSize = 0;

    printf("Podaj nazwe pliku do utworzenia: ");
    if (my_getline(&fileName, &fileNameSize, stdin) == -1) {
        perror("Error reading file name");
        return;
    }
    strtok(fileName, "\n");

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        perror("Error creating file");
        free(fileName);
        return;
    }

    printf("Podaj tekst do zapisania w pliku (maksymalnie 1000 znakow):\n");
    if (my_getline(&text, &textSize, stdin) == -1) {
        perror("Error reading text");
        free(fileName);
        fclose(file);
        return;
    }

    fprintf(file, "%s", text);
    fclose(file);

    printf("Plik %s zostal utworzony\n", fileName);

    free(fileName);
    free(text);
}

void openFile() {
    char *fileName = NULL;
    size_t fileNameSize = 0;

    printf("Podaj nazwe pliku do otwarcia: ");
    if (my_getline(&fileName, &fileNameSize, stdin) == -1) {
        perror("Error reading file name");
        return;
    }
    strtok(fileName, "\n");

    printf("Proba otwarcia pliku %s\n", fileName);

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        free(fileName);
        return;
    }

    printf("Plik %s otwarty pomyslnie\n", fileName);
    fclose(file);

    free(fileName);
}

void appendToFile() {
    char *fileName = NULL;
    size_t fileNameSize = 0;
    char *text = NULL;
    size_t textSize = 0;

    printf("Podaj nazwe pliku, ktory chcesz edytowac: ");
    if (my_getline(&fileName, &fileNameSize, stdin) == -1) {
        perror("Error reading file name");
        return;
    }
    strtok(fileName, "\n");

    FILE *file = fopen(fileName, "a");
    if (file == NULL) {
        perror("Error opening file");
        free(fileName);
        return;
    }

    printf("Podaj tekst ktory chcesz dopisac:\n");
    if (my_getline(&text, &textSize, stdin) == -1) {
        perror("Error reading text");
        free(fileName);
        fclose(file);
        return;
    }

    fprintf(file, "%s", text);
    fclose(file);

    printf("Tekst zostal dodany do pliku %s\n", fileName);

    free(fileName);
    free(text);
}

void readFile() {
    char *fileName = NULL;
    size_t fileNameSize = 0;
    char ch;

    printf("Podaj nazwe pliku do odczytu: ");
    if (my_getline(&fileName, &fileNameSize, stdin) == -1) {
        perror("Error reading file name");
        return;
    }
    strtok(fileName, "\n");

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        free(fileName);
        return;
    }

    printf("Zawartosc pliku %s:\n", fileName);
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);

    free(fileName);
}

void countTextStats() {
    char *fileName = NULL;
    size_t fileNameSize = 0;
    char ch;
    int charCount = 0, wordCount = 0, inWord = 0;

    printf("Podaj nazwe pliku do analizy: ");
    if (my_getline(&fileName, &fileNameSize, stdin) == -1) {
        perror("Error reading file name");
        return;
    }
    strtok(fileName, "\n");

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        free(fileName);
        return;
    }

    while ((ch = fgetc(file)) != EOF) {
        charCount++;
        if (ch == ' ' || ch == '\n' || ch == '\t') {
            if (inWord) {
                wordCount++;
                inWord = 0;
            }
        } else {
            inWord = 1;
        }
    }
    if (inWord) wordCount++;

    printf("Plik %s zawiera %d znakow i %d slow/a/o\n", fileName, charCount, wordCount);
    fclose(file);

    free(fileName);
}