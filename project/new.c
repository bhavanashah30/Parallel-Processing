#include"mpi.h"
#include<stdio.h>
#include<math.h>

void sequential(int arr[],int arr_size,int param){
    int rank, size,i;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status status;
    int pass=0;
    if(rank == 0) {
    	if(param==1)//input
    	{
        	printf("\nEnter %d elements: Processor %d ",arr_size,rank+1);
        	for(i=0; i<arr_size; i++) {
            	scanf("%d",&arr[i]);
        	}
        }
        else{
        	printf("\n Processor %d ",rank+1);
                for(i=0; i<arr_size; i++) {
                        printf("\n %d",arr[i]);
                }
        }
		MPI_Send(&pass, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
    } else 
    {

    	MPI_Recv(&pass, 1, MPI_INT, rank-1, rank, MPI_COMM_WORLD,&status) ;     
        
            if(param==1)//input 
            {
            	printf("\nEnter %d elements: Processor %d ",arr_size,rank+1);
                for(i=0; i<arr_size; i++) {
                        scanf("%d",&arr[i]);
                }
            }
            else{
            	printf("\n Processor %d ",rank+1);
                for(i=0; i<arr_size; i++) {
                        printf("\n %d",arr[i]);
                }
            }
               	if(rank!=(size-1)){
               		MPI_Send(&pass, 1, MPI_INT, (rank+1), (rank+1), MPI_COMM_WORLD);	
               	}
    }

    MPI_Barrier(MPI_COMM_WORLD);

}

void SHIFT(int act_array[], int output_array[], int n,int arr_size) {
    int rank, size;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Request request;

    int temp_val = 0;
    for(;temp_val<arr_size;temp_val++) {
        output_array[temp_val] = act_array[temp_val];
    }
    if(rank == 0) {
        
        if(n > 0){
            int i=0;
            for(;i<n;i++){
                int temp=output_array[0],j;
                            MPI_Isend(&output_array[arr_size-1], 1, MPI_INT, (rank + 1)%size, (rank + 1)%size, MPI_COMM_WORLD, &request) ;
                            MPI_Recv(&output_array[0], 1 , MPI_INT, size -1, rank, MPI_COMM_WORLD, &status);
                for(j=arr_size-1;j>1;j--){
                    output_array[j] = output_array[j-1];
                }
                output_array[j] = temp;
            }
        } else {
            int i=0;
            for(;i>n;i--){
                int temp=output_array[arr_size-1],j;
                MPI_Isend(&output_array[0], 1, MPI_INT, size-1, size-1, MPI_COMM_WORLD, &request);
                MPI_Recv(&output_array[arr_size-1], 1, MPI_INT, (rank + 1)%size, rank, MPI_COMM_WORLD, &status);
                for(j=0; j < arr_size-2 ; j++){
                    output_array[j] = output_array[j+1];
                }
                output_array[j] = temp;
            }
        }
    }
    else {
        if(n > 0){
            int i;
            for(i=0; i < n ; i++){
                int temp=output_array[0],j;
                MPI_Recv(&output_array[0], 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, &status);
                MPI_Send(&output_array[arr_size-1], 1, MPI_INT, (rank + 1)%size, (rank + 1)%size, MPI_COMM_WORLD);
                for(j=arr_size-1;j>1;j--){
                                        output_array[j] = output_array[j-1];
                                }
                output_array[j] = temp;
            }
        } else {
            int fabs_n = -n;
            int i = 0;
            for(; i > n; i--) {
                int temp=output_array[arr_size-1],j;
                MPI_Recv(&output_array[arr_size-1], 1, MPI_INT, (rank + 1)%size, rank, MPI_COMM_WORLD, &status);
                MPI_Send(&output_array[0], 1, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD);
                for(j=0; j < arr_size-2 ; j++){
                                        output_array[j] = output_array[j+1];
                                }
                output_array[j] = temp;
            }
        }
    }

}

int main(int argc,char *argv[])
{

    int rank, size, n;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Status status;
    int pass=0;
    
    int arr_size;
    if(rank == 0) {
        printf("\nEnter the size of the array : ");
        scanf("%d",&arr_size);
        printf("\nEnter the value of shift factor : ");
        scanf("%d",&n);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&arr_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int input_array[arr_size], output_array[arr_size];
    int i;
    // Reading data
    sequential(input_array,arr_size,1);

    // Reading shift factor
    n=n%(size*arr_size);
    SHIFT(input_array, output_array, n,arr_size);

    
    if(rank == 0) {
        printf("\nBefore shift\n");
	}
	sequential(input_array,arr_size,2);

    if(rank == 0) {
        printf("\nAfter shift\n");
	}
	sequential(output_array,arr_size,2);

    MPI_Finalize();
    return 0;
}

