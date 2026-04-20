#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

int main(){
        int my_rank, comm_sz;
        int n, num;
        int *arr, *sub;
        int sub_size;
        int local=0, total=0;

        MPI_Init(NULL,NULL);
        MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
        MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

        if(my_rank==0){
                printf("\nEnter number of elements : ");
                fflush(stdout);
                scanf("%d", &n);
                arr=(int*)malloc(n*sizeof(int));
                printf("\nArray elements:\n");
                int i;
                for(i=0;i<n;i++){
                        arr[i]=rand()%10;
                        printf("%d\t", arr[i]);
                }
                printf("\n");
                printf("Enter number to find frequency : ");
                fflush(stdout);
                scanf("%d", &num);
        }

        MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&num,1,MPI_INT,0,MPI_COMM_WORLD);

        sub_size=n/comm_sz;
        sub=(int*)malloc(sub_size*sizeof(int));
        MPI_Scatter(arr,sub_size,MPI_INT,sub,sub_size,MPI_INT,0,MPI_COMM_WORLD);

        local=0;
        int i;
        for(i=0;i<sub_size;i++){
                if(sub[i]==num)
                        local++;
        }

        MPI_Reduce(&local,&total,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
        if(my_rank==0){
                printf("Frequency of %d = %d\n\n", num, total);
        }
        MPI_Finalize();
}
