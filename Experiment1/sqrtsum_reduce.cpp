#include <iostream>  
#include <cmath>
#include <cstdlib>
#include <mpi.h> 
using namespace std;

int main(int argc, char** argv)
{
	int myid, numprocs; 
	int N = atoi(argv[1]); // get N from cmd
	double totalSum=0.0, SqrtSum=0.0;
	double *data =new double[N];
	for(int i = 0; i < N; ++i){
		data[i] = i*(i+1);
	}
	//double data[]= {1,2,3,4,5,6,7,8,9,10,11};
	MPI_Status status; 

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid); 
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs); 
	--numprocs; /*数据分配时除去0号主节点*/ 
	if (myid == 0){ /*0号主节点，主要负责数据分发和结果收集*/
		for (int i = 0; i < N; ++i ) /*数据分发: 0, */
			MPI_Send(&data[i], 1, MPI_DOUBLE, i%numprocs+1, 1 ,MPI_COMM_WORLD);
	}
	else { 
		for (int i = myid-1; i < N; i=i+numprocs) /*各子节点接受数据计算开平方，本地累加*/{
			double d;
			MPI_Recv(&d, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status); 
			SqrtSum+=sqrt(d); 
		}
		//MPI_Reduce(&SqrtSum, &totalSum, 1, MPI_DOUBLE,MPI_SUM, 0, MPI_COMM_WORLD);
	}
	MPI_Reduce(&SqrtSum, &totalSum, 1, MPI_DOUBLE,MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (myid == 0){
		printf("I am process %d. SqrtSum=%f.\n", myid, totalSum);  
	}
	else{
		printf("I am process %d. SqrtSum=%f.\n", myid, SqrtSum);  
	}
	MPI_Finalize();

}
