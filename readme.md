# TEXT EDITOR

Program umożliwiający użytkownikowi podstawową edycje plików tekstowych. Pozwala na tworzenie, otwieranie, edycje oraz analize plików. Umożliwiają mu to **funkcje** takie jak:

1. createfile()
2. openFile()
3. appendToFile()
4. readFile()
5. countTextStats()

W dodatku program wyświetla podstawowe GUI za pomocą funckji **displayMenu()**. Ponadto oferuje funkcje do czyszczenia bufforu pamięci **clearBuffer()** 

### createFile():

Funkcja `createFile()` prosi użytkownika o podanie nazwy pliku który ma utworzyć oraz umożliwia pierwsze dopisanie tekstu do pliku. Ponadto używa funckji `my_getline()` aby odczytać zawartość pliku i zapisać ją do danego pliku.

### openFile():

Funkcja `openFile()` jest prostą funkcją która umożliwia otworzenie pliku danego nazwą wpisaną przez użytkownika w tzw. **read mode**


### appendToFile():

Funkcja `appendToFile()` umożliwia dopisanie tekstu do istniejącego już pliku. Prosi użytkownika o wpisanie nazwy pliku oraz tekstu który ma być dopisany

### readFile():

Funkcja `readFile()` umożliwia wyświetlenie całej zawartości pliku danego nazwą wpisaną przez użytkownika

### countTextStats():

Funkcja `countTextStats()` analizuje plik dany nazwą wpisaną przez użytkownika. Funkcja ta czyta plik znak po znaku i zlicza je wszystkie oraz rozpoznaje kiedy konczy się słowo.

---

Dodatkowo program zawiera w sobie własną implementacje funkcji `getline()` mianowicie `my_getline()`, która ma identyczne działanie

### my_getline():

Funkcja której deklaracja wygląda tak:

`long my_getline(char **lineptr, size_t *n, FILE *stream)`

Czyta ona całą linie tekstu w danym pliku oraz trzyma wartości buforu który zawiera jakikolwiek teskst w `*lineptr`. `*n` wskaźnik który odpowiada za rozmiar buforu, `stream` jest to ciąg który funkcja "czyta".

### Omówienie poszczególnych części funckji:

```
if (*lineptr == NULL || *n == 0) {
    *n = 128; // Initial buffer size
    *lineptr = (char *)malloc(*n);
    if (*lineptr == NULL) {
        return -1; // Memory allocation error
    }
}
```

Wstępna alokacja buforu pamięci. Zasada działania to: jeżeli `*lineptr` ma wartość `NULL` albo jeśli `*n` ma wartość `0` to bufor ustawiany jest na 128 bajtów oraz jest tu obsługa błędów gdy alokacja pamięci nie powiedzie się to zwraca `-1`.

```
char *ptr = *lineptr;
size_t len = 0;
int ch;

while ((ch = fgetc(stream)) != EOF) {
    if (len + 1 >= *n) {
        *n *= 2;
        char *new_ptr = realloc(*lineptr, *n);
        if (new_ptr == NULL) {
            return -1; // Memory allocation error
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
```

"Czytanie" wszystkich znaków:
1. Utworznenie `*ptr` aby wskazywał na początek buforu oraz `len = 0`
2. "Czytanie" wszystkich znaków za pomocą funkcji `fgetc`
3. Jeżeli bufor jest zapełniony (`len + 1 >= *n`), podwaja swój rozmiar za pomocą `realloc`
4. Trzymanie odczytanego znaku w buforze oraz przejście do następnego
5. Jeżeli następny znak jest to `\n` to pętla jest przerywana
6. Ostatnia część to wyświetlanie koncowego efektu gdy program nie odczytał żadnego znaku (`len == 0`) zwraca `-1` jeżeli nie to wyczyść buffor i zwróć długość odczytanej lini (`len`)