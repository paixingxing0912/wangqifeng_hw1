#include "algebra.h"
#include <stdio.h>
#include <math.h>

Matrix create_matrix(int row, int col)
{
    Matrix m;
    m.rows = row;
    m.cols = col;
    return m;
}

Matrix add_matrix(Matrix a, Matrix b)
{
    int i,j;
    if (a.rows == b.rows && a.cols == b.cols) {
        Matrix result = create_matrix(a.rows, a.cols);
        for (i = 0; i < a.rows; i++)
            for (j = 0; j < a.cols; j++)
                result.data[i][j] = a.data[i][j] + b.data[i][j];
    return result;
    }
    else 
        printf("Error: Matrix a and b must have the same rows and cols.\n");
    return create_matrix(0, 0);
}

Matrix sub_matrix(Matrix a, Matrix b)
{
    int i,j;
    if (a.rows == b.rows && a.cols == b.cols) {
        Matrix result = create_matrix(a.rows, a.cols);
        for ( i = 0; i < a.rows; i++)
            for ( j = 0; j < a.cols; j++)
                result.data[i][j] = a.data[i][j] - b.data[i][j];
    return result;
    }
    else 
        printf("Error: Matrix a and b must have the same rows and cols.\n");
    return create_matrix(0, 0);
}

Matrix mul_matrix(Matrix a, Matrix b)
{
    int i,j,k;
    if (a.cols == b.rows) {
        Matrix result = create_matrix(a.rows, b.cols);
        for ( i = 0; i < a.rows; i++)
            for ( j = 0; j < b.cols; j++)
                for ( k = 0; k < a.cols; k++)
                    result.data[i][j] += a.data[i][k] * b.data[k][j];
        return result;
    }
    printf("Error: The number of cols of matrix a must be equal to the number of rows of matrix b.\n");
    return create_matrix(0, 0);
}

Matrix scale_matrix(Matrix a, double k)
{
    int i,j;
    Matrix result = create_matrix(a.rows, a.cols);
    for ( i = 0; i < a.rows; i++)
        for ( j = 0; j < a.cols; j++)
            result.data[i][j] = a.data[i][j] * k;
    return result;
    return create_matrix(0, 0);
}

Matrix transpose_matrix(Matrix a)
{
    int i,j;
    Matrix result = create_matrix(a.cols, a.rows);
    for ( i = 0; i < a.rows; i++)
        for ( j = 0; j < a.cols; j++)
            result.data[j][i] = a.data[i][j];
    return result;
    return create_matrix(0, 0);
}

double det_matrix(Matrix a)
{
    int i,j,k;
    if (a.rows == a.cols) {
    int n = a.rows;
    double result = 1.0;
    Matrix temp = create_matrix(n, n);
    
    for ( i = 0; i < n; i++)
        for ( j = 0; j < n; j++)
            temp.data[i][j] = a.data[i][j];
    
    for ( i = 0; i < n; i++) {
        int pivot = i;
        while (pivot < n && fabs(temp.data[pivot][i]) < 1e-10)
            pivot++;
        
        if (pivot == n) return 0;
        
        if (pivot != i) {
            result *= -1;
            for ( j = i; j < n; j++)
                swap(&temp.data[i][j], &temp.data[pivot][j]);
        }
        
        result *= temp.data[i][i];
        
        for ( j = i + 1; j < n; j++)
            for ( k = i + 1; k < n; k++)
                temp.data[j][k] -= temp.data[j][i] * temp.data[i][k] / temp.data[i][i];
    }
    
    return result;
    }
    else
    {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
}

Matrix inv_matrix(Matrix a)
{
    int i,j,k;
    if (a.rows != a.cols) {
        printf("Error: The matrix must be a square matrix.\n");
        return create_matrix(0, 0);
    }
    
    int n = a.rows;
    Matrix augmented = create_matrix(n, 2 * n);
    
    for ( i = 0; i < n; i++)
        for ( j = 0; j < n; j++)
            augmented.data[i][j] = a.data[i][j];
    
    for ( i = 0; i < n; i++)
        augmented.data[i][i + n] = (i == (i + n - n)) ? 1.0 : 0.0;
    
    for ( i = 0; i < n; i++) {
        int pivot = i;
        while (pivot < n && fabs(augmented.data[pivot][i]) < 1e-10)
            pivot++;
        
        if (pivot == n) return create_matrix(0, 0);
        
        if (pivot != i)
            swap_rows(&augmented, i, pivot);
        
        double pivot_val = augmented.data[i][i];
        for ( j = i; j < 2 * n; j++)
            augmented.data[i][j] /= pivot_val;
        
        for ( j = 0; j < n; j++) {
            if (j != i) {
                double factor = augmented.data[j][i];
                for ( k = i; k < 2 * n; k++)
                    augmented.data[j][k] -= factor * augmented.data[i][k];
            }
        }
    }
    
    Matrix inverse = create_matrix(n, n);
    for ( i = 0; i < n; i++)
        for ( j = 0; j < n; j++)
            inverse.data[i][j] = augmented.data[i][j + n];
    
    return inverse;
}

int rank_matrix(Matrix a) {
    int rows = a.rows;
    int cols = a.cols;
    Matrix temp;
    
    if (cols > rows) {
        temp = transpose_matrix(a);
        rows = temp.rows;
        cols = temp.cols;
    } else {
        temp = create_matrix(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                temp.data[i][j] = a.data[i][j];
            }
        }
    }

    int rank = 0;
    for (int col = 0; col < cols; col++) {
        int pivot = -1;
        for (int row = col; row < rows; row++) { 
            if (fabs(temp.data[row][col]) > 1e-10) {
                pivot = row;
                break;
            }
        }
        if (pivot == -1) continue;
        
        if (pivot != col) {
            swap_rows(&temp, col, pivot);
        }
        
        double pivot_val = temp.data[col][col];
        for (int j = col; j < cols; j++) {
            temp.data[col][j] /= pivot_val;
        }

        for (int row = 0; row < rows; row++) {
            if (row != col && fabs(temp.data[row][col]) > 1e-10) {
                double factor = temp.data[row][col];
                for (int j = col; j < cols; j++) {
                    temp.data[row][j] -= factor * temp.data[col][j];
                }
            }
        }
        rank++;
    }
    return rank;
}

double trace_matrix(Matrix a)
{
    int i,j;
    if (a.rows != a.cols) {
        printf("Error: The matrix must be a square matrix.\n");
        return 0;
    }
    double result = 0.0;
    for ( i = 0; i < a.rows; i++)
        result += a.data[i][i];
    return result;
}
void print_matrix(Matrix a)
{
    int i,j;
    for ( i = 0; i < a.rows; i++)
    {
        for ( j = 0; j < a.cols; j++)
        {
            // 按行打印，每个元素占8个字符的宽度，小数点后保留2位，左对齐
            printf("%-8.2f", a.data[i][j]);
        }
        printf("\n");
    }
}
void swap(double *a, double *b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}
void swap_rows(Matrix *m, int r1, int r2) {
    int j;
    for ( j = 0; j < m->cols; j++)
        swap(&m->data[r1][j], &m->data[r2][j]);
}