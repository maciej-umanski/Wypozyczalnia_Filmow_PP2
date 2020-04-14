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

int film_dodaj(struct film **head_film, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]){

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
    return 0;
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

    printf(">> Nr. |%30s |%20s |%10s | Rok produkcji | Dostępne | Wypożyczone |\n\n", "Tytuł", "Reżyser", "Gatunek");

    for(int i = 1 ; head_film != NULL; i++) {
        printf(">> %2d. |%30s |%20s |%10s | %13d |   %2d     |     %2d      |\n", i, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_dostepne, head_film->sztuki_wypozyczone);
        head_film = head_film -> nastepny;
    }
    return 0;
}

int film_wypisz_dostepne(struct film *head_film){

    system("cls");
    puts(">> Dostępne filmy");

    if(head_film == NULL){
        puts(">> Baza filmów jest pusta!");
        return -1;
    }

    printf(">> Nr. |%30s |%20s |%10s | Rok produkcji | Dostępne sztuki\n\n", "Tytuł", "Reżyser", "Gatunek");

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
        puts(">> Żaden film nie jest aktualnie dostępny!");
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
