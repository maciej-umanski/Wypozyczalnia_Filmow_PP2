#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "film.h"


struct film{
    unsigned int id_filmu;
    int sztuki_dostepne;
    int sztuki_wypozyczone;
    int rok_produkcji;
    char tytul[30];
    char rezyser[20];
    char gatunek[10];
    struct film *nastepny;
};

bool film_dodaj(struct film **head_film, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]){
        if(!dodaj_folder("data")) return false;
        if(!dodaj_folder("data/id")) return false;
        FILE *file = fopen("data/id/last_id_film.db", "r");
        if (file==NULL && (*head_film)!=NULL) {
            return false;
        }
        unsigned int id;
        char temp[10];
        fgets(temp, 10, file);
        sscanf(temp,"%d", &id);
        fclose(file);

    struct film *film_nowy = (struct film *)malloc(sizeof(struct film));
        if((*head_film)== NULL) {
            if(!file) {
                film_nowy->id_filmu = 1;
                id = 1;
            }
            else {
                film_nowy->id_filmu = ++id;
            }
        }
        else {
            film_nowy->id_filmu = ++id;
        }
    film_nowy -> sztuki_dostepne = sztuki_dostepne;
    film_nowy -> sztuki_wypozyczone = 0;
    film_nowy -> nastepny = (*head_film);
    film_nowy -> rok_produkcji =  rok_produkcji;
    strcpy(film_nowy -> tytul, tytul);
    strcpy(film_nowy -> rezyser, rezyser);
    strcpy(film_nowy -> gatunek, gatunek);
    *head_film = film_nowy;

        FILE *file2 = fopen("data/id/last_id_film.db", "w");
        fprintf(file2, "%d", id);
        fclose(file2);
        return true;
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

struct film *film_szukaj_po_numerze(struct film **head_film, unsigned int id_filmu){
    struct film *film_bufor = *head_film;
    while(film_bufor -> id_filmu != id_filmu){
        film_bufor = film_bufor->nastepny;
    }
    return film_bufor;
}

struct film *film_szukaj_po_kolejnosci(struct film **head_film, unsigned int id){
    struct film *film_bufor = *head_film;
    for(int i = 1 ; i != id; i++){
        film_bufor = film_bufor->nastepny;
    }
    return film_bufor;
}

struct film *film_szukaj_po_kolejnosci_dostepne(struct film **head_film, unsigned int id){
    /*
    struct film *film_bufor = *head_film;
    int i = 0;
    while(i != id){
        if(film_bufor->sztuki_dostepne >0){
            i++;
        }
        if(i == id){
            break;
        }
        film_bufor = film_bufor->nastepny;
    }
    return film_bufor;
    */
    struct film *film_bufor = *head_film;
    while (id != film_bufor->id_filmu) {
        film_bufor = film_bufor -> nastepny;
    }
    return film_bufor;
}

struct film *film_szukaj_po_kolejnosci_dostepne_poprzedni(struct film **head_film, unsigned int id){
    struct film *film_bufor = *head_film;
    struct film *poprzedni = NULL;
    /*
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
    */
    int i=0;
    while(film_bufor != NULL) {
        if(id == film_bufor->id_filmu) {
            i++;
            break;
        }
        poprzedni = film_bufor;
        film_bufor = film_bufor->nastepny;
    }
    if(i==0) return NULL;
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
        puts(">> Baza film�w jest pusta!");
        return -1;
    }

    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypo�yczone | Dost�pne |\n\n", "Tytu�", "Re�yser", "Gatunek");

    while(head_film != NULL) {
        printf(">> %2d. |%30s |%20s |%10s | %13d | %11d | %8d |\n", head_film->id_filmu, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
               head_film->sztuki_wypozyczone, head_film->sztuki_dostepne);
        head_film = head_film -> nastepny;
    }
    return 0;
}

int film_wypisz_dostepne(struct film *head_film){

    system("cls");
    puts(">> Dost�pne filmy");

    if(head_film == NULL){
        puts(">> Baza film�w jest pusta!");
        return -1;
    }

    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypo�yczone | Dost�pne |\n\n", "Tytu�", "Re�yser", "Gatunek");

    int i;
    for(i = 1 ; head_film != NULL;) {
        if(head_film -> sztuki_dostepne > 0) {
            printf(">> %2d. |%30s |%20s |%10s | %13d | %11d | %8d |\n", head_film->id_filmu, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_wypozyczone, head_film->sztuki_dostepne);
            i++;
        }
        head_film = head_film -> nastepny;
    }

    if(i == 1){
        system("cls");
        puts(">> �aden film nie jest aktualnie dost�pny!");
        return -1;
    }

    return 0;
}

int film_wypisz_sztuki_dostepne(struct film *head_film, int sztuki_dostepne){
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypo�yczone | Dost�pne |\n\n", "Tytu�", "Re�yser", "Gatunek");
    int i;
    for(i = 1 ; head_film != NULL;) {
        if(head_film -> sztuki_dostepne == sztuki_dostepne) {
            printf(">> %2d. |%30s |%20s |%10s | %13d | %11d | %8d |\n", head_film->id_filmu, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_wypozyczone, head_film->sztuki_dostepne);
            i++;
        }
        head_film = head_film -> nastepny;
    }

    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono �adnego filmu!");
        return -1;
    }

    return 0;
}

int film_wypisz_sztuki_wypozyczone(struct film *head_film, int sztuki_wypozyczone){
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypo�yczone | Dost�pne |\n\n", "Tytu�", "Re�yser", "Gatunek");
    int i;
    for(i = 1 ; head_film != NULL;) {
        if(head_film -> sztuki_wypozyczone == sztuki_wypozyczone) {
            printf(">> %2d. |%30s |%20s |%10s | %13d | %11d | %8d |\n", head_film->id_filmu, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_wypozyczone, head_film->sztuki_dostepne);
            i++;
        }
        head_film = head_film -> nastepny;
    }

    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono �adnego filmu!");
        return -1;
    }

    return 0;
}

int film_wypisz_rok_produkcji(struct film *head_film, int rok_produkcji){
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypo�yczone | Dost�pne |\n\n", "Tytu�", "Re�yser", "Gatunek");
    int i;
    for(i = 1 ; head_film != NULL;) {
        if(head_film -> rok_produkcji == rok_produkcji) {
            printf(">> %2d. |%30s |%20s |%10s | %13d | %11d | %8d |\n", head_film->id_filmu, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_wypozyczone, head_film->sztuki_dostepne);
            i++;
        }
        head_film = head_film -> nastepny;
    }

    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono �adnego filmu!");
        return -1;
    }

    return 0;
}

int film_wypisz_tytul(struct film *head_film, char tytul[]){
    tytul = strlwr(tytul);
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypo�yczone | Dost�pne |\n\n", "Tytu�", "Re�yser", "Gatunek");
    int i;
    for(i = 1;head_film != NULL;) {
        strlwr(head_film->tytul);
        int z=0, n=0;
        do {
            if (tytul[n] != head_film->tytul[n]) z++;
            n++;
        } while(strlen(tytul) > n);
        if(z==0) {
            printf(">> %2d. |%30s |%20s |%10s | %13d | %11d | %8d |\n", head_film->id_filmu, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_wypozyczone, head_film->sztuki_dostepne);
            i++;
        }
        head_film = head_film->nastepny;

    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono ?adnego filmu!");
        return -1;
    }
    return 0;
}

int film_wypisz_rezyser(struct film *head_film, char rezyser[]){
    rezyser = strlwr(rezyser);
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypo�yczone | Dost�pne |\n\n", "Tytu�", "Re�yser", "Gatunek");
    int i;
    for(i = 1;head_film != NULL;) {
        strlwr(head_film->rezyser);
        int z=0, n=0;
        do {
            if (rezyser[n] != head_film->rezyser[n]) z++;
            n++;
        } while(strlen(rezyser) > n);
        if(z==0) {
            printf(">> %2d. |%30s |%20s |%10s | %13d | %11d | %8d |\n", head_film->id_filmu, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_wypozyczone, head_film->sztuki_dostepne);
            i++;
        }
        head_film = head_film->nastepny;

    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono ?adnego filmu!");
        return -1;
    }
    return 0;
}

int film_wypisz_gatunek(struct film *head_film, char gatunek[]){
    gatunek = strlwr(gatunek);
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypo�yczone | Dost�pne |\n\n", "Tytu�", "Re�yser", "Gatunek");
    int i;
    for(i = 1;head_film != NULL;) {
        strlwr(head_film->gatunek);
        int z=0, n=0;
        do {
            if (gatunek[n] != head_film->gatunek[n]) z++;
            n++;
        } while(strlen(gatunek) > n);
        if(z==0) {
            printf(">> %2d. |%30s |%20s |%10s | %13d | %11d | %8d |\n", head_film->id_filmu, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_wypozyczone, head_film->sztuki_dostepne);
            i++;
        }
        head_film = head_film->nastepny;

    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono ?adnego filmu!");
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
    if(!dodaj_folder("data")) return false;
    if(!dodaj_folder("data/databases")) return false;
    FILE *file = fopen("data/databases/film.db", "w");
    if (file == NULL)  {
        return false;
    } else {
        while (head_film != NULL){
            fprintf(file, "%d %d %d %d %s %s %s\n", head_film->id_filmu, head_film->sztuki_dostepne, head_film->sztuki_wypozyczone, head_film->rok_produkcji, head_film->tytul, head_film->rezyser, head_film->gatunek);
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
    if(!dodaj_folder("data")) return false;
    if(!dodaj_folder("data/databases")) return false;
    FILE *file = fopen("data/databases/film.db", "r");
    if (file == NULL) {
        return false;
    }
    else{
        unsigned int id_filmu;
        int sztuki_dostepne, sztuki_wypozyczone, rok_produkcji;
        char tytul[30], rezyser[20], gatunek[10];
        while(fscanf(file, "%d %d %d %d %s %s %s", &id_filmu, &sztuki_dostepne, &sztuki_wypozyczone, &rok_produkcji, tytul, rezyser, gatunek) != EOF){
            if(*head_film == NULL){
                *head_film = (struct film *)malloc(sizeof(struct film));
                (*head_film) -> id_filmu = id_filmu;
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
                film_nowy->nastepny -> id_filmu = id_filmu;
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