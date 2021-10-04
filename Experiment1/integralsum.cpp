#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

#define N 100000000
#define a 10
#define b 100
using namespace std;

int main(int argc, char** argv)
{
	double start = 0;
	int myid,numprocs;
	double local=0.0, totalSum=0.0, dx=(double)(b-a)/N; /* 小矩形宽度 */ 

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	--numprocs;

	if(myid==0){
		start = clock();
		for (int source = 1; source <= numprocs; ++source) { /*结果收集*/
			double d;
			MPI_Recv(&d, 1, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
			totalSum += d;
		}
	}
	else{
		for(int i=myid;i<N;i=i+numprocs) { 
			double x;
			x = a + i*dx +dx/2; 
			local += x*x*x*dx; 
		}
		MPI_Send(&local, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD); /*本地累加结果送回主节点*/
	}
	
	if(myid==0) {
		printf("I am process %d. The finalresult is %f.\n", myid, totalSum); 
		printf("time: %f\n", (clock() - start) / CLOCKS_PER_SEC); 
	}
	else{
		printf("I am process %d. My result is %f.\n", myid, local);
	}
	
	MPI_Finalize();
	
}
