#include "mpi/mpi.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include<iostream>

using namespace std;

int getMaxDiv(int p){
    //busca el max comun divisor
    int maxdiv = p-1;
    while(p%maxdiv != 0 && maxdiv > 0){
        maxdiv--;
    }
    //si es primo
    if(maxdiv == 1){
        maxdiv = p;
    }
    return maxdiv;
}

int getAreaTotal(int tamano){
    int area = tamano*tamano;
    //cout <<"El area total es : " <<  area << endl;
    return area;
}

int getCuadrantes(int maxdiv){
    int numCuadrantes = maxdiv*maxdiv;
    //cout << "Numero de cuadrantes: " << numCuadrantes << endl;
    return numCuadrantes;
}

void fillMatrix(int **matriz, int tamano){
    srand(time(NULL));
    for(int i =0; i<tamano; i++){
        for(int j=0; j<tamano; j++){
            matriz[i][j] = rand()%3;
        }
    }
}


int main(int argc, char **argv) {
    int p = 0, myid, numprocs;
    //double startwtime, endwtime;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int stripSize;
    int **matriz;
    int *filaLocal, *data;

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

        int maxdiv = getMaxDiv(p);
        int tamano = p*maxdiv;

        stripSize = tamano/p;

        cout << "El area es: " << getAreaTotal(tamano) << endl;
        cout << "Numero de cuadrantes: " << getCuadrantes(maxdiv) << endl;
        cout << "Total de hectareas por cuadrante: " << p*p << endl;
        cout << "Porcentaje del area a reforestar: " <<  endl;

        //genera la matriz
        data = (int *)malloc(sizeof(int)*tamano*tamano);
        matriz = (int **)malloc(sizeof(int*) * tamano);
        for(int i=0; i< tamano; i++){
            matriz[i] = &(data[i*tamano]);
        }

        cout << "genera la matriz..." << endl;

        fillMatrix(matriz, tamano);

        cout << "La matriz es la siguiente:" <<  endl;
        for(int i =0; i<tamano; i++){
            for(int j=0; j<tamano; j++){
                cout << matriz[i][j] << " ";
            }
            cout << endl;
        }
    }
    MPI_Scatter(matriz, stripSize, MPI_INT, filaLocal, stripSize, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}