#include<stdio.h>
#include<omp.h>
int main(){
        int n,s,thread_count;
        printf("\nEnter the total number of elements: ");
        scanf("%d",&n);
        int arr[n];
        printf("\nEnter elements: ");
        for(int i=0;i<n;i++){
                scanf("%d ",&arr[i]);
        }
        printf("Enter the constant value:");
        scanf("%d",&s);
        printf("\nEnter the thread count:");
        scanf("%d",&thread_count);
#pragma omp parallel for num_threads(thread_count)
        for(int i=0;i<n;i++){
                arr[i]=arr[i]+s;
        }
        printf("\nElements plus constant value: ");
        for(int i=0;i<n;i++){
                printf("%d ",arr[i]);
        }
        printf("\n");
        return 0;
}
