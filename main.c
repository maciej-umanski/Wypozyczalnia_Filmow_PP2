#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//BAZY DANYCH//
struct klient{
    unsigned long long numer_klienta;
    unsigned long long numer_telefonu;
    char imie[15];
    char nazwisko[15];
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
    char data_zwrotu[11];
    struct klient *znacznik_klienta;
    struct film *znacznik_filmu;
    struct wypozyczenie *nastepny;
};

//FUNKCJE POMOCNICZE//
int input(int a, int b);
unsigned long long input_ull(unsigned long long a, unsigned long long b);
void clear();

// FUNKCJE ZARZĄDZANIA LISTAMI//
int klient_dodaj(struct klient **head_klient, unsigned long long numer_klienta, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]);     // zwraca -1 jeżeli pesel już istnieje w bazie
void klient_usun(struct klient **head_klient, unsigned long long numer_klienta);
void klient_edytuj(struct klient **klient_edytowany, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]);
int film_dodaj(struct film **head_film, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);                                      // zwraca -1 jeżeli film już istnieje w bazie
void film_usun(struct film **head_film, int numer_filmu);
void film_edytuj(struct film **film_edytowany, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
void wypozyczenie_dodaj(struct wypozyczenie **head_wypozyczenie, struct film *film_wypozyczany, struct klient *klient_wypozyczajacy,  int ile_tygodni);
void wypozyczenie_usun(struct wypozyczenie **head_wypozyczenie, int numer_wypozyczenia);
void wypozyczenie_edytuj(struct wypozyczenie **wypozyczenie_edytowane, char data_zwrotu[11]);

// FUNKCJE OPERUJĄCE NA LISTACH//
void dodawanie_klient(struct klient **head_klient);
void usuwanie_klient(struct klient **head_klient);
void edytowanie_klient(struct klient **head_klient);
void wypisz_klient(struct klient **head_klient);
void dodawanie_film(struct film **head_film);
void usuwanie_film(struct film **head_film);
void edytowanie_film(struct film **head_film);
void wypisz_film(struct film **head_film);
void dodawanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);
void usuwanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie);
void edytowanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie);
void wypisz_wypozyczenie(struct wypozyczenie **head_wypozyczenie);

// PODPROGRAMY //
void zarzadznie_klient(struct klient **head_klient);
void zarzadzanie_film(struct film **head_film);
void zarzadzanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);
void DEBUG_MENU(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);

// FUNKCJA GłÓWNA//
int main() {
    
    struct klient *head_klient = NULL;
    struct film *head_film = NULL;
    struct wypozyczenie *head_wypozyczenie = NULL;

    while(1){
        clear();
        puts("////// Menu Główne //////");
        puts(">> 1. Zarządzanie bazą klientów");
        puts(">> 2. Zarządzanie bazą filmów");
        puts(">> 3. Zarządzanie wypożyczaniami");
        puts(">> 4. DEBUG_MENU");
        puts(">> 5. Wyjdź z programu");

        switch(input(1,5)) {
            case 5: {
                return 0;
            }
            case 1: {
                zarzadznie_klient(&head_klient);
                break;
            }
            case 2: {
                zarzadzanie_film(&head_film);
                break;
            }
            case 3:{
                zarzadzanie_wypozyczenie(&head_wypozyczenie, &head_klient, &head_film);
                break;
            }
            case 4:{
                DEBUG_MENU(&head_wypozyczenie,&head_klient,&head_film);
                break;
            }
        }
        clear();
    }
}

////////////////////

int input(int a, int b){
    char bufortab[10], *koniec;
    scanf("%s", bufortab);
    int bufor = strtol(bufortab, &koniec, 10);
    if(a == 0 && b == 0) {
        while (*koniec == *bufortab) {
            puts("\nBłąd, zła dana wejściowa. Proszę wpisz ponownie.");
            scanf("%s", bufortab);
            bufor = strtol(bufortab, &koniec, 10);
        }
        return bufor;
    }
    else{
        while ((*koniec == *bufortab) || (bufor < a) || (bufor > b)) {
            puts("\nBłąd, zła dana wejściowa. Proszę wpisz ponownie.");
            scanf("%s", bufortab);
            bufor = strtol(bufortab, &koniec, 10);
        }
        return bufor;
    }
}

unsigned long long input_ull(unsigned long long a, unsigned long long b){
    char bufortab[10], *koniec;
    scanf("%s", bufortab);
    unsigned long long bufor = strtoull(bufortab, &koniec, 10);
    if(a == 0 && b == 0) {
        while (*koniec == *bufortab) {
            puts("\nBłąd, zła dana wejściowa. Proszę wpisz ponownie.");
            scanf("%s", bufortab);
            bufor = strtol(bufortab, &koniec, 10);
        }
        return bufor;
    }
    else{
        while ((*koniec == *bufortab) || (bufor < a) || (bufor > b)) {
            puts("\nBłąd, zła dana wejściowa. Proszę wpisz ponownie.");
            scanf("%s", bufortab);
            bufor = strtoull(bufortab, &koniec, 10);
        }
        return bufor;
    }
}

void clear(){
#ifdef _WIN32
    system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    system("clear");
#endif
}

///////////////////

int klient_dodaj(struct klient **head_klient, unsigned long long numer_klienta, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]){
    struct klient *klient_nowy = (*head_klient);
    if((*head_klient) != NULL) {
        while ((klient_nowy->nastepny != NULL) && (klient_nowy->numer_klienta != numer_klienta)) {
            klient_nowy = klient_nowy->nastepny;
        }
        if (klient_nowy->numer_klienta == numer_klienta) {
            return -1;
        }
    }
    klient_nowy = (struct klient *) malloc(sizeof(struct klient));
    klient_nowy->numer_klienta = numer_klienta;
    klient_nowy->numer_telefonu = numer_telefonu;
    strcpy(klient_nowy->imie, imie);
    strcpy(klient_nowy->nazwisko, nazwisko);
    strcpy(klient_nowy->email, email);
    klient_nowy->ilosc_posiadanych_filmow = 0;
    klient_nowy->nastepny = (*head_klient);
    *head_klient = klient_nowy;
    return 0;
}

void klient_usun(struct klient **head_klient, unsigned long long numer_klienta){
    struct klient *klient_usuwany = *head_klient;
    struct klient *klient_bufor;
    if ((*head_klient)-> numer_klienta == numer_klienta) {
        klient_bufor = (*head_klient)->nastepny;
        free(*head_klient);
        (*head_klient) = klient_bufor;
    } else {
        while(klient_usuwany->nastepny->numer_klienta != numer_klienta) {
            klient_usuwany = klient_usuwany->nastepny;
        }
        if(klient_usuwany->nastepny->nastepny == NULL){
            free(klient_usuwany->nastepny);
            klient_usuwany->nastepny = NULL;
        }else{
            klient_bufor = klient_usuwany->nastepny;
            klient_usuwany->nastepny = klient_bufor->nastepny;
            free(klient_bufor);
        }
    }
}

void klient_edytuj(struct klient **klient_edytowany, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]){
    (*klient_edytowany) -> numer_telefonu = numer_telefonu;
    strcpy((*klient_edytowany) -> imie, imie);
    strcpy((*klient_edytowany) -> nazwisko, nazwisko);
    strcpy((*klient_edytowany) -> email, email);
}

int film_dodaj(struct film **head_film, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]){
    struct film *film_nowy = *head_film;
    if((*head_film)!=NULL){
        while ((film_nowy->nastepny != NULL) && ((rok_produkcji != film_nowy->rok_produkcji) && (strcmp(tytul, film_nowy->tytul) != 0) && (strcmp(rezyser, film_nowy->rezyser) != 0) && (strcmp(gatunek, film_nowy->gatunek) != 0))){
            film_nowy = film_nowy->nastepny;
        }
        if (rok_produkcji == film_nowy->rok_produkcji && strcmp(tytul, film_nowy->tytul) == 0 && strcmp(rezyser, film_nowy->rezyser) == 0 && strcmp(gatunek, film_nowy->gatunek) == 0){
            return -1;
        }
    }
    film_nowy = (struct film *)malloc(sizeof(struct film));
    if((*head_film)==NULL){
        film_nowy -> numer_filmu = 0;
    }
    else{
        film_nowy -> numer_filmu = (*head_film)->numer_filmu+1;
    }
    film_nowy -> sztuki_dostepne = sztuki_dostepne;
    film_nowy -> sztuki_wypozyczone = 0;
    film_nowy -> rok_produkcji = rok_produkcji;
    film_nowy -> nastepny = (*head_film);
    strcpy(film_nowy -> tytul, tytul);
    strcpy(film_nowy -> rezyser, rezyser);
    strcpy(film_nowy -> gatunek, gatunek);
    *head_film = film_nowy;
    return 0;
}

void film_usun(struct film **head_film, int numer_filmu){
    struct film *film_usuwany = *head_film;
    struct film *film_bufor;
    if((*head_film)->numer_filmu == numer_filmu){
        film_bufor = (*head_film) -> nastepny;
        free(*head_film);
        (*head_film) = film_bufor;
    } else {
        while(film_usuwany -> nastepny -> numer_filmu != numer_filmu){
            film_usuwany = film_usuwany -> nastepny;
        }
        if(film_usuwany -> nastepny -> nastepny == NULL){
            free(film_usuwany->nastepny);
            film_usuwany->nastepny = NULL;
        } else{
            film_bufor = film_usuwany -> nastepny;
            film_usuwany -> nastepny = film_bufor -> nastepny;
            free(film_bufor);
        }

    }
}

void film_edytuj(struct film **film_edytowany, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]){
    (*film_edytowany) -> sztuki_dostepne = sztuki_dostepne;
    (*film_edytowany) -> rok_produkcji = rok_produkcji;
    strcpy((*film_edytowany) -> tytul, tytul);
    strcpy((*film_edytowany) -> rezyser, rezyser);
    strcpy((*film_edytowany) -> gatunek, gatunek);
}

void wypozyczenie_dodaj(struct wypozyczenie **head_wypozyczenie, struct film *film_wypozyczany, struct klient *klient_wypozyczajacy,  int ile_tygodni){
    char data_zwrotu[11];
    time_t czas1;
    char czas3[11] = {0};
    struct wypozyczenie *nowe_wypozyczenie = (struct wypozyczenie *) malloc(sizeof(struct wypozyczenie));
    struct tm *czas2;
    time(&czas1);
    czas2 = localtime(&czas1);
    strftime(czas3, 11, "%Y/%m/%d", czas2);
    czas1 = czas1 + (ile_tygodni * 604800);
    czas2 = localtime(&czas1);
    strftime(data_zwrotu, 11, "%Y/%m/%d", czas2);
    if((*head_wypozyczenie)==NULL){
        nowe_wypozyczenie -> numer_filmu = 0;
    }
    else{
        nowe_wypozyczenie -> numer_filmu = (*head_wypozyczenie)->numer_filmu+1;
    }
    nowe_wypozyczenie->numer_filmu = film_wypozyczany->numer_filmu;
    nowe_wypozyczenie->znacznik_filmu = film_wypozyczany;
    nowe_wypozyczenie->numer_klienta = klient_wypozyczajacy->numer_klienta;
    nowe_wypozyczenie->znacznik_klienta = klient_wypozyczajacy;
    nowe_wypozyczenie->nastepny = (*head_wypozyczenie);
    strcpy(nowe_wypozyczenie->data_zwrotu, data_zwrotu);
    strcpy(nowe_wypozyczenie->data_wypozyczenia, czas3);
    film_wypozyczany->sztuki_dostepne--;
    film_wypozyczany->sztuki_wypozyczone++;
    klient_wypozyczajacy->ilosc_posiadanych_filmow++;
    (*head_wypozyczenie) = nowe_wypozyczenie;
}

void wypozyczenie_usun(struct wypozyczenie **head_wypozyczenie, int numer_wypozyczenia){
    struct wypozyczenie *wypozyczenie_usuwane = *head_wypozyczenie;
    struct wypozyczenie *wypozyczenie_bufor;
    if((*head_wypozyczenie)->numer_wypozyczenia == numer_wypozyczenia){
        (*head_wypozyczenie)->znacznik_klienta->ilosc_posiadanych_filmow--;
        (*head_wypozyczenie)->znacznik_filmu->sztuki_wypozyczone--;
        (*head_wypozyczenie)->znacznik_filmu->sztuki_dostepne++;
        wypozyczenie_bufor = (*head_wypozyczenie) -> nastepny;
        free(*head_wypozyczenie);
        (*head_wypozyczenie) = wypozyczenie_bufor;
    }
    else{
        while(wypozyczenie_usuwane->nastepny->numer_wypozyczenia != numer_wypozyczenia){
            wypozyczenie_usuwane = wypozyczenie_usuwane -> nastepny;
        }
        if(wypozyczenie_usuwane->nastepny->nastepny == NULL){
            wypozyczenie_usuwane->znacznik_klienta->ilosc_posiadanych_filmow--;
            wypozyczenie_usuwane->znacznik_filmu->sztuki_wypozyczone--;
            wypozyczenie_usuwane->znacznik_filmu->sztuki_dostepne++;
            free(wypozyczenie_usuwane->nastepny);
            wypozyczenie_usuwane->nastepny = NULL;
        } else {
            wypozyczenie_usuwane->znacznik_klienta->ilosc_posiadanych_filmow--;
            wypozyczenie_usuwane->znacznik_filmu->sztuki_wypozyczone--;
            wypozyczenie_usuwane->znacznik_filmu->sztuki_dostepne++;
            wypozyczenie_bufor = wypozyczenie_usuwane -> nastepny;
            wypozyczenie_usuwane -> nastepny = wypozyczenie_bufor -> nastepny;
            free(wypozyczenie_bufor);
        }
    }
}

void wypozyczenie_edytuj(struct wypozyczenie **wypozyczenie_edytowane, char data_zwrotu[11]){
    strcpy((*wypozyczenie_edytowane)->data_zwrotu, data_zwrotu);
}


/////////////////

void dodawanie_klient(struct klient **head_klient){
    unsigned long long numer_klienta;
    unsigned long long numer_telefonu;
    char imie[15];
    char nazwisko[15];
    char email[30];
    do {
        clear();
        puts(">> UWAGA! Numeru pesel klienta nie można potem zmienić!");
        puts(">> Wpisz numer pesel klienta:");
        numer_klienta = input_ull(10000000000, 99999999999);
        puts(">> Wpisz numer telefonu klienta:");
        numer_telefonu = input_ull(100000000, 999999999);
        puts(">> Wpisz imię klienta: (max 15 znaków)");
        scanf("%s", imie);
        puts(">> Wpisz nazwisko klienta: (max 15 znaków)");
        scanf("%s", nazwisko);
        puts(">> Wpisz email klienta: (max 30 znaków)");
        scanf("%s", email);
        int bufor_bledow = klient_dodaj(head_klient, numer_klienta, numer_telefonu, imie, nazwisko, email);
        if(bufor_bledow == 0){
            break;
        }
        else{
            clear();
            puts(">> Istnieje już klient o takim numerze pesel, nie można nadpisać danych");
            puts(">> Czy chcesz wpisać nowe dane?\n>> 1. Tak\n>> 2. Nie");
            switch(input(1,2)){
                case 1: {
                    break;
                }
                case 2: {
                    return;
                }
            }
        }
    }while(1);
    clear();
}

void usuwanie_klient(struct klient **head_klient) {
    struct klient *klient_bufor = *head_klient;
    clear();
    puts(">> Jeżeli jakiegoś użytkownika nie ma na poniższej liście, posiada on aktualnie wypożyczony film");
    puts(">> klienci możliwi do usunięcia:\n");
    int i = 1;
    while (klient_bufor != NULL) {
        if (klient_bufor->ilosc_posiadanych_filmow == 0) {
            printf(">> %d. %llu / %s / %s\n", i, klient_bufor->numer_klienta, klient_bufor->imie, klient_bufor->nazwisko);
            i++;
        }
        klient_bufor = klient_bufor -> nastepny;
    }
    if(i == 1){
        puts(">> Wszyscy użytkownicy aktualnie wypożyczają filmy!");
        puts("\n>> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    klient_bufor = *head_klient;
    puts("\n>> Wpisz numer użytkownika którego chcesz usunąć:");
    int numer = input(1, i-1);
    i = 1;
    while (i != numer){
        if(klient_bufor->ilosc_posiadanych_filmow == 0) {
            i++;
        }
        klient_bufor = klient_bufor->nastepny;
    }
    klient_usun(head_klient, klient_bufor -> numer_klienta);
    clear();
}

void edytowanie_klient(struct klient **head_klient){
    clear();
    struct klient *klient_bufor = *head_klient;
    puts(">> klienci możliwi do edycji:\n");
    int i = 1;
    while (klient_bufor != NULL) {
        printf(">> %d. %llu/ %llu / %s / %s / %s\n", i, klient_bufor->numer_klienta, klient_bufor-> numer_telefonu, klient_bufor->imie, klient_bufor->nazwisko, klient_bufor->email);
        i++;
        klient_bufor = klient_bufor -> nastepny;
    }
    klient_bufor = *head_klient;
    puts("\n>> Wpisz numer użytkownika którego chcesz edytować:");
    int numer = input(1, i);
    i = 1;
    while (i != numer){
        i++;
        klient_bufor = klient_bufor->nastepny;
    }

    unsigned long long numer_telefonu = klient_bufor->numer_telefonu;
    char imie[15];
    char nazwisko[15];
    char email[30];
    strcpy(imie, klient_bufor->imie);
    strcpy(nazwisko, klient_bufor->nazwisko);
    strcpy(email, klient_bufor->email);
    while(1){
        clear();
        puts(">> Wybierz pole które chcesz edytować:");
        puts(">> 1. Numer telefonu");
        puts(">> 2. Imię");
        puts(">> 3. Nazwisko");
        puts(">> 4. E-Mail");
        puts(">> 5. Wszystkie");
        switch (input(1, 5)) {
            case 1: {
                clear();
                puts("Wpisz nowy numer telefonu:");
                numer_telefonu = input_ull(99999999, 1000000000);
                break;
            }
            case 2: {
                clear();
                puts("Wpisz nowe imię klienta:");
                scanf("%s", imie);
                break;
            }
            case 3: {
                clear();
                puts("Wpisz nowe nazwisko klienta:");
                scanf("%s", nazwisko);
                break;
            }
            case 4: {
                clear();
                puts("Wpisz nowy E-mail klienta:");
                scanf("%s", email);
                break;
            }
            case 5: {
                clear();
                puts("Wpisz nowy numer telefonu:");
                numer_telefonu = input_ull(99999999, 1000000000);
                puts("Wpisz nowe imię klienta:");
                scanf("%s", imie);
                puts("Wpisz nowe nazwisko klienta:");
                scanf("%s", nazwisko);
                puts("Wpisz nowy E-mail klienta:");
                scanf("%s", email);
                break;
            }
        }
        clear();
        printf(">> Stare dane klienta: %llu / %llu / %s / %s / %s\n", klient_bufor->numer_klienta,
               klient_bufor->numer_telefonu, klient_bufor->imie, klient_bufor->nazwisko, klient_bufor->email);
        printf(">> Nowe dane klienta:  %llu / %llu / %s / %s / %s\n", klient_bufor->numer_klienta, numer_telefonu, imie,
               nazwisko, email);
        puts(">> Czy chcesz zaakceptować zmiany?\n>> 1. Tak\n>> 2. Nie");
        switch (input(1, 2)) {
            case 1: {
                klient_edytuj(&klient_bufor, numer_telefonu, imie, nazwisko, email);
                clear();
                return;
            }
            case 2: {
                break;
            }
        }
        clear();
        puts(">> Czy chcesz wpisać nowe dane?\n>> 1. Tak\n>> 2. Nie, wyjdź z edycji danych");
        switch (input(1, 2)) {
            case 1: {
                break;
            }
            case 2: {
                clear();
                return;
            }
        }
    }
}

void wypisz_klient(struct klient **head_klient){
    struct klient *klient_bufor = *head_klient;
    clear();
    if(klient_bufor == NULL){
        puts(">> Baza klientów jest pusta!");
        puts(">> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    printf(">> Nr. / Pesel / Imie i Nazwisko / Telefon / E-Mail / Ilość posiadanych ksiażek\n\n");
    for(int i=1; klient_bufor != NULL; i++){
        printf(">> %d. %llu / %s / %s / %llu / %s / %d\n", i, klient_bufor->numer_klienta, klient_bufor->imie, klient_bufor->nazwisko, klient_bufor-> numer_telefonu, klient_bufor->email, klient_bufor->ilosc_posiadanych_filmow);
        klient_bufor = klient_bufor -> nastepny;
    }
    puts("\n>> Wpisz '1' aby kontynuować.");
    input(1,1);
}

void dodawanie_film(struct film **head_film){

    char tytul[30];
    char rezyser[20];
    char gatunek[10];

    do {
        clear();
        puts(">> Wpisz ile sztuk filmu chcesz dodać do bazy:");
        int sztuki_dostepne = input(0, 0);
        puts(">> Wpisz rok produkcji filmu:");
        int rok_produkcji = input(0, 0);
        puts(">> Wpisz tytuł filmi: (max. 30 liter)");
        scanf("%s", tytul);
        puts(">> Wpisz rezysera filmu: (max. 20 liter)");
        scanf("%s", rezyser);
        puts(">> Wpisz gatunek filmu: (max. 10 liter)");
        scanf("%s", gatunek);
        int bufor_bledow = film_dodaj(head_film, sztuki_dostepne, rok_produkcji, tytul, rezyser, gatunek);
        if (bufor_bledow == 0) {
            clear();
            break;
        } else {
            clear();
            puts(">> Istnieje już film o takich samych danych");
            puts(">> Wybierz interesującą Cię opcje:");
            puts(">> 1. Wpisz dane na nowo");
            puts(">> 2. Anuluj wprowadzanie nowego filmu");
            switch (input(1, 2)) {
                case 1:
                    break;
                case 2: {
                    clear();
                    return;
                }
            }
        }
    }while(1);
}

void usuwanie_film(struct film **head_film){
    clear();
    struct film *film_bufor = *head_film;
    if(film_bufor == NULL){
        puts(">> Lista filmów jest pusta!");
        puts("\n>> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    puts(">> Jeżeli jakiegoś filmu nie ma na poniższej liście to wszystkie jego egzemplarze są wypożyczone");
    puts(">> filmy możliwe do usunięcia:\n");
    int i = 1;
    while (film_bufor != NULL) {
        if (film_bufor->sztuki_dostepne > 0) {
            printf(">> %d. %d / %d / %s / %s / %s\n", i, film_bufor->sztuki_dostepne, film_bufor->rok_produkcji, film_bufor->tytul, film_bufor->rezyser, film_bufor->gatunek);
            i++;
        }
        film_bufor = film_bufor -> nastepny;
    }
    if(i == 1){
        clear();
        puts(">> Wszystkie filmy są wypożyczone!");
        puts("\n>> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    film_bufor = *head_film;
    puts("\n>> Wpisz numer filmu który chcesz usunąć:");
    int numer = input(1, i-1);
    i = 1;
    while (i != numer){
        if(film_bufor->sztuki_dostepne > 0) {
            i++;
        }
        film_bufor = film_bufor->nastepny;
    }
    if(film_bufor->sztuki_dostepne == 1){
        film_usun(head_film,film_bufor->numer_filmu);
        return;
    } else{
        puts(">> Wpisz ile sztuk filmu chcesz usunąć:");
        film_bufor -> sztuki_dostepne = film_bufor -> sztuki_dostepne - input(1,film_bufor->sztuki_dostepne);
        if(film_bufor -> sztuki_dostepne == 0 && film_bufor->sztuki_wypozyczone == 0){
            film_usun(head_film, film_bufor->numer_filmu);
        }
    }
}

void edytowanie_film(struct film **head_film){
    clear();
    struct film *film_bufor = *head_film;
    puts(">> filmy możliwe do edycji:\n");
    int i = 1;
    while (film_bufor != NULL) {
        printf(">> %d. %d / %d / %s / %s / %s\n", i, film_bufor->sztuki_dostepne, film_bufor->rok_produkcji, film_bufor->tytul, film_bufor->rezyser, film_bufor->gatunek);
        film_bufor = film_bufor -> nastepny;
        i++;
    }
    film_bufor = *head_film;
    puts("\n>> Wpisz numer filmu który chcesz edytować:");
    int numer = input(1, i);
    i = 1;
    while (i != numer){
        i++;
        film_bufor = film_bufor->nastepny;
    }

    char tytul[30];
    char rezyser[20];
    char gatunek[10];
    int sztuki_dostepne = film_bufor -> sztuki_dostepne;
    int rok_produkcji = film_bufor -> rok_produkcji;
    strcpy(tytul, film_bufor->tytul);
    strcpy(rezyser,film_bufor -> rezyser);
    strcpy(gatunek,film_bufor -> gatunek);

    while(1){
        clear();
        puts(">> Wybierz pole które chcesz edytować:");
        puts(">> 1. Ilość dostępnych sztuk");
        puts(">> 2. Rok produkcji");
        puts(">> 3. Tytuł");
        puts(">> 4. Rezyser");
        puts(">> 5. Gatunek");
        puts(">> 6. Wszystkie");
        switch(input(1,6)){
            case 1:{
                clear();
                puts(">> Wpisz nową ilość dostępnych sztuk:");
                sztuki_dostepne = input(0, 0);
                do {
                    if (film_bufor->sztuki_wypozyczone == 0 && sztuki_dostepne == 0) {
                        clear();
                        puts(">> Uwaga, zmieniajac liczbę sztuk tego filmu na 0 usuniesz go z bazy danych ponieważ nikt aktualnie go nie wypożycza!");
                        puts(">> Czy aby na pewno chcesz kontynuować?\n>> 1. Tak, usuń film z bazy danych\n>> 2. Nie, wpisz ilość sztuk na nowo");
                        switch (input(1, 2)) {
                            case 1: {
                                film_usun(head_film, film_bufor->numer_filmu);
                                clear();
                                return;
                            }
                            case 2: {
                                clear();
                                puts(">> Wpisz nową ilość dostępnych sztuk:");
                                sztuki_dostepne = input(0, 0);
                            }
                        }
                    }
                    else{
                        break;
                    }
                }while(1);
                clear();
                break;
            }
            case 2:{
                clear();
                puts(">> Wpisz nowy rok produkcji:");
                rok_produkcji = input(0,0);
                break;
            }
            case 3:{
                clear();
                puts(">> Wpisz nowy tytuł: (max. 30 znaków)");
                scanf("%s", tytul);
                break;
            }
            case 4:{
                clear();
                puts(">> Wpisz nowego reżysera: (max. 20 znaków)");
                scanf("%s", rezyser);
                break;
            }
            case 5:{
                clear();
                puts(">> Wpisz nowy gatunek: (max. 10 znaków)");
                scanf("%s", gatunek);
                break;
            }
            case 6:{
                clear();
                puts(">> Wpisz nową ilość dostępnych sztuk:");
                sztuki_dostepne = input(0, 0);
                do {
                    if (film_bufor->sztuki_wypozyczone == 0 && sztuki_dostepne == 0) {
                        clear();
                        puts(">> Uwaga, zmieniajac liczbę sztuk tego filmu na 0 usuniesz go z bazy danych ponieważ nikt aktualnie go nie wypożycza!");
                        puts(">> Czy aby na pewno chcesz kontynuować?\n>> 1. Tak, usuń film z bazy danych\n>> 2. Nie, wpisz ilość sztuk na nowo");
                        switch (input(1, 2)) {
                            case 1: {
                                clear();
                                film_usun(head_film, film_bufor->numer_filmu);
                                return;
                            }
                            case 2: {
                                clear();
                                puts(">> Wpisz nową ilość dostępnych sztuk:");
                                sztuki_dostepne = input(0, 0);
                            }
                        }
                    }
                    else{
                        clear();
                        break;
                    }
                }while(1);
                clear();
                puts(">> Wpisz nowy rok produkcji:");
                rok_produkcji = input(0,0);
                puts(">> Wpisz nowy tytuł: (max. 30 znaków)");
                scanf("%s", tytul);
                puts(">> Wpisz nowego reżysera: (max. 20 znaków)");
                scanf("%s", rezyser);
                puts(">> Wpisz nowy gatunek: (max. 10 znaków)");
                scanf("%s", gatunek);
                break;
            }
        }
        clear();
        printf(">> Stare dane filmu: %d / %d / %s / %s / %s\n", film_bufor->sztuki_dostepne, film_bufor->rok_produkcji, film_bufor->tytul, film_bufor->rezyser, film_bufor->gatunek);
        printf(">> Nowe dane filmu:  %d / %d / %s / %s / %s\n", sztuki_dostepne, rok_produkcji, tytul, rezyser, gatunek);
        puts(">> Czy chcesz zaakceptować zmiany?\n>> 1. Tak\n>> 2. Nie");
        switch (input(1, 2)) {
            case 1: {
                clear();
                film_edytuj(&film_bufor, sztuki_dostepne, rok_produkcji, tytul, rezyser ,gatunek);
                return;
            }
            case 2: {
                break;
            }
        }
        puts(">> Czy chcesz wpisać nowe dane?\n>> 1. Tak\n>> 2. Nie, wyjdź z edycji danych");
        switch (input(1, 2)) {
            case 1: {
                clear();
                break;
            }
            case 2: {
                clear();
                return;
            }
        }
    }
}

void wypisz_film(struct film **head_film){
    clear();
    struct film *film_bufor = *head_film;
    if(film_bufor == NULL){
        puts(">> Baza filmów jest pusta!");
        puts(">> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    puts(">> Nr. / Sztuki dostępne / Sztuki wypożyczone / rok produkcji / Tytuł / reżyser / gatunek");
    for(int i=1; film_bufor != NULL; i++){
        printf("\n>> %d. / %d / %d / %d / %s / %s / %s", i, film_bufor->sztuki_dostepne, film_bufor->sztuki_wypozyczone, film_bufor->rok_produkcji, film_bufor->tytul, film_bufor->rezyser, film_bufor->gatunek);
        film_bufor = film_bufor -> nastepny;
    }
    printf("\n\n>> Wpisz '1' aby kontynuować");
    input(1,1);
}

void dodawanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
    struct klient *klient_bufor = *head_klient;
    struct film *film_bufor = *head_film;
    int i;
    int czas_wypozyczenia;
    clear();
    if(klient_bufor == NULL){
        puts(">> Baza klientów jest pusta!");
        puts(">> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    else if(film_bufor == NULL){
        puts(">> Baza filmów jest pusta!");
        puts(">> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    printf(">> Nr. / Pesel / Imie i Nazwisko / Telefon / E-Mail / Ilość posiadanych ksiażek\n\n");
    klient_bufor = *head_klient;
    for(i=1; klient_bufor != NULL; i++){
        printf(">> %d. / %llu / %s %s / %llu / %s / %d\n", i, klient_bufor->numer_klienta, klient_bufor->imie, klient_bufor->nazwisko, klient_bufor-> numer_telefonu, klient_bufor->email, klient_bufor->ilosc_posiadanych_filmow);
        klient_bufor = klient_bufor -> nastepny;
    }
    puts("\n>> Wybierz numer klienta któremu chcesz wypożyczyć książkę:");
    klient_bufor = *head_klient;
    int numer_klienta = input(1,i-1);
    i = 1;
    clear();
    while(i != numer_klienta){
        klient_bufor = klient_bufor -> nastepny;
        i++;
    }
    puts(">> filmy możliwe do wypożyczenia:\n");
    puts(">> Nr. / Sztuki dostępne / Sztuki wypożyczone / rok produkcji / Tytuł / reżyser / gatunek\n");
    i = 1;
    while (film_bufor != NULL) {
        if(film_bufor -> sztuki_dostepne > 0) {
            printf(">> %d. / %d / %d / %d / %s / %s / %s\n", i, film_bufor->sztuki_dostepne,
                   film_bufor->sztuki_dostepne, film_bufor->rok_produkcji, film_bufor->tytul, film_bufor->rezyser,
                   film_bufor->gatunek);
            i++;
        }
        film_bufor = film_bufor -> nastepny;
    }
    if(i==1){
        clear();
        puts(">> Wszystkie filmy są wypożyczone!");
        puts("\n>> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    int numer_filmu = input(1,i-1);
    i = 1;
    film_bufor = *head_film;
    while(i != numer_filmu){
        if(film_bufor ->sztuki_dostepne > 0){
            i++;
        }
        film_bufor = film_bufor -> nastepny;
    }
    clear();
    puts(">> Na ile tygodni chcesz wypożyczyć film: (conajmniej 1, maksymalnie 52)");
    czas_wypozyczenia = input(1,52);
    wypozyczenie_dodaj(head_wypozyczenie, film_bufor, klient_bufor, czas_wypozyczenia);
}

void usuwanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie){
    clear();
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    if(wypozyczenie_bufor == NULL){
        puts(">> Lista wypożyczeń jest pusta!");
        puts("\n>> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    int i;
    puts(">> Aktywne wypożyczenia:\n");
    for(i=1; wypozyczenie_bufor != NULL; i++){
        printf("\n>> %d. / %s %s / %s / %s / %s ", i, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    wypozyczenie_bufor = *head_wypozyczenie;
    puts("\n>> Wpisz numer wypożyczenia które chcesz zwrócić:");
    int numer = input(1,i-1);
    for(i=1; i != numer; i++){
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    wypozyczenie_usun(head_wypozyczenie, wypozyczenie_bufor->numer_wypozyczenia);
}

void edytowanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie){
    clear();
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    if(wypozyczenie_bufor == NULL){
        puts(">> Lista wypożyczeń jest pusta!");
        puts("\n>> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    puts(">> Nr. / Imię i Nazwisko / Tytuł filmu / Data wypożyczenia / Data zwrotu");
    int i;
    for(i=1; wypozyczenie_bufor != NULL; i++){
        printf("\n>> %d. / %s %s / %s / %s / %s ", i, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    wypozyczenie_bufor = *head_wypozyczenie;
    puts("\n\n>> Wpisz numer wypożyczenia które chcesz edytować:");
    int numer = input(1,i-1);
    for(i=1; i != numer; i++){
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    while(1){
        clear();
        puts(">> Wpisz ilość tygodni do końca wypożyczenia (0-52)");
        puts(">> Tygodnie liczą się od tego momentu, przy wpisaniu 0 wypożyczenie zostaje uznane jako przeterminowane.");
        int czas_do_konca = input(0, 52);
        time_t czas1 = time(NULL) + (czas_do_konca * 604800);
        struct tm *czas2;
        czas2 = localtime(&czas1);
        char czas_tablica[11];
        strftime(czas_tablica, 11, "%Y/%m/%d", czas2);
        clear();
        printf("Poprzednia data zwrotu wypożyczenia: %s\n", wypozyczenie_bufor->data_zwrotu);
        printf("Nowa data zwrotu wypożyczenia: %s\n", czas_tablica);
        puts(">> Czy chcesz zaakceptować zmiany?\n>> 1. Tak\n>> 2. Nie");
        switch (input(1, 2)) {
            case 1: {
                wypozyczenie_edytuj(&wypozyczenie_bufor, czas_tablica);
                clear();
                return;
            }
            case 2: {
                break;
            }
        }
        clear();
        puts(">> Czy chcesz wpisać nowe dane?\n>> 1. Tak\n>> 2. Nie, wyjdź z edycji danych");
        switch (input(1, 2)) {
            case 1: {
                break;
            }
            case 2: {
                clear();
                return;
            }
        }
    }
}

void wypisz_wypozyczenie(struct wypozyczenie **head_wypozyczenie){
    clear();
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    if(wypozyczenie_bufor == NULL){
        puts(">> Lista wypożyczeń jest pusta!");
        puts("\n>> Wpisz '1' aby kontynuować.");
        input(1,1);
        return;
    }
    puts(">> Nr. / Imię i Nazwisko / Tytuł filmu / Data wypożyczenia / Data zwrotu");
    for(int i=1; wypozyczenie_bufor != NULL; i++){
        printf("\n>> %d. / %s %s / %s / %s / %s ", i, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    printf("\n\n>> Wpisz '1' aby kontynuować\n");
    input(1,1);
}

///////////////////

void zarzadznie_klient(struct klient **head_klient){
    do {
        clear();
        puts("////// Zarządzanie Klientami //////");
        puts(">> 1. Dodaj klienta");
        puts(">> 2. Usuń klienta");
        puts(">> 3. Edytuj klienta");
        puts(">> 4. Wyświetl całą bazę");
        puts(">> 5. Wróć do menu głównego");
        switch (input(1, 5)) {
            case 5: {
                return;
            }
            case 1: {
                dodawanie_klient(head_klient);
                break;
            }
            case 2: {
                usuwanie_klient(head_klient);
                break;
            }
            case 3: {
                edytowanie_klient(head_klient);
                break;
            }
            case 4: {
                wypisz_klient(head_klient);
                break;
            }
        }
    }while(1);
}

void zarzadzanie_film(struct film **head_film){
    do {
        clear();
        puts("////// Zarządzanie Filmami //////");
        puts(">> 1. Dodaj film");
        puts(">> 2. Usuń film");
        puts(">> 3. Edytuj film");
        puts(">> 4. Wyświetl całą bazę");
        puts(">> 5. Wróć do menu głównego");
        switch (input(1, 5)) {
            case 5: {
                return;
            }
            case 1: {
                dodawanie_film(head_film);
                break;
            }
            case 2: {
                usuwanie_film(head_film);
                break;
            }
            case 3: {
                edytowanie_film(head_film);
                break;
            }
            case 4: {
                wypisz_film(head_film);
                break;
            }
        }
    }while(1);
}

void zarzadzanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
    do {
        clear();
        puts("////// Zarządzanie Wypożyczeniami //////");
        puts(">> 1. Dodaj wypożyczenie");
        puts(">> 2. Zwrot wypożyczenia");
        puts(">> 3. Edytuj wypożyczenie");
        puts(">> 4. Wyświetl wypożyczenia");
        puts(">> 5. Wróć do menu głównego");
        switch (input(1, 5)) {
            case 5: {
                return;
            }
            case 1: {
                dodawanie_wypozyczenie(head_wypozyczenie, head_klient, head_film);
                break;
            }
            case 2: {
                usuwanie_wypozyczenie(head_wypozyczenie);
                break;
            }
            case 3: {
                edytowanie_wypozyczenie(head_wypozyczenie);
                break;
            }
            case 4: {
                wypisz_wypozyczenie(head_wypozyczenie);
                break;
            }
        }
    }while(1);
}

void DEBUG_MENU(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
    do {
        clear();
        puts("////// DEBUG MENU//////");
        puts(">> 1. Dodaj przykładowych klientów");
        puts(">> 2. Dodaj przykładowe filmy");
        puts(">> 3. Dodaj przykładowe wypożyczenia (+powyższe)");
        puts(">> 4. Wyczyść wszystkie bazy");
        puts(">> 5. Wróć do menu głównego");
        switch (input(1, 5)) {
            case 5: {
                return;
            }
            case 1: {
                klient_dodaj(head_klient, 98932401321, 123542864, "Maciej", "Kowalski", "m.kowalski123@gmail.com");
                klient_dodaj(head_klient, 32455123458, 115512467, "Michal", "Szewczyk", "szewczyk@buziaczek.pl");
                klient_dodaj(head_klient, 12356234123, 123672134, "Tomasz", "Nowak", "t.Nowaczek@op.pl");
                klient_dodaj(head_klient, 12562341233, 634126234, "Jakub", "Milek", "JakubMilek@gmail.com");
                break;
            }
            case 2: {
                film_dodaj(head_film,3,1998,"Harry Potter - Komnata Tajemnic", "J.k. Rowiling", "Horror");
                film_dodaj(head_film,1,2010,"Kobbit - Ostatnia costam", "Al Pacino", "Sci-Fi");
                film_dodaj(head_film,1,2005,"Die Hard", "John Rambo", "Fabularne");
                film_dodaj(head_film,2,2020,"8 Mila", "Sylverster Stallone", "Przygodowe");
                break;
            }
            case 3: {
                klient_dodaj(head_klient, 98932401321, 123542864, "Maciej", "Kowalski", "m.kowalski123@gmail.com");
                klient_dodaj(head_klient, 32455123458, 115512467, "Michal", "Szewczyk", "szewczyk@buziaczek.pl");
                klient_dodaj(head_klient, 12356234123, 123672134, "Tomasz", "Nowak", "t.Nowaczek@op.pl");
                klient_dodaj(head_klient, 12562341233, 634126234, "Jakub", "Milek", "JakubMilek@gmail.com");
                film_dodaj(head_film,3,1998,"Harry Potter - Komnata Tajemnic", "J.k. Rowiling", "Horror");
                film_dodaj(head_film,1,2010,"Kobbit - Ostatnia costam", "Al Pacino", "Sci-Fi");
                film_dodaj(head_film,1,2005,"Die Hard", "John Rambo", "Fabularne");
                film_dodaj(head_film,2,2020,"8 Mila", "Sylverst", "Przygodo");
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film), (*head_klient), 10);
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film)->nastepny, (*head_klient)->nastepny, 10);
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film)->nastepny->nastepny, (*head_klient)->nastepny->nastepny, 30);
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film)->nastepny->nastepny->nastepny, (*head_klient)->nastepny->nastepny->nastepny, 50);
                break;
            }
            case 4: {
                while((*head_wypozyczenie) != NULL){
                    wypozyczenie_usun(head_wypozyczenie, (*head_wypozyczenie)->numer_wypozyczenia);
                }
                while((*head_film) != NULL){
                    film_usun(head_film,(*head_film)->numer_filmu);
                }
                while((*head_klient) != NULL){
                    klient_usun(head_klient, (*head_klient)->numer_klienta);
                }
                break;
            }
        }
    }while(1);
}