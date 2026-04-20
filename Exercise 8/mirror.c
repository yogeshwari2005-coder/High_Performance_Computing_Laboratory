#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

int main(){
        int n,thread_count;
        printf("Enter the size of the square matrix:");
        scanf("%d",&n);
        int a[n][n],res[n][n];
        int i,j;
        printf("\nEnter the matrix:");
        printf("\n");
        for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                        scanf("%d ",&a[i][j]);
                }
                //printf("\n");
        }

        printf("\nEnter the number of threads:");
        scanf("%d",&thread_count);
#pragma omp parallel for num_threads(thread_count)
        for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                        res[i][j]=a[i][n-1-j];
                }
        }
        printf("\nMirror Matrix:");
        printf("\n");
        for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                        printf("%d ",res[i][j]);
                }
                printf("\n");
        }
        printf("\n");
        return 0;
}
