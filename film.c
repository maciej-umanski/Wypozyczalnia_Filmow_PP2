#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "film.h"


struct film{
    int numer_filmu;
    int sztuki_dostepne;
    int sztuki_wypozyczone;
    int rok_produkcji;
    char tytul[30];
    char rezyser[20];
    char gatunek[10];
    struct film *nastepny;
};

void film_dodaj(struct film **head_film, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]){

    struct film *film_nowy = (struct film *)malloc(sizeof(struct film));

    if((*head_film)==NULL){
        film_nowy -> numer_filmu = 0;
    }
    else{
        film_nowy -> numer_filmu = (*head_film)->numer_filmu+1;
    }

    film_nowy -> sztuki_dostepne = sztuki_dostepne;
    film_nowy -> sztuki_wypozyczone = 0;
    film_nowy -> nastepny = (*head_film);
    film_nowy -> rok_produkcji =  rok_produkcji;
    strcpy(film_nowy -> tytul, tytul);
    strcpy(film_nowy -> rezyser, rezyser);
    strcpy(film_nowy -> gatunek, gatunek);
    *head_film = film_nowy;
}

void film_usun(struct film **head_film, struct film *film_usuwany_poprzedni){
    struct film *film_bufor = NULL;
    if(film_usuwany_poprzedni == NULL){
        film_bufor = (*head_film) -> nastepny;
        free(*head_film);
        (*head_film) = film_bufor;
    } else {
        film_bufor = film_usuwany_poprzedni -> nastepny;
        film_usuwany_poprzedni -> nastepny = film_bufor -> nastepny;
        free(film_bufor);
    }
}

void film_edytuj(struct film **film_edytowany, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]){
    (*film_edytowany) -> sztuki_dostepne = sztuki_dostepne;
    (*film_edytowany) -> rok_produkcji = rok_produkcji;
    strcpy((*film_edytowany) -> tytul, tytul);
    strcpy((*film_edytowany) -> rezyser, rezyser);
    strcpy((*film_edytowany) -> gatunek, gatunek);
}

struct film *film_szukaj_po_numerze(struct film **head_film, int numer_filmu){
    struct film *film_bufor = *head_film;
    while(film_bufor -> numer_filmu != numer_filmu){
        film_bufor = film_bufor->nastepny;
    }
    return film_bufor;
}

struct film *film_szukaj_po_kolejnosci(struct film **head_film, int kolejnosc){
    struct film *film_bufor = *head_film;
    for(int i = 1 ; i != kolejnosc; i++){
        film_bufor = film_bufor->nastepny;
    }
    return film_bufor;
}

struct film *film_szukaj_po_kolejnosci_dostepne(struct film **head_film, int kolejnosc){
    struct film *film_bufor = *head_film;
    int i = 0;
    while(i != kolejnosc){
        if(film_bufor->sztuki_dostepne >0){
            i++;
        }
        if(i == kolejnosc){
            break;
        }
        film_bufor = film_bufor->nastepny;
    }
    return film_bufor;
}

struct film *film_szukaj_po_kolejnosci_dostepne_poprzedni(struct film **head_film, int kolejnosc){
    struct film *film_bufor = *head_film;
    struct film *poprzedni = NULL;
    int i = 0;
    while(i != kolejnosc){
        if(film_bufor->sztuki_dostepne >0){
            i++;
        }
        if(i == kolejnosc){
            break;
        }
        poprzedni = film_bufor;
        film_bufor = film_bufor->nastepny;
    }
    return poprzedni;
}

int film_licz(struct film *head_film){
    int ilosc = 0;
    while(head_film != NULL){
            ilosc++;
        head_film = head_film -> nastepny;
    }
    return ilosc;
}

int film_licz_dostepne(struct film *head_film){
    int ilosc = 0;
    while(head_film != NULL){
        if(head_film -> sztuki_dostepne > 0){
            ilosc++;
        }
        head_film = head_film -> nastepny;
    }
    return ilosc;
}

int film_wypisz(struct film *head_film){

    if(head_film == NULL){
        puts(">> Baza filmów jest pusta!");
        return -1;
    }

    printf(">> Nr. |%30s |%20s |%10s | Rok produkcji | Dostêpne | Wypo¿yczone |\n\n", "Tytu³", "Re¿yser", "Gatunek");

    for(int i = 1 ; head_film != NULL; i++) {
        printf(">> %2d. |%30s |%20s |%10s | %13d |   %2d     |     %2d      |\n", i, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_dostepne, head_film->sztuki_wypozyczone);
        head_film = head_film -> nastepny;
    }
    return 0;
}

int film_wypisz_dostepne(struct film *head_film){

    system("cls");
    puts(">> Dostêpne filmy");

    if(head_film == NULL){
        puts(">> Baza filmów jest pusta!");
        return -1;
    }

    printf(">> Nr. |%30s |%20s |%10s | Rok produkcji | Dostêpne sztuki\n\n", "Tytu³", "Re¿yser", "Gatunek");

    int i;
    for(i = 1 ; head_film != NULL;) {
        if(head_film -> sztuki_dostepne > 0) {
            printf(">> %2d. |%30s |%20s |%10s | %13d | %2d\n", i, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_dostepne);
            i++;
        }
        head_film = head_film -> nastepny;
    }

    if(i == 1){
        system("cls");
        puts(">> ¯aden film nie jest aktualnie dostêpny!");
        return -1;
    }

    return 0;
}

bool film_czy_istnieje(struct film *head_film, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]){
    while(head_film != NULL){
        if((rok_produkcji - head_film->rok_produkcji) == 0 && strcmp(tytul, head_film->tytul) == 0 && strcmp(rezyser, head_film->rezyser) == 0 && strcmp(gatunek, head_film->gatunek) == 0){
            return true;
        }
        head_film = head_film->nastepny;
    }
    return false;
}

bool film_czy_jedna_sztuka_poprzedni(struct film **head_film, struct film *film_sprawdzany_poprzedni){
    if(film_sprawdzany_poprzedni == NULL){
        if((*head_film) -> sztuki_wypozyczone == 0 && (*head_film) -> sztuki_dostepne == 1) {
            return false;
        }
    }
    else if((film_sprawdzany_poprzedni -> nastepny -> sztuki_wypozyczone == 0 ) && (film_sprawdzany_poprzedni -> nastepny -> sztuki_dostepne == 1)){
        return false;
    }
    return true;
}

bool film_zapisz_do_pliku(struct film *head_film){
    FILE *file = fopen("film.db", "w");
    if (file == NULL)  {
        return false;
    } else {
        while (head_film != NULL){
            fprintf(file, "%d %d %d %d %s %s %s\n", head_film->numer_filmu, head_film->sztuki_dostepne, head_film->sztuki_wypozyczone, head_film->rok_produkcji, head_film->tytul, head_film->rezyser, head_film->gatunek);
            head_film = head_film->nastepny;
        }
    }
    fclose(file);
    return true;
}

void film_zamien_spacje_na_tylde(struct film *head_film){
    while(head_film != NULL){
        for(int i = 0; i < strlen(head_film->rezyser); i++){
            if(head_film->rezyser[i] == 32){
                head_film->rezyser[i] = 96;
            }
        }
        for(int i = 0; i < strlen(head_film->tytul); i++){
            if(head_film->tytul[i] == 32){
                head_film->tytul[i] = 96;
            }
        }
        for(int i = 0; i < strlen(head_film->gatunek); i++){
            if(head_film->gatunek[i] == 32){
                head_film->gatunek[i] = 96;
            }
        }
        head_film = head_film->nastepny;
    }
}

void film_zamien_tylde_na_spacje(struct film *head_film){
    if(head_film == NULL){
        return;
    }
    while(head_film != NULL){
        for(int i = 0; i < strlen(head_film->rezyser); i++){
            if(head_film->rezyser[i] == 96){
                head_film->rezyser[i] = 32;
            }
        }
        for(int i = 0; i < strlen(head_film->tytul); i++){
            if(head_film->tytul[i] == 96){
                head_film->tytul[i] = 32;
            }
        }
        for(int i = 0; i < strlen(head_film->gatunek); i++){
            if(head_film->gatunek[i] == 96){
                head_film->gatunek[i] = 32;
            }
        }
        head_film = head_film->nastepny;
    }
}

bool film_wczytaj_z_pliku(struct film **head_film){
    FILE *file = fopen("film.db", "r");
    if (file == NULL) {
        return false;
    }
    else{
        int numer_filmu, sztuki_dostepne, sztuki_wypozyczone, rok_produkcji;
        char tytul[30], rezyser[20], gatunek[10];
        while(fscanf(file, "%d %d %d %d %s %s %s", &numer_filmu, &sztuki_dostepne, &sztuki_wypozyczone, &rok_produkcji, tytul, rezyser, gatunek) != EOF){
            if(*head_film == NULL){
                *head_film = (struct film *)malloc(sizeof(struct film));
                (*head_film) -> numer_filmu = numer_filmu;
                (*head_film) -> sztuki_dostepne = sztuki_dostepne;
                (*head_film) -> sztuki_wypozyczone = sztuki_wypozyczone;
                (*head_film) -> rok_produkcji =  rok_produkcji;
                (*head_film) -> nastepny = NULL;
                strcpy((*head_film) -> tytul, tytul);
                strcpy((*head_film) -> rezyser, rezyser);
                strcpy((*head_film) -> gatunek, gatunek);
            }
            else {
                struct film *film_nowy = *head_film;
                while (film_nowy->nastepny != NULL) {
                    film_nowy = film_nowy->nastepny;
                }
                film_nowy->nastepny = (struct film *)malloc(sizeof(struct film));
                film_nowy->nastepny -> numer_filmu = numer_filmu;
                film_nowy->nastepny -> sztuki_dostepne = sztuki_dostepne;
                film_nowy->nastepny -> sztuki_wypozyczone = sztuki_wypozyczone;
                film_nowy->nastepny -> rok_produkcji = rok_produkcji;
                film_nowy->nastepny -> nastepny = NULL;
                strcpy(film_nowy->nastepny -> tytul, tytul);
                strcpy(film_nowy->nastepny -> rezyser, rezyser);
                strcpy(film_nowy->nastepny -> gatunek, gatunek);
            }
        }
        fclose(file);
    }
    return true;
}