#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int sign1 = s21_get_sign(value_1);
    int sign2 = s21_get_sign(value_2);

    int error = OK;

    // перевести нужно value_1, value_2 и result в big_decimal
    s21_big_decimal val_1 = transformDecimalToBig(value_1);
    s21_big_decimal val_2 = transformDecimalToBig(value_2);
    s21_big_decimal result_big = {{0, 0, 0, 0, 0, 0, 0}};

    // чтобы не было переполнения, нормализация будет происходить в big decimal
    s21_normalize_big(&val_1, &val_2);
    int commonScale = s21_get_big_scale(val_1);

    // создаются копии value_1, value_2
    s21_big_decimal positiveValue_1 = val_1; // копия value_1 без знака
    s21_big_decimal positiveValue_2 = val_2;
    s21_set_big_sign(&positiveValue_1, 0); // из числа преобразуем в положительное
    s21_set_big_sign(&positiveValue_2, 0);

    // степени должны быть одинаковыми во всех случаях
    s21_set_big_scale(&result_big, commonScale);

    if (sign1 == sign2) {
        if(!s21_is_greater_or_equal(positiveValue_1, positiveValue_2)) {
            // знак в результат записать и поменять знак
            s21_set_big_sign(&result_big, !sign2);
            // функция для вычитания
            s21_sub_big_dec(val_2, val_1, &result_big);
        } else if (s21_is_greater_or_equal(positiveValue_1, positiveValue_2)) {
            // записываем знак в результат
            s21_set_big_sign(&result_big, sign1);
            // функция для вычитания
            s21_sub_big_dec(val_1, val_2, &result_big);
        }
        // случай когда числа равны => sign = 0
    } else {
        // вариант когда второе число < 0
        if (sign2) {
            s21_set_big_sign(&result_big, sign1);
        } else {
            s21_set_big_sign(&result_big, sign1 ^ sign2);
        }
        // из большего вычитаем меньшее
        s21_add_big(val_1, val_2, &result_big);
    }
    error = s21_from_big_to_decimal(result_big, result);

    return error;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = OK;

  /* переводим value_1, value_2 и result в big_decimal */
  s21_big_decimal val_1 = transformDecimalToBig(value_1);
  s21_big_decimal val_2 = transformDecimalToBig(value_2);
  s21_big_decimal result_big = {{0, 0, 0, 0, 0, 0, 0}};

  s21_mul_big_dec(val_1, val_2, &result_big);

  // перевод из большого в малый decimal
  error = s21_from_big_to_decimal(result_big, result);

  return error;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int sign1 = s21_get_sign(value_1);
    int sign2 = s21_get_sign(value_2);
    int sign = sign1 ^ sign2;
    int error = OK;
    s21_decimal zero = {{0, 0, 0, 0}};
    s21_decimal one = {{1, 0, 0, 0}};

    // создаем копии value_1, value_2 без учета знака
    s21_decimal positiveValue_1 = value_1;
    s21_decimal positiveValue_2 = value_2;
    s21_set_sign(&positiveValue_1, 0); // из числа делаем положительное
    s21_set_sign(&positiveValue_2, 0);

    if (s21_is_equal(positiveValue_2, zero)) {
        error = ZERO_DIV;
    } else if (s21_is_equal(positiveValue_1, zero) ||
               s21_is_equal(positiveValue_1, one)) {
        s21_set_sign(result, sign);
        *result = value_1;
        } else {
            // переводим value_1, value_2 и result в big_decimal
            s21_big_decimal val_1 = transformDecimalToBig(value_1);
            s21_big_decimal val_2 = transformDecimalToBig(value_2);
            s21_big_decimal result_big = {{0, 0, 0, 0, 0, 0, 0}};
            s21_big_decimal mod = {{0, 0, 0, 0, 0, 0, 0}};

            // нормализация приводит оба числа к одной степени
            // нормализация происходит в big_decimal, чтобы избежать переполнения
            s21_normalize_big(&val_1, &val_2);

            // степень result_big < 29
            int scaleResult = 0;
            while ((checkWorkBitsBigDecimal(mod) && scaleResult < 28) ||
                    scaleResult == 0) {
                s21_div_big_dec(&val_1, val_2, &mod);
                s21_mul_by_10_big(&result_big);
                s21_add_big_dec(result_big, val_1, &result_big);
                s21_mul_by_10_big(&mod);
                val_1 = mod;
                scaleResult++;
            }

            // знак записываем в рез-ат и в остаток от деления
            s21_set_big_sign(&result_big, sign);
            s21_set_big_sign(&mod, sign);
            s21_set_big_scale(&result_big, scaleResult - 1);

            // перевод из большого децимала в малый
            error = s21_from_big_to_decimal(result_big, result);
        }
        return error;
    }

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int sign1 = s21_get_sign(value_1);
    int error = OK;
    s21_decimal zero = {{0, 0, 0, 0}};

    if (s21_is_equal(value_2, zero)) {
        error = ZERO_DIV;
    } else if (s21_is_equal(value_1, zero)) {
        *result = value_1;
    } else {
        // переводим value_1, value_2 и result in big_decimal
        s21_big_decimal val_1 = transformDecimalToBig(value_1);
        s21_big_decimal val_2 = transformDecimalToBig(value_2);

        s21_big_decimal mod = {{0, 0, 0, 0, 0, 0, 0}};
        // после нормализации делимое и делитель приводятся к одной степени
        // нормализация будет происходить в big_decimal чтобы избежать епреполнения
        s21_normalize_big(&val_1, &val_2);

        s21_div_big_dec(&val_1, val_2, &mod);

        // знак записывается в остаток от деления
        s21_set_big_sign(&mod, sign1);

        // перевод из юольшого в малый decimal
        error = s21_from_big_to_decimal(mod, result);
    }
    return error;
}