#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "klient.h"

struct klient{
    unsigned long long numer_klienta;
    unsigned long long numer_telefonu;
    char imie[10];
    char nazwisko[10];
    char email[30];
    int ilosc_posiadanych_filmow;
    struct klient *nastepny;
};

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
        puts(">> Baza klientów jest pusta!");
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
        system("cls");
        puts(">> Baza klientów jest pusta!");
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
        system("cls");
        puts(">> Żaden klient nie może zostać usunięty!");
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
