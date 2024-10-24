#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void createFile();
void openFile();
void appendToFile();
void readFile();
void countTextStats();
void displayMenu();
void clearBuffer();
long my_getline(char **lineptr, size_t *n, FILE *stream); // Deklaracja własnej funkcji getline

int main() {
    int choice;
    do {
        displayMenu();
        printf("Wybierz opcje: ");
        scanf("%d", &choice);
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

// Własna implementacja funkcji getline()
long my_getline(char **lineptr, size_t *n, FILE *stream) {
    if (*lineptr == NULL || *n == 0) {
        *n = 128; // Rozpoczynamy od początkowego rozmiaru bufora
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            return -1; // Błąd alokacji pamięci
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
                return -1; // Błąd alokacji pamięci
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
        return -1; // Nic nie wczytano
    }

    *ptr = '\0';
    return len;
}

void createFile() {
    char *fileName = NULL;
    size_t fileNameSize = 0;
    char *text = NULL;
    size_t textSize = 0;

    printf("Podaj nazwe pliku do utworzenia: ");
    my_getline(&fileName, &fileNameSize, stdin);
    strtok(fileName, "\n");

    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Blad przy tworzeniu pliku !\n");
        free(fileName);
        return;
    }

    printf("Podaj tekst do zapisania w pliku (maksymalnie 1000 znakow):\n");
    my_getline(&text, &textSize, stdin);

    fprintf(file, "%s", text);
    fclose(file);

    printf("Plik %s zostal utworzony\n", fileName);

    // Zwolnienie pamięci
    free(fileName);
    free(text);
}

void openFile() {
    char *fileName = NULL;
    size_t fileNameSize = 0;

    printf("Podaj nazwe pliku do otwarcia: ");
    my_getline(&fileName, &fileNameSize, stdin);
    strtok(fileName, "\n");

    printf("Proba otwarcia pliku %s\n", fileName);

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Plik %s nie istnieje\n", fileName);
        free(fileName);
        return;
    }

    printf("Plik %s otwarty pomyslnie\n", fileName);
    fclose(file);

    // Zwolnienie pamięci
    free(fileName);
}

void appendToFile() {
    char *fileName = NULL;
    size_t fileNameSize = 0;
    char *text = NULL;
    size_t textSize = 0;

    printf("Podaj nazwe pliku, ktory chcesz edytowac: ");
    my_getline(&fileName, &fileNameSize, stdin);
    strtok(fileName, "\n");

    FILE *file = fopen(fileName, "a");
    if (file == NULL) {
        printf("Blad przy otwieraniu pliku!\n");
        free(fileName);
        return;
    }

    printf("Podaj tekst ktory chcesz dopisac:\n");
    my_getline(&text, &textSize, stdin);

    fprintf(file, "%s", text);
    fclose(file);

    printf("Tekst zostal dodany do pliku %s\n", fileName);

    // Zwolnienie pamięci
    free(fileName);
    free(text);
}

void readFile() {
    char *fileName = NULL;
    size_t fileNameSize = 0;
    char ch;

    printf("Podaj nazwe pliku do odczytu: ");
    my_getline(&fileName, &fileNameSize, stdin);
    strtok(fileName, "\n");

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Plik %s nie istnieje !\n", fileName);
        free(fileName);
        return;
    }

    printf("Zawartosc pliku %s:\n", fileName);
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);

    // Zwolnienie pamięci
    free(fileName);
}

void countTextStats() {
    char *fileName = NULL;
    size_t fileNameSize = 0;
    char ch;
    int charCount = 0, wordCount = 0, inWord = 0;

    printf("Podaj nazwe pliku do analizy: ");
    my_getline(&fileName, &fileNameSize, stdin);
    strtok(fileName, "\n");

    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Plik %s nie istnieje !\n", fileName);
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

    printf("Plik %s zawiera %d znakow i %d slow\n", fileName, charCount, wordCount);
    fclose(file);

    // Zwolnienie pamięci
    free(fileName);
}
