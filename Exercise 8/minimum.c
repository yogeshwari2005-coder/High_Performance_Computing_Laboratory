#include <stdio.h>
#include <omp.h>

int main() {
    int n, i;

    printf("\nEnter the size: ");
    scanf("%d", &n);

    int a[n], current[n];

    printf("Enter the elements: ");
    for (i = 0; i < n; i++){
        scanf("%d", &a[i]);
    }

    current[0] = a[0];
    for (i = 1; i < n; i++){
        current[i] = (current[i - 1] < a[i]) ? current[i - 1] : a[i];
    }

    printf("Minimum array: ");
    for (i = 0; i < n; i++){
        printf("%d ", current[i]);
    }

    printf("\n\n");
    return 0;
}
