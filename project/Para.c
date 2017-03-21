#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<time.h>
#include "mpi.h"
#include "sll.h"


int main(argc, argv)
int argc;
char **argv;
{   clock_t begin, end;
    //timespec start,end;
    double time_spent;
	int rank, size;
	int i,m,j=0;
	int barrier,count=0,tot_size=0,temp_size=0;
	int global_count=0;
	char str[100];
	char filePath[100];
    int rowSize=0,tempSize=0;
    int n=1024;
    char delim[] = " ,";
    char* token;
    int *col_1,*col_2,*col_3;
    double t1, t2; 

	FILE *fp;

	int total;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	MPI_Request request;
    int counter[size-1];
    int size_predict=0,proc_rank;


	if(rank==0)
	{   
        printf("\n Enter the number of files ");
        scanf("%d",&total); 

        //begin = clock();
        t1 = MPI_Wtime();
        //clock_gettime(CLOCK_REALTIME, &start);
				
	}
	MPI_Bcast(&total, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank!=0)
	{
		int k=0;
		for(i=rank;i<=total;i=i+(size-1))
		{	
			sprintf(filePath,"FILES/file_%d.csv",i);
			
			//printf("\n rank:%d - file %d",rank,i);
			fp=fopen(filePath,"r");
			if(k==0)
			{
				while(fgets(str,n,fp)!= NULL)
    			{
	     			tempSize=tempSize+1;
			 	}

			 		barrier=(total%(size-1));
			 		rowSize=tempSize*(total/(size-1));

	     			if(rank<=barrier){
	     				rowSize=rowSize+tempSize;
	     			}
	     			printf("\n %d : rank : %d \n",rowSize,rank);

 				// printf("Row Size is %d \n",rowSize);
			
    				col_1 = (int*)malloc(rowSize * sizeof(int));
    				col_2 = (int*)malloc(rowSize * sizeof(int));
    				col_3 = (int*)malloc(rowSize * sizeof(int));

		        	rewind(fp);
			}


    			for(j=0; j<tempSize;j++)
    			{
        			fgets(str,n,fp) ;

			        for (token = strtok(str, delim),m=0; token,m<3; token = strtok(NULL, delim),m++)
        			{
            				if(m==0)
            				{
                				col_1[(k*tempSize)+j]= atoi(token);
            				}
            				else if(m==1)
            				{
                				col_2[(k*tempSize)+j]= atoi(token);
            				}
            				else
            				{
             					col_3[(k*tempSize)+j]= atoi(token);
            				}
        			}
    			}
   			fclose(fp);
			k=k+1;
		} 

		/*if(rank!=0)
		{
   				printf("\n rank:%d ",rank);
    			for(j=1; j<=rowSize;j++)
    			{
					printf("%d \t %d \t %d",col_1[j],col_2[j],col_3[j]);
        			printf("\n");
				}	
		}*/
		
	}


	// Process 0 find null elements


    if(rank!=0){
        
        MPI_Send(&rowSize, 1, MPI_INTEGER, 0, rank, MPI_COMM_WORLD);
        int col_1_null[rowSize/4];
        int col_2_null[rowSize/4];
        int index_null[rowSize/4];
        
        for(i=0;i<rowSize;i++)
        {
            if(col_3[i]==-999)
            {   
                col_1_null[count]=col_1[i];
                col_2_null[count]=col_2[i];
                index_null[count]=i;
                count+=1;
            }

        }
        //MPI_Wait(&request,&status);
        MPI_Barrier(MPI_COMM_WORLD);

        
        MPI_Send(&count, 1, MPI_INTEGER, 0, rank, MPI_COMM_WORLD);
        for(i=0;i<count;i++)
        {
            MPI_Send(&col_1_null[i], 1, MPI_INTEGER, 0, (rank*100 + i), MPI_COMM_WORLD);
            MPI_Send(&col_2_null[i], 1, MPI_INTEGER, 0, (rank*1000 + i), MPI_COMM_WORLD);
        }

    }

    if(rank==0)
    {   

        // Taking datasize from each processor

        for(i=1;i<size;i++)
        {
            MPI_Recv(&temp_size, 1, MPI_INTEGER, i, i, MPI_COMM_WORLD, &status);
            tot_size+=temp_size;
        }

    }
        //printf("tot size: %d",tot_size);
        //master table of null values;
    int col_1_gnull[tot_size/4];
    int col_2_gnull[tot_size/4];
    int col_3_gnull[tot_size/4];
    int proc_null[tot_size/4];

    if (rank==0)
    {


        MPI_Barrier(MPI_COMM_WORLD);
        for(i=1;i<size;i++){
            MPI_Recv(&count, 1, MPI_INTEGER, i, i, MPI_COMM_WORLD, &status);
            counter[i-1]=count;
            printf("\ncount: %d",count);
        }

        for(i=1;i<size;i++){
            int c;
            for(j=0;j<counter[i-1];j++){

                //printf("\nglobal_count: %d    rank: %d",global_count,rank);
                MPI_Recv(&c, 1, MPI_INTEGER, i, (i*100 + j), MPI_COMM_WORLD, &status);
                col_1_gnull[global_count]=c;
                proc_null[global_count]=i;
                
                MPI_Recv(&c, 1, MPI_INTEGER, i, (i*1000 + j ), MPI_COMM_WORLD, &status);
                col_2_gnull[global_count]=c;
                //printf("\nglobal_count: %d    rank: %d",global_count,c);
                global_count+=1;
            }

        }

        //for(i=0;i<global_count;i++){
        //    printf("\n   %d:  %d",col_1_gnull[i],col_1_gnull[i]);
        //}
    }

    /*if(rank==0){
        for(i=0;i<global_count;i++){
            printf("\n   %d:  %d:  %d    :==proc no=: %d",col_1_gnull[i],col_2_gnull[i],col_2_gnull[i],proc_null[i]);
        }
        printf("\n global_size:  %d",global_count);

    }
    */


	//// End

    MPI_Bcast(&global_count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    ////// Calculations on each processor

        for(i=0;i<global_count;i++)
    {
        int data1,data2;
        int flag=0;
        if(rank==0)
        {   
            for(j=0;j<i;j++){
                if((col_1_gnull[j]==col_1_gnull[i])&&(col_2_gnull[j]==col_2_gnull[i]))
                {
                    col_3_gnull[i]=col_3_gnull[j];
                    flag=1;
                    break;
                }
            }
            data1=col_1_gnull[i];
            data2=col_2_gnull[i];
        }

        MPI_Bcast(&flag, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if(flag==1)
        {
            continue;
        }

        MPI_Bcast(&data1, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&data2, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        /*if(rank==0)
        {
            printf("\n bcast  %d:  %d",data1,data2);
        }*/

        int temp_value;
        int temp_count;


        if(rank!=0)
        {   
            NODE * head=NULL;
            create_sll(&head);  
            //printf("\n success :%d :%d",rank,i);   
            for(j=0;j<rowSize;j++)
            {
                if(col_1[j]==data1 && col_2[j]==data2 && col_3[j]!=-999)
                {
                    NODE * temp=locate(head, col_3[j]);
                    if(temp)
                        temp->valcount=temp->valcount+1; //changes required
                    else
                        add_at_end(&head, col_3[j],1); //changes required
                }
            }
            size_predict=count_nodes(head); //changes required
            MPI_Send(&size_predict, 1, MPI_INTEGER, 0, rank, MPI_COMM_WORLD);   
            //printf("\n simple size  :%d:  %d",size_predict,rank);

            for(j=0;j<size_predict;j++)
            {
                temp_value=head->value; //changes required
                temp_count=head->valcount; //changes required
                MPI_Send(&temp_value, 1, MPI_INTEGER, 0, (rank*100 + j), MPI_COMM_WORLD);
                MPI_Send(&temp_count, 1, MPI_INTEGER, 0, (rank*1000 + j), MPI_COMM_WORLD);

                head->next=head;
                //printf("\n sent size of list  :%d:  %d: %d",size_predict,rank,temp_value);
            }

        }
        else
        {
            NODE * head=NULL;
            create_sll(&head);
            for(j=1;j<size;j++){
                MPI_Recv(&size_predict, 1, MPI_INTEGER, j, j, MPI_COMM_WORLD,&status);
                int k=0;
                for(k;k<size_predict;k++)
                {
                    MPI_Recv(&temp_value, 1, MPI_INTEGER, j, (j*100 + k), MPI_COMM_WORLD,&status);
                    MPI_Recv(&temp_count, 1, MPI_INTEGER, j, (j*1000 + k), MPI_COMM_WORLD,&status);
                    
                    NODE * temp=locate(head, temp_value);
                    if(temp!=NULL)
                        temp->valcount=temp->valcount+temp_count;  //changes required
                    else
                        add_at_end(&head, temp_value,temp_count); //changes required
                }
                //printf("\n size of list  :%d:  %d ",size_predict,j);
            }

            int check_size=count_nodes(head); //changes required

            if(check_size==0){
                col_3_gnull[i]=-999;
                continue;
            }

            //printf("\n global_size:  %d",check_size);
            //printf("\n");
            //print_list(head);

            int max=head->valcount;  //changes required
            int max_value=head->value;  //changes required

            head=head->next;

            for(j=1;j<check_size;j++){
                if(head->valcount>=max)  //changes required
                {
                    max=head->valcount; //changes required
                    max_value=head->value; //changes required
                }
                head=head->next;  //changes required
            }
            col_3_gnull[i]=max_value;

            //printf("\n bcast  %d:  %d",data1,data2);
            //printf("\n value:  %d",max_value);

        }
        //MPI_Barrier(MPI_COMM_WORLD);
    }

    /////end of calculations

    if(rank==0){
        //end = clock();
        t2 = MPI_Wtime();
        //clock_gettime(CLOCK_REALTIME, &end);
        //time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    
    
        for(i=0;i<global_count;i++){
            printf("\n   %d:  %d:  %d    :==proc no=: %d",col_1_gnull[i],col_2_gnull[i],col_3_gnull[i],proc_null[i]);
        }
        printf("\n global_size:  %d",global_count);
        printf("\n Time it took is: %f",t2-t1);

    }    

	MPI_Finalize(); 	
	return 0;
}
 
