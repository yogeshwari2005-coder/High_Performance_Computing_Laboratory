/*Write an MPI program to create n processes.Let add ranked process send a
 * string to master process with msg type 1,and even ranked process send a
 * string to master process with message type 2. Master process wll convert
 * the string to uppercase if it is a type 1 msg.It checks if the string is
 * palindrome for a type 2 message.Print the result along with rank of the process */


#include<stdio.h>
#include<string.h>
#include<mpi.h>
#include<ctype.h>
int main()
{
        char str[100];
        int comm_size;
        int my_rank;
        MPI_Init(NULL,NULL);
        MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
        MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

        MPI_Status status;

        if(my_rank==1)
        {
                sprintf(str,"Hello from first process");
                MPI_Send(str,strlen(str)+1,MPI_CHAR,0,1,MPI_COMM_WORLD);
        }
        else if(my_rank==2)
        {
                sprintf(str,"malayalams");
                MPI_Send(str,strlen(str)+1,MPI_CHAR,0,2,MPI_COMM_WORLD);
        }
        else if(my_rank==3)
        {
                sprintf(str,"Hello from third process");
                MPI_Send(str,strlen(str)+1,MPI_CHAR,0,1,MPI_COMM_WORLD);
        }
        else if(my_rank==4)
        {
                sprintf(str,"level");
                MPI_Send(str,strlen(str)+1,MPI_CHAR,0,2,MPI_COMM_WORLD);
        }
        else
        {
                int i,j;
                printf("Master process :%d\n",my_rank);
                for(i=1;i<comm_size;i++)
                {
                        MPI_Recv(str,100,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
                        if(status.MPI_TAG==1)
                        {
                                for(j=0;str[j]!='\0';j++)
                                {
                                        str[j]=toupper(str[j]);
                                }
                                printf("Result:%s from process %d\n",str,status.MPI_SOURCE);
                        }
                        else if(status.MPI_TAG==2)
                        {
                                int len = strlen(str);
                                int is_palindrome = 1;
                                for (int j = 0; j < len / 2; j++)
                                {
                                        if (str[j] != str[len - 1 - j])
                                        {
                                                is_palindrome = 0;
                                                break;
                                        }
                                }
                                if(is_palindrome)
                                {
                                        printf("Result:%s is a  Palindrome from process %d\n",str,status.MPI_SOURCE);
                                }
                                else
                                {
                                        printf("Result:%s is  Not palindrome from process %d\n",str,status.MPI_SOURCE);
                                }
                        }
                }
        }
        MPI_Finalize();
}
