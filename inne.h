#ifndef PP2_INNE_H
#define PP2_INNE_H
#include "inne.c"

int input_int(int a, int b);
unsigned int input_uint(unsigned int a, unsigned int b);
unsigned long long input_ull(unsigned long long a, unsigned long long b);
double input_dbl(double a, double b);
void czekaj_na_input_ESCAPE();
void wyczysc_ekran();
bool dodaj_folder(char folder[]);
void input_string(char *result, int max);

#endif //PP2_INNE_H
