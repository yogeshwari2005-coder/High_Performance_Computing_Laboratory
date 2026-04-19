/*To create n processes and print a Hello message with rank from each process*/

#include<stdio.h>
#include<string.h>
#include<mpi.h>

const int MAX_STRING=100;
int main(){
   char msg[100];
   int comm_size;
   int rank;
   MPI_Init(NULL,NULL);
   MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   int i;
   if(rank!=0){
      sprintf(msg,"Hello from process %d\n",rank);
      MPI_Send(msg,strlen(msg)+1,MPI_CHAR,0,0,MPI_COMM_WORLD);
   }
   else{
      printf("Hello from process %d\n",rank);
      for(i=1;i<comm_size;i++){
         MPI_Recv(msg,100,MPI_CHAR,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
         printf("%s\n",msg);
      }
   }
   MPI_Finalize();
}
