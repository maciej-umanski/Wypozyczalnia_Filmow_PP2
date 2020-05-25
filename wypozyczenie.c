#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include "wypozyczenie.h"

/**
 * \brief Struktura przechowująca wypożyczenia filmów
 * @param id_wypozyczenia unikalny identyfikator elementu
 * @param numer_klienta id klienta wypożyczającego
 * @param numer_filmu id wypożyczonego filmu
 * @param data_wypozyczenia[11] data wypożyczenia filmu
 * @param data_wypozyczenia_sekundy ilość sekund, która upłynęła od 1.01.1970 do momentu wypożyczenia
 * @param data_zwrotu[11] data planowanego zwrotu filmu
 * @param data_zwrotu_sekundy ilość sekund, która upłynęła od 1.01.1970 do planowego zwrotu filmu
 * @param *znacznik_klienta wskaźnik na element z listy klientów
 * @param *znacznik_filmu wskaźnik na element z listy filmów
 * @param *nastepny wskaźnik na następny element listy
 */

struct wypozyczenie{
    unsigned int id_wypozyczenia;
    unsigned int numer_klienta;
    unsigned int numer_filmu;
    char data_wypozyczenia[11];
    time_t data_wypozyczenia_sekundy;
    char data_zwrotu[11];
    time_t data_zwrotu_sekundy;
    struct klient *znacznik_klienta;
    struct film *znacznik_filmu;
    struct wypozyczenie *nastepny;
};

/**
 * \brief Dodanie wypożyczenia do listy
 * @param **head_wypozyczenie wskaźnik na listę wypożyczeń
 * @param *film_wypozyczany wskaźnik na wypożyczany film
 * @param *klient_wypozyczajacy wskaźnik na klienta wypożyczającego
 * @param ile_tygodni czas wypożyczenia liczony w tygodniach
 * @return true - dodanie udane, false - dodanie nieudane
 */

bool wypozyczenie_dodaj(struct wypozyczenie **head_wypozyczenie, struct film *film_wypozyczany, struct klient *klient_wypozyczajacy,  int ile_tygodni){
            if(!dodaj_folder("data")) return false;
            if(!dodaj_folder("data/id")) return false;
            FILE *file = fopen("data/id/last_id_wypozyczenie.db", "r");
            if (file==NULL && (*head_wypozyczenie)!=NULL) {
                return false;
            }
            unsigned int id;
            char temp[10];
            fgets(temp, 10, file);
            sscanf(temp,"%d", &id);
            fclose(file);
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

    if((*head_wypozyczenie)== NULL) {
        if(!file) {
            nowe_wypozyczenie->id_wypozyczenia = 1;
            id = 1;
        }
        else {
            nowe_wypozyczenie->id_wypozyczenia = ++id;
        }
    }
    else {
        nowe_wypozyczenie->id_wypozyczenia = ++id;
    }

    nowe_wypozyczenie->numer_filmu = film_wypozyczany->id_filmu;
    nowe_wypozyczenie->znacznik_filmu = film_wypozyczany;
    nowe_wypozyczenie->numer_klienta = klient_wypozyczajacy->id_klienta;
    nowe_wypozyczenie->znacznik_klienta = klient_wypozyczajacy;
    nowe_wypozyczenie->nastepny = (*head_wypozyczenie);
    nowe_wypozyczenie->data_wypozyczenia_sekundy = data_wypozyczenia_sekundy;
    nowe_wypozyczenie->data_zwrotu_sekundy = data_zwrotu_sekundy;
    strcpy(nowe_wypozyczenie->data_zwrotu, data_zwrotu);
    strcpy(nowe_wypozyczenie->data_wypozyczenia, data_wypozyczenia);
    film_wypozyczany->sztuki_dostepne--;
    film_wypozyczany->sztuki_wypozyczone++;
    klient_wypozyczajacy->ilosc_posiadanych_filmow++;
    (*head_wypozyczenie) = nowe_wypozyczenie;
    FILE *file2 = fopen("data/id/last_id_wypozyczenie.db", "w");
    fprintf(file2, "%d", id);
    fclose(file2);
    return true;
}

/**
 * \brief Usunięcie wypożyczenia z listy
 * @param **head_wypozyczenie wskaźnik na listę wypożyczeń
 * @param *wypozyczenie_przed_usuwanym wskaźnik na element poprzedzający element usuwany
 */

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

/**
 * \brief Edytowanie istniejącego elementu listy wypożyczeń
 * @param **wypozyczenie_edytowane wskaźnik na edytowany element
 * @param czas_do_zwrotu nowy czas zwrotu wypożyczenia
 */

void wypozyczenie_edytuj(struct wypozyczenie **wypozyczenie_edytowane, int czas_do_zwrotu){
    time_t czas1 = time(NULL) + (czas_do_zwrotu * 604800);
    struct tm *czas2;
    czas2 = localtime(&czas1);
    strftime((*wypozyczenie_edytowane)->data_zwrotu, 11, "%Y/%m/%d", czas2);
    (*wypozyczenie_edytowane) -> data_zwrotu_sekundy = czas1;
}

/**
 * \brief Kopiowanie wypożyczenia podczas sortowania
 *
 * Funkcja tworzy w podanej liście nowy element i kopiuje do niego wszystkie dane z podanego bufora
 * @param **head_wypozyczenie wskaźnik na listę do której zostanie dodany nowy element
 * @param **bufor wskaźnik na element przechowujący dane do skopiowania
 */

void push_wypozyczenie (struct wypozyczenie **head_wypozyczenie, struct wypozyczenie **bufor){
    struct wypozyczenie *nowe_wypozyczenie = (struct wypozyczenie *) malloc(sizeof(struct wypozyczenie));
    nowe_wypozyczenie->id_wypozyczenia = (*bufor)->id_wypozyczenia;
    nowe_wypozyczenie->numer_filmu = (*bufor)->numer_filmu;
    nowe_wypozyczenie->znacznik_filmu = (*bufor)->znacznik_filmu;
    nowe_wypozyczenie->numer_klienta = (*bufor)->numer_klienta;
    nowe_wypozyczenie->znacznik_klienta = (*bufor)->znacznik_klienta;
    nowe_wypozyczenie->data_wypozyczenia_sekundy = (*bufor)->data_wypozyczenia_sekundy;
    nowe_wypozyczenie->data_zwrotu_sekundy = (*bufor)->data_zwrotu_sekundy;
    strcpy(nowe_wypozyczenie->data_zwrotu, (*bufor)->data_zwrotu);
    strcpy(nowe_wypozyczenie->data_wypozyczenia, (*bufor)->data_wypozyczenia);
    nowe_wypozyczenie->nastepny = (*head_wypozyczenie);
    *head_wypozyczenie = nowe_wypozyczenie;
}

/**
 * \brief Wyszukiwanie wypożyczenia po ID
 * @param **head_wypozyczenie wskaźnik na listę wypożyczeń
 * @param id szukany identyfikator
 * @return poszukiwany element lub NULL, jeśli nie znaleziony
 */

struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci(struct wypozyczenie **head_wypozyczenie, unsigned int id){
    struct wypozyczenie *bufor = *head_wypozyczenie;
    int i=0;
    while (bufor != NULL){
        if(id == bufor->id_wypozyczenia) {
            i++;
            break;
        }
        bufor = bufor -> nastepny;
    }
    if(i==0) return NULL;
    return bufor;
}

/**
 * \brief Wyszukiwanie wypożyczenia po ID
 * @param **head_wypozyczenie wskaźnik na listę wypożyczeń
 * @param id szukany identyfikator
 * @return element poprzedzający poszukiwany element lub NULL, jeśli nie znaleziony
 */

struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci_poprzedni(struct wypozyczenie **head_wypozyczenie, unsigned int id){
    struct wypozyczenie *poprzedni = NULL;
    struct wypozyczenie *bufor = *head_wypozyczenie;
    int i=0;
    while (bufor != NULL){
        if(id == bufor->id_wypozyczenia) {
            i++;
            break;
        }
        poprzedni = bufor;
        bufor = bufor -> nastepny;
    }
    if(i==0) return NULL;
    return poprzedni;
}

/**
 * \brief Wypisanie listy wypożyczeń
 * @param **head_wypozyczenie wskaźnik na listę do wypisania
 * @return 0 w przypadku powodzenia, -1 gdy lista jest pusta
 */

int wypozyczenie_wypisz(struct wypozyczenie **head_wypozyczenie){
    system("cls");
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    if(wypozyczenie_bufor == NULL){
        puts(">> Lista wypożyczeń jest pusta!");
        return -1;
    }
    printf(">> Aktywne wypożyczenia:\n");
    printf(">> ID. |%15s |%15s |%30s |%18s |%12s |\n", "Imie", "Nazwisko", "Tytuł Filmu", "Data wypożyczenia", "Data Zwrotu");
    while(wypozyczenie_bufor != NULL){
        printf("\n>> %2d. |%15s |%15s |%30s |%18s |%12s | ", wypozyczenie_bufor->id_wypozyczenia, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    return 0;
}

/**
 * \brief Wypisanie listy wypożyczeń o określonej dacie wypożyczenia
 * @param *head_wypozyczenie wskaźnik na listę do wypisania
 * @param data[] poszukiwana data
 * @return 0 w przypadku powodzenia, -1 gdy lista jest pusta
 */

int wypozeczenie_wypisz_data (struct wypozyczenie *head_wypozyczenie, char data[]) {
    struct wypozyczenie *wypozyczenie_bufor = head_wypozyczenie;
    printf(">> ID. |%15s |%15s |%30s |%18s |%12s |\n", "Imie", "Nazwisko", "Tytuł Filmu", "Data wypożyczenia", "Data Zwrotu");
    int i;
    for(i = 1;wypozyczenie_bufor != NULL;) {
        int z=0, n=0;
        do {
            if (data[n] != wypozyczenie_bufor->data_wypozyczenia[n]) z++;
            n++;
        } while(strlen(data) > n);
        if(z==0) {
            printf("\n>> %2d. |%15s |%15s |%30s |%18s |%12s | ", wypozyczenie_bufor->id_wypozyczenia, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
            i++;
        }
        wypozyczenie_bufor = wypozyczenie_bufor->nastepny;

    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono żadnego wypożyczenia!");
        return -1;
    }
    return 0;
}

/**
 * \brief Wypisanie listy wypożyczeń o określonej dacie zwrotu
 * @param *head_wypozyczenie wskaźnik na listę do wypisania
 * @param data[] poszukiwana data
 * @return 0 w przypadku powodzenia, -1 gdy lista jest pusta
 */

int wypozeczenie_wypisz_data_zwrot (struct wypozyczenie *head_wypozyczenie, char data[]) {
    struct wypozyczenie *wypozyczenie_bufor = head_wypozyczenie;
    printf(">> ID. |%15s |%15s |%30s |%18s |%12s |\n", "Imie", "Nazwisko", "Tytuł Filmu", "Data wypożyczenia", "Data Zwrotu");
    int i;
    for(i = 1;wypozyczenie_bufor != NULL;) {
        int z=0, n=0;
        do {
            if (data[n] != wypozyczenie_bufor->data_zwrotu[n]) z++;
            n++;
        } while(strlen(data) > n);
        if(z==0) {
            printf("\n>> %2d. |%15s |%15s |%30s |%18s |%12s | ", wypozyczenie_bufor->id_wypozyczenia, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
            i++;
        }
        wypozyczenie_bufor = wypozyczenie_bufor->nastepny;

    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono żadnego wypożyczenia!");
        return -1;
    }
    return 0;
}

/**
 * \brief Wypisanie listy wypożyczeń o określonym ID klienta
 * @param *head_wypozyczenie wskaźnik na listę do wypisania
 * @param id poszukiwany identyfikator
 * @return 0 w przypadku powodzenia, -1 gdy lista jest pusta
 */

int wypozeczenie_wypisz_klient(struct wypozyczenie *head_wypozyczenie, unsigned int id) {
    struct wypozyczenie *wypozyczenie_bufor = head_wypozyczenie;
    printf(">> ID. |%15s |%15s |%30s |%18s |%12s |\n", "Imie", "Nazwisko", "Tytuł Filmu", "Data wypożyczenia", "Data Zwrotu");
    int i;
        for(i = 1;wypozyczenie_bufor != NULL;) {
            if(id == wypozyczenie_bufor->znacznik_klienta->id_klienta) {
            printf("\n>> %2d. |%15s |%15s |%30s |%18s |%12s | ", wypozyczenie_bufor->id_wypozyczenia, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
            i++;
            }
            wypozyczenie_bufor = wypozyczenie_bufor->nastepny;
        }
        if(i == 1){
            system("cls");
            puts(">> Nie znaleziono żadnego wypożyczenia!");
            return -1;
        }
    return 0;
}

/**
 * \brief Wypisanie listy wypożyczeń o określonym ID filmu
 * @param *head_wypozyczenie wskaźnik na listę do wypisania
 * @param id poszukiwany identyfikator
 * @return 0 w przypadku powodzenia, -1 gdy lista jest pusta
 */

int wypozeczenie_wypisz_film(struct wypozyczenie *head_wypozyczenie, unsigned int id) {
    struct wypozyczenie *wypozyczenie_bufor = head_wypozyczenie;
    printf(">> ID. |%15s |%15s |%30s |%18s |%12s |\n", "Imie", "Nazwisko", "Tytuł Filmu", "Data wypożyczenia", "Data Zwrotu");
    int i;
    for(i = 1;wypozyczenie_bufor != NULL;) {
        if(id == wypozyczenie_bufor->znacznik_filmu->id_filmu) {
            printf("\n>> %2d. |%15s |%15s |%30s |%18s |%12s | ", wypozyczenie_bufor->id_wypozyczenia, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
            i++;
        }
        wypozyczenie_bufor = wypozyczenie_bufor->nastepny;
    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono żadnego wypożyczenia!");
        return -1;
    }
    return 0;
}

/**
 * \brief Wypisanie listy zaległych wypożyczeń
 * @param **head_wypozyczenie wskaźnik na listę do wypisania
 * @param kara wysokość kary naliczana za dzień zwłoki
 * @return 0 w przypadku powodzenia, -1 gdy lista jest pusta
 */

int wypozyczenie_wypisz_zalegle(struct wypozyczenie **head_wypozyczenie, const double kara){
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    time_t aktualna_data = time(NULL);
    system("cls");
    printf(">> Zaległe wypożyczenia:\n");
    printf(">> ID. |%15s |%15s |%30s |%18s |%12s |\n", "Imie", "Nazwisko", "Tytuł Filmu", "Data wypożyczenia", "Data Zwrotu");
    int i;
    for(i=1; wypozyczenie_bufor != NULL;){
        if(aktualna_data > wypozyczenie_bufor->data_zwrotu_sekundy){
            printf("\n>> %2d. |%15s |%15s |%30s |%18s |%12s | ", wypozyczenie_bufor->id_wypozyczenia, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
            int dni_opoznienia = ((aktualna_data - wypozyczenie_bufor->data_zwrotu_sekundy) / 86400);
            printf("\n>> Dni opóźnienia: %d, Naliczona kara: %.2fzł\n", (int)dni_opoznienia, dni_opoznienia*kara);
            i++;
        }
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    if(i == 1){
        system("cls");
        printf(">> Brak zaległych wypożyczeń!");
        return -1;
    }
    return 0;
}

/**
 * \brief Sortowanie listy wypożyczeń po dacie wypożyczenia
 *
 * Funkcja działa w oparciu o metodę usuwania. To znaczy, że po znalezieniu odpowiedniego elementu
 * dodaje go do tymczasowej listy oraz usuwa go z listy głównej, aby nie był już sprawdzany podczas kolejnych
 * iteracji.
 * @param **head_wypozyczenie wskaźnik na listę do posortowania
 * @param mode 0 - rosnąco, 1 - malejąco
 */

void wypozyczenie_data_wypozyczenia(struct wypozyczenie **head_wypozyczenie, int mode) {
    struct wypozyczenie *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_wypozyczenie)->nastepny != NULL) {
        bufor = *head_wypozyczenie;
        temp = *head_wypozyczenie;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            if (temp->data_wypozyczenia_sekundy < bufor->nastepny->data_wypozyczenia_sekundy && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (temp->data_wypozyczenia_sekundy > bufor->nastepny->data_wypozyczenia_sekundy && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }
        push_wypozyczenie(&main, &temp);
        wypozyczenie_usun(head_wypozyczenie, prev);
    }
    push_wypozyczenie(&main, head_wypozyczenie);
    wypozyczenie_usun(head_wypozyczenie, NULL);
    *head_wypozyczenie = main;
}

/**
 * \brief Sortowanie listy wypożyczeń po dacie zwrotu
 *
 * Funkcja działa w oparciu o metodę usuwania. To znaczy, że po znalezieniu odpowiedniego elementu
 * dodaje go do tymczasowej listy oraz usuwa go z listy głównej, aby nie był już sprawdzany podczas kolejnych
 * iteracji.
 * @param **head_wypozyczenie wskaźnik na listę do posortowania
 * @param mode 0 - rosnąco, 1 - malejąco
 */

void wypozyczenie_data_zwrotu(struct wypozyczenie **head_wypozyczenie, int mode) {
    struct wypozyczenie *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_wypozyczenie)->nastepny != NULL) {
        bufor = *head_wypozyczenie;
        temp = *head_wypozyczenie;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            if (temp->data_zwrotu_sekundy < bufor->nastepny->data_zwrotu_sekundy && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (temp->data_zwrotu_sekundy > bufor->nastepny->data_zwrotu_sekundy && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }
        push_wypozyczenie(&main, &temp);
        wypozyczenie_usun(head_wypozyczenie, prev);
    }
    push_wypozyczenie(&main, head_wypozyczenie);
    wypozyczenie_usun(head_wypozyczenie, NULL);
    *head_wypozyczenie = main;
}

/**
 * \brief Sortowanie listy wypożyczeń po imieniu klienta
 *
 * Funkcja działa w oparciu o metodę usuwania. To znaczy, że po znalezieniu odpowiedniego elementu
 * dodaje go do tymczasowej listy oraz usuwa go z listy głównej, aby nie był już sprawdzany podczas kolejnych
 * iteracji.
 * @param **head_wypozyczenie wskaźnik na listę do posortowania
 * @param mode 0 - rosnąco, 1 - malejąco
 */

void wypozyczenie_imie(struct wypozyczenie **head_wypozyczenie, int mode) {
    struct wypozyczenie *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_wypozyczenie)->nastepny != NULL) {
        bufor = *head_wypozyczenie;
        temp = *head_wypozyczenie;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            strlwr(temp->znacznik_klienta->imie);
            strlwr(bufor->nastepny->znacznik_klienta->imie);
            if (strcmp(temp->znacznik_klienta->imie, bufor->nastepny->znacznik_klienta->imie)<0 && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (strcmp(temp->znacznik_klienta->imie, bufor->nastepny->znacznik_klienta->imie)>0 && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_wypozyczenie(&main, &temp);
        wypozyczenie_usun(head_wypozyczenie, prev);
    }
    push_wypozyczenie(&main, head_wypozyczenie);
    wypozyczenie_usun(head_wypozyczenie, NULL);
    *head_wypozyczenie = main;
}

/**
 * \brief Sortowanie listy wypożyczeń po nazwisku klienta
 *
 * Funkcja działa w oparciu o metodę usuwania. To znaczy, że po znalezieniu odpowiedniego elementu
 * dodaje go do tymczasowej listy oraz usuwa go z listy głównej, aby nie był już sprawdzany podczas kolejnych
 * iteracji.
 * @param **head_wypozyczenie wskaźnik na listę do posortowania
 * @param mode 0 - rosnąco, 1 - malejąco
 */

void wypozyczenie_nazwisko(struct wypozyczenie **head_wypozyczenie, int mode) {
    struct wypozyczenie *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_wypozyczenie)->nastepny != NULL) {
        bufor = *head_wypozyczenie;
        temp = *head_wypozyczenie;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            strlwr(temp->znacznik_klienta->nazwisko);
            strlwr(bufor->nastepny->znacznik_klienta->nazwisko);
            if (strcmp(temp->znacznik_klienta->nazwisko, bufor->nastepny->znacznik_klienta->nazwisko)<0 && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (strcmp(temp->znacznik_klienta->nazwisko, bufor->nastepny->znacznik_klienta->nazwisko)>0 && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_wypozyczenie(&main, &temp);
        wypozyczenie_usun(head_wypozyczenie, prev);
    }
    push_wypozyczenie(&main, head_wypozyczenie);
    wypozyczenie_usun(head_wypozyczenie, NULL);
    *head_wypozyczenie = main;
}

/**
 * \brief Sortowanie listy wypożyczeń po tytule filmu
 *
 * Funkcja działa w oparciu o metodę usuwania. To znaczy, że po znalezieniu odpowiedniego elementu
 * dodaje go do tymczasowej listy oraz usuwa go z listy głównej, aby nie był już sprawdzany podczas kolejnych
 * iteracji.
 * @param **head_wypozyczenie wskaźnik na listę do posortowania
 * @param mode 0 - rosnąco, 1 - malejąco
 */

void wypozyczenie_tytul(struct wypozyczenie **head_wypozyczenie, int mode) {
    struct wypozyczenie *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_wypozyczenie)->nastepny != NULL) {
        bufor = *head_wypozyczenie;
        temp = *head_wypozyczenie;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            strlwr(temp->znacznik_filmu->tytul);
            strlwr(bufor->nastepny->znacznik_filmu->tytul);
            if (strcmp(temp->znacznik_filmu->tytul, bufor->nastepny->znacznik_filmu->tytul)<0 && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (strcmp(temp->znacznik_filmu->tytul, bufor->nastepny->znacznik_filmu->tytul)>0 && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_wypozyczenie(&main, &temp);
        wypozyczenie_usun(head_wypozyczenie, prev);
    }
    push_wypozyczenie(&main, head_wypozyczenie);
    wypozyczenie_usun(head_wypozyczenie, NULL);
    *head_wypozyczenie = main;
}

/**
 * \brief Zliczanie ilości zaległych wypożyczeń
 * @param *head_wypozyczenie wskaźnik na listę wypożyczeń
 * @return liczba zaległych wypożyczeń
 */

int licz_zalegle(struct wypozyczenie *head_wypozyczenie){
    time_t aktualna_data = time(NULL);
    int count = 0;
    while(head_wypozyczenie != NULL){
        if(aktualna_data > head_wypozyczenie->data_zwrotu_sekundy){
            count++;
        }
        head_wypozyczenie = head_wypozyczenie -> nastepny;
    }
    return count;
}

/**
 * \brief Zapis listy wypożyczeń do pliku
 * @param *head_wypozyczenie wskaźnik na listę do zapisu
 * @return true - zapis udany, false - zapis nieudany
 */

bool wypozyczenie_zapisz_do_pliku(struct wypozyczenie *head_wypozyczenie){
    if(!dodaj_folder("data")) return false;
    if(!dodaj_folder("data/databases")) return false;
    FILE *file = fopen("data/databases/wypozyczenie.db", "w");
    if (file == NULL)  {
        return false;
    } else {
        while (head_wypozyczenie != NULL){
            fprintf(file, "%d %d %d %s %ld %s %ld\n",head_wypozyczenie->id_wypozyczenia, head_wypozyczenie->numer_klienta, head_wypozyczenie->numer_filmu, head_wypozyczenie->data_wypozyczenia, head_wypozyczenie->data_wypozyczenia_sekundy, head_wypozyczenie->data_zwrotu, head_wypozyczenie->data_zwrotu_sekundy);
            head_wypozyczenie = head_wypozyczenie->nastepny;
        }
    }
    fclose(file);
    return true;
}

/**
 * \brief Odczyt listy wypożyczeń z pliku
 * @param *head_wypozyczenie wskaźnik na listę do zapisu
 * @return true - odczyt udany, false - odczyt nieudany
 */

bool wypozyczenie_wczytaj_z_pliku(struct wypozyczenie **head_wypozyczenie){
    if(!dodaj_folder("data")) return false;
    if(!dodaj_folder("data/databases")) return false;
    FILE *file = fopen("data/databases/wypozyczenie.db", "r");
    if (file == NULL) {
        return false;
    }
    else{
        unsigned int numer_klienta, numer_filmu, numer_wypozyczenia;
        char data_wypozyczenia[11], data_zwrotu[11];
        time_t data_wypozyczenia_sekundy, data_zwrotu_sekundy;
        while(fscanf(file, "%d %d %d %s %ld %s %ld", &numer_wypozyczenia, &numer_klienta, &numer_filmu, data_wypozyczenia, &data_wypozyczenia_sekundy, data_zwrotu, &data_zwrotu_sekundy) != EOF){
            if(*head_wypozyczenie == NULL){
                *head_wypozyczenie = (struct wypozyczenie *)malloc(sizeof(struct wypozyczenie));
                (*head_wypozyczenie) -> id_wypozyczenia = numer_wypozyczenia;
                (*head_wypozyczenie) -> numer_klienta = numer_klienta;
                (*head_wypozyczenie) -> numer_filmu = numer_filmu;
                (*head_wypozyczenie) -> data_wypozyczenia_sekundy = data_wypozyczenia_sekundy;
                (*head_wypozyczenie) -> data_zwrotu_sekundy = data_zwrotu_sekundy;
                (*head_wypozyczenie) -> nastepny = NULL;
                strcpy((*head_wypozyczenie) -> data_wypozyczenia, data_wypozyczenia);
                strcpy((*head_wypozyczenie) -> data_zwrotu, data_zwrotu);
            }
            else {
                struct wypozyczenie *wypozyczenie_nowy = *head_wypozyczenie;
                while (wypozyczenie_nowy->nastepny != NULL) {
                    wypozyczenie_nowy = wypozyczenie_nowy->nastepny;
                }
                wypozyczenie_nowy-> nastepny = (struct wypozyczenie *)malloc(sizeof(struct wypozyczenie));
                wypozyczenie_nowy-> nastepny -> id_wypozyczenia = numer_wypozyczenia;
                wypozyczenie_nowy-> nastepny -> numer_klienta = numer_klienta;
                wypozyczenie_nowy-> nastepny -> numer_filmu = numer_filmu;
                wypozyczenie_nowy-> nastepny -> data_wypozyczenia_sekundy = data_wypozyczenia_sekundy;
                wypozyczenie_nowy-> nastepny -> data_zwrotu_sekundy = data_zwrotu_sekundy;
                wypozyczenie_nowy-> nastepny->nastepny = NULL;
                strcpy(wypozyczenie_nowy-> nastepny -> data_wypozyczenia, data_wypozyczenia);
                strcpy(wypozyczenie_nowy-> nastepny -> data_zwrotu, data_zwrotu);
            }
        }
        fclose(file);
    }
    return true;
}

/**
 * \brief Poprawa znaczników w wypożyczeniach
 *
 * Funkcja niezbędna do poprawnego funkcjonowania programu, używana chociażby w przypadku sortowania, aby
 * po posortowaniu nie okazało się, że dane wypożyczenie zmieniło klienta lub film.
 * @param *head_wypozyczenie wskaźnik na listę wypożyczeń
 * @param *head_film wskaźnik na listę filmów
 * @param *head_klient wskaźnik na listę klientów
 */

void wypozyczenie_przebuduj_znaczniki(struct wypozyczenie *head_wypozyczenie, struct film *head_film, struct klient *head_klient){
    while(head_wypozyczenie != NULL){
        head_wypozyczenie->znacznik_klienta = klient_szukaj_po_numerze(&head_klient, head_wypozyczenie->numer_klienta);
        head_wypozyczenie->znacznik_filmu = film_szukaj_po_numerze(&head_film, head_wypozyczenie->numer_filmu);
        head_wypozyczenie = head_wypozyczenie -> nastepny;
    }
}

/**
 * \brief Wczytanie wartości kary z pliku
 * @return wartość kary (pobrana z pliku, lub 2.0 w przypadku braku pliku)
 */

double wczytaj_kare_z_pliku(){
        double bufor;
        if(!dodaj_folder("data")){
            system("cls");
            printf(">> Plik z wartością kary nie został wczytany, ustawiona została domyślna wartość (2zł)");
            czekaj_na_input_ESCAPE();
            system("cls");
            return 2.0;
        }
        FILE *file = fopen("data/kara.db", "r");
        if (file == NULL)  {
            system("cls");
            printf(">> Plik z wartością kary nie został wczytany, ustawiona została domyślna wartość (2zł)");
            czekaj_na_input_ESCAPE();
            system("cls");
            return 2.0;
        }
        else {
            fscanf(file, "%lf", &bufor);
            fclose(file);
            return bufor;
        }
}

/**
 * \brief Zapis wartości kary do pliku
 * @param kara wartość kary naliczanej za każdy dzień zwłoki
 * @return 0 w przypadku powodzenia, -1 w przypadku niepowodzenia
 */

int zapisz_kare_do_pliku(double kara){
    if(!dodaj_folder("data")) return -1;
    FILE *file = fopen("data/kara.db", "w");
    if(file != NULL){
        fprintf(file, "%f", kara);
        fclose(file);
        return 0;
    }
    else{
        return -1;
    }
}

/**
 * \brief Powiadamianie o ilości zaległych wypożyczeń
 * @param *head_wypozyyczenie wskaźnik na listę wypożyczeń
 */

void powiadom_zalegle(struct wypozyczenie *head_wypozyczenie){
    if(licz_zalegle(head_wypozyczenie) > 0){
        system("cls");
        printf("Masz %d zaległych wypożyczeń! Sprawdź menu wypożyczeń!", licz_zalegle(head_wypozyczenie));
        czekaj_na_input_ESCAPE();
    }
}
