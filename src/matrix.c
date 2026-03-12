#include "matrix.h"

int create_matrix(int rows, int columns, matrix_t *result) {
  int flag = 0;
  if ((rows <= 0) || (columns <= 0)) return 1;
  result->matrix = (double **)malloc(rows * sizeof(double *));
  if (result->matrix == NULL)
    flag = 1;
  else {
    result->rows = rows;
    result->columns = columns;
    for (int i = 0; (i < rows) && (flag != 1); i++) {
      result->matrix[i] = (double *)malloc(columns * sizeof(double));
      if (result->matrix[i] == NULL) {
        flag = 1;
        for (int j = 0; j < i; j++) {
          free(result->matrix[j]);
          result->matrix[j] = NULL;
        }
      }
    }
    if (flag == 1) {
      free(result->matrix);
      result->matrix = NULL;
    } else {
      for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
          result->matrix[i][j] = 0.0;
        }
      }
      result->columns = columns;
      result->rows = rows;
    }
  }
  return flag;
}

void remove_matrix(matrix_t *A) {
  for (int i = 0; i < (A->rows); i++) {
    free(A->matrix[i]);
    A->matrix[i] = NULL;
  }
  free(A->matrix);
  A->matrix = NULL;
  A->columns = 0;
  A->rows = 0;
}

int eq_matrix(matrix_t *A, matrix_t *B) {
  if ((A == NULL) || (B == NULL)) return FAILURE;
  int flag = SUCCESS;
  if ((A->columns != B->columns) || (A->rows != B->rows)) flag = FAILURE;
  for (int i = 0; (i < A->rows) && flag; i++) {
    for (int j = 0; (j < B->columns) && flag; j++) {
      if (!(fabs((A->matrix[i][j]) - (B->matrix[i][j])) < 1e-6)) flag = FAILURE;
    }
  }
  return flag;
}

int sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if ((A == NULL) || (B == NULL)) return INCORRECT_MATRIX;
  if ((A->columns != B->columns) || (A->rows != B->rows)) return CALC_ERROR;
  int flag = create_matrix(A->rows, A->columns, result);
  for (int i = 0; (i < A->rows) && !flag; i++) {
    for (int j = 0; (j < A->columns) && !flag; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }
  return flag;
}

int sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if ((A == NULL) || (B == NULL)) return INCORRECT_MATRIX;
  if ((A->columns != B->columns) || (A->rows != B->rows)) return CALC_ERROR;
  int flag = create_matrix(A->rows, A->columns, result);
  for (int i = 0; (i < A->rows) && !flag; i++) {
    for (int j = 0; (j < A->columns) && !flag; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return flag;
}

int mult_number(matrix_t *A, double number, matrix_t *result) {
  if (A == NULL) return INCORRECT_MATRIX;
  int flag = create_matrix(A->rows, A->columns, result);
  for (int i = 0; (i < A->rows) && !flag; i++) {
    for (int j = 0; (j < A->columns) && !flag; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return flag;
}

int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (A == NULL || B == NULL || result == NULL) return INCORRECT_MATRIX;
  if (A->columns != B->rows) return CALC_ERROR;
  int flag = create_matrix(A->rows, B->columns, result);
  for (int i = 0; i < A->rows && !flag; i++) {
    for (int j = 0; j < B->columns; j++) {
      for (int k = 0; k < A->columns; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
  return flag;
}

int transpose(matrix_t *A, matrix_t *result) {
  if (A == NULL) return INCORRECT_MATRIX;
  int flag = create_matrix(A->columns, A->rows, result);
  for (int i = 0; (i < A->rows) && !flag; i++) {
    for (int j = 0; (j < A->columns) && !flag; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }
  return flag;
}

int minor_matrix(matrix_t *A, matrix_t *result, int m, int n) {
  int flag = create_matrix(((A->rows) - 1), ((A->columns) - 1), result);
  for (int i = 0, k = 0; (i < A->rows) && (k < (A->rows - 1)) && !flag; i++) {
    for (int j = 0, l = 0; (j < A->columns) && (l < (A->columns - 1)); j++) {
      if ((i != m) && (j != n)) {
        result->matrix[k][l] = A->matrix[i][j];
        l++;
      }
    }
    if (i != m) k++;
  }
  return flag;
}

int calc_complements(matrix_t *A, matrix_t *result) {
  if (A == NULL) return INCORRECT_MATRIX;
  int flag = OK;
  if (A->columns != A->rows)
    flag = CALC_ERROR;
  else
    flag = create_matrix(A->rows, A->columns, result);
  if ((A->columns > 1) && (A->rows > 1)) {
    for (int i = 0; (i < A->rows) && !flag; i++) {
      for (int j = 0; (j < A->columns) && !flag; j++) {
        matrix_t temp = {NULL, 0, 0};
        flag = minor_matrix(A, &temp, i, j);
        double det = 0.0;
        if (!flag) flag = determinant(&temp, &det);
        if (!flag) result->matrix[i][j] = det * (pow(-1, (i + j)));
        remove_matrix(&temp);
      }
    }
  } else {
    result->matrix[0][0] = 1.0;
  }
  return flag;
}

int determinant(matrix_t *A, double *result) {
  if (A == NULL) return INCORRECT_MATRIX;
  matrix_t temp = {NULL, 0, 0};
  int flag = OK;
  if (A->columns != A->rows) flag = CALC_ERROR;
  if (flag == OK) {
    int sign = 1;
    flag = triangulation(A, &temp, &sign);
    if (!flag) {
      (*result) = temp.matrix[0][0];
      for (int i = 1; i < temp.columns; i++) {
        (*result) *= temp.matrix[i][i];
      }
      (*result) *= sign;
      remove_matrix(&temp);
    }
  }
  return flag;
}

int assign(matrix_t *A, matrix_t *B) {
  int flag = create_matrix(A->rows, A->columns, B);
  for (int i = 0; i < A->rows && !flag; i++) {
    for (int j = 0; j < A->columns; j++) {
      B->matrix[i][j] = A->matrix[i][j];
    }
  }
  return flag;
}

int triangulation(matrix_t *A, matrix_t *result, int *sign) {
  matrix_t temp = {NULL, 0, 0};
  int flag = assign(A, &temp);
  (*sign) = 1;
  for (int i = 0; i < temp.columns && !flag; i++) {
    double max_item = fabs(temp.matrix[i][i]);
    int row_max = i;
    for (int j = i; j < temp.rows; j++) {
      if (fabs(temp.matrix[j][i]) > fabs(max_item)) {
        max_item = fabs(temp.matrix[j][i]);
        row_max = j;
      }
    }
    if (i != row_max) {
      (*sign) *= (-1);
      double *temp_ptr = temp.matrix[i];
      temp.matrix[i] = temp.matrix[row_max];
      temp.matrix[row_max] = temp_ptr;
    }
    for (int j = i + 1; (j < temp.rows) && (fabs(max_item) >= 1e-6); j++) {
      double koeff = temp.matrix[j][i] / temp.matrix[i][i];
      for (int k = 0; k < temp.columns; k++) {
        temp.matrix[j][k] = temp.matrix[j][k] - (koeff * temp.matrix[i][k]);
      }
    }
  }
  if (!flag) {
    assign(&temp, result);
  }
  if (temp.matrix != NULL) remove_matrix(&temp);
  if (flag) remove_matrix(result);
  return flag;
}

int inverse_matrix(matrix_t *A, matrix_t *result) {
  if (A == NULL || A->matrix == NULL) return INCORRECT_MATRIX;
  double det = 0.0;
  int flag = determinant(A, &det);
  if (flag == OK && det != 0) {
    matrix_t temp_compl = {NULL, 0, 0};
    matrix_t temp_transp = {NULL, 0, 0};
    flag = calc_complements(A, &temp_compl);
    if (!flag) {
      flag = transpose(&temp_compl, &temp_transp);
    }
    if (!flag) {
      double koeff = 1.0 / det;
      flag = mult_number(&temp_transp, koeff, result);
    }
    if (temp_compl.matrix != NULL) remove_matrix(&temp_compl);
    if (temp_transp.matrix != NULL) remove_matrix(&temp_transp);
  } else if (det == 0)
    flag = CALC_ERROR;
  return flag;
}