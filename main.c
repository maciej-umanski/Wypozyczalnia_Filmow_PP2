#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <locale.h>
#include "film.h"
#include "klient.h"
#include "wypozyczenie.h"
#include "inne.h"

// FUNKCJE MENU//
void dodawanie_klient(struct klient **head_klient);
void usuwanie_klient(struct klient **head_klient);
void edytowanie_klient(struct klient **head_klient);
void dodawanie_film(struct film **head_film);
void usuwanie_film(struct film **head_film);
void edytowanie_film(struct film **head_film);
void dodawanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient *head_klient, struct film *head_film);
void usuwanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie);
void edytowanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie);

// MENU //
void zarzadznie_klient(struct klient **head_klient);
void zarzadzanie_film(struct film **head_film);
void zarzadzanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);
void DEBUG_MENU(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    setlocale(LC_ALL, "Polish");
    struct klient *head_klient = NULL;
    struct film *head_film = NULL;
    struct wypozyczenie *head_wypozyczenie = NULL;
    while(1){
        wyczysc_ekran();
        puts("////// Menu G³ówne //////\n");
        puts(">> 1. Zarz¹dzanie baz¹ klientów");
        puts(">> 2. Zarz¹dzanie baz¹ filmów");
        puts(">> 3. Zarz¹dzanie wypo¿yczeniami");
        puts(">> 4. DEBUG_MENU");
        puts("\n>> ESCAPE -> WyjdŸ z programu");

        switch(getch()) {
            default:{
                break;
            }
            case 27:{
                wyczysc_ekran();
                puts(">> Czy na pewno chcesz opuœciæ program?\n");
                puts(">> COKOLWIEK -> WyjdŸ");
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

void dodawanie_klient(struct klient **head_klient){

    unsigned long long numer_klienta;
    unsigned long long numer_telefonu;
    char imie[15];
    char nazwisko[15];
    char email[30];

    do {

        wyczysc_ekran();
        puts(">> UWAGA! Numeru pesel klienta nie mo¿na potem zmieniæ!");
        printf(">> Wpisz numer pesel klienta:\n<< ");

        numer_klienta = input_ull(10000000000, 99999999999);

        if(klient_czy_pesel_istnieje(*head_klient, numer_klienta) == true){

            wyczysc_ekran();
            puts(">> Istnieje ju¿ klient o takim numerze pesel, nie mo¿na nadpisaæ danych");
            puts(">> Czy chcesz wpisaæ nowy numer?\n>> 1. Tak\n>> 2. Nie, wyjdŸ do menu");

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
            printf("\n>> Wpisz imiê klienta: (max 15 znaków)\n<< ");
            scanf("%s", imie);
            printf("\n>> Wpisz nazwisko klienta: (max 15 znaków)\n<< ");
            scanf("%s", nazwisko);
            printf("\n>> Wpisz email klienta: (max 30 znaków)\n<< ");
            scanf("%s", email);

            klient_dodaj(head_klient, numer_klienta, numer_telefonu, imie, nazwisko, email);

            wyczysc_ekran();
            printf(">> Klient zosta³ poprawnie dodany!");
            czekaj_na_input_ESCAPE();
            break;
        }
    }while(1);
    wyczysc_ekran();
}

void usuwanie_klient(struct klient **head_klient) {
    wyczysc_ekran();
    puts(">> Je¿eli jakiegoœ u¿ytkownika nie ma na poni¿szej liœcie, posiada on aktualnie wypo¿yczony film");
    puts(">> klienci mo¿liwi do usuniêcia:\n");

    if(klient_wypisz_dostepni(head_klient) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    puts("\n>> Wpisz numer u¿ytkownika którego chcesz usun¹æ:");

    int ilosc_dostepnych_filmow = klient_licz_dostepne(*head_klient);
    int numer_klienta = input_int(1,ilosc_dostepnych_filmow);
    struct klient *klient_bufor = klient_szukaj_po_kolejnosci_dostepne_poprzedni(head_klient,numer_klienta);

    klient_usun(head_klient, klient_bufor);

    wyczysc_ekran();
    puts(">> Usuniêcie klienta przebie³o pomyœlnie!");
    czekaj_na_input_ESCAPE();
}

void edytowanie_klient(struct klient **head_klient){

    wyczysc_ekran();
    puts(">> klienci mo¿liwi do edycji:\n");

    if(klient_wypisz(head_klient) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wpisz numer u¿ytkownika którego chcesz edytowaæ:\n<< ");

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
        puts(">> Wybierz pole które chcesz edytowaæ:");
        puts(">> 1. Numer telefonu");
        puts(">> 2. Imiê");
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
                printf(">> Wpisz nowe imiê klienta:\n<< ");
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
                printf("\n>> Wpisz nowe imiê klienta:\n<< ");
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
        puts("\n>> Czy chcesz zaakceptowaæ zmiany?\n>> 1. Tak\n>> 2. Nie");

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
        puts(">> Czy chcesz wpisaæ nowe dane?\n>> 1. Tak\n>> 2. Nie, wyjdŸ z edycji danych");

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
        printf(">> Wpisz ile sztuk filmu chcesz dodaæ do bazy (1-99):\n<< ");
        sztuki_dostepne = input_int(1, 99);
        printf("\n>> Wpisz rok produkcji filmu:\n<< ");
        rok_produkcji = input_int(1800,2020);
        printf("\n>> Wpisz tytu³ filmi: (max. 30 liter)\n<< ");
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
            puts(">> Istnieje ju¿ film o takich samych danych");
            puts(">> Wybierz interesuj¹c¹ Ciê opcje:");
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

    printf("\n>> Wpisz numer filmu który chcesz usun¹æ:\n<< ");

    int ilosc_dostepnych_filmow = film_licz_dostepne(*head_film);
    int numer_filmu = input_int(1, ilosc_dostepnych_filmow);
    struct film *film_usuwany_poprzedni = film_szukaj_po_kolejnosci_dostepne_poprzedni(head_film, numer_filmu);
    struct film *film_usuwany = film_szukaj_po_kolejnosci_dostepne(head_film, numer_filmu);

    if(film_czy_jedna_sztuka_poprzedni(head_film, film_usuwany_poprzedni) == false) {

        film_usun(head_film, film_usuwany_poprzedni);
    } else {
        printf(">> Wpisz ile sztuk filmu chcesz usun¹æ:\n<< ");

        int sztuki = input_int(1,film_usuwany->sztuki_dostepne);
        film_usuwany -> sztuki_dostepne = film_usuwany -> sztuki_dostepne - sztuki;

        if(film_usuwany -> sztuki_dostepne == 0 && film_usuwany->sztuki_wypozyczone == 0){
            film_usun(head_film, film_usuwany_poprzedni);
        }
    }
    wyczysc_ekran();
    puts(">> Usuniêcie filmu przebie³o pomyœlnie!");
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
    puts(">> filmy mo¿liwe do edycji:\n");

    if(film_wypisz(*head_film) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wpisz numer filmu który chcesz edytowaæ:\n<< ");

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
        puts(">> Wybierz pole które chcesz edytowaæ:");
        puts(">> 1. Iloœæ dostêpnych sztuk");
        puts(">> 2. Rok produkcji");
        puts(">> 3. Tytu³");
        puts(">> 4. Rezyser");
        puts(">> 5. Gatunek");
        puts(">> 6. Wszystkie");
        switch(getch()){
            case 49:{
                wyczysc_ekran();
                printf(">> Wpisz now¹ iloœæ dostepnych sztuk: (1-99)\n<< ");
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
                printf(">> Wpisz nowy tytu³: (max. 30 znaków)\n<< ");
                scanf("%s", tytul);
                break;
            }
            case 52:{
                wyczysc_ekran();
                printf(">> Wpisz nowego re¿ysera: (max. 20 znaków)\n<< ");
                scanf("%s", rezyser);
                break;
            }
            case 53:{
                wyczysc_ekran();
                printf(">> Wpisz nowy gatunek: (max. 10 znaków)\n<< ");
                scanf("%s", gatunek);
                break;
            }
            case 54:{
                wyczysc_ekran();
                printf(">> Wpisz now¹ iloœæ dostêpnych sztuk: (1-99)\n<< ");
                sztuki_dostepne = input_int(1, 99);
                wyczysc_ekran();
                printf(">> Wpisz nowy rok produkcji:\n<< ");
                rok_produkcji = input_int(1800, 2020);
                printf(">> Wpisz nowy tytu³: (max. 30 znaków)\n<< ");
                scanf("%s", tytul);
                printf(">> Wpisz nowego re¿ysera: (max. 20 znaków)\n<< ");
                scanf("%s", rezyser);
                printf(">> Wpisz nowy gatunek: (max. 10 znaków)\n<< ");
                scanf("%s", gatunek);
                break;
            }
        }

        wyczysc_ekran();
        printf(">> Stare dane filmu: %d / %d / %s / %s / %s\n", film_bufor->sztuki_dostepne, film_bufor->rok_produkcji, film_bufor->tytul, film_bufor->rezyser, film_bufor->gatunek);
        printf(">> Nowe dane filmu:  %d / %d / %s / %s / %s\n", sztuki_dostepne, rok_produkcji, tytul, rezyser, gatunek);
        puts(">> Czy chcesz zaakceptowaæ zmiany?\n>> 1. Tak\n>> 2. Nie");
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

        puts(">> Czy chcesz wpisaæ nowe dane?\n>> 1. Tak\n>> 2. Nie, wyjdŸ z edycji danych");
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
    printf(">> Dostêpni klienci:\n");

    if(klient_wypisz(&head_klient) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wybierz numer klienta któremu chcesz wypo¿yczyæ film:\n<< ");

    struct klient *klient_bufor = klient_szukaj_po_kolejnosci(&head_klient, input_int(1,klient_licz(head_klient)));
    wyczysc_ekran();

    if(film_wypisz_dostepne(head_film) != 0) {
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wybierz numer filmu który chcesz wypo¿yczyæ:\n<< ");

    struct film *film_bufor = film_szukaj_po_kolejnosci_dostepne(&head_film, input_int(1,film_licz_dostepne(head_film)));

    printf(">> Na ile tygodni chcesz wypo¿yczyæ film: (conajmniej 1, maksymalnie 52)\n<< ");

    int czas_wypozyczenia = input_int(1,52);
    wypozyczenie_dodaj(head_wypozyczenie, film_bufor, klient_bufor, czas_wypozyczenia);

    wyczysc_ekran();
    puts(">> Dodanie wypo¿yczenia przebie³o pomyœlnie!");
    czekaj_na_input_ESCAPE();
}

void usuwanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie){

    if(wypozyczenie_wypisz(head_wypozyczenie) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n\n>> Wpisz numer wypo¿yczenia które chcesz zwróciæ:\n<< ");

    wypozyczenie_usun(head_wypozyczenie, wypozyczenie_szukaj_po_kolejnosci_poprzedni(head_wypozyczenie, input_int(1,wypozyczenie_licz(head_wypozyczenie))));

    wyczysc_ekran();
    puts(">> Usuniêcie wypo¿yczenia przebie³o pomyœlnie!");
    czekaj_na_input_ESCAPE();
}

void edytowanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie){

    if(wypozyczenie_wypisz(head_wypozyczenie) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n\n>> Wpisz numer wypo¿yczenia które chcesz edytowaæ:\n<< ");

    struct wypozyczenie *wypozyczenie_bufor = wypozyczenie_szukaj_po_kolejnosci(head_wypozyczenie, input_int(1,wypozyczenie_licz(head_wypozyczenie)));

    wyczysc_ekran();
    printf(">> Aktualna data zwrotu wypo¿yczenia: %s\n\n", wypozyczenie_bufor->data_zwrotu);
    printf(">> Wpisz iloœæ tygodni do koñca wypo¿yczenia (0-52)\n");
    printf(">> Tygodnie licz¹ siê od tego momentu, przy wpisaniu 0 wypo¿yczenie zostaje uznane jako przeterminowane.\n<< ");

    wypozyczenie_edytuj(&wypozyczenie_bufor, input_int(0,52));

    wyczysc_ekran();
    printf(">> Edycja wypo¿yczenia przebieg³a pomyœlnie!\n");
    printf(">> Nowa data zwrotu wypo¿yczenia: %s", wypozyczenie_bufor->data_zwrotu);
    czekaj_na_input_ESCAPE();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void zarzadznie_klient(struct klient **head_klient){
    do {
        wyczysc_ekran();
        puts("////// Zarz¹dzanie Klientami //////\n");
        puts(">> 1. Dodaj klienta");
        puts(">> 2. Usuñ klienta");
        puts(">> 3. Edytuj klienta");
        puts(">> 4. Wyœwietl ca³¹ bazê");
        puts("\n>> ESCAPE -> Wróæ do menu g³ównego");
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
        puts("////// Zarz¹dzanie Filmami //////\n");
        puts(">> 1. Dodaj film");
        puts(">> 2. Usuñ film");
        puts(">> 3. Edytuj film");
        puts(">> 4. Wyœwietl ca³¹ bazê");
        puts("\n>> ESCAPE -> Wróæ do menu g³ównego");
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
        puts("////// Zarz¹dzanie Wypo¿yczeniami //////\n");
        puts(">> 1. Dodaj wypo¿yczenie");
        puts(">> 2. Zwrot wypo¿yczenia");
        puts(">> 3. Edytuj wypo¿yczenie");
        puts(">> 4. Wyœwietl wypo¿yczenia");
        puts("\n>> ESCAPE -> Wróæ do menu g³ównego");
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
        puts(">> 1. Dodaj przyk³adowych klientów");
        puts(">> 2. Dodaj przyk³adowe filmy");
        puts(">> 3. Dodaj przyk³adowe wypo¿yczenia (+powy¿sze)");
        puts("\n>> ESCEAPE -> Wróæ do menu g³ównego");

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
                film_dodaj(head_film,1,2010,"Kobbitm", "Al Pacino", "Sci-Fi");
                film_dodaj(head_film,1,2005,"Die Hard", "John Rambo", "Fabularne");
                film_dodaj(head_film,2,2020,"8 Mila", "Sylverst", "Przygodo");
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
        printf("Zmiany zosta³y zapisane");
        czekaj_na_input_ESCAPE();
    }while(1);
}
