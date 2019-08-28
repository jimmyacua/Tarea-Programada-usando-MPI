#include "mpi/mpi.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include<iostream>

using namespace std;

int main(int argc, char **argv) {
    int p = 0, myid, numprocs;
    double mypi;
    double startwtime, endwtime;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    /*  Se inicia el trabajo con MPI */
    MPI_Init(&argc, &argv);

    /*  MPI almacena en numprocs el numero total de procesos que se pusieron a correr */
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    /*  MPI almacena en myid la identificacion del proceso actual */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    /*  MPI almacena en processor_name el nombre de la computadora en la que corre el
    proceso actual, y en namelen la longitud de este */
    MPI_Get_processor_name(processor_name, &namelen);

    if (myid == 0){
        cout << "Digite el numero de procesos" << endl;
        cin >> p;
        //busca el max comun divisor
        int maxdiv = p-1;
        while(p%maxdiv != 0 && maxdiv > 0){
            maxdiv--;
        }
        if(maxdiv == 1){
            maxdiv = p;
        }
        cout << "max divisor: " << maxdiv << endl;
        int tamano = p*maxdiv;
        int matriz[tamano][tamano];
        srand(time(NULL));
        for(int i =0; i<tamano; i++){
            for(int j=0; j<tamano; j++){
                matriz[i][j] = rand()%3;
            }
        }

        for(int i =0; i<tamano; i++){
            for(int j=0; j<tamano; j++){
                cout << matriz[i][j] << " ";
            }
            cout << endl;
        }

        cout <<"El area total es : " <<  tamano*tamano << endl;
        cout << "Numero de cuadrantes: " << maxdiv*maxdiv << endl;
        cout << "Total de hectareas por cuadrante: " << p*p << endl;
        cout << "Porcentaje del area a reforestar " << endl;

    }


    MPI_Finalize();
    return 0;
}