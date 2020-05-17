#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "inne.h"
#include <sys/stat.h>
#include <errno.h>
#include <io.h>

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

void czekaj_na_input_ESCAPE(){
    puts("\n\n>> Wcisnij ESCAPE aby powrócić.");
    while(getch() != 27){

    }
}

void wyczysc_ekran(){
    system("cls");
}

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

void input_string(char *result, int max) {
    fgets(result, max, stdin);
    fflush(stdin);
}