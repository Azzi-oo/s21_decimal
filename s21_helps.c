#include "s21_decimal.h"

// узнать знак + или -
void s21_get_sign(s21_decimal num) {
    int mask = 1 << 31;
    return !!(num.bits[SCALE] & mask);
}

// установить знак + / -
void s21_set_sign(s21_decimal *num, int sign) {
    return s21_set_bit(num, 127, sign);
}

// узнать бит по порядковому номеру
int s21_get_bit(s21_decimal num, int index) {
    int mask = 1 << (index % 32);
    return !!(num.bits[index / 32] & mask);
}

// установить бит по порядковому номеру
int s21_set_bit(s21_decimal *num, int index, int bit) {
    if (bit == 1) {
        num->bits[index / 32] = num->bits[index / 32] | (1 << index % 32);
    } else {
        num->bits[index / 32] = num->bits[index / 32] & ~(1 << index % 32);
    }
}

// узнать степень
int s21_get_scale(s21_decimal num) {
    s21_set_sign(&num, PLUS_SIGN);
    return num.bits[SCALE] >> 16;
}

// установить степень
void s21_set_scale(s21_decimal *num, int scale) {
    int sign = s21_get_sign(*num);
    num->bits[SCALE] = (scale << 16);
    s21_set_sign(num, sign);
}

// делим число в двоичном виде на 10
void s21_div_by_10(s21_decimal *num) {
    unsigned long long dec = 0, tmp;
    for (int i = 2; i >= 0; i--) {
        tmp = 0;
        tmp = (unsigned long long)num->bits[i] + dec;
        num->bits[i] = (unsigned int)(tmp / 10);
        dec = (tmp % 10) << 32;
    }
}

// проверка децимала на нуль
int is_dec_zero(s21_decimal d) {
    return (d.bits[LOW] == 0 && d.bits[MID] == 0 && d.bits[HIGH] == 0);
}
