#ifndef PP2_WYPOZYCZENIE_H
#define PP2_WYPOZYCZENIE_H
#include "wypozyczenie.c"

void wypozyczenie_dodaj(struct wypozyczenie **head_wypozyczenie, struct film *film_wypozyczany, struct klient *klient_wypozyczajacy,  int ile_tygodni);
void wypozyczenie_usun(struct wypozyczenie **head_wypozyczenie, struct wypozyczenie *wypozyczenie_przed_usuwanym);
void wypozyczenie_edytuj(struct wypozyczenie **wypozyczenie_edytowane, int czas_do_zwrotu);
struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci(struct wypozyczenie **head_wypozyczenie, unsigned int id);
struct wypozyczenie *wypozyczenie_szukaj_po_kolejnosci_poprzedni(struct wypozyczenie **head_wypozyczenie, unsigned int id);
int wypozyczenie_licz(struct wypozyczenie **head_wypozyczenie);
int wypozyczenie_wypisz(struct wypozyczenie **head_wypozyczenie);
int wypozyczenie_wypisz_zalegle(struct wypozyczenie **head_wypozyczenie, const double kara);
int licz_zalegle(struct wypozyczenie *head_wypozyczenie);
bool wypozyczenie_wczytaj_z_pliku(struct wypozyczenie **head_wypozyczenie);
void wypozyczenie_przebuduj_znaczniki(struct wypozyczenie *head_wypozyczenie, struct film *head_film, struct klient *head_klient);
double wczytaj_kare_z_pliku();
int zapisz_kare_do_pliku(double kara);
void powiadom_zalegle(struct wypozyczenie *head_wypozyczenie);
int wypozeczenie_wypisz_data (struct wypozyczenie *head_wypozyczenie, char data[]);
int wypozeczenie_wypisz_data_zwrot (struct wypozyczenie *head_wypozyczenie, char data[]);
int wypozeczenie_wypisz_klient (struct wypozyczenie *head_wypozyczenie, unsigned int id);
int wypozeczenie_wypisz_film (struct wypozyczenie *head_wypozyczenie, unsigned int id);

#endif //PP2_WYPOZYCZENIE_H
