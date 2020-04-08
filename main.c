#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>

//BAZY DANYCH//
struct klient{
    unsigned long long numer_klienta;
    unsigned long long numer_telefonu;
    char imie[10];
    char nazwisko[10];
    char email[30];
    int ilosc_posiadanych_filmow;
    struct klient *nastepny;
};
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
struct wypozyczenie{
    int numer_wypozyczenia;
    unsigned long long numer_klienta;
    int numer_filmu;
    char data_wypozyczenia[11];
    time_t data_wypozyczenia_sekundy;
    char data_zwrotu[11];
    time_t data_zwrotu_sekundy;
    struct klient *znacznik_klienta;
    struct film *znacznik_filmu;
    struct wypozyczenie *nastepny;
};

//FUNKCJE POMOCNICZE//
int input_int(int a, int b);
unsigned long long input_ull(unsigned long long a, unsigned long long b);
void czekaj_na_input_ESCAPE();
void wyczysc_ekran();

// FUNKCJE ZARZ¤DZANIA LIST¤ KLIENTàW//
void klient_dodaj(struct klient **head_klient, unsigned long long numer_klienta, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]);
void klient_usun(struct klient **head_klient, struct klient *klient_przed_usuwanym);
void klient_edytuj(struct klient **klient_edytowany, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]);
struct klient *klient_szukaj_po_kolejnosci(struct klient **head_klient, int kolejnosc);
struct klient *klient_szukaj_po_kolejnosci_dostepne_poprzedni(struct klient **head_klient, int kolejnosc);
int klient_licz_dostepne(struct klient *head_klient);
int klient_licz(struct klient *head_klient);
int klient_wypisz(struct klient **head_klient);
int klient_wypisz_dostepni(struct klient **head_klient);
bool klient_czy_pesel_istnieje(struct klient *head_klient, unsigned long long numer_klienta);

//FUNKCJE ZARZ¤DZANIA LIST¤ FILMàW
int film_dodaj(struct film **head_film, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
void film_usun(struct film **head_film, struct film *film_usuwany_poprzedni);
void film_edytuj(struct film **film_edytowany, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
struct film *film_szukaj_po_kolejnosci(struct film **head_film, int kolejnosc);
struct film *film_szukaj_po_kolejnosci_dostepne(struct film **head_film, int kolejnosc);
struct film *film_szukaj_po_kolejnosci_dostepne_poprzedni(struct film **head_film, int kolejnosc);
int film_licz(struct film *head_film);
int film_licz_dostepne(struct film *head_film);
int film_wypisz(struct film *head_film);
int film_wypisz_dostepne(struct film *head_film);
bool film_czy_istnieje(struct film *head_film, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
bool film_czy_jedna_sztuka_poprzedni(struct film **head_film, struct film *film_sprawdzany_poprzedni);

//FUNKCJE ZARZ¤DZANIA LIST¤ WYPO½YCZEN
void wypozyczenie_dodaj(struct wypozyczenie **head_wypozyczenie, struct film *film_wypozyczany, struct klient *klient_wypozyczajacy,  int ile_tygodni);
void wypozyczenie_usun(struct wypozyczenie **head_wypozyczenie, struct wypozyczenie *wypozyczenie_przed_usuwanym);
void wypozyczenie_edytuj(struct wypozyczenie **wypozyczenie_edytowane, int czas_do_zwrotu);
struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci(struct wypozyczenie **head_wypozyczenie, int kolejnosc);
struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci_poprzedni(struct wypozyczenie **head_wypozyczenie, int kolejnosc);
int wypozyczenie_licz(struct wypozyczenie **head_wypozyczenie);
int wypozyczenie_wypisz(struct wypozyczenie **head_wypozyczenie);

// FUNKCJE OPERUJ¤CE NA LISTACH//
void dodawanie_klient(struct klient **head_klient);
void usuwanie_klient(struct klient **head_klient);
void edytowanie_klient(struct klient **head_klient);
void dodawanie_film(struct film **head_film);
void usuwanie_film(struct film **head_film);
void edytowanie_film(struct film **head_film);
void dodawanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient *head_klient, struct film *head_film);
void usuwanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie);
void edytowanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie);

// PODPROGRAMY //
void zarzadznie_klient(struct klient **head_klient);
void zarzadzanie_film(struct film **head_film);
void zarzadzanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);
void DEBUG_MENU(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {

    struct klient *head_klient = NULL;
    struct film *head_film = NULL;
    struct wypozyczenie *head_wypozyczenie = NULL;
    while(1){
        wyczysc_ekran();
        puts("////// Menu Gˆ¢wne //////\n");
        puts(">> 1. Zarz¥dzanie baz¥ klient¢w");
        puts(">> 2. Zarz¥dzanie baz¥ film¢w");
        puts(">> 3. Zarz¥dzanie wypo¾yczaniami");
        puts(">> 4. DEBUG_MENU");
        puts("\n>> ESCAPE -> Wyjd« z programu");

        switch(getch()) {
            default:{
                break;
            }
            case 27:{
                wyczysc_ekran();
                puts(">> Czy na pewno chcesz opu˜ci† program?\n");
                puts(">> COKOLWIEK -> Wyjd«");
                puts(">> ESCAPE -> Anuluj\n");
                switch(getch()){
                    default: {
                        return 0;
                    }
                    case 27:{
                        break;
                    }
                }
                break;
            }
            case 49: {
                zarzadznie_klient(&head_klient);
                break;
            }
            case 50: {
                zarzadzanie_film(&head_film);
                break;
            }
            case 51:{
                zarzadzanie_wypozyczenie(&head_wypozyczenie, &head_klient, &head_film);
                break;
            }
            case 52:{
                DEBUG_MENU(&head_wypozyczenie,&head_klient,&head_film);
                break;
            }
        }
        wyczysc_ekran();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int input_int(int a, int b){
    char bufortab[10], *koniec;
    scanf("%s", bufortab);
    int bufor = strtol(bufortab, &koniec, 10);
    while ((*koniec == *bufortab) || (bufor < a) || (bufor > b)) {
        printf("\nBˆ¥d, zˆa dana wej˜ciowa. Prosz© wpisz ponownie.\n<< ");
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
        printf("\nBˆ¥d, zˆa dana wej˜ciowa. Prosz© wpisz ponownie.\n<< ");
        memset(bufortab, 0, 11 * sizeof(char));
        scanf("%s", bufortab);
        bufor = strtoull(bufortab, &koniec, 10);
    }
    return bufor;
}

void czekaj_na_input_ESCAPE(){
    puts("\n\n>> Wci˜nij ESCAPE aby powr¢ci†.");
    while(getch() != 27){

    }
}

void wyczysc_ekran(){
   system("cls");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void klient_dodaj(struct klient **head_klient, unsigned long long numer_klienta, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]){
    struct klient *klient_nowy = (struct klient *) malloc(sizeof(struct klient));
    klient_nowy->numer_klienta = numer_klienta;
    klient_nowy->numer_telefonu = numer_telefonu;
    strcpy(klient_nowy->imie, imie);
    strcpy(klient_nowy->nazwisko, nazwisko);
    strcpy(klient_nowy->email, email);
    klient_nowy->ilosc_posiadanych_filmow = 0;
    klient_nowy->nastepny = (*head_klient);
    *head_klient = klient_nowy;
}

void klient_usun(struct klient **head_klient, struct klient *klient_przed_usuwanym){
    struct klient *klient_bufor = NULL;
    if (klient_przed_usuwanym == NULL) {
        klient_bufor = (*head_klient)->nastepny;
        free(*head_klient);
        (*head_klient) = klient_bufor;
    } else {
        klient_bufor = klient_przed_usuwanym->nastepny;
        klient_przed_usuwanym->nastepny = klient_bufor->nastepny;
        free(klient_bufor);
    }
}

void klient_edytuj(struct klient **klient_edytowany, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]){
    (*klient_edytowany) -> numer_telefonu = numer_telefonu;
    strcpy((*klient_edytowany) -> imie, imie);
    strcpy((*klient_edytowany) -> nazwisko, nazwisko);
    strcpy((*klient_edytowany) -> email, email);
}

struct klient *klient_szukaj_po_kolejnosci(struct klient **head_klient, int kolejnosc){
    struct klient *klient_bufor = *head_klient;
    for(int i = 1; i != kolejnosc;i++){
        klient_bufor = klient_bufor -> nastepny;
    }
    return klient_bufor;
}

struct klient *klient_szukaj_po_kolejnosci_dostepne_poprzedni(struct klient **head_klient, int kolejnosc){
    struct klient *poprzedni = NULL;
    struct klient *klient_bufor = *head_klient;
    for(int i=0; i != kolejnosc;){
        if(klient_bufor->ilosc_posiadanych_filmow == 0) {
            i++;
        }
        if(i == kolejnosc){
            break;
        }
        poprzedni = klient_bufor;
        klient_bufor = klient_bufor->nastepny;
    }
    return poprzedni;
}

int klient_licz(struct klient *head_klient){
    int ilosc = 0;
    while(head_klient != 0){
        head_klient = head_klient -> nastepny;
        ilosc++;
    }
    return ilosc;
}

int klient_licz_dostepne(struct klient *head_klient){
    int ilosc = 0;
    while(head_klient != 0){
        if(head_klient -> ilosc_posiadanych_filmow == 0){
            ilosc++;
        }
        head_klient = head_klient -> nastepny;
    }
    return ilosc;
}

int klient_wypisz(struct klient **head_klient){
    struct klient *klient_bufor = *head_klient;
    if(klient_bufor == NULL){
        puts(">> Baza klient¢w jest pusta!");
        return -1;
    }
    printf(">> Nr. | %11s | %10s | %10s | %9s |%25s | Posiadane filmy \n\n", "Pesel", "Imie", "Nazwisko", "Telefon", "E-Mail");
    for(int i = 1;klient_bufor != NULL;i++){
        printf(">> %2d. | %llu | %10s | %10s | %9llu |%25s |%2d\n", i, klient_bufor->numer_klienta, klient_bufor->imie, klient_bufor->nazwisko, klient_bufor-> numer_telefonu, klient_bufor->email, klient_bufor->ilosc_posiadanych_filmow);
        klient_bufor = klient_bufor -> nastepny;
    }
    return 0;
}

int klient_wypisz_dostepni(struct klient **head_klient){
    struct klient *klient_bufor = *head_klient;
    if(klient_bufor == NULL){
        wyczysc_ekran();
        puts(">> Baza klient¢w jest pusta!");
        return -1;
    }
    printf(">> Nr. | %11s | %10s | %10s | %9s |%25s | Posiadane filmy \n\n", "Pesel", "Imie", "Nazwisko", "Telefon", "E-Mail");
    int i;
    for(i=1; klient_bufor != NULL;){
        if(klient_bufor->ilosc_posiadanych_filmow == 0) {
            printf(">> %2d. | %llu | %10s | %10s | %9llu |%25s |%2d\n", i, klient_bufor->numer_klienta,
                   klient_bufor->imie, klient_bufor->nazwisko, klient_bufor->numer_telefonu, klient_bufor->email,
                   klient_bufor->ilosc_posiadanych_filmow);
            i++;
        }
        klient_bufor = klient_bufor -> nastepny;
    }
    if(i==1){
        wyczysc_ekran();
        puts(">> ½aden klient nie mo¾e zosta† usuni©ty!");
        return -1;
    }
    return 0;
}

bool klient_czy_pesel_istnieje(struct klient *head_klient, unsigned long long numer_klienta){
    while(head_klient != NULL){
        if(head_klient -> numer_klienta == numer_klienta){
            return true;
        }
        head_klient = head_klient -> nastepny;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

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
        puts(">> Baza film¢w jest pusta!");
        return -1;
    }

    printf(">> Nr. |%30s |%20s |%10s | Rok produkcji | Dost©pne | Wypo¾yczone |\n\n", "Tytuˆ", "Re¾yser", "Gatunek");

    for(int i = 1 ; head_film != NULL; i++) {
        printf(">> %2d. |%30s |%20s |%10s | %13d |   %2d     |     %2d      |\n", i, head_film->tytul, head_film->rezyser, head_film->gatunek, head_film->rok_produkcji,
                   head_film->sztuki_dostepne, head_film->sztuki_wypozyczone);
        head_film = head_film -> nastepny;
    }
    return 0;
}

int film_wypisz_dostepne(struct film *head_film){

    wyczysc_ekran();
    puts(">> Dost©pne filmy");

    if(head_film == NULL){
        puts(">> Baza film¢w jest pusta!");
        return -1;
    }

    printf(">> Nr. |%30s |%20s |%10s | Rok produkcji | Dost©pne sztuki\n\n", "Tytuˆ", "Re¾yser", "Gatunek");

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
        wyczysc_ekran();
        puts(">> ½aden film nie jest aktualnie dost©pny!");
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void wypozyczenie_dodaj(struct wypozyczenie **head_wypozyczenie, struct film *film_wypozyczany, struct klient *klient_wypozyczajacy,  int ile_tygodni){
    char data_zwrotu[11];
    char data_wypozyczenia[11];
    time_t data_zwrotu_sekundy;
    time_t data_wypozyczenia_sekundy;
    struct tm *data;
    struct wypozyczenie *nowe_wypozyczenie = (struct wypozyczenie *) malloc(sizeof(struct wypozyczenie));
    time(&data_wypozyczenia_sekundy);
    data = localtime(&data_wypozyczenia_sekundy);
    strftime(data_wypozyczenia, 11, "%Y/%m/%d", data);
    data_zwrotu_sekundy = data_wypozyczenia_sekundy + (ile_tygodni * 604800);
    data = localtime(&data_zwrotu_sekundy);
    strftime(data_zwrotu, 11, "%Y/%m/%d", data);
    if((*head_wypozyczenie)==NULL){
        nowe_wypozyczenie -> numer_wypozyczenia = 0;
    }
    else{
        nowe_wypozyczenie -> numer_wypozyczenia = (*head_wypozyczenie)->numer_wypozyczenia+1;
    }
    nowe_wypozyczenie->numer_filmu = film_wypozyczany->numer_filmu;
    nowe_wypozyczenie->znacznik_filmu = film_wypozyczany;
    nowe_wypozyczenie->numer_klienta = klient_wypozyczajacy->numer_klienta;
    nowe_wypozyczenie->znacznik_klienta = klient_wypozyczajacy;
    nowe_wypozyczenie->nastepny = (*head_wypozyczenie);
    nowe_wypozyczenie->data_wypozyczenia_sekundy = data_wypozyczenia_sekundy;
    nowe_wypozyczenie->data_zwrotu_sekundy = data_wypozyczenia_sekundy;
    strcpy(nowe_wypozyczenie->data_zwrotu, data_zwrotu);
    strcpy(nowe_wypozyczenie->data_wypozyczenia, data_wypozyczenia);
    film_wypozyczany->sztuki_dostepne--;
    film_wypozyczany->sztuki_wypozyczone++;
    klient_wypozyczajacy->ilosc_posiadanych_filmow++;
    (*head_wypozyczenie) = nowe_wypozyczenie;
}

void wypozyczenie_usun(struct wypozyczenie **head_wypozyczenie, struct wypozyczenie *wypozyczenie_przed_usuwanym){
    struct wypozyczenie *wypozyczenie_bufor = NULL;
    if(wypozyczenie_przed_usuwanym == NULL){

        (*head_wypozyczenie)->znacznik_klienta->ilosc_posiadanych_filmow--;
        (*head_wypozyczenie)->znacznik_filmu->sztuki_wypozyczone--;
        (*head_wypozyczenie)->znacznik_filmu->sztuki_dostepne++;

        wypozyczenie_bufor = (*head_wypozyczenie) -> nastepny;
        free(*head_wypozyczenie);
        (*head_wypozyczenie) = wypozyczenie_bufor;
    }
    else{

        wypozyczenie_przed_usuwanym-> nastepny -> znacznik_klienta->ilosc_posiadanych_filmow--;
        wypozyczenie_przed_usuwanym-> nastepny -> znacznik_filmu->sztuki_wypozyczone--;
        wypozyczenie_przed_usuwanym-> nastepny -> znacznik_filmu->sztuki_dostepne++;

        wypozyczenie_bufor = wypozyczenie_przed_usuwanym -> nastepny;
        wypozyczenie_przed_usuwanym -> nastepny = wypozyczenie_bufor -> nastepny;
        free(wypozyczenie_bufor);
    }
}

void wypozyczenie_edytuj(struct wypozyczenie **wypozyczenie_edytowane, int czas_do_zwrotu){
    time_t czas1 = time(NULL) + (czas_do_zwrotu * 604800);
    struct tm *czas2;
    czas2 = localtime(&czas1);
    strftime((*wypozyczenie_edytowane)->data_zwrotu, 11, "%Y/%m/%d", czas2);
    (*wypozyczenie_edytowane) -> data_zwrotu_sekundy = czas1;
}

struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci(struct wypozyczenie **head_wypozyczenie, int kolejnosc){
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    for(int i=1; i != kolejnosc; i++){
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    return wypozyczenie_bufor;
}

struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci_poprzedni(struct wypozyczenie **head_wypozyczenie, int kolejnosc){
    struct wypozyczenie *poprzedni = NULL;
    struct wypozyczenie *bufor = *head_wypozyczenie;
    for(int i=1; i != kolejnosc; i++){
        poprzedni = bufor;
        bufor = bufor -> nastepny;
    }
    return poprzedni;
}

int wypozyczenie_licz(struct wypozyczenie **head_wypozyczenie){
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    int ilosc = 0;
    while(wypozyczenie_bufor != 0){
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
        ilosc++;
    }
    return ilosc;
}

int wypozyczenie_wypisz(struct wypozyczenie **head_wypozyczenie){
    wyczysc_ekran();
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    if(wypozyczenie_bufor == NULL){
        puts(">> Lista wypo¾yczeä jest pusta!");
        return -1;
    }
    printf(">> Aktywne wypo¾yczenia:\n");
    printf(">> Nr. |%10s |%10s |%30s |%18s |%12s |\n", "Imie", "Nazwisko", "Tytuˆ Filmu", "Data wypo¾yczenia", "Data Zwrotu");
    for(int i=1; wypozyczenie_bufor != NULL; i++){
        printf("\n>> %2d. |%10s |%10s |%30s |%18s |%12s | ", i, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////

void dodawanie_klient(struct klient **head_klient){

    unsigned long long numer_klienta;
    unsigned long long numer_telefonu;
    char imie[15];
    char nazwisko[15];
    char email[30];

    do {

        wyczysc_ekran();
        puts(">> UWAGA! Numeru pesel klienta nie mo¾na potem zmieni†!");
        printf(">> Wpisz numer pesel klienta:\n<< ");

        numer_klienta = input_ull(10000000000, 99999999999);

        if(klient_czy_pesel_istnieje(*head_klient, numer_klienta) == true){

            wyczysc_ekran();
            puts(">> Istnieje ju¾ klient o takim numerze pesel, nie mo¾na nadpisa† danych");
            puts(">> Czy chcesz wpisa† nowy numer?\n>> 1. Tak\n>> 2. Nie, wyjd« do menu");

            while(1) {
                int wybor = getch();
                if(wybor == 49){
                    break;
                }
                else if(wybor == 50){
                    return;
                }
            }
        }else {

            printf("\n>> Wpisz numer telefonu klienta:\n<< ");
            numer_telefonu = input_ull(111111111,999999999);
            printf("\n>> Wpisz imi© klienta: (max 15 znak¢w)\n<< ");
            scanf("%s", imie);
            printf("\n>> Wpisz nazwisko klienta: (max 15 znak¢w)\n<< ");
            scanf("%s", nazwisko);
            printf("\n>> Wpisz email klienta: (max 30 znak¢w)\n<< ");
            scanf("%s", email);

            klient_dodaj(head_klient, numer_klienta, numer_telefonu, imie, nazwisko, email);

            wyczysc_ekran();
            printf(">> Klient zostaˆ poprawnie dodany!");
            czekaj_na_input_ESCAPE();
            break;
        }
    }while(1);
    wyczysc_ekran();
}

void usuwanie_klient(struct klient **head_klient) {
    wyczysc_ekran();
    puts(">> Je¾eli jakiego˜ u¾ytkownika nie ma na poni¾szej li˜cie, posiada on aktualnie wypo¾yczony film");
    puts(">> klienci mo¾liwi do usuni©cia:\n");

    if(klient_wypisz_dostepni(head_klient) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    puts("\n>> Wpisz numer u¾ytkownika kt¢rego chcesz usun¥†:");

    int ilosc_dostepnych_filmow = klient_licz_dostepne(*head_klient);
    int numer_klienta = input_int(1,ilosc_dostepnych_filmow);
    struct klient *klient_bufor = klient_szukaj_po_kolejnosci_dostepne_poprzedni(head_klient,numer_klienta);

    klient_usun(head_klient, klient_bufor);

    wyczysc_ekran();
    puts(">> Usuni©cie klienta przebieˆo pomy˜lnie!");
    czekaj_na_input_ESCAPE();
}

void edytowanie_klient(struct klient **head_klient){

    wyczysc_ekran();
    puts(">> klienci mo¾liwi do edycji:\n");

    if(klient_wypisz(head_klient) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wpisz numer u¾ytkownika kt¢rego chcesz edytowa†:\n<< ");

    struct klient *klient_bufor;
    unsigned long long numer_telefonu;
    char imie[15];
    char nazwisko[15];
    char email[30];

    klient_bufor = klient_szukaj_po_kolejnosci(head_klient, input_int(1,klient_licz(*head_klient)));
    numer_telefonu = klient_bufor->numer_telefonu;
    strcpy(imie, klient_bufor->imie);
    strcpy(nazwisko, klient_bufor->nazwisko);
    strcpy(email, klient_bufor->email);

    while(1){

        wyczysc_ekran();
        puts(">> Wybierz pole kt¢re chcesz edytowa†:");
        puts(">> 1. Numer telefonu");
        puts(">> 2. Imi©");
        puts(">> 3. Nazwisko");
        puts(">> 4. E-Mail");
        puts(">> 5. Wszystkie");

        switch (getch()) {
            case 49: {
                wyczysc_ekran();
                printf(">> Wpisz nowy numer telefonu:\n<< ");
                numer_telefonu = input_ull(99999999, 1000000000);
                break;
            }
            case 50: {
                wyczysc_ekran();
                printf(">> Wpisz nowe imi© klienta:\n<< ");
                scanf("%s", imie);
                break;
            }
            case 51: {
                wyczysc_ekran();
                printf(">> Wpisz nowe nazwisko klienta:\n<< ");
                scanf("%s", nazwisko);
                break;
            }
            case 52: {
                wyczysc_ekran();
                printf(">> Wpisz nowy E-mail klienta:\n<< ");
                scanf("%s", email);
                break;
            }
            case 53: {
                wyczysc_ekran();
                printf(">> Wpisz nowy numer telefonu:\n<< ");
                numer_telefonu = input_ull(99999999, 1000000000);
                printf("\n>> Wpisz nowe imi© klienta:\n<< ");
                scanf("%s", imie);
                printf("\n>> Wpisz nowe nazwisko klienta:\n<< ");
                scanf("%s", nazwisko);
                printf("\n>> Wpisz nowy E-mail klienta:\n<< ");
                scanf("%s", email);
                break;
            }
        }

        wyczysc_ekran();
        printf(">> Stare dane klienta: %llu / %llu / %s / %s / %s\n", klient_bufor->numer_klienta,
               klient_bufor->numer_telefonu, klient_bufor->imie, klient_bufor->nazwisko, klient_bufor->email);
        printf("\n>> Nowe dane klienta:  %llu / %llu / %s / %s / %s\n", klient_bufor->numer_klienta, numer_telefonu, imie,
               nazwisko, email);
        puts("\n>> Czy chcesz zaakceptowa† zmiany?\n>> 1. Tak\n>> 2. Nie");

        while(1){
            int wybor = getch();
            if(wybor == 49){
                klient_edytuj(&klient_bufor, numer_telefonu, imie, nazwisko, email);
                wyczysc_ekran();
                return;
            }else if(wybor == 50){
                break;
            }
        }

        wyczysc_ekran();
        puts(">> Czy chcesz wpisa† nowe dane?\n>> 1. Tak\n>> 2. Nie, wyjd« z edycji danych");

        while(1){
            int wybor = getch();
            if(wybor == 49){
                break;
            }else if(wybor == 50){
                wyczysc_ekran();
                return;
            }
        }
    }
}

void dodawanie_film(struct film **head_film){

    int sztuki_dostepne;
    int rok_produkcji;
    char tytul[30];
    char rezyser[20];
    char gatunek[10];

    do {
        wyczysc_ekran();
        printf(">> Wpisz ile sztuk filmu chcesz doda† do bazy (1-99):\n<< ");
        sztuki_dostepne = input_int(1, 99);
        printf("\n>> Wpisz rok produkcji filmu:\n<< ");
        rok_produkcji = input_int(1800,2020);
        printf("\n>> Wpisz tytuˆ filmi: (max. 30 liter)\n<< ");
        scanf("%s", tytul);
        printf("\n>> Wpisz rezysera filmu: (max. 20 liter)\n<< ");
        scanf("%s", rezyser);
        printf("\n>> Wpisz gatunek filmu: (max. 10 liter)\n<< ");
        scanf("%s", gatunek);

        if(film_czy_istnieje(*head_film, rok_produkcji,tytul,rezyser,gatunek) == true){

            film_dodaj(head_film,sztuki_dostepne,rok_produkcji,tytul,rezyser,gatunek);
            wyczysc_ekran();
            break;

        } else {

            wyczysc_ekran();
            puts(">> Istnieje ju¾ film o takich samych danych");
            puts(">> Wybierz interesuj¥c¥ Ci© opcje:");
            puts(">> 1. Wpisz dane na nowo");
            puts(">> 2. Anuluj wprowadzanie nowego filmu");

            while(1){
                int wybor = getch();
                if(wybor == 1){
                    break;
                }else if(wybor == 2){
                    wyczysc_ekran();
                    return;
                }
            }
        }
    }while(1);
}

void usuwanie_film(struct film **head_film){
    wyczysc_ekran();

    if(film_wypisz_dostepne(*head_film) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wpisz numer filmu kt¢ry chcesz usun¥†:\n<< ");

    int ilosc_dostepnych_filmow = film_licz_dostepne(*head_film);
    int numer_filmu = input_int(1, ilosc_dostepnych_filmow);
    struct film *film_usuwany_poprzedni = film_szukaj_po_kolejnosci_dostepne_poprzedni(head_film, numer_filmu);
    struct film *film_usuwany = film_szukaj_po_kolejnosci_dostepne(head_film, numer_filmu);

    if(film_czy_jedna_sztuka_poprzedni(head_film, film_usuwany_poprzedni) == false){

        film_usun(head_film,film_usuwany_poprzedni);

    } else {
        printf(">> Wpisz ile sztuk filmu chcesz usun¥†:\n<< ");

        int sztuki = input_int(1,film_usuwany->sztuki_dostepne);
        film_usuwany -> sztuki_dostepne = film_usuwany -> sztuki_dostepne - sztuki;

        if(film_usuwany -> sztuki_dostepne == 0 && film_usuwany->sztuki_wypozyczone == 0){
            film_usun(head_film, film_usuwany_poprzedni);
        }
    }
    wyczysc_ekran();
    puts(">> Usuni©cie filmu przebieˆo pomy˜lnie!");
    czekaj_na_input_ESCAPE();
}

void edytowanie_film(struct film **head_film){

    struct film *film_bufor;
    char tytul[30];
    char rezyser[20];
    char gatunek[10];
    int rok_produkcji;
    int sztuki_dostepne;

    wyczysc_ekran();
    puts(">> filmy mo¾liwe do edycji:\n");

    if(film_wypisz(*head_film) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wpisz numer filmu kt¢ry chcesz edytowa†:\n<< ");

    int ilosc_dostepnych_filmow = film_licz(*head_film);
    int numer_filmu = input_int(1, ilosc_dostepnych_filmow);
    film_bufor = film_szukaj_po_kolejnosci(head_film, numer_filmu);

    sztuki_dostepne = film_bufor -> sztuki_dostepne;
    rok_produkcji = film_bufor -> rok_produkcji;
    strcpy(tytul, film_bufor->tytul);
    strcpy(rezyser,film_bufor -> rezyser);
    strcpy(gatunek,film_bufor -> gatunek);

    while(1){
        wyczysc_ekran();
        puts(">> Wybierz pole kt¢re chcesz edytowa†:");
        puts(">> 1. Ilo˜† dost©pnych sztuk");
        puts(">> 2. Rok produkcji");
        puts(">> 3. Tytuˆ");
        puts(">> 4. Rezyser");
        puts(">> 5. Gatunek");
        puts(">> 6. Wszystkie");
        switch(getch()){
            case 49:{
                wyczysc_ekran();
                printf(">> Wpisz now¥ ilo˜† dost©pnych sztuk: (1-99)\n<< ");
                sztuki_dostepne = input_int(1, 99);
                wyczysc_ekran();
                break;
            }
            case 50:{
                wyczysc_ekran();
                printf(">> Wpisz nowy rok produkcji:\n<< ");
                rok_produkcji = input_int(1800, 2020);
                break;
            }
            case 51:{
                wyczysc_ekran();
                printf(">> Wpisz nowy tytuˆ: (max. 30 znak¢w)\n<< ");
                scanf("%s", tytul);
                break;
            }
            case 52:{
                wyczysc_ekran();
                printf(">> Wpisz nowego re¾ysera: (max. 20 znak¢w)\n<< ");
                scanf("%s", rezyser);
                break;
            }
            case 53:{
                wyczysc_ekran();
                printf(">> Wpisz nowy gatunek: (max. 10 znak¢w)\n<< ");
                scanf("%s", gatunek);
                break;
            }
            case 54:{
                wyczysc_ekran();
                printf(">> Wpisz now¥ ilo˜† dost©pnych sztuk: (1-99)\n<< ");
                sztuki_dostepne = input_int(1, 99);
                wyczysc_ekran();
                printf(">> Wpisz nowy rok produkcji:\n<< ");
                rok_produkcji = input_int(1800, 2020);
                printf(">> Wpisz nowy tytuˆ: (max. 30 znak¢w)\n<< ");
                scanf("%s", tytul);
                printf(">> Wpisz nowego re¾ysera: (max. 20 znak¢w)\n<< ");
                scanf("%s", rezyser);
                printf(">> Wpisz nowy gatunek: (max. 10 znak¢w)\n<< ");
                scanf("%s", gatunek);
                break;
            }
        }

        wyczysc_ekran();
        printf(">> Stare dane filmu: %d / %d / %s / %s / %s\n", film_bufor->sztuki_dostepne, film_bufor->rok_produkcji, film_bufor->tytul, film_bufor->rezyser, film_bufor->gatunek);
        printf(">> Nowe dane filmu:  %d / %d / %s / %s / %s\n", sztuki_dostepne, rok_produkcji, tytul, rezyser, gatunek);
        puts(">> Czy chcesz zaakceptowa† zmiany?\n>> 1. Tak\n>> 2. Nie");
        while(1){
            int wybor = getch();
             if(wybor == 49){
                 wyczysc_ekran();
                 film_edytuj(&film_bufor, sztuki_dostepne, rok_produkcji, tytul, rezyser, gatunek);
                 return;
             }else if(wybor == 50){
                 break;
             }
        }

        puts(">> Czy chcesz wpisa† nowe dane?\n>> 1. Tak\n>> 2. Nie, wyjd« z edycji danych");
        while(1) {
            int wybor = getch();
            if(wybor == 49){
                wyczysc_ekran();
                break;
            }
            else if(wybor == 50){
                wyczysc_ekran();
                return;
            }
        }
    }
}

void dodawanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient *head_klient, struct film *head_film){

    wyczysc_ekran();
    printf(">> Dost©pni klienci:\n");

    if(klient_wypisz(&head_klient) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wybierz numer klienta kt¢remu chcesz wypo¾yczy† film:\n<< ");

    struct klient *klient_bufor = klient_szukaj_po_kolejnosci(&head_klient, input_int(1,klient_licz(head_klient)));
    wyczysc_ekran();
    if(film_wypisz_dostepne(head_film) != 0) {
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wybierz numer filmu kt¢ry chcesz wypo¾yczy†:\n<< ");

    struct film *film_bufor = film_szukaj_po_kolejnosci_dostepne(&head_film, input_int(1,film_licz_dostepne(head_film)));

    printf(">> Na ile tygodni chcesz wypo¾yczy† film: (conajmniej 1, maksymalnie 52)\n<< ");

    int czas_wypozyczenia = input_int(1,52);
    wypozyczenie_dodaj(head_wypozyczenie, film_bufor, klient_bufor, czas_wypozyczenia);

    wyczysc_ekran();
    puts(">> Dodanie wypo¾yczenia przebieˆo pomy˜lnie!");
    czekaj_na_input_ESCAPE();
}

void usuwanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie){

    if(wypozyczenie_wypisz(head_wypozyczenie) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n\n>> Wpisz numer wypo¾yczenia kt¢re chcesz zwr¢ci†:\n<< ");

    wypozyczenie_usun(head_wypozyczenie, wypozyczenie_szukaj_po_kolejnosci_poprzedni(head_wypozyczenie, input_int(1,wypozyczenie_licz(head_wypozyczenie))));

    wyczysc_ekran();
    puts(">> Usuni©cie wypo¾yczenia przebieˆo pomy˜lnie!");
    czekaj_na_input_ESCAPE();
}

void edytowanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie){

    if(wypozyczenie_wypisz(head_wypozyczenie) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n\n>> Wpisz numer wypo¾yczenia kt¢re chcesz edytowa†:\n<< ");

    struct wypozyczenie *wypozyczenie_bufor = wypozyczenie_szukaj_po_kolejnosci(head_wypozyczenie, input_int(1,wypozyczenie_licz(head_wypozyczenie)));

    wyczysc_ekran();
    printf(">> Aktualna data zwrotu wypo¾yczenia: %s\n\n", wypozyczenie_bufor->data_zwrotu);
    printf(">> Wpisz ilo˜† tygodni do koäca wypo¾yczenia (0-52)\n");
    printf(">> Tygodnie licz¥ si© od tego momentu, przy wpisaniu 0 wypo¾yczenie zostaje uznane jako przeterminowane.\n<< ");

    wypozyczenie_edytuj(&wypozyczenie_bufor, input_int(0,52));

    wyczysc_ekran();
    printf(">> Edycja wypo¾yczenia przebiegˆa pomy˜lnie!\n");
    printf(">> Nowa data zwrotu wypo¾yczenia: %s", wypozyczenie_bufor->data_zwrotu);
    czekaj_na_input_ESCAPE();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void zarzadznie_klient(struct klient **head_klient){
    do {
        wyczysc_ekran();
        puts("////// Zarz¥dzanie Klientami //////\n");
        puts(">> 1. Dodaj klienta");
        puts(">> 2. Usuä klienta");
        puts(">> 3. Edytuj klienta");
        puts(">> 4. Wy˜wietl caˆ¥ baz©");
        puts("\n>> ESCAPE -> Wr¢† do menu gˆ¢wnego");
        switch (getch()) {
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49: {
                dodawanie_klient(head_klient);
                break;
            }
            case 50: {
                usuwanie_klient(head_klient);
                break;
            }
            case 51: {
                edytowanie_klient(head_klient);
                break;
            }
            case 52: {
                wyczysc_ekran();
                klient_wypisz(head_klient);
                czekaj_na_input_ESCAPE();
                break;
            }
        }
    }while(1);
}

void zarzadzanie_film(struct film **head_film){
    do {
        wyczysc_ekran();
        puts("////// Zarz¥dzanie Filmami //////\n");
        puts(">> 1. Dodaj film");
        puts(">> 2. Usuä film");
        puts(">> 3. Edytuj film");
        puts(">> 4. Wy˜wietl caˆ¥ baz©");
        puts("\n>> ESCAPE -> Wr¢† do menu gˆ¢wnego");
        switch (getch()) {
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49: {
                dodawanie_film(head_film);
                break;
            }
            case 50: {
                usuwanie_film(head_film);
                break;
            }
            case 51: {
                edytowanie_film(head_film);
                break;
            }
            case 52: {
                wyczysc_ekran();
                film_wypisz(*head_film);
                czekaj_na_input_ESCAPE();
                break;
            }
        }
    }while(1);
}

void zarzadzanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
    do {
        wyczysc_ekran();
        puts("////// Zarz¥dzanie Wypo¾yczeniami //////\n");
        puts(">> 1. Dodaj wypo¾yczenie");
        puts(">> 2. Zwrot wypo¾yczenia");
        puts(">> 3. Edytuj wypo¾yczenie");
        puts(">> 4. Wy˜wietl wypo¾yczenia");
        puts("\n>> ESCAPE -> Wr¢† do menu gˆ¢wnego");
        switch (getch()) {
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49: {
                dodawanie_wypozyczenie(head_wypozyczenie, *head_klient, *head_film);
                break;
            }
            case 50: {
                usuwanie_wypozyczenie(head_wypozyczenie);
                break;
            }
            case 51: {
                edytowanie_wypozyczenie(head_wypozyczenie);
                break;
            }
            case 52: {
                wypozyczenie_wypisz(head_wypozyczenie);
                czekaj_na_input_ESCAPE();
                break;
            }
        }
    }while(1);
}

void DEBUG_MENU(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
    do {
        wyczysc_ekran();
        puts("////// DEBUG MENU//////\n");
        puts(">> 1. Dodaj przykˆadowych klient¢w");
        puts(">> 2. Dodaj przykˆadowe filmy");
        puts(">> 3. Dodaj przykˆadowe wypo¾yczenia (+powy¾sze)");
        puts("\n>> ESCEAPE -> Wr¢† do menu gˆ¢wnego");

        switch (getch()) {
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49: {
                klient_dodaj(head_klient, 98932401321, 123542864, "Maciej", "Kowalski", "m.kowalski123@gmail.com");
                klient_dodaj(head_klient, 32455123458, 115512467, "Michal", "Szewczyk", "szewczyk@buziaczek.pl");
                klient_dodaj(head_klient, 12356234123, 123672134, "Tomasz", "Nowak", "t.Nowaczek@op.pl");
                klient_dodaj(head_klient, 12562341233, 634126234, "Jakub", "Milek", "JakubMilek@gmail.com");
                break;
            }
            case 50: {
                film_dodaj(head_film,3,1998,"Harry Potter", "J.k. ", "Horror");
                film_dodaj(head_film,1,2010,"Kobbit", "Al Pacino", "Sci-Fi");
                film_dodaj(head_film,1,2005,"Die Hard", "John Rambo", "Fabularne");
                film_dodaj(head_film,2,2020,"8 Mila", "Sylverster ", "Przygodowe");
                break;
            }
            case 51: {
                klient_dodaj(head_klient, 98932401321, 123542864, "Maciej", "Kowalski", "m.kowalski123@gmail.com");
                klient_dodaj(head_klient, 32455123458, 115512467, "Michal", "Szewczyk", "szewczyk@buziaczek.pl");
                klient_dodaj(head_klient, 12356234123, 123672134, "Tomasz", "Nowak", "t.Nowaczek@op.pl");
                klient_dodaj(head_klient, 12562341233, 634126234, "Jakub", "Milek", "JakubMilek@gmail.com");
                film_dodaj(head_film,3,1998,"Harry Potter", "J.k. ", "Horror");
                film_dodaj(head_film,1,2010,"Kobbitm", "Al Pacino", "Sci-Fi");
                film_dodaj(head_film,1,2005,"Die Hard", "John Rambo", "Fabularne");
                film_dodaj(head_film,2,2020,"8 Mila", "Sylverst", "Przygodo");
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film), (*head_klient), 10);
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film)->nastepny, (*head_klient)->nastepny, 10);
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film)->nastepny->nastepny, (*head_klient)->nastepny->nastepny, 30);
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film)->nastepny->nastepny->nastepny, (*head_klient)->nastepny->nastepny->nastepny, 50);
                break;
            }
        }
        wyczysc_ekran();
        printf("Zmiany zostaˆy zapisane");
        czekaj_na_input_ESCAPE();
    }while(1);
}