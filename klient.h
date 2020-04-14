#ifndef PP2_KLIENT_H
#define PP2_KLIENT_H
#include "klient.c"

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

#endif //PP2_KLIENT_H
