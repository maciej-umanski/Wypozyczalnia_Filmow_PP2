#ifndef PP2_FILM_H
#define PP2_FILM_H
#include "film.c"

int film_dodaj(struct film **head_film, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
void film_usun(struct film **head_film, struct film *film_usuwany_poprzedni);
void film_edytuj(struct film **film_edytowany, int sztuki_dostepne, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
struct film *film_szukaj_po_kolejnosci(struct film **head_film, int kolejnosc);
struct film *film_szukaj_po_kolejnosci_dostepne(struct film **head_film, int kolejnosc);
struct film *film_szukaj_po_kolejnosci_dostepne_poprzedni(struct film **head_film, int kolejnosc);
int film_licz(struct film *head_film);
int film_licz_dostepne(struct film *head_film);
int film_wypisz(struct film *head_film);
int film_wypisz_dostepne(struct film *head_film);
bool film_czy_istnieje(struct film *head_film, int rok_produkcji, char tytul[], char rezyser[], char gatunek[]);
bool film_czy_jedna_sztuka_poprzedni(struct film **head_film, struct film *film_sprawdzany_poprzedni);

#endif
