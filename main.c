#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <locale.h>
#include "inne.h"
#include "film.h"
#include "klient.h"
#include "wypozyczenie.h"

// FUNKCJE MENU//
void dodawanie_klient(struct klient **head_klient);
void usuwanie_klient(struct klient **head_klient);
void edytowanie_klient(struct klient **head_klient);
void szukaj_klient(struct klient *head_klient);
void dodawanie_film(struct film **head_film);
void usuwanie_film(struct film **head_film);
void edytowanie_film(struct film **head_film);
void szukaj_film(struct film *head_film);
void dodawanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient *head_klient, struct film *head_film);
void usuwanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie);
void edytowanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie);
void szukaj_wypozyczenie(struct wypozyczenie *head_wypozyczenie, struct film *head_film, struct klient *head_klient);
void zalegle_wypozyczenie(struct wypozyczenie *head_wypozyczenie);
void wczytywanie_baz_danych(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);
void zapisywanie_baz_danych(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);


// MENU //
void zarzadznie_klient(struct klient **head_klient);
void zarzadzanie_film(struct film **head_film);
void zarzadzanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);
void wyszukiwarka(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);
void sortowanie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);
void DEBUG_MENU(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film);

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    setlocale(LC_ALL, "Polish");
    struct klient *head_klient = NULL;
    struct film *head_film = NULL;
    struct wypozyczenie *head_wypozyczenie = NULL;
    wczytywanie_baz_danych(&head_wypozyczenie, &head_klient, &head_film);
    powiadom_zalegle(head_wypozyczenie);
    while(1){
        wyczysc_ekran();
        puts("////// Menu Główne //////\n");
        puts(">> 1. Zarządzanie klientami");
        puts(">> 2. Zarządzanie filmami");
        puts(">> 3. Zarządzanie wypożyczeniami");
        puts(">> 4. Wyszukiwarka");
        puts(">> 5. Sortowanie");
        puts(">> 6. DEBUG_MENU");
        puts("\n>> ESCAPE -> Wyjdź z programu");
        switch(getch()) {
            default:{
                break;
            }
            case 27:{
                zapisywanie_baz_danych(&head_wypozyczenie,&head_klient,&head_film);
                wyczysc_ekran();
                puts(">> Czy na pewno chcesz opuścić program?\n");
                puts(">> COKOLWIEK -> Wyjdź");
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
              wyszukiwarka(&head_wypozyczenie, &head_klient, &head_film);
              break;
            }
            case 53:{
                sortowanie(&head_wypozyczenie, &head_klient, &head_film);
                break;
            }
            case 54:{
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
        puts(">> UWAGA! Numeru pesel klienta nie można potem zmienić!");
        printf(">> Wpisz numer pesel klienta:\n<< ");

        numer_klienta = input_ull(10000000000, 99999999999);

        if(klient_czy_pesel_istnieje(*head_klient, numer_klienta) == true){

            wyczysc_ekran();
            puts(">> Istnieje już klient o takim numerze pesel, nie można nadpisać danych");
            puts(">> Czy chcesz wpisać nowy numer?\n>> 1. Tak\n>> 2. Nie, wyjdź do menu");

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
            printf("\n>> Wpisz imię klienta: (max 15 znaków)\n<< ");
            fflush(stdin);
            input_string(imie, 15);
            printf("\n>> Wpisz nazwisko klienta: (max 15 znaków)\n<< ");
            input_string(nazwisko, 15);
            printf("\n>> Wpisz email klienta: (max 30 znaków)\n<< ");
            while(1) {
                input_string(email, 30);
                if(!check_email(email)) puts("Podano nieprawidłowy adres email. Podaj jeszcze raz.\n");
                else break;
            }


            if(!klient_dodaj(head_klient, numer_klienta, numer_telefonu, imie, nazwisko, email)) {
                wyczysc_ekran();
                printf("Nie można otworzyć pliku wymaganego do utworzenia użytkownika. Dodanie nieudane.\n");
                czekaj_na_input_ESCAPE();
                break;
            }

            wyczysc_ekran();
            printf(">> Klient został poprawnie dodany!");
            czekaj_na_input_ESCAPE();
            break;
        }
    }while(1);
    wyczysc_ekran();
}

void usuwanie_klient(struct klient **head_klient) {
    wyczysc_ekran();
    FILE *file = fopen("data/id/last_id_klient.db", "r");
    unsigned int id_klienta;
    if(file == NULL) {
        printf("Nie można usunąć klienta.\n");
        czekaj_na_input_ESCAPE();
        return;
    }
    puts(">> Jeżeli jakiegoś użytkownika nie ma na poniższej liście, posiada on aktualnie wypożyczony film");
    puts(">> klienci możliwi do usunięcia:\n");

    if(klient_wypisz_dostepni(head_klient) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }
    fscanf(file, "%d", &id_klienta);
    fclose(file);
    puts("\n>> Wpisz ID użytkownika którego chcesz usunąć:");

    unsigned int id = input_uint(1,id_klienta);
    struct klient *klient_bufor = klient_szukaj_po_kolejnosci_dostepne_poprzedni(head_klient, id);
    if ((*head_klient)->id_klienta == id && (*head_klient)->ilosc_posiadanych_filmow ==0) {
        klient_usun(head_klient, NULL);
    }
    else if (klient_bufor==NULL || klient_bufor->nastepny->ilosc_posiadanych_filmow != 0) {
        puts("Nie można usunąć klienta o podanym ID.\n");
        czekaj_na_input_ESCAPE();
        return;
    }
    else {
        if (klient_bufor->nastepny->id_klienta != id) {
            printf("Nie znaleziono klienta o takim ID.\n");
            czekaj_na_input_ESCAPE();
            return;
        }
        klient_usun(head_klient, klient_bufor);
    }
    wyczysc_ekran();
    puts(">> Usunięcie klienta przebiegło pomyślnie!");
    czekaj_na_input_ESCAPE();
}

void edytowanie_klient(struct klient **head_klient){

    wyczysc_ekran();
    puts(">> klienci możliwi do edycji:\n");

    if(klient_wypisz(head_klient) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wpisz id użytkownika którego chcesz edytować:\n<< ");

    struct klient *klient_bufor;
    unsigned long long numer_telefonu;
    char imie[15];
    char nazwisko[15];
    char email[30];

    klient_bufor = klient_szukaj_po_kolejnosci(head_klient, input_uint(1,4294967295));
    if (klient_bufor == NULL) {
        puts("Nie znaleziono klienta o takim ID. Edytowanie nieudane.");
        czekaj_na_input_ESCAPE();
        return;
    }
    numer_telefonu = klient_bufor->numer_telefonu;
    strcpy(imie, klient_bufor->imie);
    strcpy(nazwisko, klient_bufor->nazwisko);
    strcpy(email, klient_bufor->email);

    while(1){

        wyczysc_ekran();
        puts(">> Wybierz pole które chcesz edytować:");
        puts(">> 1. Numer telefonu");
        puts(">> 2. Imię");
        puts(">> 3. Nazwisko");
        puts(">> 4. E-Mail");
        puts(">> 5. Wszystkie");
        fflush(stdin);
        switch (getch()) {
            case 49: {
                wyczysc_ekran();
                printf(">> Wpisz nowy numer telefonu:\n<< ");
                numer_telefonu = input_ull(99999999, 1000000000);
                break;
            }
            case 50: {
                wyczysc_ekran();
                printf(">> Wpisz nowe imię klienta:\n<< ");
                input_string(imie, 15);
                break;
            }
            case 51: {
                wyczysc_ekran();
                printf(">> Wpisz nowe nazwisko klienta:\n<< ");
                input_string(nazwisko, 15);
                break;
            }
            case 52: {
                wyczysc_ekran();
                printf(">> Wpisz nowy E-mail klienta:\n<< ");
                while(1) {
                    input_string(email, 30);
                    if(!check_email(email)) puts("Podano nieprawidłowy adres email. Podaj jeszcze raz.\n");
                    else break;
                }
                break;
            }
            case 53: {
                wyczysc_ekran();
                printf(">> Wpisz nowy numer telefonu:\n<< ");
                numer_telefonu = input_ull(99999999, 1000000000);
                printf("\n>> Wpisz nowe imię klienta:\n<< ");
                input_string(imie, 15);
                printf("\n>> Wpisz nowe nazwisko klienta:\n<< ");
                input_string(nazwisko, 15);
                printf("\n>> Wpisz nowy E-mail klienta:\n<< ");
                while(1) {
                    input_string(email, 30);
                    if(!check_email(email)) puts("Podano nieprawidłowy adres email. Podaj jeszcze raz.\n");
                    else break;
                }
                break;
            }
        }

        wyczysc_ekran();
        printf(">> Stare dane klienta: %llu / %llu / %s / %s / %s\n", klient_bufor->numer_klienta,
               klient_bufor->numer_telefonu, klient_bufor->imie, klient_bufor->nazwisko, klient_bufor->email);
        printf("\n>> Nowe dane klienta:  %llu / %llu / %s / %s / %s\n", klient_bufor->numer_klienta, numer_telefonu, imie,
               nazwisko, email);
        puts("\n>> Czy chcesz zaakceptować zmiany?\n>> 1. Tak\n>> 2. Nie");

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
        puts(">> Czy chcesz wpisać nowe dane?\n>> 1. Tak\n>> 2. Nie, wyjdź z edycji danych");

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

void szukaj_klient(struct klient *head_klient){ //TODO: wyszukiwarki używają oryginałów (po wyszukaniu wszystko jest z małych liter)
  while(1){
    wyczysc_ekran();
    puts(">> Po jakim polu chcesz szukać klienta?\n");
    puts(">> 1. Numer Telefonu");
    puts(">> 2. Imię");
    puts(">> 3. Nazwisko");
    puts(">> 4. E-mail");
    puts(">> 5. Ilość posiadanych filmów\n");
    puts(">> ESCAPE -> Wróć");

    switch(getch()){
      default:{
        break;
      }
      case 27:{
        return;
      }
      case 49:{
          wyczysc_ekran();
          printf(">> Wpisz numer telefonu:\n<< ");
          unsigned long long numer_telefonu = input_ull(99999999, 1000000000);
          wyczysc_ekran();
          klient_wypisz_numer_telefonu(&head_klient, numer_telefonu);
          czekaj_na_input_ESCAPE();
        break;
      }
      case 50:{
          wyczysc_ekran();
          printf(">> Wpisz imię klienta:\n<< ");
          char imie[15];
          input_string(imie, 15);
          wyczysc_ekran();
          klient_wypisz_imie(head_klient, imie);
          czekaj_na_input_ESCAPE();
        break;
      }
      case 51:{
          wyczysc_ekran();
          printf(">> Wpisz nazwisko klienta:\n<< ");
          char nazwisko[15];
          input_string(nazwisko, 15);
          wyczysc_ekran();
          klient_wypisz_nazwisko(head_klient, nazwisko);
          czekaj_na_input_ESCAPE();
        break;
      }
      case 52:{
          wyczysc_ekran();
          printf(">> Wpisz adres e-mail klienta:\n<< ");
          char mail[30];
          input_string(mail, 30);
          wyczysc_ekran();
          klient_wypisz_mail(head_klient, mail);
          czekaj_na_input_ESCAPE();
        break;
      }
      case 53:{
          wyczysc_ekran();
          printf(">> Wpisz Ilość posiadanych filmów:\n<< ");
          int ilosc_posiadanych_filmow = input_int(0, 2147483647);
          wyczysc_ekran();
          klient_wypisz_ilosc_posiadanych_filmow(&head_klient, ilosc_posiadanych_filmow);
          czekaj_na_input_ESCAPE();
        break;
      }
    }
  }
}

void sortuj_klient(struct klient **head_klient){
    while(1){
        wyczysc_ekran();
        puts(">> Po jakim polu chcesz sortować bazę klientów?\n");
        puts(">> 1. Numer Telefonu");
        puts(">> 2. Imię");
        puts(">> 3. Nazwisko");
        puts(">> 4. E-mail");
        puts(">> 5. Ilość posiadanych filmów");
        puts(">> 6. Numer pesel\n");
        puts(">> ESCAPE -> Wróć");

        switch(getch()){
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        klient_telefon(head_klient, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        klient_telefon(head_klient, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }

            case 50:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        klient_imie(head_klient, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        klient_imie(head_klient, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }

            case 51:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        klient_nazwisko(head_klient, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        klient_nazwisko(head_klient, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }

            case 52:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        klient_mail(head_klient, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        klient_mail(head_klient, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }

            case 53:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        klient_filmy(head_klient, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        klient_filmy(head_klient, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }

            case 54:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        klient_pesel(head_klient, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        klient_pesel(head_klient, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
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
        printf(">> Wpisz ile sztuk filmu chcesz dodać do bazy (1-99):\n<< ");
        sztuki_dostepne = input_int(1, 99);
        printf("\n>> Wpisz rok produkcji filmu:\n<< ");
        rok_produkcji = input_int(1800,2020);
        fflush(stdin);
        printf("\n>> Wpisz tytuł filmu: (max. 30 liter)\n<< ");
        input_string(tytul, 30);
        printf("\n>> Wpisz reżysera filmu: (max. 20 liter)\n<< ");
        input_string(rezyser, 20);
        printf("\n>> Wpisz gatunek filmu: (max. 10 liter)\n<< ");
        input_string(gatunek, 10);

        if(film_czy_istnieje(*head_film, rok_produkcji,tytul,rezyser,gatunek) == false){
            if(!film_dodaj(head_film,sztuki_dostepne,rok_produkcji,tytul,rezyser,gatunek)) {
                wyczysc_ekran();
                printf("Nie można otworzyć pliku wymaganego do utworzenia filmu. Dodanie nieudane.\n");
                czekaj_na_input_ESCAPE();
                break;
            }
            wyczysc_ekran();
            puts("Dodawanie pomyślne.\n");
            czekaj_na_input_ESCAPE();
            break;

        } else {

            wyczysc_ekran();
            puts(">> Istnieje już film o takich samych danych");
            puts(">> Wybierz interesującą Cię opcje:");
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

    FILE *file = fopen("data/id/last_id_film.db", "r");
    unsigned int ilosc_dostepnych_filmow;
    if(file == NULL) {
        printf("Nie można usunąć filmu.\n");
        czekaj_na_input_ESCAPE();
        return;
    }
    fscanf(file, "%d", &ilosc_dostepnych_filmow);
    fclose(file);
    if(film_wypisz_dostepne(*head_film) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    puts("\n>> Wpisz ID filmu który chcesz usunąć:\n<< ");

    unsigned int id = input_uint(1,ilosc_dostepnych_filmow);
    struct film *film_bufor = film_szukaj_po_kolejnosci_dostepne_poprzedni(head_film, id);

    if ((*head_film)->id_filmu == id && (*head_film)->sztuki_wypozyczone ==0) {
        if(film_czy_jedna_sztuka_poprzedni(head_film, NULL) == false) {
            film_usun(head_film, NULL);
        } else {
            printf(">> Wpisz ile sztuk filmu chcesz usunąć:\n<< ");

            int sztuki = input_int(1,(*head_film)->sztuki_dostepne);
            (*head_film) -> sztuki_dostepne = (*head_film)-> sztuki_dostepne - sztuki;

            if((*head_film) -> sztuki_dostepne == 0){
                film_usun(head_film, NULL);
            }
        }
    }
    else if (film_bufor==NULL || (film_bufor->nastepny->sztuki_wypozyczone != 0 && film_bufor->nastepny->sztuki_dostepne == 0)) {
        puts("Nie można usunąć klienta o podanym ID.\n");
        czekaj_na_input_ESCAPE();
        return;
    }
    else {
        if (film_bufor->nastepny->id_filmu != id) {
            printf("Nie znaleziono filmu o takim ID.\n");
            czekaj_na_input_ESCAPE();
            return;
        }
        if(film_czy_jedna_sztuka_poprzedni(head_film, film_bufor) == false) {

            film_usun(head_film, film_bufor);
        } else {
            printf(">> Wpisz ile sztuk filmu chcesz usunąć:\n<< ");

            int sztuki = input_int(1,film_bufor->nastepny->sztuki_dostepne);
            film_bufor->nastepny->sztuki_dostepne = film_bufor->nastepny->sztuki_dostepne-sztuki;

            if(film_bufor->nastepny->sztuki_dostepne == 0 && film_bufor->nastepny->sztuki_wypozyczone == 0){
                film_usun(head_film, film_bufor);
            }
        }
    }

    wyczysc_ekran();
    puts(">> Usunięcie filmu przebiegło pomyślnie!");
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
    puts(">> filmy możliwe do edycji:\n");

    if(film_wypisz(*head_film) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }
    unsigned int ilosc_dostepnych_filmow;
    printf("\n>> Wpisz ID filmu który chcesz edytować:\n<< ");
        FILE *file = fopen("data/id/last_id_film.db", "r");
        if(file == NULL) {
            printf("Nie można usunąć filmu.\n");
            return;
        }
        fscanf(file, "%d", &ilosc_dostepnych_filmow);
        fclose(file);
    unsigned int numer_filmu = input_uint(1, ilosc_dostepnych_filmow);
    film_bufor = film_szukaj_po_kolejnosci(head_film, numer_filmu);
        if (film_bufor == NULL) {
            printf("Nie znaleziono filmu o takim ID.\n");
            czekaj_na_input_ESCAPE();
            return;
        }
    sztuki_dostepne = film_bufor -> sztuki_dostepne;
    rok_produkcji = film_bufor -> rok_produkcji;
    strcpy(tytul, film_bufor->tytul);
    strcpy(rezyser,film_bufor -> rezyser);
    strcpy(gatunek,film_bufor -> gatunek);

    while(1){
        wyczysc_ekran();
        puts(">> Wybierz pole które chcesz edytować:");
        puts(">> 1. Ilość dostępnych sztuk");
        puts(">> 2. Rok produkcji");
        puts(">> 3. Tytuł");
        puts(">> 4. Reżyser");
        puts(">> 5. Gatunek");
        puts(">> 6. Wszystkie");
        fflush(stdin);
        switch(getch()){
            case 49:{
                wyczysc_ekran();
                printf(">> Wpisz nową ilość dostępnych sztuk: (1-99)\n<< ");
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
                printf(">> Wpisz nowy tytuł: (max. 30 znaków)\n<< ");
                input_string(tytul,30);
                break;
            }
            case 52:{
                wyczysc_ekran();
                printf(">> Wpisz nowego reżysera: (max. 20 znaków)\n<< ");
                input_string(rezyser, 20);
                break;
            }
            case 53:{
                wyczysc_ekran();
                printf(">> Wpisz nowy gatunek: (max. 10 znaków)\n<< ");
                input_string(gatunek, 10);
                break;
            }
            case 54:{
                wyczysc_ekran();
                printf(">> Wpisz nową ilość dostępnych sztuk: (1-99)\n<< ");
                sztuki_dostepne = input_int(1, 99);
                wyczysc_ekran();
                printf(">> Wpisz nowy rok produkcji:\n<< ");
                rok_produkcji = input_int(1800, 2020);
                printf(">> Wpisz nowy tytuł: (max. 30 znaków)\n<< ");
                input_string(tytul,30);
                printf(">> Wpisz nowego reżysera: (max. 20 znaków)\n<< ");
                input_string(rezyser, 20);
                printf(">> Wpisz nowy gatunek: (max. 10 znaków)\n<< ");
                input_string(gatunek, 10);
                break;
            }
        }

        wyczysc_ekran();
        printf(">> Stare dane filmu: %d / %d / %s / %s / %s\n", film_bufor->sztuki_dostepne, film_bufor->rok_produkcji, film_bufor->tytul, film_bufor->rezyser, film_bufor->gatunek);
        printf(">> Nowe dane filmu:  %d / %d / %s / %s / %s\n", sztuki_dostepne, rok_produkcji, tytul, rezyser, gatunek);
        puts(">> Czy chcesz zaakceptować zmiany?\n>> 1. Tak\n>> 2. Nie");
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

        puts(">> Czy chcesz wpisać nowe dane?\n>> 1. Tak\n>> 2. Nie, wyjdź z edycji danych");
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

void szukaj_film(struct film *head_film){ //TODO: wyszukiwarki używają oryginałów (po wyszukaniu wszystko jest z małych liter)
    while(1){
        wyczysc_ekran();
        puts(">> Po jakim polu chcesz szukać filmu?\n");
        puts(">> 1. Sztuki Dostępne");
        puts(">> 2. Sztuki Wypożyczone");
        puts(">> 3. Rok Produkcji");
        puts(">> 4. Tytuł");
        puts(">> 5. Reżyser");
        puts(">> 6. Gatunek\n");
        puts(">> ESCAPE -> Wróć");
        fflush(stdin);
        switch(getch()){
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49:{
                wyczysc_ekran();
                printf(">> Wpisz Sztuki Dostępne:\n<< ");
                int sztuki_dostepne = input_int(0, 2147483647);
                wyczysc_ekran();
                film_wypisz_sztuki_dostepne(head_film, sztuki_dostepne);
                czekaj_na_input_ESCAPE();
                break;
            }
            case 50:{
                wyczysc_ekran();
                printf(">> Wpisz Sztuki Wypożyczone:\n<< ");
                int sztuki_wypozyczone = input_int(0, 2147483647);
                wyczysc_ekran();
                film_wypisz_sztuki_wypozyczone(head_film, sztuki_wypozyczone);
                czekaj_na_input_ESCAPE();
                break;
            }
            case 51:{
                wyczysc_ekran();
                printf(">> Wpisz Rok Produkcji:\n<< ");
                int rok_produkcji = input_int(0, 2147483647);
                wyczysc_ekran();
                film_wypisz_rok_produkcji(head_film, rok_produkcji);
                czekaj_na_input_ESCAPE();
                break;
            }
            case 52:{
                wyczysc_ekran();
                printf(">> Wpisz tytuł filmu:\n<< ");
                char tytul[30];
                input_string(tytul, 30);
                wyczysc_ekran();
                film_wypisz_tytul(head_film, tytul);
                czekaj_na_input_ESCAPE();
                break;
            }
            case 53:{
                wyczysc_ekran();
                printf(">> Wpisz reżysera filmu:\n<< ");
                char rezyser[20];
                input_string(rezyser, 20);
                wyczysc_ekran();
                film_wypisz_rezyser(head_film, rezyser);
                czekaj_na_input_ESCAPE();
                break;
            }
            case 54:{
                wyczysc_ekran();
                printf(">> Wpisz gatunek filmu:\n<< ");
                char gatunek[10];
                input_string(gatunek, 10);
                wyczysc_ekran();
                film_wypisz_gatunek(head_film, gatunek);
                czekaj_na_input_ESCAPE();
                break;
            }
        }
    }
}

void sortuj_film(struct film **head_film){
    while(1){
        wyczysc_ekran();
        puts(">> Po jakim polu chcesz sortować bazę filmów?\n");
        puts(">> 1. Sztuki dostępne");
        puts(">> 2. Sztuki wypożyczone");
        puts(">> 3. Rok produkcji");
        puts(">> 4. Tytuł");
        puts(">> 5. Reżyser");
        puts(">> 6. Gatunek\n");
        puts(">> ESCAPE -> Wróć");

        switch(getch()){
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        film_dostepne(head_film, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        film_dostepne(head_film, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }
            case 50:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        film_wypozyczone(head_film, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        film_wypozyczone(head_film, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }
            case 51:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        film_rok(head_film, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        film_rok(head_film, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }
            case 52:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        film_tytul(head_film, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        film_tytul(head_film, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }
            case 53:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        film_rezyser(head_film, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        film_rezyser(head_film, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }
            case 54:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        film_gatunek(head_film, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        film_gatunek(head_film, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }
        }
    }
}

void dodawanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie, struct klient *head_klient, struct film *head_film){

    wyczysc_ekran();
    printf(">> Dostępni klienci:\n");

    if(klient_wypisz(&head_klient) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wybierz ID klienta któremu chcesz wypożyczyć film:\n<< ");
    unsigned int id_klienta =input_uint(1,4294967295);
    struct klient *klient_bufor = klient_szukaj_po_kolejnosci(&head_klient, id_klienta);
    if(klient_bufor == NULL) {
        printf("Nie znaleziono klienta o takim ID.\n");
        czekaj_na_input_ESCAPE();
        return;
    }
    wyczysc_ekran();

    if(film_wypisz_dostepne(head_film) != 0) {
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n>> Wybierz ID filmu który chcesz wypożyczyć:\n<< ");
        unsigned int id_filmu =input_uint(1,4294967295);
    struct film *film_bufor = film_szukaj_po_kolejnosci_dostepne(&head_film, id_filmu);
        if(film_bufor == NULL) {
            printf("Nie znaleziono filmu o takim ID.\n");
            czekaj_na_input_ESCAPE();
            return;
        }
    printf(">> Na ile tygodni chcesz wypożyczyć film: (conajmniej 1, maksymalnie 52)\n<< ");

    int czas_wypozyczenia = input_int(1,52);


    if(!wypozyczenie_dodaj(head_wypozyczenie, film_bufor, klient_bufor, czas_wypozyczenia)) {
        wyczysc_ekran();
        printf("Nie można otworzyć pliku wymaganego do utworzenia wypożyczenia. Dodanie nieudane.\n");
        czekaj_na_input_ESCAPE();
        return;
    }

    wyczysc_ekran();
    puts(">> Dodanie wypożyczenia przebiegło pomyślnie!");
    czekaj_na_input_ESCAPE();
}

void usuwanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie){
    double kara = wczytaj_kare_z_pliku();
    if(wypozyczenie_wypisz(head_wypozyczenie) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }
    printf("\n\n>> Wpisz ID wypożyczenia które chcesz zwrócić:\n<< ");
    time_t aktualna_data = time(NULL);
    unsigned int numer_wypozyczenia = input_uint(1,4294967295);
    struct wypozyczenie *wypozyczenie_bufor = wypozyczenie_szukaj_po_kolejnosci_poprzedni(head_wypozyczenie, numer_wypozyczenia);
    if(wypozyczenie_bufor != NULL && aktualna_data > wypozyczenie_bufor->nastepny->data_zwrotu_sekundy){
        wyczysc_ekran();
        int dni_opoznienia = ((aktualna_data - wypozyczenie_bufor->nastepny->data_zwrotu_sekundy)/ 86400);
        printf(">> Dni opónienia: %d, Naliczona kara: %.2fzł. Nie zapomnij pobrać opłaty!\n", dni_opoznienia, dni_opoznienia*kara);
        czekaj_na_input_ESCAPE();
    }
    else if(wypozyczenie_bufor == NULL && aktualna_data > (*head_wypozyczenie)->data_zwrotu_sekundy){
        wyczysc_ekran();
        int dni_opoznienia = ((aktualna_data - (*head_wypozyczenie)->data_zwrotu_sekundy)/ 86400);
        printf(">> Dni opónienia: %d, Naliczona kara: %.2fzł. Nie zapomnij pobrać opłaty!\n", dni_opoznienia, dni_opoznienia*kara);
        czekaj_na_input_ESCAPE();
    }
    else if (wypozyczenie_bufor ==NULL && numer_wypozyczenia != (*head_wypozyczenie)->id_wypozyczenia){
        wyczysc_ekran();
        puts("Nie znaleziono wypożyczenia o podanym ID.\n");
        czekaj_na_input_ESCAPE();
        return;
    }
    wypozyczenie_usun(head_wypozyczenie, wypozyczenie_bufor);
    wyczysc_ekran();
    puts(">> Usunięcie wypożyczenia przebiegło pomyślnie!");
    czekaj_na_input_ESCAPE();
}

void edytowanie_wypozyczenie(struct wypozyczenie **head_wypozyczenie){

    if(wypozyczenie_wypisz(head_wypozyczenie) != 0){
        czekaj_na_input_ESCAPE();
        return;
    }

    printf("\n\n>> Wpisz ID wypożyczenia które chcesz edytować:\n<< ");
    unsigned int id = input_uint(1,4294967295);
    struct wypozyczenie *wypozyczenie_bufor = wypozyczenie_szukaj_po_kolejnosci(head_wypozyczenie, id);
        if(wypozyczenie_bufor == NULL){
            wyczysc_ekran();
            puts("Nie znaleziono wypożyczenia o podanym numerze ID.\n");
            czekaj_na_input_ESCAPE();
            return;
        }
    wyczysc_ekran();
    printf(">> Aktualna data zwrotu wypożyczenia: %s\n\n", wypozyczenie_bufor->data_zwrotu);
    printf(">> Wpisz ilość tygodni do końca wypożyczenia (0-52)\n");
    printf(">> Tygodnie liczą się od tego momentu, przy wpisaniu 0 wypożyczenie zostaje uznane jako zwrot dzisiejszy.\n<< ");

    wypozyczenie_edytuj(&wypozyczenie_bufor, input_int(0,52));

    wyczysc_ekran();
    printf(">> Edycja wypożyczenia przebiegła pomyślnie!\n");
    printf(">> Nowa data zwrotu wypożyczenia: %s", wypozyczenie_bufor->data_zwrotu);
    czekaj_na_input_ESCAPE();
}

void szukaj_wypozyczenie(struct wypozyczenie *head_wypozyczenie, struct film *head_film, struct klient *head_klient){
    while(1){
        wyczysc_ekran();
        puts(">> Po jakim polu chcesz szukać Wypożyczenia?\n");
        puts(">> 1. Klient wypożyczający");
        puts(">> 2. Film wypożoczony");
        puts(">> 3. Data Wypożyczenia");
        puts(">> 4. Data Zwrotu\n");
        puts(">> ESCAPE -> Wróć");

        switch(getch()){
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49:{
                wyczysc_ekran();
                klient_wypisz(&head_klient);
                printf("\nWpisz ID klienta, którego wypożyczenia chcesz wyszukać:");
                unsigned int id;
                id = input_uint(1, 4294967295);
                wyczysc_ekran();
                wypozeczenie_wypisz_klient(head_wypozyczenie,id);
                czekaj_na_input_ESCAPE();
                break;
            }
            case 50:{
                wyczysc_ekran();
                film_wypisz(head_film);
                printf("\nWpisz ID filmu, którego wypożyczenia chcesz wyszukać:");
                unsigned int id;
                id = input_uint(1, 4294967295);
                wyczysc_ekran();
                wypozeczenie_wypisz_film(head_wypozyczenie,id);
                czekaj_na_input_ESCAPE();
                break;
            }
            case 51:{
                wyczysc_ekran();
                printf(">> Wpisz date wypożyczenia filmu (format rrrr/mm/dd):\n<< "); //TODO: do poprawy (użyć też sekund daty)
                char data[11];
                input_string(data, 11);
                wyczysc_ekran();
                wypozeczenie_wypisz_data(head_wypozyczenie,data);
                czekaj_na_input_ESCAPE();
                break;
            }
            case 52:{
                wyczysc_ekran();
                printf(">> Wpisz date zwrotu filmu (format rrrr/mm/dd):\n<< "); //TODO: do poprawy (użyć też sekund daty)
                char data[11];
                input_string(data, 11);
                wyczysc_ekran();
                wypozeczenie_wypisz_data_zwrot(head_wypozyczenie,data);
                czekaj_na_input_ESCAPE();
                break;
            }
        }
    }
}

void zalegle_wypozyczenie(struct wypozyczenie *head_wypozyczenie){
    while(1){
        wyczysc_ekran();
        puts(">> 1. Wyświetl zaległe wypożyczenia");
        puts(">> 2. Ustal wysokość kary");
        puts("\n>> ESCAPE -> Wróć");

        switch(getch()){
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49:{
                if(head_wypozyczenie != NULL){
                    double kara = wczytaj_kare_z_pliku();
                    wypozyczenie_wypisz_zalegle(&head_wypozyczenie, kara);
                    czekaj_na_input_ESCAPE();
                }
                else{
                    wyczysc_ekran();
                    printf(">> Lista wypożyczeń jest pusta!");
                    czekaj_na_input_ESCAPE();
                }
                break;
            }
            case 50:{
                wyczysc_ekran();
                double kara = wczytaj_kare_z_pliku();
                printf(">> Aktualna kara : %.2f\n", kara);
                printf(">> Wpisz nową wielkość kary za 1 dzień zwłoki: (0zł-200zł)\n<< ");
                kara = input_dbl(0.0,200.0);
                zapisz_kare_do_pliku(kara);
                break;
            }
        }
    }
}

void sortuj_wypozyczenie(struct wypozyczenie **head_wypozyczenie){
    while(1){
        wyczysc_ekran();
        puts(">> Po jakim polu chcesz sortować bazę wypożyczeń?\n");
        puts(">> 1. Data wypożyczenia");
        puts(">> 2. Data zwrotu");
        puts(">> 3. Imię klienta");
        puts(">> 4. Nazwisko klienta");
        puts(">> 5. Tytuł\n");
        puts(">> ESCAPE -> Wróć");

        switch(getch()){
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49:{
                wyczysc_ekran();
                puts(">> 1. Sortuj od najstarszej daty");
                puts(">> 2. Sortuj od najnowszej daty\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        wypozyczenie_data_wypozyczenia(head_wypozyczenie, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        wypozyczenie_data_wypozyczenia(head_wypozyczenie, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }
            case 50:{
                wyczysc_ekran();
                puts(">> 1. Sortuj od najstarszej daty");
                puts(">> 2. Sortuj od najnowszej daty\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        wypozyczenie_data_zwrotu(head_wypozyczenie, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        wypozyczenie_data_zwrotu(head_wypozyczenie, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }

            case 51:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        wypozyczenie_imie(head_wypozyczenie, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        wypozyczenie_imie(head_wypozyczenie, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }

            case 52:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        wypozyczenie_nazwisko(head_wypozyczenie, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        wypozyczenie_nazwisko(head_wypozyczenie, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }

            case 53:{
                wyczysc_ekran();
                puts(">> 1. Sortuj rosnąco");
                puts(">> 2. Sortuj malejąco\n");
                puts(">> Inne -> Wróć");
                switch (getch()) {
                    default:{
                        break;
                    }
                    case 49: {
                        wyczysc_ekran();
                        wypozyczenie_tytul(head_wypozyczenie, 0);
                        puts("Sortowanie rosnące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                    case 50: {
                        wyczysc_ekran();
                        wypozyczenie_tytul(head_wypozyczenie, 1);
                        puts("Sortowanie malejące zakończone pomyślnie.");
                        czekaj_na_input_ESCAPE();
                        break;
                    }
                }
                break;
            }
        }
    }
}


void wczytywanie_baz_danych(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
    FILE *file_wypozyczenie = fopen("data/databases/wypozyczenie.db", "r");
    FILE *file_klient = fopen("data/databases/klient.db", "r");
    FILE *file_film = fopen("data/databases/film.db", "r");
    if(file_wypozyczenie == NULL || file_klient == NULL || file_film == NULL){
        return;
    }
    puts(">> Zostały wykryte bazy danych, czy chcesz wczytać je do programu?\n ");
    puts(">> ENTER -> Tak");
    puts(">> ESC -> Nie");
    while(1) {
        switch (getch()) {
            default: {
                break;
            }
            case 13: {
                film_wczytaj_z_pliku(head_film);
                film_zamien_tylde_na_spacje(*head_film);
                klient_wczytaj_z_pliku(head_klient);
                klient_zamien_tylde_na_spacje(*head_klient);
                wypozyczenie_wczytaj_z_pliku(head_wypozyczenie);
                wypozyczenie_przebuduj_znaczniki(*head_wypozyczenie, *head_film, *head_klient);
                fclose(file_film);
                fclose(file_klient);
                fclose(file_wypozyczenie);
                return;
            }
            case 27: {
                fclose(file_film);
                fclose(file_klient);
                fclose(file_wypozyczenie);
                return;
            }
        }
    }
}

void zapisywanie_baz_danych(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
    while(1){
        wyczysc_ekran();
        puts(">> Czy chcesz zapisać aktualny stan baz danych?");
        puts(">> UWAGA! Wyjście bez zapisywania skutkuje utratą zmian w aktualnej sesji!\n");
        puts(">> ENTER -> TAK");
        puts(">> ESCAPE -> NIE");
        switch(getch()){
            default:{
                break;
            }
            case 13: {
                film_zamien_spacje_na_tylde(*head_film);
                film_zapisz_do_pliku(*head_film);
                klient_zamien_spacje_na_tylde(*head_klient);
                klient_zapisz_do_pliku(*head_klient);
                wypozyczenie_zapisz_do_pliku(*head_wypozyczenie);
            }
            case 27:{
                return;
            }
        }
    }


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

void zarzadznie_klient(struct klient **head_klient){
    do {
        wyczysc_ekran();
        puts("////// Zarządzanie Klientami //////\n");
        puts(">> 1. Dodaj klienta");
        puts(">> 2. Usuń klienta");
        puts(">> 3. Edytuj klienta");
        puts(">> 4. Wyświetl wszystkich klientów");
        puts("\n>> ESCAPE -> Wróć do menu głównego");
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
        puts("////// Zarządzanie Filmami //////\n");
        puts(">> 1. Dodaj film");
        puts(">> 2. Usuń film");
        puts(">> 3. Edytuj film");
        puts(">> 4. Wyświetl wszystkie filmy");
        puts("\n>> ESCAPE -> Wróć do menu głównego");
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
        puts("////// Zarządzanie Wypożyczeniami //////\n");
        puts(">> 1. Dodaj wypożyczenie");
        puts(">> 2. Zwrot wypożyczenia");
        puts(">> 3. Edytuj wypożyczenie");
        puts(">> 4. Aktualne wypożyczenia");
        puts(">> 5. Zaległe zwroty");
        puts("\n>> ESCAPE -> Wróć do menu głównego");
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
            case 53: {
                zalegle_wypozyczenie((*head_wypozyczenie));
            }
        }
    }while(1);
}

void wyszukiwarka(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
  while(1){
    wyczysc_ekran();
    puts("////// Wyszukiwarka //////\n");
    puts(">> 1. Szukaj klienta");
    puts(">> 2. Szukaj film");
    puts(">> 3. Szukaj wypożyczenia");
    puts("\n>> ESCAPE -> Wróć do menu głównego");
    switch(getch()){
      default:{
        break;
      }
      case 27:{
        return;
      }
      case 49:{
          if((*head_klient) != NULL){
              szukaj_klient((*head_klient));
          }
          else{
              wyczysc_ekran();
              puts(">> Baza klientów jest pusta!");
              czekaj_na_input_ESCAPE();
          }
        break;
      }
      case 50:{
          if((*head_film) != NULL){
              szukaj_film((*head_film));
          }
          else{
              wyczysc_ekran();
              puts(">> Baza filmów jest pusta!");
              czekaj_na_input_ESCAPE();
          }
        break;
      }
      case 51:{
          if((*head_wypozyczenie) != NULL){
              szukaj_wypozyczenie((*head_wypozyczenie), (*head_film), (*head_klient));
          }
          else{
              wyczysc_ekran();
              puts(">> Baza wypożyczeń jest pusta!");
              czekaj_na_input_ESCAPE();
          }
        break;
      }
    }
  }
}

void sortowanie(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
    while(1){
        wyczysc_ekran();
        puts("////// Sortowanie //////\n");
        puts(">> 1. Sortuj bazę klientów");
        puts(">> 2. Sortuj bazę filmów");
        puts(">> 3. Sortuj bazę wypożyczeń");
        puts("\n>> ESCAPE -> Wróć do menu głównego");
        switch(getch()){
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49:{
                if((*head_klient) != NULL){
                    sortuj_klient(head_klient);
                }
                else{
                    wyczysc_ekran();
                    puts(">> Baza klientów jest pusta!");
                    czekaj_na_input_ESCAPE();
                }
                break;
            }
            case 50:{
                if((*head_film) != NULL){
                    sortuj_film(head_film);
                }
                else{
                    wyczysc_ekran();
                    puts(">> Baza filmów jest pusta!");
                    czekaj_na_input_ESCAPE();
                }
                break;
            }
            case 51:{
                if((*head_wypozyczenie) != NULL){
                    sortuj_wypozyczenie(head_wypozyczenie);
                }
                else{
                    wyczysc_ekran();
                    puts(">> Baza wypożyczeń jest pusta!");
                    czekaj_na_input_ESCAPE();
                }
                break;
            }
        }
        wypozyczenie_przebuduj_znaczniki(*head_wypozyczenie, *head_film, *head_klient);
    }
}

void DEBUG_MENU(struct wypozyczenie **head_wypozyczenie, struct klient **head_klient, struct film **head_film){
    do {
        wyczysc_ekran();
        puts("////// DEBUG MENU//////\n");
        puts(">> UWAGA, korzystanie z tych funkcji może zepsuć działanie programu\n");
        puts(">> 1. Dodaj przykładowych klientów");
        puts(">> 2. Dodaj przykładowe filmy");
        puts(">> 3. Dodaj przykładowe wypożyczenia (+powyższe)");
        puts(">> 4. Zapisz bazy danych do plików");
        puts(">> 5. Wczytaj bazy danych z plików");
        puts("\n>> ESCEAPE -> Wróć do menu głównego");
        switch (getch()) {
            default:{
                break;
            }
            case 27:{
                return;
            }
            case 49: {
                klient_dodaj(head_klient, 98932401321, 123456789, "Maciej", "Kowalski", "m.kowalski123@gmail.com");
                klient_dodaj(head_klient, 32455123458, 123456789, "Michal", "Szewczyk", "szewczyk@buziaczek.pl");
                klient_dodaj(head_klient, 12356234123, 123672134, "Tomasz", "Nowak", "t.Nowaczek@op.pl");
                klient_dodaj(head_klient, 12562341233, 634126234, "Jakub", "Milek", "JakubMilek@gmail.com");
                break;
            }
            case 50: {
                film_dodaj(head_film,3,1998,"Harry Potter", "J.k.", "Horror");
                film_dodaj(head_film,1,2010,"Kobbitm", "Al Pacino", "Sci-Fi");
                film_dodaj(head_film,1,2005,"Die Hard", "John Rambo", "Fabularne");
                film_dodaj(head_film,2,2020,"8 Mila", "Sylverst", "Przygodo");
                break;
            }
            case 51: {
                klient_dodaj(head_klient, 98932401321, 123456789, "Maciej", "Kowalski", "m.kowalski123@gmail.com");
                klient_dodaj(head_klient, 32455123458, 123456789, "Michal", "Szewczyk", "szewczyk@buziaczek.pl");
                klient_dodaj(head_klient, 12356234123, 123672134, "Tomasz", "Nowak", "t.Nowaczek@op.pl");
                klient_dodaj(head_klient, 12562341233, 634126234, "Jakub", "Milek", "JakubMilek@gmail.com");
                film_dodaj(head_film,3,1998,"Harry Potter", "J.k.", "Horror");
                film_dodaj(head_film,1,2010,"Kobbitm", "Al Pacino", "Sci-Fi");
                film_dodaj(head_film,1,2005,"Die Hard", "John Rambo", "Fabularne");
                film_dodaj(head_film,2,2020,"8 Mila", "Sylverst", "Przygodo");
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film), (*head_klient), -2);
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film)->nastepny, (*head_klient)->nastepny, 10);
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film)->nastepny->nastepny, (*head_klient)->nastepny->nastepny, 30);
                wypozyczenie_dodaj(head_wypozyczenie, (*head_film)->nastepny->nastepny->nastepny, (*head_klient)->nastepny->nastepny->nastepny, -1);
                break;
            }
            case 52:{
                film_zamien_spacje_na_tylde(*head_film);
                film_zapisz_do_pliku(*head_film);
                klient_zamien_spacje_na_tylde(*head_klient);
                klient_zapisz_do_pliku(*head_klient);
                wypozyczenie_zapisz_do_pliku(*head_wypozyczenie);
                break;
            }
            case 53:{
                film_wczytaj_z_pliku(head_film);
                film_zamien_tylde_na_spacje(*head_film);
                klient_wczytaj_z_pliku(head_klient);
                klient_zamien_tylde_na_spacje(*head_klient);
                wypozyczenie_wczytaj_z_pliku(head_wypozyczenie);
                wypozyczenie_przebuduj_znaczniki(*head_wypozyczenie, *head_film, *head_klient);
                break;
            }
        }
        wyczysc_ekran();
        printf("Zmiany zostały zapisane");
        czekaj_na_input_ESCAPE();
    }while(1);
}
