#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "inne.h"
#include <sys/stat.h>
#include <errno.h>
#include <io.h>

/**
* \brief Walidacja danych typu int wprowadzanych przez użytkownika.
*
* Funkcja sprawdza czy w podanym ciągu znaków wprowadzanym przez użytkownika nie znajdują
* się żadne litery czy znaki interpunkcyjne i zwraca jedynie liczby całkowite.
* @param a Najmniejsza akceptowana wprowadzona liczba.
* @param b Najwieksza akceptowana wprowadzona liczba.
* @return Liczba wprowadzona przez użytkownika.
*
*/

int input_int(int a, int b){
    char bufortab[10], *koniec;
    scanf("%s", bufortab);
    int bufor = strtol(bufortab, &koniec, 10);
    while ((*koniec == *bufortab) || (bufor < a) || (bufor > b)) {
        printf("\nBłąd, zła dana wejściowa. Proszę wpisz ponownie.\n<< ");
        scanf("%s", bufortab);
        bufor = strtol(bufortab, &koniec, 10);
    }
    return bufor;
}

/**
* \brief Walidacja danych typu unsigned int wprowadzanych przez użytkownika.
*
* Funkcja sprawdza czy w podanym ciągu znaków wprowadzanym przez użytkownika nie znajdują
* się żadne litery czy znaki interpunkcyjne i zwraca jedynie liczby całkowite większe od zera.
* @param a Najmniejsza akceptowana wprowadzona liczba.
* @param b Najwieksza akceptowana wprowadzona liczba.
* @return Liczba wprowadzona przez użytkownika.
*
*/

unsigned int input_uint(unsigned int a, unsigned int b){
    char bufortab[10], *koniec;
    scanf("%s", bufortab);
    unsigned int bufor = strtol(bufortab, &koniec, 10);
    while (((*koniec == *bufortab) || (bufor < a) || (bufor > b)) || bufor < 0) {
        printf("\nBłąd, zła dana wejściowa. Proszę wpisz ponownie.\n<< ");
        scanf("%s", bufortab);
        bufor = strtol(bufortab, &koniec, 10);
    }
    return bufor;
}

/**
* \brief Walidacja danych typu unsigned long long int wprowadzanych przez użytkownika.
*
* Funkcja sprawdza czy w podanym ciągu znaków wprowadzanym przez użytkownika nie znajdują
* się żadne litery czy znaki interpunkcyjne i zwraca jedynie liczby całkowite.
* @param a Najmniejsza akceptowana wprowadzona liczba.
* @param b Najwieksza akceptowana wprowadzona liczba.
* @return Liczba wprowadzona przez użytkownika.
*
*/

unsigned long long input_ull(unsigned long long a, unsigned long long b){
    char bufortab[15], *koniec;
    scanf("%s", bufortab);
    unsigned long long bufor = strtoull(bufortab, &koniec, 10);
    while ((*koniec == *bufortab) || (bufor < a) || (bufor > b)){
        printf("\nBłąd, zła dana wejściowa. Proszę wpisz ponownie.\n<< ");
        memset(bufortab, 0, 11 * sizeof(char));
        scanf("%s", bufortab);
        bufor = strtoull(bufortab, &koniec, 10);
    }
    return bufor;
}

/**
* \brief Walidacja danych typu double wprowadzanych przez użytkownika.
*
* Funkcja sprawdza czy w podanym ciągu znaków wprowadzanym przez użytkownika nie znajdują
* się żadne litery czy znaki interpunkcyjne i zwraca jedynie liczby rzeczywiste.
* @param a Najmniejsza akceptowana wprowadzona liczba.
* @param b Najwieksza akceptowana wprowadzona liczba.
* @return Liczba wprowadzona przez użytkownika.
*
*/

double input_dbl(double a, double b){
    char bufortab[10], *koniec;
    scanf("%s", bufortab);
    double bufor = strtod(bufortab, &koniec);
    while ((*koniec == *bufortab) || (bufor < a) || (bufor > b)) {
        printf("\nBłąd, zła dana wejściowa. Proszę wpisz ponownie.\n<< ");
        scanf("%s", bufortab);
        bufor = strtod(bufortab, &koniec);
    }
    return bufor;
}

/**
 * \brief Oczekiwanie na wciśnięcie klawisza ESC na klawiaturze
 */

void czekaj_na_input_ESCAPE(){
    puts("\n\n>> Wcisnij ESCAPE aby powrócić.");
    while(getch() != 27){

    }
}

/**
 * \brief Usuwanie całej wyświetlonej zawartości terminala
 */

void wyczysc_ekran(){
    system("cls");
}

/**
 * \brief Dodawanie folderu
 *
 * Funkcja tworzy folder. Jeśli folder został utworzony, lub już istnieje, to zwraca prawdę.
 * W przeciwnym wypadku wyświetla komunikat oraz kod błędu wraz z wyjaśnieniem i zwraca fałsz.
 * @param folder[] nazwa folderu do utworzenia
 * @return true w przypadku powodzenia, false w przypadku błędu
 */

bool dodaj_folder(char folder[]) {
    int errnum;
    int dir_result = mkdir(folder);
    if (dir_result != 0 && errno != 17) {
        errnum = errno;
        fprintf(stderr, "Nie można utworzyć folderu.\n");
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Błąd wypisany przez funkcję perror");
        fprintf(stderr, "Błąd utworzenia folderu: %s\n", strerror(errnum));

    } else {
        return true;
    }
}

/**
 * \brief Pobieranie łańcucha znaków
 *
 * Funkcja pobiera łańcuch o określonej długości, usuwając z niego znak końca linii oraz czyszcząc
 * strumień wejściowy.
 * @param *result wskaźnik zawierający podany łańcuch
 * @param max maksymalna ilość znaków
 */

void input_string(char *result, int max) {
    do {
        fgets(result, max, stdin);
    }while(result[0] < 48);
    strtok(result, "\n");
    fflush(stdin);
}