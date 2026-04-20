#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

int main(){
        int n,thread_count;
        printf("\nEnter the total number of elements:");
        scanf("%d",&n);
        int a[n],b[n],result[n];
        printf("\nArray 1: ");
        for(int i=0;i<n;i++){
                scanf("%d",&a[i]);
        }
        printf("\nArray 2: ");
        for(int i=0;i<n;i++)
        {
                scanf("%d",&b[i]);
        }
#pragma omp parallel for num_threads(thread_count)
        for(int i=0;i<n;i++){
                result[i]=a[i]+b[i];
        }
        printf("\nSum of the array: ");
        for(int i=0;i<n;i++){
                printf("%d ",result[i]);
        }
        printf("\n");
        return 0;
}
