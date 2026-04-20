#include<stdio.h>
#include<omp.h>

int main(){
   int m,n,k;
   int i,j;
   printf("\nEnter the row and column of the matrix:");
   scanf("%d %d\n",&m,&n);
   printf("Enter the k value:");
   scanf("%d",&k);
   int mat[m][n];
   printf("\nEnter the matrix:");
   for(i=0;i<m;i++){
      for(j=0;j<n;j++){
         scanf("%d",&mat[i][j]);
      }
   }
   int res=0;
#pragma omp parallel for reduction(+:res)
   for(i=0;i<m;i++){
      for(j=0;j<n;j++){
         if(mat[i][j]%k == 0){
            res+=mat[i][j];
         }
      }
   }
   printf("\nThe sum of elements:%d",res);
   printf("\n");
   return 0;
}
