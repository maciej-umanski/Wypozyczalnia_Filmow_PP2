#ifndef PP2_KLIENT_H
#define PP2_KLIENT_H
#include "klient.c"

void klient_dodaj(struct klient **head_klient, unsigned long long numer_klienta, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]);
void klient_usun(struct klient **head_klient, struct klient *klient_przed_usuwanym);
void klient_edytuj(struct klient **klient_edytowany, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]);
struct klient *klient_szukaj_po_numerze(struct klient **head_klient, unsigned long long numer_klienta);
struct klient *klient_szukaj_po_kolejnosci(struct klient **head_klient, int kolejnosc);
struct klient *klient_szukaj_po_kolejnosci_dostepne_poprzedni(struct klient **head_klient, int kolejnosc);
int klient_licz_dostepne(struct klient *head_klient);
int klient_licz(struct klient *head_klient);
int klient_wypisz(struct klient **head_klient);
int klient_wypisz_dostepni(struct klient **head_klient);
bool klient_czy_pesel_istnieje(struct klient *head_klient, unsigned long long numer_klienta);
bool klient_zapisz_do_pliku(struct klient *head_klient);
void klient_zamien_spacje_na_tylde(struct klient *head_klient);
void klient_zamien_tylde_na_spacje(struct klient *head_klient);
bool klient_wczytaj_z_pliku(struct klient **head_klient);

#endif //PP2_KLIENT_H
