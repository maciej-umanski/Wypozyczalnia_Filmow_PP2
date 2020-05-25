#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "film.h"

/**
 * \brief Struktura przechowująca filmy
 * @param id_filmu identyfikator filmu
 * @param sztuki_dostepne ilość sztuk dostępnych do wypożyczenia
 * @param sztuki_wypozyczone ilość sztuk aktualnie wypożyczonych
 * @param rok_produkcji Rok produkcji filmu
 * @param tytul[30] tytuł filmu
 * @param rezyser[20] reżyser filmu
 * @param gatunek[10] gatunek filmu
 * @param *nastepny wskaźnik na następny element
 */

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

/**
 * \brief Dodanie filmu do listy
 * @param **head_film wskaźnik na listę filmów
 * @param sztuki_dostepne Ilość dostępnych sztuk filmu
 * @param rok_produkcji rok produkcji filmu
 * @param tytul[] tytuł filmu
 * @param rezyser[] reżyser filmu
 * @param gatunek[] gatunek filmu
 * @return true - dodanie udane, false - dodanie nieudane
 */

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

/**
 * \brief Usuwanie elementu z listy filmów
 * @param **head_film wskaźnik na listę filmów
 * @param *film_usuwany_poprzedni wskaźnik na element poprzedzający element usuwany
 */

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

/**
 * \brief Edycja elementu z listy flmów
 * @param **film_edytowany wskaźnik na edytowany element
 * @param sztuki_dostepne ilosc dostepnych sztuk filmu
 * @param rok_produkcji rok produkcji filmu
 * @param tytul[] tytuł filmu
 * @param rezyser[] reżyser filmu
 * @param gatunek[] gatunek filmu
 */

void film_edytuj(struct film **film_edytowany, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]){
    (*film_edytowany) -> sztuki_dostepne = sztuki_dostepne;
    (*film_edytowany) -> rok_produkcji = rok_produkcji;
    strcpy((*film_edytowany) -> tytul, tytul);
    strcpy((*film_edytowany) -> rezyser, rezyser);
    strcpy((*film_edytowany) -> gatunek, gatunek);
}

/**
 * \brief Funkcja dodająca element podczas sortowania
 * @param **head_film wskaźnik na listę, do której zostanie dodany nowy element
 * @param **bufor wskaźnik na element przechowujący dane filmu
 */

void push_film (struct film **head_film, struct film **bufor){
    struct film *film_nowy = (struct film *)malloc(sizeof(struct film));
    film_nowy->id_filmu = (*bufor)->id_filmu;
    film_nowy -> sztuki_dostepne = (*bufor)->sztuki_dostepne;
    film_nowy -> sztuki_wypozyczone = (*bufor)->sztuki_wypozyczone;
    film_nowy -> rok_produkcji =  (*bufor)->rok_produkcji;
    strcpy(film_nowy -> tytul, (*bufor)->tytul);
    strcpy(film_nowy -> rezyser, (*bufor)->rezyser);
    strcpy(film_nowy -> gatunek, (*bufor)->gatunek);
    film_nowy->nastepny = (*head_film);
    *head_film = film_nowy;
}

/**
 * \brief Przeszukiwanie listy filmów po identyfikatorze
 * @param **head_film wskaźnik na listę filmów
 * @param id_filmu identyfikator poszukiwanego elementu
 * @return element o szukanym ID
 */

struct film *film_szukaj_po_numerze(struct film **head_film, unsigned int id_filmu){
    struct film *film_bufor = *head_film;
    while(film_bufor -> id_filmu != id_filmu){
        film_bufor = film_bufor->nastepny;
    }
    return film_bufor;
}

/**
 * \brief Przeszukiwanie listy filmów po identyfikatorze
 * @param **head_film wskaźnik na listę filmów
 * @param id poszukiwany identyfikator
 * @return powodzenie - element listy, niepowodzenie - NULL
 */

struct film *film_szukaj_po_kolejnosci(struct film **head_film, unsigned int id){
    struct film *film_bufor = *head_film;
    int i=0;
    while(film_bufor != NULL) {
        if(id == film_bufor->id_filmu) {
            i++;
            break;
        }
        film_bufor = film_bufor->nastepny;
    }
    if(i==0) return NULL;
    return film_bufor;
}

/**
 * \brief Przeszukiwanie listy dostępnych filmów po identyfikatorze
 * @param **head_film wskaźnik na listę filmów
 * @param id poszukiwany identyfikator
 * @return powodzenie - element listy, niepowodzenie - NULL
 */

struct film *film_szukaj_po_kolejnosci_dostepne(struct film **head_film, unsigned int id){
    struct film *film_bufor = *head_film;
    int i=0;
    while(film_bufor != NULL) {
        if(id == film_bufor->id_filmu) {
            i++;
            break;
        }
        film_bufor = film_bufor->nastepny;
    }
    if(i==0) return NULL;
    return film_bufor;
}

/**
 * \brief Przeszukiwanie listy dostępnych filmów po identyfikatorze
 * @param **head_film wskaźnik na listę filmów
 * @param id poszukiwany identyfikator
 * @return powodzenie - poprzedzający element listy, niepowodzenie - NULL
 */

struct film *film_szukaj_po_kolejnosci_dostepne_poprzedni(struct film **head_film, unsigned int id){
    struct film *film_bufor = *head_film;
    struct film *poprzedni = NULL;
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

/**
 * \brief Wypisywanie listy filmów
 * @param *head_film wskaźnik na listę filmów
 * return 0 - powodzenie, -1 - lista jest pusta
 */

int film_wypisz(struct film *head_film){

    if(head_film == NULL){
        puts(">> Baza filmów jest pusta!");
        return -1;
    }

    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypożyczone | Dostępne |\n\n", "Tytuł", "Reżyser", "Gatunek");

    while(head_film != NULL) {
        printf(">> %2d. |%30s |%20s |%10s | %13d | %11d | %8d |\n", head_film->id_filmu, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
               head_film->sztuki_wypozyczone, head_film->sztuki_dostepne);
        head_film = head_film -> nastepny;
    }
    return 0;
}

/**
 * \brief Wypisywanie listy dostępnych filmów
 * @param *head_film wskaźnik na listę filmów
 * return 0 - powodzenie, -1 - lista jest pusta
 */

int film_wypisz_dostepne(struct film *head_film){

    system("cls");
    puts(">> Dostępne filmy");

    if(head_film == NULL){
        puts(">> Baza filmów jest pusta!");
        return -1;
    }

    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypożyczone | Dostępne |\n\n", "Tytuł", "Reżyser", "Gatunek");

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
        puts(">> Żaden film nie jest aktualnie dostępny!");
        return -1;
    }

    return 0;
}

/**
 * \brief Wypisywanie listy filmów z podaną ilością sztuk dostępnych
 * @param *head_film wskaźnik na listę filmów
 * @param sztuki_dostępne poszukiwana liczba sztuk
 * return 0 - powodzenie, -1 - lista jest pusta
 */

int film_wypisz_sztuki_dostepne(struct film *head_film, int sztuki_dostepne){
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypożyczone | Dostępne |\n\n", "Tytuł", "Reżyser", "Gatunek");
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
        puts(">> Nie znaleziono żadnego filmu!");
        return -1;
    }

    return 0;
}

/**
 * \brief Wypisywanie listy filmów z podaną ilością sztuk wypożyczonych
 * @param *head_film wskaźnik na listę filmów
 * @param sztuki_wypozyczone poszukiwana liczba sztuk
 * return 0 - powodzenie, -1 - lista jest pusta
 */

int film_wypisz_sztuki_wypozyczone(struct film *head_film, int sztuki_wypozyczone){
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypożyczone | Dostępne |\n\n", "Tytuł", "Reżyser", "Gatunek");
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
        puts(">> Nie znaleziono żadnego filmu!");
        return -1;
    }

    return 0;
}

/**
 * \brief Wypisywanie listy filmów z podanym rokiem produkcji
 * @param *head_film wskaźnik na listę filmów
 * @param rok_produkcji poszukiwana rok produkcji
 * return 0 - powodzenie, -1 - lista jest pusta
 */

int film_wypisz_rok_produkcji(struct film *head_film, int rok_produkcji){
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypożyczone | Dostępne |\n\n", "Tytuł", "Reżyser", "Gatunek");
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
        puts(">> Nie znaleziono żadnego filmu!");
        return -1;
    }

    return 0;
}

/**
 * \brief Wypisywanie listy filmów o podanym tytule
 * @param *head_film wskaźnik na listę filmów
 * @param tytul[] poszukiwany tytuł
 * return 0 - powodzenie, -1 - lista jest pusta
 */

int film_wypisz_tytul(struct film *head_film, char tytul[]){
    tytul = strlwr(tytul);
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypożyczone | Dostępne |\n\n", "Tytuł", "Reżyser", "Gatunek");
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
        puts(">> Nie znaleziono żadnego filmu!");
        return -1;
    }
    return 0;
}

/**
 * \brief Wypisywanie listy filmów o podanym reżyserze
 * @param *head_film wskaźnik na listę filmów
 * @param rezyser[] poszukiwany reżyser
 * return 0 - powodzenie, -1 - lista jest pusta
 */

int film_wypisz_rezyser(struct film *head_film, char rezyser[]){
    rezyser = strlwr(rezyser);
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypożyczone | Dostępne |\n\n", "Tytuł", "Reżyser", "Gatunek");
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
        puts(">> Nie znaleziono żadnego filmu!");
        return -1;
    }
    return 0;
}

/**
 * \brief Wypisywanie listy filmów o podanym gatunku
 * @param *head_film wskaźnik na listę filmów
 * @param gatunek[] poszukiwany gatunek
 * return 0 - powodzenie, -1 - lista jest pusta
 */

int film_wypisz_gatunek(struct film *head_film, char gatunek[]){
    gatunek = strlwr(gatunek);
    printf(">> ID. |%30s |%20s |%10s | Rok produkcji | Wypożyczone | Dostępne |\n\n", "Tytuł", "Reżyser", "Gatunek");
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
        puts(">> Nie znaleziono żadnego filmu!");
        return -1;
    }
    return 0;
}

/**
 * \brief Sortowanie filmów po ilości dostępnych sztuk
 * @param **head_film wskaźnik na listę filmów
 * @param mode tryb sortowania, 0 rosnące, 1 malejące
 */

void film_dostepne(struct film **head_film, int mode) {
    struct film *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_film)->nastepny != NULL) {
        bufor = *head_film;
        temp = *head_film;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            if (temp->sztuki_dostepne < bufor->nastepny->sztuki_dostepne && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (temp->sztuki_dostepne > bufor->nastepny->sztuki_dostepne && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_film(&main, &temp);
        film_usun(head_film, prev);
    }
    push_film(&main, head_film);
    film_usun(head_film, NULL);
    *head_film = main;
}

/**
 * \brief Sortowanie filmów po ilości wypożyczonych sztuk
 * @param **head_film wskaźnik na listę filmów
 * @param mode tryb sortowania, 0 rosnące, 1 malejące
 */

void film_wypozyczone(struct film **head_film, int mode) {
    struct film *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_film)->nastepny != NULL) {
        bufor = *head_film;
        temp = *head_film;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            if (temp->sztuki_wypozyczone < bufor->nastepny->sztuki_wypozyczone && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (temp->sztuki_wypozyczone > bufor->nastepny->sztuki_wypozyczone && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_film(&main, &temp);
        film_usun(head_film, prev);
    }
    push_film(&main, head_film);
    film_usun(head_film, NULL);
    *head_film = main;
}

/**
 * \brief Sortowanie filmów po ilości roku produkcji
 * @param **head_film wskaźnik na listę filmów
 * @param mode tryb sortowania, 0 rosnące, 1 malejące
 */

void film_rok(struct film **head_film, int mode) {
    struct film *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_film)->nastepny != NULL) {
        bufor = *head_film;
        temp = *head_film;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            if (temp->rok_produkcji < bufor->nastepny->rok_produkcji && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (temp->rok_produkcji > bufor->nastepny->rok_produkcji && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_film(&main, &temp);
        film_usun(head_film, prev);
    }
    push_film(&main, head_film);
    film_usun(head_film, NULL);
    *head_film = main;
}

/**
 * \brief Sortowanie filmów po tytule
 * @param **head_film wskaźnik na listę filmów
 * @param mode tryb sortowania, 0 rosnące, 1 malejące
 */

void film_tytul(struct film **head_film, int mode) {
    struct film *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_film)->nastepny != NULL) {
        bufor = *head_film;
        temp = *head_film;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            strlwr(temp->tytul);
            strlwr(bufor->nastepny->tytul);
            if (strcmp(temp->tytul, bufor->nastepny->tytul)<0 && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (strcmp(temp->tytul, bufor->nastepny->tytul)>0 && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }
        push_film(&main, &temp);
        film_usun(head_film, prev);
    }
    push_film(&main, head_film);
    film_usun(head_film, NULL);
    *head_film = main;
}

/**
 * \brief Sortowanie filmów po reżyserze
 * @param **head_film wskaźnik na listę filmów
 * @param mode tryb sortowania, 0 rosnące, 1 malejące
 */

void film_rezyser(struct film **head_film, int mode) {
    struct film *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_film)->nastepny != NULL) {
        bufor = *head_film;
        temp = *head_film;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            strlwr(temp->rezyser);
            strlwr(bufor->nastepny->rezyser);
            if (strcmp(temp->rezyser, bufor->nastepny->rezyser)<0 && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (strcmp(temp->rezyser, bufor->nastepny->rezyser)>0 && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }
        push_film(&main, &temp);
        film_usun(head_film, prev);
    }
    push_film(&main, head_film);
    film_usun(head_film, NULL);
    *head_film = main;
}

/**
 * \brief Sortowanie filmów po gatunku filmu
 * @param **head_film wskaźnik na listę filmów
 * @param mode tryb sortowania, 0 rosnące, 1 malejące
 */

void film_gatunek(struct film **head_film, int mode) {
    struct film *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_film)->nastepny != NULL) {
        bufor = *head_film;
        temp = *head_film;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            strlwr(temp->gatunek);
            strlwr(bufor->nastepny->gatunek);
            if (strcmp(temp->gatunek, bufor->nastepny->gatunek)<0 && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (strcmp(temp->gatunek, bufor->nastepny->gatunek)>0 && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }
        push_film(&main, &temp);
        film_usun(head_film, prev);
    }
    push_film(&main, head_film);
    film_usun(head_film, NULL);
    *head_film = main;
}

/**
 * \brief Sprawdzanie, czy film o podanych danych istnieje
 * @param *head_film wskaźnik na listę filmów
 * @param rok_produkcji rok produkcji filmu
 * @param tytul[] tytuł filmu
 * @param rezyser[] reżyser filmu
 * @param gatunek[] gatunek filmu
 * @return true - film istnieje, false - film nie został znaleziony
 */

bool film_czy_istnieje(struct film *head_film, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]){
    while(head_film != NULL){
        if((rok_produkcji - head_film->rok_produkcji) == 0 && strcmp(tytul, head_film->tytul) == 0 && strcmp(rezyser, head_film->rezyser) == 0 && strcmp(gatunek, head_film->gatunek) == 0){
            return true;
        }
        head_film = head_film->nastepny;
    }
    return false;
}

/**
 * \brief Sprawdzanie, czy film posiada więcej, niż 1 dostępną sztukę
 * @param **head_film wskaźnik na listę filmów
 * @param *film_sprawdzany_poprzedni wskaźnik na element poprzedzający w stosunku do sprawdzanego
 * @return true - więcej niż 1 dostępna sztuka, false - jedna lub brak dostępnych sztuk
 */

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

/**
 * \brief Zapis listy filmów do pliku
 * @param *head_film wskaźnik na listę filmów
 * @return true - zapis udany, false - zapis się nie powiódł
 */

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

/**
 * \brief Zamiana spacji na tyldęw zmiennych łańcuchowych
 * @param *head_film wskaźnik na listę filmów
 */

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

/**
 * \brief Zamiana tyldy na spacje w zmiennych łańcuchowych
 * @param *head_film wskaźnik na listę filmów
 */

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

/**
 * \brief Wczytywanie listy filmów z pliku
 * @param **head_film wskaźnik na listę filmów
 * @return true - wczytanie udane, false - wczytanie nieudane
 */

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