#include <stdio.h>
#include <omp.h>
#include <limits.h>
int main() {
    int m, n;
    int i, j;
    printf("\nEnter the row and column of the matrix: ");
    if (scanf("%d %d", &m, &n) != 2) return -1;
    int mat[m][n];
    printf("Enter the matrix elements row-wise:\n");
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &mat[i][j]);
        }
    }
    int count = 0;
    // Arrays to store pre-calculated row minimums and column maximums
    int row_min[m];
    int col_max[n];
    // Find row minimums in parallel
    #pragma omp parallel for
    for (i = 0; i < m; i++) {
        int min = mat[i][0];
        for (j = 1; j < n; j++) {
            if (mat[i][j] < min) {
                min = mat[i][j];
            }
        }
        row_min[i] = min;
    }
    // Find col maximums in parallel
    #pragma omp parallel for
    for (j = 0; j < n; j++) {
        int max = mat[0][j];
        for (i = 1; i < m; i++) {
            if (mat[i][j] > max) {
                max = mat[i][j];
            }
        }
        col_max[j] = max;
    }

    #pragma omp parallel for reduction(+:count)
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            if (mat[i][j] == row_min[i] && mat[i][j] == col_max[j]) {
                count++;
            }
        }
    }
    printf("Total saddle points found: %d\n", count);
    return 0;
}
