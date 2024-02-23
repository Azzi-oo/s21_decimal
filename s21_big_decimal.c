#include "s21_decimal.h"

// установка бита в s21_big_decimal
void s21_set_big_bit(s21_big_decimal *num, int index, int bit) {
    if (bit == 1) {
        num->bits[index / 32] = num->bits[index / 32] | (1 << index % 32);
    } else {
        num->bits[index / 32] = num->bits[index / 32] & ~(1 << index % 32);
    }
}

// установка знака в s21_big_decimal
void s21_set_big_sign(s21_big_decimal *num, int sign) {
    return s21_set_big_bit(num, 223, sign);
}

// установить степень в s21_big_decimal
void s21_set_big_scale(s21_big_decimal *num, int scale) {
    int sign = s21_get_big_sign(*num);
    num->bits[6] = (scale << 16);
    s21_set_big_sign(num, sign);
}

// узнать степень s21_big_decimal
int s21_get_big_scale(s21_big_decimal num) {
    s21_set_big_sign(&num, PLUS_SIGN);
    return num.bits[6] >> 16;
}

