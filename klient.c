#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "klient.h"
#include <ctype.h>

struct klient{
    unsigned int id_klienta;
    unsigned long long numer_klienta;
    unsigned long long numer_telefonu;
    char imie[15];
    char nazwisko[15];
    char email[30];
    int ilosc_posiadanych_filmow;
    struct klient *nastepny;
};

bool klient_dodaj(struct klient **head_klient, unsigned long long numer_klienta, unsigned long long numer_telefonu, char imie[], char nazwisko[], char email[]){
        if(!dodaj_folder("data")) return false;
        if(!dodaj_folder("data/id")) return false;
        FILE *file = fopen("data/id/last_id_klient.db", "r");

        if (file==NULL && (*head_klient)!=NULL) {
            return false;
        }
        unsigned int id;
        char temp[10];
        fgets(temp, 10, file);
        sscanf(temp,"%d", &id);
        fclose(file);

    struct klient *klient_nowy = (struct klient *) malloc(sizeof(struct klient));

            if((*head_klient)== NULL) {
                if(!file) {
                    klient_nowy->id_klienta = 1;
                    id = 1;
                }
                else {
                    klient_nowy->id_klienta = ++id;
                }
            }
            else {
                klient_nowy->id_klienta = ++id;
            }

    klient_nowy->numer_klienta = numer_klienta;
    klient_nowy->numer_telefonu = numer_telefonu;
    strcpy(klient_nowy->imie, imie);
    strcpy(klient_nowy->nazwisko, nazwisko);
    strcpy(klient_nowy->email, email);
    klient_nowy->ilosc_posiadanych_filmow = 0;
    klient_nowy->nastepny = (*head_klient);
    *head_klient = klient_nowy;

    FILE *file2 = fopen("data/id/last_id_klient.db", "w");
    fprintf(file2, "%d", id);
    fclose(file2);
    return true;
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

void push_klient (struct klient **head_klient, struct klient **bufor){
    struct klient *klient_nowy = (struct klient *) malloc(sizeof(struct klient));
    klient_nowy->id_klienta = (*bufor)->id_klienta;
    klient_nowy->numer_klienta = (*bufor)->numer_klienta;
    klient_nowy->numer_telefonu = (*bufor)->numer_telefonu;
    strcpy(klient_nowy->imie, (*bufor)->imie);
    strcpy(klient_nowy->nazwisko, (*bufor)->nazwisko);
    strcpy(klient_nowy->email, (*bufor)->email);
    klient_nowy->ilosc_posiadanych_filmow = (*bufor)->ilosc_posiadanych_filmow;
    klient_nowy->nastepny = (*head_klient);
    *head_klient = klient_nowy;
}

struct klient *klient_szukaj_po_numerze(struct klient **head_klient, unsigned int id){
    struct klient *klient_bufor = *head_klient;
    while(klient_bufor -> id_klienta != id){
        klient_bufor = klient_bufor -> nastepny;
    }
    return klient_bufor;
}

struct klient *klient_szukaj_po_kolejnosci(struct klient **head_klient, unsigned int id){
    struct klient *klient_bufor = *head_klient;
    int i=0;
    while(klient_bufor != NULL) {
        if(id == klient_bufor->id_klienta) {
            i++;
            break;
        }
        klient_bufor = klient_bufor->nastepny;
    }
    if(i==0) return NULL;
    return klient_bufor;
}

struct klient *klient_szukaj_po_kolejnosci_dostepne_poprzedni(struct klient **head_klient, unsigned int id){
    struct klient *poprzedni = NULL;
    struct klient *klient_bufor = *head_klient;
    int i=0;
    while(klient_bufor != NULL) {
        if(id == klient_bufor->id_klienta) {
            i++;
            break;
        }
        poprzedni = klient_bufor;
        klient_bufor = klient_bufor->nastepny;
    }
    if(i==0) return NULL;
    return poprzedni;
}

int klient_wypisz(struct klient **head_klient){
    struct klient *klient_bufor = *head_klient;
    if(klient_bufor == NULL){
        puts(">> Baza klientów jest pusta!");
        return -1;
    }
    printf(">> ID. | %11s | %15s | %15s | %9s |%30s | Posiadane filmy \n\n", "Pesel", "Imie", "Nazwisko", "Telefon", "E-Mail");
    while(klient_bufor != NULL){
        printf(">> %2d. | %llu | %15s | %15s | %9llu |%30s |%2d\n", klient_bufor->id_klienta, klient_bufor->numer_klienta,
                klient_bufor->imie, klient_bufor->nazwisko, klient_bufor-> numer_telefonu, klient_bufor->email,
                klient_bufor->ilosc_posiadanych_filmow);
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
    printf(">> ID. | %11s | %15s | %15s | %9s |%30s | Posiadane filmy \n\n", "Pesel", "Imie", "Nazwisko", "Telefon", "E-Mail");
    int i;
    for(i=1; klient_bufor != NULL;){
        if(klient_bufor->ilosc_posiadanych_filmow == 0) {
            printf(">> %2d. | %llu | %15s | %15s | %9llu |%30s |%2d\n", klient_bufor->id_klienta, klient_bufor->numer_klienta,
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

int klient_wypisz_numer_telefonu(struct klient **head_klient, unsigned long long numer_telefonu){
    struct klient *klient_bufor = *head_klient;
    printf(">> ID. | %11s | %15s | %15s | %9s |%30s | Posiadane filmy \n\n", "Pesel", "Imie", "Nazwisko", "Telefon", "E-Mail");
    int i;
    for(i = 1;klient_bufor != NULL;){
        if(numer_telefonu == klient_bufor->numer_telefonu){
            printf(">> %2d. | %llu | %15s | %15s | %9llu |%30s |%2d\n", klient_bufor->id_klienta, klient_bufor->numer_klienta,
                    klient_bufor->imie, klient_bufor->nazwisko, klient_bufor-> numer_telefonu, klient_bufor->email,
                    klient_bufor->ilosc_posiadanych_filmow);
            i++;
        }
        klient_bufor = klient_bufor -> nastepny;
    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono żadnego klienta!");
        return -1;
    }
    return 0;
}

int klient_wypisz_ilosc_posiadanych_filmow(struct klient **head_klient, int ilosc_posiadanych_filmow){
    struct klient *klient_bufor = *head_klient;
    printf(">> ID. | %11s | %15s | %15s | %9s |%30s | Posiadane filmy \n\n", "Pesel", "Imie", "Nazwisko", "Telefon", "E-Mail");
    int i;
    for(i = 1;klient_bufor != NULL;){
        if(ilosc_posiadanych_filmow == klient_bufor->ilosc_posiadanych_filmow){
            printf(">> %2d. | %llu | %15s | %15s | %9llu |%30s |%2d\n", klient_bufor->id_klienta, klient_bufor->numer_klienta, klient_bufor->imie, klient_bufor->nazwisko, klient_bufor-> numer_telefonu, klient_bufor->email, klient_bufor->ilosc_posiadanych_filmow);
            i++;
        }
        klient_bufor = klient_bufor -> nastepny;
    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono żadnego klienta!");
        return -1;
    }
    return 0;
}

int klient_wypisz_imie(struct klient *head_klient, char imie[]){
    struct klient *klient_bufor = head_klient;
    imie = strlwr(imie);
    printf(">> ID. | %11s | %15s | %15s | %9s |%30s | Posiadane filmy \n\n", "Pesel", "Imie", "Nazwisko", "Telefon", "E-Mail");
    int i;
    for(i = 1;klient_bufor != NULL;) {
        strlwr(klient_bufor->imie);
        int z=0, n=0;
        do {
            if (imie[n] != klient_bufor->imie[n]) z++;
            n++;
        } while(strlen(imie) > n);
        if(z==0) {
            printf(">> %2d. | %llu | %15s | %15s | %9llu |%30s |%2d\n", klient_bufor->id_klienta, klient_bufor->numer_klienta,
                   klient_bufor->imie, klient_bufor->nazwisko, klient_bufor->numer_telefonu,
                   klient_bufor->email,
                   klient_bufor->ilosc_posiadanych_filmow);
            i++;
        }
        klient_bufor = klient_bufor->nastepny;

    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono żadnego klienta!");
        return -1;
    }
    return 0;
}

int klient_wypisz_nazwisko(struct klient *head_klient, char nazwisko[]){
    struct klient *klient_bufor = head_klient;
    nazwisko = strlwr(nazwisko);
    printf(">> ID. | %11s | %15s | %15s | %9s |%30s | Posiadane filmy \n\n", "Pesel", "Imie", "Nazwisko", "Telefon", "E-Mail");
    int i;
    for(i = 1;klient_bufor != NULL;) {
        strlwr(klient_bufor->nazwisko);
        int z=0, n=0;
        do {
            if (nazwisko[n] != klient_bufor->nazwisko[n]) z++;
            n++;
        } while(strlen(nazwisko) > n);
        if(z==0) {
            printf(">> %2d. | %llu | %15s | %15s | %9llu |%30s |%2d\n", klient_bufor->id_klienta, klient_bufor->numer_klienta,
                   klient_bufor->imie, klient_bufor->nazwisko, klient_bufor->numer_telefonu,
                   klient_bufor->email,
                   klient_bufor->ilosc_posiadanych_filmow);
            i++;
        }
        klient_bufor = klient_bufor->nastepny;

    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono żadnego klienta!");
        return -1;
    }
    return 0;
}
 
int klient_wypisz_mail(struct klient *head_klient, char mail[]){
    struct klient *klient_bufor = head_klient;
    mail = strlwr(mail);
    printf(">> ID. | %11s | %15s | %15s | %9s |%30s | Posiadane filmy \n\n", "Pesel", "Imie", "Nazwisko", "Telefon", "E-Mail");
    int i;
    for(i = 1;klient_bufor != NULL;) {
        strlwr(klient_bufor->email);
        int z=0, n=0;
        do {
            if (mail[n] != klient_bufor->email[n]) z++;
            n++;
        } while(strlen(mail) > n);
        if(z==0) {
            printf(">> %2d. | %llu | %15s | %15s | %9llu |%30s |%2d\n", klient_bufor->id_klienta, klient_bufor->numer_klienta,
                   klient_bufor->imie, klient_bufor->nazwisko, klient_bufor->numer_telefonu,
                   klient_bufor->email,
                   klient_bufor->ilosc_posiadanych_filmow);
            i++;
        }
        klient_bufor = klient_bufor->nastepny;

    }
    if(i == 1){
        system("cls");
        puts(">> Nie znaleziono żadnego klienta!");
        return -1;
    }
    return 0;
}

void klient_telefon(struct klient **head_klient, int mode) {
    struct klient *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_klient)->nastepny != NULL) {
        bufor = *head_klient;
        temp = *head_klient;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            if (temp->numer_telefonu < bufor->nastepny->numer_telefonu && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (temp->numer_telefonu > bufor->nastepny->numer_telefonu && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }
        push_klient(&main, &temp);
        klient_usun(head_klient, prev);
    }
    push_klient(&main, head_klient);
    klient_usun(head_klient, NULL);
    *head_klient = main;
}

void klient_pesel(struct klient **head_klient, int mode) {
    struct klient *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_klient)->nastepny != NULL) {
        bufor = *head_klient;
        temp = *head_klient;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            if (temp->numer_klienta < bufor->nastepny->numer_klienta && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (temp->numer_klienta > bufor->nastepny->numer_klienta && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_klient(&main, &temp);
        klient_usun(head_klient, prev);
    }
    push_klient(&main, head_klient);
    klient_usun(head_klient, NULL);
    *head_klient = main;
}

void klient_filmy(struct klient **head_klient, int mode) {
    struct klient *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_klient)->nastepny != NULL) {
        bufor = *head_klient;
        temp = *head_klient;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            if (temp->ilosc_posiadanych_filmow < bufor->nastepny->ilosc_posiadanych_filmow && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (temp->ilosc_posiadanych_filmow > bufor->nastepny->ilosc_posiadanych_filmow && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_klient(&main, &temp);
        klient_usun(head_klient, prev);
    }
    push_klient(&main, head_klient);
    klient_usun(head_klient, NULL);
    *head_klient = main;
}

void klient_imie(struct klient **head_klient, int mode) {
    struct klient *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_klient)->nastepny != NULL) {
        bufor = *head_klient;
        temp = *head_klient;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            strlwr(temp->imie);
            strlwr(bufor->nastepny->imie);
            if (strcmp(temp->imie, bufor->nastepny->imie)<0 && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (strcmp(temp->imie, bufor->nastepny->imie)>0 && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_klient(&main, &temp);
        klient_usun(head_klient, prev);
    }
    push_klient(&main, head_klient);
    klient_usun(head_klient, NULL);
    *head_klient = main;
}

void klient_nazwisko(struct klient **head_klient, int mode) {
    struct klient *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_klient)->nastepny != NULL) {
        bufor = *head_klient;
        temp = *head_klient;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            strlwr(temp->nazwisko);
            strlwr(bufor->nastepny->nazwisko);
            if (strcmp(temp->nazwisko, bufor->nastepny->nazwisko)<0 && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (strcmp(temp->nazwisko, bufor->nastepny->nazwisko)>0 && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_klient(&main, &temp);
        klient_usun(head_klient, prev);
    }
    push_klient(&main, head_klient);
    klient_usun(head_klient, NULL);
    *head_klient = main;
}

void klient_mail(struct klient **head_klient, int mode) {
    struct klient *main = NULL, *bufor = NULL, *temp = NULL, *prev = NULL;
    while ((*head_klient)->nastepny != NULL) {
        bufor = *head_klient;
        temp = *head_klient;
        prev = NULL;
        while (bufor->nastepny != NULL) {
            strlwr(temp->email);
            strlwr(bufor->nastepny->email);
            if (strcmp(temp->email, bufor->nastepny->email)<0 && mode == 0) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            if (strcmp(temp->email, bufor->nastepny->email)>0 && mode == 1) {
                temp = bufor->nastepny;
                prev = bufor;
            }
            bufor = bufor->nastepny;
        }

        push_klient(&main, &temp);
        klient_usun(head_klient, prev);
    }
    push_klient(&main, head_klient);
    klient_usun(head_klient, NULL);
    *head_klient = main;
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

bool klient_zapisz_do_pliku(struct klient *head_klient){
    if(!dodaj_folder("data")) return false;
    if(!dodaj_folder("data/databases")) return false;
    FILE *file = fopen("data/databases/klient.db", "w");
    if (file == NULL)  {
        return false;
    } else {
        while (head_klient != NULL){
            fprintf(file, "%d %llu %llu %s %s %s %d\n",head_klient->id_klienta, head_klient->numer_klienta, head_klient->numer_telefonu, head_klient->imie, head_klient->nazwisko, head_klient->email, head_klient->ilosc_posiadanych_filmow);
            head_klient = head_klient->nastepny;
        }
    }
    fclose(file);
    return true;
}

void klient_zamien_spacje_na_tylde(struct klient *head_klient){
    while(head_klient != NULL){
        for(int i = 0; i < strlen(head_klient->imie); i++){
            if(head_klient->imie[i] == 32){
                head_klient->imie[i] = 96;
            }
        }
        for(int i = 0; i < strlen(head_klient->nazwisko); i++){
            if(head_klient->nazwisko[i] == 32){
                head_klient->nazwisko[i] = 96;
            }
        }
        for(int i = 0; i < strlen(head_klient->email); i++){
            if(head_klient->email[i] == 32){
                head_klient->email[i] = 96;
            }
        }
        head_klient = head_klient->nastepny;
    }
}

void klient_zamien_tylde_na_spacje(struct klient *head_klient){
    while(head_klient != NULL){
        for(int i = 0; i < strlen(head_klient->imie); i++){
            if(head_klient->imie[i] == 96){
                head_klient->imie[i] = 32;
            }
        }
        for(int i = 0; i < strlen(head_klient->nazwisko); i++){
            if(head_klient->nazwisko[i] == 96){
                head_klient->nazwisko[i] = 32;
            }
        }
        for(int i = 0; i < strlen(head_klient->email); i++){
            if(head_klient->email[i] == 96){
                head_klient->email[i] = 32;
            }
        }
        head_klient = head_klient->nastepny;
    }
}

bool klient_wczytaj_z_pliku(struct klient **head_klient){
    if(!dodaj_folder("data")) return false;
    if(!dodaj_folder("data/databases")) return false;
    FILE *file = fopen("data/databases/klient.db", "r");
    if (file == NULL) {
        return false;
    }
    else{
        unsigned int id;
        unsigned long long numer_klienta, numer_telefonu;
        int ilosc_posiadanych_filmow;
        char imie[15], nazwisko[15], email[30];
        while(fscanf(file, "%d %llu %llu %s %s %s %d", &id, &numer_klienta, &numer_telefonu, imie, nazwisko, email, &ilosc_posiadanych_filmow) != EOF){
            if(*head_klient == NULL){
                *head_klient = (struct klient *)malloc(sizeof(struct klient));
                (*head_klient) -> id_klienta = id;
                (*head_klient) -> numer_klienta = numer_klienta;
                (*head_klient) -> numer_telefonu = numer_telefonu;
                (*head_klient) -> ilosc_posiadanych_filmow = ilosc_posiadanych_filmow;
                (*head_klient) -> nastepny = NULL;
                strcpy((*head_klient) -> imie, imie);
                strcpy((*head_klient) -> nazwisko, nazwisko);
                strcpy((*head_klient) -> email, email);
            }
            else {
                struct klient *klient_nowy = *head_klient;
                while (klient_nowy->nastepny != NULL) {
                    klient_nowy = klient_nowy->nastepny;
                }
                klient_nowy-> nastepny = (struct klient *)malloc(sizeof(struct klient));
                klient_nowy-> nastepny -> id_klienta = id;
                klient_nowy-> nastepny -> numer_klienta = numer_klienta;
                klient_nowy-> nastepny -> numer_telefonu = numer_telefonu;
                klient_nowy-> nastepny -> ilosc_posiadanych_filmow = ilosc_posiadanych_filmow;
                klient_nowy-> nastepny->nastepny = NULL;
                strcpy(klient_nowy-> nastepny -> imie, imie);
                strcpy(klient_nowy-> nastepny -> nazwisko, nazwisko);
                strcpy(klient_nowy-> nastepny -> email, email);
            }
        }
        fclose(file);
    }
    return true;
}

bool check_email(char mail[]) {
    int exact, exact2;
    mail = strlwr(mail);
    int chars[5]= {0};
    if(strlen(mail)<5) return false;
    else {
        for(int i=0;i<strlen(mail)-1; i++){
            if(mail[i]=='@') {
                chars[0]++;
                exact = i;
            }
            if(mail[i]=='.') chars[1]++;
            if(mail[i]==' ') chars[2]++;
        }
        if(chars[0]!=1) return false;
        if(chars[1]==0 || chars[1]==strlen(mail)-chars[0]) return false;
        if(chars[2]!=0) return false;
        if(57<mail[0] && mail[0]<97) return false;
        if(48>mail[0] || mail[0]>122) return false;
        for(;exact<strlen(mail);exact++){
            if(mail[exact]=='.') {
                chars[3]++;
                exact2=exact;
            }
        }
        exact2++;
        if(chars[3]==0) return false;
        if(exact2 == strlen(mail)-1) return false;
        for(;exact2<strlen(mail)-1;exact2++){
            if(48>mail[exact2] || mail[exact2]>122) return false;
            if(57<mail[exact2] && mail[exact2]<97) return false;
        }
        return true;
    }
}