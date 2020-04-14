#ifndef PP2_WYPOZYCZENIE_H
#define PP2_WYPOZYCZENIE_H
#include "wypozyczenie.c"

void wypozyczenie_dodaj(struct wypozyczenie **head_wypozyczenie, struct film *film_wypozyczany, struct klient *klient_wypozyczajacy,  int ile_tygodni);
void wypozyczenie_usun(struct wypozyczenie **head_wypozyczenie, struct wypozyczenie *wypozyczenie_przed_usuwanym);
void wypozyczenie_edytuj(struct wypozyczenie **wypozyczenie_edytowane, int czas_do_zwrotu);
struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci(struct wypozyczenie **head_wypozyczenie, int kolejnosc);
struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci_poprzedni(struct wypozyczenie **head_wypozyczenie, int kolejnosc);
int wypozyczenie_licz(struct wypozyczenie **head_wypozyczenie);
int wypozyczenie_wypisz(struct wypozyczenie **head_wypozyczenie);


#endif //PP2_WYPOZYCZENIE_H
