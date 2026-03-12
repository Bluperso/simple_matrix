#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

START_TEST(test_init_and_clear_1) {
  matrix_t mat = {NULL, 0, 0};
  int m = 6;
  int n = 7;
  int result = create_matrix(m, n, &mat);

  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(mat.columns, n);
  ck_assert_int_eq(mat.rows, m);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      ck_assert_double_eq(mat.matrix[i][j], 0.0);
    }
  }
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_init_and_clear_2) {
  matrix_t mat = {NULL, 0, 0};
  int m = -6;
  int n = 7;
  int result = create_matrix(m, n, &mat);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_init_and_clear_3) {
  matrix_t mat = {NULL, 0, 0};
  int m = -6;
  int n = -7;
  int result = create_matrix(m, n, &mat);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_init_and_clear_4) {
  matrix_t mat = {NULL, 0, 0};
  int m = 0;
  int n = 0;
  int result = create_matrix(m, n, &mat);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_eq_matrix_1) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  int m = 6;
  int n = 6;
  int result_1 = create_matrix(m, n, &mat_1);
  int result_2 = create_matrix(m, n, &mat_2);

  ck_assert_int_eq(result_1, 0);
  ck_assert_int_eq(result_2, 0);

  int result_comp = eq_matrix(&mat_1, &mat_2);

  ck_assert_int_eq(result_comp, SUCCESS);

  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
}
END_TEST

START_TEST(test_eq_matrix_2) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  int m = 6;
  int n = 6;
  int result_1 = create_matrix(m, n, &mat_1);
  int result_2 = create_matrix(m + 1, n + 4, &mat_2);

  ck_assert_int_eq(result_1, 0);
  ck_assert_int_eq(result_2, 0);

  int result_comp = eq_matrix(&mat_1, &mat_2);

  ck_assert_int_eq(result_comp, FAILURE);

  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
}
END_TEST

START_TEST(test_eq_matrix_3) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  int m = 6;
  int n = 6;
  int result_1 = create_matrix(m, n, &mat_1);
  int result_2 = create_matrix(m, n, &mat_2);

  ck_assert_int_eq(result_1, 0);
  ck_assert_int_eq(result_2, 0);

  mat_1.matrix[3][4] = 99;

  int result_comp = eq_matrix(&mat_1, &mat_2);

  ck_assert_int_eq(result_comp, FAILURE);

  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
}
END_TEST

START_TEST(test_eq_matrix_4) {
  matrix_t* mat_1 = NULL;
  matrix_t* mat_2 = NULL;

  int result_comp = eq_matrix(mat_1, mat_2);

  ck_assert_int_eq(result_comp, FAILURE);
}
END_TEST

START_TEST(test_eq_matrix_5) {
  // Тест на сравнение с точностью до 1e-6
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  int m = 3;
  int n = 3;

  // Создаём две одинаковые матрицы 3x3
  create_matrix(m, n, &mat_1);
  create_matrix(m, n, &mat_2);

  // Заполняем матрицы одинаковыми значениями
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      mat_1.matrix[i][j] = 0.0;
      mat_2.matrix[i][j] = 0.0;
    }
  }

  // Проверяем, что одинаковые матрицы равны
  ck_assert_int_eq(eq_matrix(&mat_1, &mat_2), SUCCESS);

  // Тест 1: Разница в седьмом знаке после запятой (1e-7) - должны быть равны
  mat_1.matrix[0][2] = 1.1234567;  // 1.1234567
  mat_2.matrix[0][2] = 1.1234568;  // Разница 0.0000001 (1e-7)
  ck_assert_int_eq(eq_matrix(&mat_1, &mat_2), SUCCESS);

  // Тест 2: Разница меньше 1e-7 - должны быть равны
  mat_1.matrix[0][2] = 2.9999999;
  mat_2.matrix[0][2] = 2.9999998;  // Разница 0.0000001 (1e-7)
  ck_assert_int_eq(eq_matrix(&mat_1, &mat_2), SUCCESS);

  // Тест 3: Отрицательные числа с малой разницей
  mat_1.matrix[0][2] = -3.141591;
  mat_2.matrix[0][2] = -3.141592;  // Разница 0.000001 (1e-6)
  ck_assert_int_eq(eq_matrix(&mat_1, &mat_2), FAILURE);

  // Тест 4: Разница ровно на границе 1e-6 - должны НЕ быть равны
  mat_1.matrix[0][2] = 0.2;
  mat_2.matrix[0][2] = 0.200001;  // Разница 0.000001 (1e-6)
  ck_assert_int_eq(eq_matrix(&mat_1, &mat_2), FAILURE);

  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
}
END_TEST

START_TEST(test_sum_1) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};

  int m = 6;
  int n = 6;
  create_matrix(m, n, &mat_1);
  create_matrix(m, n, &mat_2);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      mat_1.matrix[i][j] = (i + j) * 1.5;
      mat_2.matrix[i][j] = (i + j) * (-1.5);
    }
  }

  int result = sum_matrix(&mat_1, &mat_2, &res);
  ck_assert_int_eq(result, OK);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      ck_assert_int_eq(res.matrix[i][j], 0);
    }
  }

  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
  remove_matrix(&res);
}
END_TEST

START_TEST(test_sum_2) {
  matrix_t res = {NULL, 0, 0};

  int result = sum_matrix(NULL, NULL, &res);

  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_sum_3) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};

  int m = 6;
  int n = 6;
  create_matrix(m, n, &mat_1);
  create_matrix(m + 1, n - 5, &mat_2);

  int result = sum_matrix(&mat_1, &mat_2, &res);
  ck_assert_int_eq(result, CALC_ERROR);

  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
}
END_TEST

START_TEST(test_sub_1) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};

  int m = 6;
  int n = 6;
  create_matrix(m, n, &mat_1);
  create_matrix(m, n, &mat_2);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      mat_1.matrix[i][j] = (i + j) * 1.5;
      mat_2.matrix[i][j] = (i + j) * 1.5;
    }
  }

  int result = sub_matrix(&mat_1, &mat_2, &res);
  ck_assert_int_eq(result, OK);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      ck_assert_int_eq(res.matrix[i][j], 0);
    }
  }

  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
  remove_matrix(&res);
}
END_TEST

START_TEST(test_sub_2) {
  matrix_t res = {NULL, 0, 0};

  int result = sub_matrix(NULL, NULL, &res);

  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_sub_3) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};

  int m = 6;
  int n = 6;
  create_matrix(m, n, &mat_1);
  create_matrix(m + 1, n - 5, &mat_2);

  int result = sub_matrix(&mat_1, &mat_2, &res);
  ck_assert_int_eq(result, CALC_ERROR);

  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
}
END_TEST

START_TEST(test_mult_number_1) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};
  int number = 113.77;
  int m = 6;
  int n = 6;
  create_matrix(m, n, &mat_1);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      mat_1.matrix[i][j] = (i + j) * 1.5;
    }
  }

  int result = mult_number(&mat_1, number, &res);
  ck_assert_int_eq(result, OK);

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      ck_assert_int_eq(res.matrix[i][j], ((i + j) * 1.5) * number);
    }
  }

  remove_matrix(&mat_1);
  remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_number_2) {
  matrix_t res = {NULL, 0, 0};

  int result = mult_number(NULL, 10, &res);
  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_mult_matrix_1) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};
  int rows = 3;
  int cols = 3;
  // создание матрицы 3x3
  ck_assert_int_eq(create_matrix(rows, cols, &mat_1), OK);
  ck_assert_int_eq(create_matrix(rows, cols, &mat_2), OK);

  // 1 2 3
  // 4 5 6
  // 7 8 9
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      mat_1.matrix[i][j] = i * cols + j + 1;
    }
  }

  // 9 8 7
  // 6 5 4
  // 3 2 1
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      mat_2.matrix[i][j] = (rows - i) * cols - j;
    }
  }

  int result = mult_matrix(&mat_1, &mat_2, &res);
  ck_assert_int_eq(result, OK);

  // Проверяем размеры результата
  ck_assert_int_eq(res.rows, rows);
  ck_assert_int_eq(res.columns, cols);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double expected = 0;
      for (int k = 0; k < cols; k++) {
        expected += mat_1.matrix[i][k] * mat_2.matrix[k][j];
      }
      ck_assert_double_eq_tol(res.matrix[i][j], expected, 1e-6);
    }
  }

  // Освобождаем память
  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
  remove_matrix(&res);
}
END_TEST

START_TEST(test_mult_matrix_2) {
  matrix_t res = {NULL, 0, 0};

  int result = mult_matrix(NULL, NULL, &res);

  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_mult_matrix_3) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t mat_2 = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};

  int m = 6;
  int n = 6;
  create_matrix(m, n, &mat_1);
  create_matrix(m + 1, n - 5, &mat_2);

  int result = mult_matrix(&mat_1, &mat_2, &res);
  ck_assert_int_eq(result, CALC_ERROR);

  remove_matrix(&mat_1);
  remove_matrix(&mat_2);
}
END_TEST

START_TEST(test_transpose_1) {
  matrix_t mat_1 = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};
  int m = 2;
  int n = 5;
  create_matrix(m, n, &mat_1);

  mat_1.matrix[1][2] = 12.6;
  mat_1.matrix[0][4] = -65.0;

  int result = transpose(&mat_1, &res);
  ck_assert_int_eq(result, OK);

  ck_assert_int_eq(res.matrix[2][1], 12.6);
  ck_assert_int_eq(res.matrix[4][0], -65.0);

  remove_matrix(&mat_1);
  remove_matrix(&res);
}
END_TEST

START_TEST(test_transpose_2) {
  matrix_t res = {NULL, 0, 0};

  int result = transpose(NULL, &res);
  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_determinant_1) {
  // Тест для матрицы 1x1
  matrix_t mat = {NULL, 0, 0};
  double result = 0.0;
  create_matrix(1, 1, &mat);
  mat.matrix[0][0] = 5.7;
  int status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq(result, 5.7);
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_determinant_2) {
  // Тест для единичной матрицы 3x3 (определитель = 1)
  matrix_t mat = {NULL, 0, 0};
  double result = 0.0;
  create_matrix(3, 3, &mat);
  for (int i = 0; i < 3; i++) {
    mat.matrix[i][i] = 1.0;
  }
  int status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq(result, 1.0);
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_determinant_3) {
  // Тест для вырожденной матрицы 2x2 (линейно зависимые строки, определитель =
  // 0)
  matrix_t mat = {NULL, 0, 0};
  double result = 0.0;
  create_matrix(2, 2, &mat);
  mat.matrix[0][0] = 2.0;
  mat.matrix[0][1] = 4.0;
  mat.matrix[1][0] = 1.0;
  mat.matrix[1][1] = 2.0;  // {2} = {1} / 2
  int status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq_tol(result, 0.0, 1e-6);
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_determinant_4) {
  // Тест для матрицы 2x2 с известным определителем: |3  2| = 3*4 - 2*1 = 10
  //                                                 |1  4|
  matrix_t mat = {NULL, 0, 0};
  double result = 0.0;
  create_matrix(2, 2, &mat);
  mat.matrix[0][0] = 3.0;
  mat.matrix[0][1] = 2.0;
  mat.matrix[1][0] = 1.0;
  mat.matrix[1][1] = 4.0;
  int status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq(result, 10.0);
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_determinant_5) {
  matrix_t mat = {NULL, 0, 0};
  double result = 0.0;
  create_matrix(3, 3, &mat);
  mat.matrix[0][0] = 2.0;
  mat.matrix[0][1] = 1.0;
  mat.matrix[0][2] = -3.0;
  mat.matrix[1][0] = 1.0;
  mat.matrix[1][1] = 0.0;
  mat.matrix[1][2] = 1.0;
  mat.matrix[2][0] = -3.0;
  mat.matrix[2][1] = 1.0;
  mat.matrix[2][2] = -2.0;
  // определитель должен быть -6.0
  int status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq_tol(result, -6.0, 1e-10);
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_determinant_6) {
  // Тест для верхнетреугольной матрицы 4x4 (определитель = произведение
  // диагонали)
  matrix_t mat = {NULL, 0, 0};
  double result = 0.0;
  create_matrix(4, 4, &mat);
  mat.matrix[0][0] = 2.0;
  mat.matrix[0][1] = 3.0;
  mat.matrix[0][2] = 1.0;
  mat.matrix[0][3] = 4.0;
  mat.matrix[1][1] = 5.0;
  mat.matrix[1][2] = 2.0;
  mat.matrix[1][3] = 1.0;
  mat.matrix[2][2] = 3.0;
  mat.matrix[2][3] = 2.0;
  mat.matrix[3][3] = 4.0;
  // Определитель = 2 * 5 * 3 * 4 = 120
  int status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq(result, 120.0);
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_determinant_7) {
  // |0  1  2|
  // |1  0  3| = 1 (после перестановки строк знак меняется один раз)
  // |2  3  0|
  matrix_t mat = {NULL, 0, 0};
  double result = 0.0;
  create_matrix(3, 3, &mat);
  mat.matrix[0][0] = 0.0;
  mat.matrix[0][1] = 1.0;
  mat.matrix[0][2] = 2.0;
  mat.matrix[1][0] = 1.0;
  mat.matrix[1][1] = 0.0;
  mat.matrix[1][2] = 3.0;
  mat.matrix[2][0] = 2.0;
  mat.matrix[2][1] = 3.0;
  mat.matrix[2][2] = 0.0;
  int status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq(result, 12.0);
  remove_matrix(&mat);

  // определитель матрицы ниже 7
  create_matrix(3, 3, &mat);
  mat.matrix[0][0] = 0.0;
  mat.matrix[0][1] = 1.0;
  mat.matrix[0][2] = 2.0;
  mat.matrix[1][0] = 1.0;
  mat.matrix[1][1] = 1.0;
  mat.matrix[1][2] = 3.0;
  mat.matrix[2][0] = 2.0;
  mat.matrix[2][1] = 3.0;
  mat.matrix[2][2] = 1.0;

  status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq(result, 7.0);
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_determinant_8) {
  // Тест с некорректным указателем (NULL)
  double result = 0.0;
  int status = determinant(NULL, &result);
  ck_assert_int_eq(status, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_determinant_9) {
  // Тест для матрицы 4x4 с ненулевым определителем
  // | 1  2  3  4|
  // | 5  6  7  8|
  // | 9 10 11 12| = 0 (вырожденная, строки линейно зависимы)
  // |13 14 15 16|
  matrix_t mat = {NULL, 0, 0};
  double result = 0.0;
  create_matrix(4, 4, &mat);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mat.matrix[i][j] = i * 4 + j + 1;
    }
  }
  int status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);
  ck_assert_double_eq_tol(result, 0.0, 1e-6);
  remove_matrix(&mat);

  // Дополнительный тест для невырожденной 4x4 матрицы
  create_matrix(4, 4, &mat);
  mat.matrix[0][0] = 1;
  mat.matrix[0][1] = 2;
  mat.matrix[0][2] = 3;
  mat.matrix[0][3] = 4;
  mat.matrix[1][0] = 2;
  mat.matrix[1][1] = 3;
  mat.matrix[1][2] = 4;
  mat.matrix[1][3] = 1;
  mat.matrix[2][0] = 3;
  mat.matrix[2][1] = 4;
  mat.matrix[2][2] = 1;
  mat.matrix[2][3] = 2;
  mat.matrix[3][0] = 4;
  mat.matrix[3][1] = 1;
  mat.matrix[3][2] = 2;
  mat.matrix[3][3] = 3;
  // Определитель = 160
  status = determinant(&mat, &result);
  ck_assert_int_eq(status, OK);

  remove_matrix(&mat);
}
END_TEST

START_TEST(test_determinant_10) {
  matrix_t mat = {NULL, 0, 0};
  double result = 0.0;
  create_matrix(3, 5, &mat);

  int status = determinant(&mat, &result);
  ck_assert_int_eq(status, CALC_ERROR);
  ck_assert_double_eq(result, 0.0);

  remove_matrix(&mat);
}
END_TEST

START_TEST(test_calc_complements_1) {
  matrix_t mat = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};
  create_matrix(3, 3, &mat);
  mat.matrix[0][0] = 2.0;
  mat.matrix[0][1] = 1.0;
  mat.matrix[0][2] = -3.0;
  mat.matrix[1][0] = 1.0;
  mat.matrix[1][1] = 0.0;
  mat.matrix[1][2] = 1.0;
  mat.matrix[2][0] = -3.0;
  mat.matrix[2][1] = 1.0;
  mat.matrix[2][2] = -2.0;
  // сравнивал с результатом в вальфраме
  // должно быть:
  //       | 1  1 -1|
  //       | 1 13  5|
  //       |-1  5  1|
  int result = calc_complements(&mat, &res);
  ck_assert_int_eq(result, OK);
  ck_assert_double_eq_tol(res.matrix[0][0], -1.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[0][1], -1.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[0][2], 1.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[1][0], -1.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[1][1], -13.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[1][2], -5.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[2][0], 1.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[2][1], -5.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[2][2], -1.0, 1e-10);
  remove_matrix(&mat);
  remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_2) {
  matrix_t mat = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};
  create_matrix(3, 5, &mat);

  int status = calc_complements(&mat, &res);
  ck_assert_int_eq(status, CALC_ERROR);

  remove_matrix(&mat);
  remove_matrix(&res);
}
END_TEST

START_TEST(test_calc_complements_3) {
  // Тест с некорректным указателем (NULL)
  int status = calc_complements(NULL, NULL);
  ck_assert_int_eq(status, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_inverse_matrix_1) {
  matrix_t mat = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};
  create_matrix(3, 3, &mat);
  mat.matrix[0][0] = 2.0;
  mat.matrix[0][1] = 1.0;
  mat.matrix[0][2] = -3.0;
  mat.matrix[1][0] = 1.0;
  mat.matrix[1][1] = 0.0;
  mat.matrix[1][2] = 1.0;
  mat.matrix[2][0] = -3.0;
  mat.matrix[2][1] = 1.0;
  mat.matrix[2][2] = -2.0;
  // сравнивал с результатом в вальфраме
  // должно быть:
  //       | 1  1 -1|
  // 1/6 * | 1 13  5|
  //       |-1  5  1|
  double det = -6;
  int result = inverse_matrix(&mat, &res);
  ck_assert_int_eq(result, OK);
  ck_assert_double_eq_tol(res.matrix[0][0], -1.0 / det, 1e-10);
  ck_assert_double_eq_tol(res.matrix[0][1], -1 / det, 1e-10);
  ck_assert_double_eq_tol(res.matrix[0][2], 1 / det, 1e-10);
  ck_assert_double_eq_tol(res.matrix[1][0], -1 / det, 1e-10);
  ck_assert_double_eq_tol(res.matrix[1][1], -13 / det, 1e-10);
  ck_assert_double_eq_tol(res.matrix[1][2], -5 / det, 1e-10);
  ck_assert_double_eq_tol(res.matrix[2][0], 1 / det, 1e-10);
  ck_assert_double_eq_tol(res.matrix[2][1], -5 / det, 1e-10);
  ck_assert_double_eq_tol(res.matrix[2][2], -1 / det, 1e-10);
  remove_matrix(&mat);
  remove_matrix(&res);
}
END_TEST

START_TEST(test_inverse_matrix_2) {
  matrix_t mat = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};
  create_matrix(3, 3, &mat);
  mat.matrix[0][0] = 2.0;
  mat.matrix[0][1] = 5.0;
  mat.matrix[0][2] = 7.0;
  mat.matrix[1][0] = 6.0;
  mat.matrix[1][1] = 3.0;
  mat.matrix[1][2] = 4.0;
  mat.matrix[2][0] = 5.0;
  mat.matrix[2][1] = -2.0;
  mat.matrix[2][2] = -3.0;
  // пример с проекта в таске

  int result = inverse_matrix(&mat, &res);
  ck_assert_int_eq(result, OK);
  ck_assert_double_eq_tol(res.matrix[0][0], 1.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[0][1], -1.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[0][2], 1.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[1][0], -38.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[1][1], 41.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[1][2], -34.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[2][0], 27.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[2][1], -29.0, 1e-10);
  ck_assert_double_eq_tol(res.matrix[2][2], 24, 1e-10);
  remove_matrix(&mat);
  remove_matrix(&res);
}
END_TEST

START_TEST(test_inverse_matrix_3) {
  int result = inverse_matrix(NULL, NULL);
  ck_assert_int_eq(result, INCORRECT_MATRIX);
}
END_TEST

START_TEST(test_inverse_matrix_4) {
  matrix_t mat = {NULL, 0, 0};
  create_matrix(3, 198, &mat);

  int result = inverse_matrix(&mat, NULL);
  ck_assert_int_eq(result, CALC_ERROR);
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_inverse_matrix_5) {
  matrix_t mat = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};
  create_matrix(3, 3, &mat);
  mat.matrix[0][0] = 2.0;
  mat.matrix[0][1] = 1.0;
  mat.matrix[0][2] = -3.0;
  mat.matrix[1][0] = 1.0;
  mat.matrix[1][1] = 0.0;
  mat.matrix[1][2] = 1.0;
  mat.matrix[2][0] = 3.0;
  mat.matrix[2][1] = 1.0;
  mat.matrix[2][2] = -2.0;

  int result = inverse_matrix(&mat, &res);
  ck_assert_int_eq(result, CALC_ERROR);
  remove_matrix(&mat);
}
END_TEST

START_TEST(test_inverse_matrix_6) {
  matrix_t mat = {NULL, 0, 0};
  matrix_t res = {NULL, 0, 0};
  create_matrix(1, 1, &mat);
  mat.matrix[0][0] = -666.666;

  int result = inverse_matrix(&mat, &res);
  ck_assert_int_eq(result, OK);
  ck_assert_double_eq_tol(res.matrix[0][0], -1.0 / 666.666, 1e-10);
  remove_matrix(&mat);
  remove_matrix(&res);
}
END_TEST

// =====================================================================================================================
Suite* suite_matrix(void) {
  TCase* tc_init = tcase_create("init_and_clear_matrix");

  tcase_add_test(tc_init, test_init_and_clear_1);
  tcase_add_test(tc_init, test_init_and_clear_2);
  tcase_add_test(tc_init, test_init_and_clear_3);
  tcase_add_test(tc_init, test_init_and_clear_4);

  TCase* tc_eq = tcase_create("eq_matrix");

  tcase_add_test(tc_eq, test_eq_matrix_1);
  tcase_add_test(tc_eq, test_eq_matrix_2);
  tcase_add_test(tc_eq, test_eq_matrix_3);
  tcase_add_test(tc_eq, test_eq_matrix_4);
  tcase_add_test(tc_eq, test_eq_matrix_5);

  TCase* tc_sum = tcase_create("sum");

  tcase_add_test(tc_sum, test_sum_1);
  tcase_add_test(tc_sum, test_sum_2);
  tcase_add_test(tc_sum, test_sum_3);

  TCase* tc_sub = tcase_create("sub");

  tcase_add_test(tc_sub, test_sub_1);
  tcase_add_test(tc_sub, test_sub_2);
  tcase_add_test(tc_sub, test_sub_3);

  TCase* tc_mult_number = tcase_create("mult_number");

  tcase_add_test(tc_mult_number, test_mult_number_1);
  tcase_add_test(tc_mult_number, test_mult_number_2);

  TCase* tc_mult_matrix = tcase_create("mult_matrix");

  tcase_add_test(tc_mult_matrix, test_mult_matrix_1);
  tcase_add_test(tc_mult_matrix, test_mult_matrix_2);
  tcase_add_test(tc_mult_matrix, test_mult_matrix_3);

  TCase* tc_transpose = tcase_create("transpose");

  tcase_add_test(tc_transpose, test_transpose_1);
  tcase_add_test(tc_transpose, test_transpose_2);

  TCase* tc_determinant = tcase_create("determinant");

  tcase_add_test(tc_determinant, test_determinant_1);
  tcase_add_test(tc_determinant, test_determinant_2);
  tcase_add_test(tc_determinant, test_determinant_3);
  tcase_add_test(tc_determinant, test_determinant_4);
  tcase_add_test(tc_determinant, test_determinant_5);
  tcase_add_test(tc_determinant, test_determinant_6);
  tcase_add_test(tc_determinant, test_determinant_7);
  tcase_add_test(tc_determinant, test_determinant_8);
  tcase_add_test(tc_determinant, test_determinant_9);
  tcase_add_test(tc_determinant, test_determinant_10);

  TCase* tc_calc_complements = tcase_create("calc_complements");

  tcase_add_test(tc_calc_complements, test_calc_complements_1);
  tcase_add_test(tc_calc_complements, test_calc_complements_2);
  tcase_add_test(tc_calc_complements, test_calc_complements_3);

  TCase* tc_inverse_matrix = tcase_create("inverse_matrix");

  tcase_add_test(tc_inverse_matrix, test_inverse_matrix_1);
  tcase_add_test(tc_inverse_matrix, test_inverse_matrix_2);
  tcase_add_test(tc_inverse_matrix, test_inverse_matrix_3);
  tcase_add_test(tc_inverse_matrix, test_inverse_matrix_4);
  tcase_add_test(tc_inverse_matrix, test_inverse_matrix_5);
  tcase_add_test(tc_inverse_matrix, test_inverse_matrix_6);

  Suite* s = suite_create("matrix");

  suite_add_tcase(s, tc_init);
  suite_add_tcase(s, tc_eq);
  suite_add_tcase(s, tc_sum);
  suite_add_tcase(s, tc_sub);
  suite_add_tcase(s, tc_mult_number);
  suite_add_tcase(s, tc_mult_matrix);
  suite_add_tcase(s, tc_transpose);
  suite_add_tcase(s, tc_determinant);
  suite_add_tcase(s, tc_calc_complements);
  suite_add_tcase(s, tc_inverse_matrix);

  return s;
}

int main() {
  Suite* s = suite_matrix();
  SRunner* sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);

  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}