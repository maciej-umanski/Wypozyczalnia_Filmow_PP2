#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include "wypozyczenie.h"

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
    nowe_wypozyczenie->data_zwrotu_sekundy = data_zwrotu_sekundy;
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
    system("cls");
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    if(wypozyczenie_bufor == NULL){
        puts(">> Lista wypo¿yczeñ jest pusta!");
        return -1;
    }
    printf(">> Aktywne wypo¿yczenia:\n");
    printf(">> Nr. |%10s |%10s |%30s |%18s |%12s |\n", "Imie", "Nazwisko", "Tytu³ Filmu", "Data wypo¿yczenia", "Data Zwrotu");
    for(int i=1; wypozyczenie_bufor != NULL; i++){
        printf("\n>> %2d. |%10s |%10s |%30s |%18s |%12s | ", i, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    return 0;
}

int wypozyczenie_wypisz_zalegle(struct wypozyczenie **head_wypozyczenie, const double kara){
    struct wypozyczenie *wypozyczenie_bufor = *head_wypozyczenie;
    time_t aktualna_data = time(NULL);
    system("cls");
    printf(">> Zaleg³e wypo¿yczenia:\n");
    printf(">> Nr. |%10s |%10s |%30s |%18s |%12s |\n", "Imie", "Nazwisko", "Tytu³ Filmu", "Data wypo¿yczenia", "Data Zwrotu");
    int i;
    for(i=1; wypozyczenie_bufor != NULL;){
        if(aktualna_data > wypozyczenie_bufor->data_zwrotu_sekundy){
            printf("\n>> %2d. |%10s |%10s |%30s |%18s |%12s | ", i, wypozyczenie_bufor->znacznik_klienta->imie, wypozyczenie_bufor->znacznik_klienta->nazwisko , wypozyczenie_bufor->znacznik_filmu->tytul, wypozyczenie_bufor->data_wypozyczenia, wypozyczenie_bufor->data_zwrotu);
            int dni_opoznienia = ((aktualna_data - wypozyczenie_bufor->data_zwrotu_sekundy) / 86400);
            printf("\n>> Dni opóŸnienia: %d, Naliczona kara: %.2fz³\n", (int)dni_opoznienia, dni_opoznienia*kara);
            i++;
        }
        wypozyczenie_bufor = wypozyczenie_bufor -> nastepny;
    }
    if(i == 1){
        system("cls");
        printf(">> Brak zaleg³ych wypo¿yczeñ!");
        return -1;
    }
    return 0;
}

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

bool wypozyczenie_zapisz_do_pliku(struct wypozyczenie *head_wypozyczenie){
    FILE *file = fopen("wypozyczenie.db", "w");
    if (file == NULL)  {
        return false;
    } else {
        while (head_wypozyczenie != NULL){
            fprintf(file, "%d %llu %d %s %ld %s %ld\n",head_wypozyczenie->numer_wypozyczenia, head_wypozyczenie->numer_klienta, head_wypozyczenie->numer_filmu, head_wypozyczenie->data_wypozyczenia, head_wypozyczenie->data_wypozyczenia_sekundy, head_wypozyczenie->data_zwrotu, head_wypozyczenie->data_zwrotu_sekundy);
            head_wypozyczenie = head_wypozyczenie->nastepny;
        }
    }
    fclose(file);
    return true;
}

bool wypozyczenie_wczytaj_z_pliku(struct wypozyczenie **head_wypozyczenie){
    FILE *file = fopen("wypozyczenie.db", "r");
    if (file == NULL) {
        return false;
    }
    else{
        unsigned long long numer_klienta;
        int numer_wypozyczenia, numer_filmu;
        char data_wypozyczenia[11], data_zwrotu[11];
        time_t data_wypozyczenia_sekundy, data_zwrotu_sekundy;
        while(fscanf(file, "%d %llu %d %s %ld %s %ld", &numer_wypozyczenia, &numer_klienta, &numer_filmu, data_wypozyczenia, &data_wypozyczenia_sekundy, data_zwrotu, &data_zwrotu_sekundy) != EOF){
            if(*head_wypozyczenie == NULL){
                *head_wypozyczenie = (struct wypozyczenie *)malloc(sizeof(struct wypozyczenie));
                (*head_wypozyczenie) -> numer_wypozyczenia = numer_wypozyczenia;
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
                wypozyczenie_nowy-> nastepny -> numer_wypozyczenia = numer_wypozyczenia;
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

void wypozyczenie_przebuduj_znaczniki(struct wypozyczenie *head_wypozyczenie, struct film *head_film, struct klient *head_klient){
    while(head_wypozyczenie != NULL){
        head_wypozyczenie->znacznik_klienta = klient_szukaj_po_numerze(&head_klient, head_wypozyczenie->numer_klienta);
        head_wypozyczenie->znacznik_filmu = film_szukaj_po_numerze(&head_film, head_wypozyczenie->numer_filmu);
        head_wypozyczenie = head_wypozyczenie -> nastepny;
    }
}

double wczytaj_kare_z_pliku(){
    double bufor;
    FILE *file = fopen("kara.db", "r");
    if (file == NULL)  {
        system("cls");
        printf(">> Plik z wartoœci¹ kary nie zosta³ wczytany, ustawiona zosta³a domyœlna wartoœæ (2z³)");
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

int zapisz_kare_do_pliku(double kara){
    FILE *file = fopen("kara.db", "w");
    if(file != NULL){
        fprintf(file, "%f", kara);
        fclose(file);
        return 0;
    }
    else{
        return -1;
    }
}

void powiadom_zalegle(struct wypozyczenie *head_wypozyczenie){
    if(licz_zalegle(head_wypozyczenie) > 0){
        system("cls");
        printf("Masz %d zaleg³ych wypo¿yczeñ! SprawdŸ menu wypo¿yczeñ!", licz_zalegle(head_wypozyczenie));
        czekaj_na_input_ESCAPE();
    }
}