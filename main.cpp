/*
 * Tarea programada 1
 * Curso: Arquitectura de computadoras
 * Estudiante: Jimmy Acuña Díaz (jimmyacuna@ucr.ac.cr)
 * Carné: B50060
 */

#include "mpi/mpi.h"
//#include <math.h>
#include <stdlib.h>
#include <time.h>
#include<iostream>

#include <unistd.h>

using namespace std;

int getMaxDiv(int p) {
    //busca el max comun divisor
    int maxdiv = p - 1;
    while (p % maxdiv != 0 && maxdiv > 0) {
        maxdiv--;
    }
    //si es primo
    if (maxdiv == 1) {
        maxdiv = p;
    }
    return maxdiv;
}

int getAreaTotal(int tamano) {
    int area = tamano * tamano;
    //cout <<"El area total es : " <<  area << endl;
    return area;
}

int getCuadrantes(int maxdiv) {
    int numCuadrantes = maxdiv * maxdiv;
    //cout << "Numero de cuadrantes: " << numCuadrantes << endl;
    return numCuadrantes;
}

void fillMatrix(int **matriz, int tamano, double f0 = 0, double f1=0, double f2=0) {
    srand(time(NULL));
    if(f0 == 0 && f1 == 0 && f2 == 0){
        for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                matriz[i][j] = rand() % 3;
            }
        }
    } else {
        for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                int r = rand() % 100;
                //cout <<"primer r: "<< r << endl;
                double dv = (r+0.0)/100; //dividir r por 100
                //cout <<"dv es: " << dv << endl;
                //usleep(3000000);

                if(0<=dv && dv<=f0){
                    //cout << "dv por primer if" << endl;
                    matriz[i][j] = 0;
                } else if(f0<dv && dv<=(f0+f1)){
                    //cout << "dv por segundo if" << endl;
                    matriz[i][j] = 1;
                } else if((f1+f0)<dv){
                    //cout << "v por tercer if" << endl;
                    matriz[i][j] = 2;
                }
            }
        }
    }


}


int main(int argc, char **argv) {
    int p = 0, myid, numprocs;
    //double startwtime, endwtime;
    int namelen, tamano, numCerosG, numUnosG, numDosG;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int stripSize;
    int **matriz;
    int **filaLocal, *data, *stripdata;
    MPI_Datatype strip;
    int numHectareas;

    /*  Se inicia el trabajo con MPI */
    MPI_Init(&argc, &argv);

    /*  MPI almacena en numprocs el numero total de procesos que se pusieron a correr */
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    /*  MPI almacena en myid la identificacion del proceso actual */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    /*  MPI almacena en processor_name el nombre de la computadora en la que corre el
    proceso actual, y en namelen la longitud de este */
    MPI_Get_processor_name(processor_name, &namelen);

    if (myid == 0) {
        cout << "Digite el numero de procesos" << endl;
        cin >> p;

        int maxdiv = getMaxDiv(p);
        tamano = p * maxdiv;

        stripSize = tamano / p;

        numHectareas = p*p;

        cout << "El area es: " << getAreaTotal(tamano) << endl;
        cout << "Numero de cuadrantes: " << getCuadrantes(maxdiv) << endl;
        cout << "Total de hectareas por cuadrante: " << numHectareas << endl;

        //genera la matriz
        data = (int *) malloc(sizeof(int) * tamano * tamano);
        matriz = (int **) malloc(sizeof(int *) * tamano);
        for (int i = 0; i < tamano; i++) {
            matriz[i] = &(data[i * tamano]);
        }

        cout << "Generando la matriz..." << endl;

        fillMatrix(matriz, tamano);
        numCerosG, numUnosG, numDosG = 0;

        cout << "La matriz es la siguiente:" << endl;
        for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                cout << matriz[i][j] << " ";
            }
            cout << endl;
        }
    }

    //broadcasting el tamaño de los lados y el tamaño de cada vector
    MPI_Bcast(&tamano, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&stripSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Type_vector(stripSize, tamano, tamano, MPI_INT, &strip);
    MPI_Type_commit(&strip);

    stripdata = (int *) malloc(sizeof(int) * stripSize * tamano);
    filaLocal = (int **) malloc(sizeof(int *) * stripSize);
    for (int i = 0; i < stripSize; i++) {
        filaLocal[i] = &(stripdata[i * tamano]);
    }
    MPI_Scatter(data, 1, strip, &(filaLocal[0][0]), 1, strip, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    /*for(int i = 0; i < stripSize; i++) {
        if(i == 0) {
            cout << "rank = " << myid << endl;
        }
        for(int j = 0; j < tamano; j++) {
            cout << filaLocal[i][j] << " ";
        }
        printf("\n");
    }*/

    if (myid == 0) {
        /*cout << "el del cero: " << endl;
        for(int i =0; i<stripSize; i++){
            for(int j = 0; j < tamano; j++) {
                cout << filaLocal[i][j] << " ";
            }
            cout << endl;
        }*/

        //mientras encuentro la formula de los cuadrantes
        for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                if (matriz[i][j] == 0) {
                    numCerosG++;
                } else if (matriz[i][j] == 1) {
                    numUnosG++;
                } else {
                    numDosG++;
                }
            }
        }

        int numHecTotales = numHectareas*getCuadrantes(getMaxDiv(p));

        cout << "La cantidad de ceros es: " << numCerosG << " , aprox " << (double)(100 * numCerosG)/numHecTotales << "%" << endl;
        cout << "La cantidad de unos es: " << numUnosG << " , aprox " << (double)(100 * numUnosG)/numHecTotales << "%" << endl;
        cout << "La cantidad de dos es: " << numDosG << " , aprox " <<  (double)(100 * numDosG)/numHecTotales << "%" << endl;

        if((double)(100 * numCerosG)/numHecTotales > 50){
            cout << "El costo por reforestar es muy alto. No se obtiene el beneficio" << endl;
        } else if((double)(100 * numDosG)/numHecTotales == 100){
            cout << "El costo por reforestar el bosque será gratuito." << endl;
        } else {
            cout << "Se obtiene el beneficio, pero no será gratuito." << endl;
        }

    }

    MPI_Finalize();
    return 0;
}