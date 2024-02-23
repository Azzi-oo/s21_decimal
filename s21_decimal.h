#ifndef S21_DECIMAL
#define S21_DECINAL

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum arithmetics_status { OK, ZERO_DIV };
enum number_sign { PLUS_SIGN, MINUS_SIGN };
enum bits_pos { LOW, MID, HIGH, SCALE };

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[7];
} s21_big_decimal;

int s21_sub(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Вычитание
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_get_sign(s21_decimal num);  // Узнаем знак Decimal'a
void s21_set_sign(s21_decimal *num, int sign);  // Устанавливаем знак Decimal'y
int s21_get_bit(s21_decimal num,
                int index);  // Узнаем бит num'a в позиции index
void s21_set_bit(
    s21_decimal *num, int index,
    int bit);  // Устанавливаем bit (1 or 0) в num'e в позиции index
int s21_get_scale(s21_decimal num);  // Узнаем степень в Decimal'e
void s21_set_scale(s21_decimal *num,
                   int scale);  // Устанавливаем степень в Decimal'e

void s21_div_by_10(
    s21_decimal *num);  // Уменьшаем разрядность числа Decimal (делим на 10)
int is_dec_zero(s21_decimal d);  // Равен ли s21_decimal нулю

// Для s21_big_decimal
void s21_init_big_decimal(s21_big_decimal *obj);
void s21_from_decimal_to_big(s21_decimal d1, s21_big_decimal *b1,
                             s21_decimal d2, s21_big_decimal *b2);

void s21_set_big_bit(s21_big_decimal *num, int index, int bit);
void s21_set_big_sign(s21_big_decimal *num, int sign);
void s21_set_big_scale(s21_big_decimal *num, int scale);
int s21_get_big_scale(s21_big_decimal num);
void s21_div_big_by_10(s21_big_decimal *num);

int s21_get_big_bit(s21_big_decimal num, int index);
int s21_get_big_sign(s21_big_decimal num);
int s21_from_big_to_decimal(s21_big_decimal big, s21_decimal *norm);
s21_big_decimal transformDecimalToBig(s21_decimal num);


void s21_sub_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result);
int s21_mul_big_dec(s21_big_decimal value_1, s21_big_decimal value_2,
                    s21_big_decimal *result);
void s21_div_big_dec(s21_big_decimal *value_1, s21_big_decimal value_2,
                     s21_big_decimal *mod);
void s21_mul_by_10_big(s21_big_decimal *num);
int s21_normalize_big(s21_big_decimal *value_1, s21_big_decimal *value_2);

#endif  // S21_DECIMAL