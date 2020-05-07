#ifndef PP2_FILM_H
#define PP2_FILM_H
#include "film.c"

void film_dodaj(struct film **head_film, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
void film_usun(struct film **head_film, struct film *film_usuwany_poprzedni);
void film_edytuj(struct film **film_edytowany, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
struct film *film_szukaj_po_numerze(struct film **head_film, int numer_filmu);
struct film *film_szukaj_po_kolejnosci(struct film **head_film, int kolejnosc);
struct film *film_szukaj_po_kolejnosci_dostepne(struct film **head_film, int kolejnosc);
struct film *film_szukaj_po_kolejnosci_dostepne_poprzedni(struct film **head_film, int kolejnosc);
int film_licz(struct film *head_film);
int film_licz_dostepne(struct film *head_film);
int film_wypisz(struct film *head_film);
int film_wypisz_dostepne(struct film *head_film);
int film_wypisz_sztuki_dostepne(struct film *head_film, int sztuki_dostepne);
int film_wypisz_sztuki_wypozyczone(struct film *head_film, int sztuki_wypozyczone);
int film_wypisz_rok_produkcji(struct film *head_film, int rok_produkcji);
bool film_czy_istnieje(struct film *head_film, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
bool film_czy_jedna_sztuka_poprzedni(struct film **head_film, struct film *film_sprawdzany_poprzedni);
void film_zamien_spacje_na_tylde(struct film *head_film);
void film_zamien_tylde_na_spacje(struct film *head_film);
bool film_zapisz_do_pliku(struct film *head_film);

#endif
