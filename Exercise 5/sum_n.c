#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define MAX 100

int arr[MAX];
int i,n,sum=0;
void* calculateSum(void* arg){
        for( i=0;i<n;i++){
                sum+=arr[i];
        }
        pthread_exit(NULL);
}

int main(){
        pthread_t t1;
        printf("\n*****SUM OF N NUMBERS*****\n");
        printf("Enter number of elements:");
        scanf("%d",&n);
        printf("Enter %d elements:\n",n);
        for( i=0;i<n;i++){
                scanf("%d",&arr[i]);
        }
        pthread_create(&t1,NULL,calculateSum,NULL);
        pthread_join(t1,NULL);
        printf("\nSum of %d elements = %d \n",n,sum);
        return 0;
}
