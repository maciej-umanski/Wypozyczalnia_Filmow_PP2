#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "inne.h"

int input_int(int a, int b){
    char bufortab[10], *koniec;
    scanf("%s", bufortab);
    int bufor = strtol(bufortab, &koniec, 10);
    while ((*koniec == *bufortab) || (bufor < a) || (bufor > b)) {
        printf("\nB³¹d, z³a dana wejœciowa. Proszê wpisz ponownie.\n<< ");
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
        printf("\nB³¹d, z³a dana wejœciowa. Proszê wpisz ponownie.\n<< ");
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
        printf("\nB³¹d, z³a dana wejœciowa. Proszê wpisz ponownie.\n<< ");
        scanf("%s", bufortab);
        bufor = strtod(bufortab, &koniec);
    }
    return bufor;
}

void czekaj_na_input_ESCAPE(){
    puts("\n\n>> Wciœnij ESCAPE aby powróciæ.");
    while(getch() != 27){

    }
}

void wyczysc_ekran(){
    system("cls");
}
