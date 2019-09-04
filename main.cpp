/*
 * Tarea programada 1
 * Curso: Arquitectura de computadoras
 * Estudiante: Jimmy Acuña Díaz (jimmy.acuna@ucr.ac.cr)
 * Carné: B50060
 */

#include "mpi/mpi.h"
#include <stdlib.h>
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
    return area;
}

int getCuadrantes(int maxdiv) {
    int numCuadrantes = maxdiv * maxdiv;
    return numCuadrantes;
}

void fillMatrix(int *matriz, int tamano, double f0 = 0, double f1 = 0, double f2 = 0) {
    srand(time(NULL));
    if (f0 == 0 && f1 == 0 && f2 == 0) {
        for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                matriz[i] = rand() % 3;
            }
        }
    } else {
        for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                int r = rand() % 100;
                double dv = (r + 0.0) / 100; //dividir r por 100
                if (0 <= dv && dv <= f0) {
                    matriz[i] = 0;
                } else if (f0 < dv && dv <= (f0 + f1)) {
                    matriz[i] = 1;
                } else if ((f1 + f0) < dv) {
                    matriz[i] = 2;
                }
            }
        }
    }
}

void printMatrix(int *matrix, int size, int column) {
    cout << "La matriz es la siguiente:" << endl;
    for (int i = 0; i < size; i++) {
        if (i % column == 0 && i != 0) {
            cout << endl;
        }
        cout << matrix[i] << " ";
    }
    cout << endl;
}

int main(int argc, char **argv) {
    int myid, numprocs;
    int namelen, tamano = 0, numCerosG = 0, numUnosG = 0, numDosG = 0;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int stripSize = 0;
    int **matriz;
    int **filaLocal, *data, *stripdata, *datalocal, *dataGlobal;
    MPI_Datatype strip;
    int numHectareas = 0;
    int **datosLocales, **datosGlobales;
    int maxdiv = 0;
    int *vector = 0;
    int *vectorLocal = 0;
    int *results = 0;
    int *resultadosParciales = 0;

    /*  Se inicia el trabajo con MPI */
    MPI_Init(&argc, &argv);

    /*  MPI almacena en numprocs el numero total de procesos que se pusieron a correr */
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    /*  MPI almacena en myid la identificacion del proceso actual */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    /*  MPI almacena en processor_name el nombre de la computadora en la que corre el
    proceso actual, y en namelen la longitud de este */
    MPI_Get_processor_name(processor_name, &namelen);

    maxdiv = getMaxDiv(numprocs);
    tamano = numprocs * maxdiv;
    stripSize = tamano * tamano / numprocs;

    numHectareas = numprocs * numprocs;

    if (myid == 0) {
        //vector = new int[tamano*tamano];
        vector = (int *) malloc((tamano * tamano) * (sizeof(int))); //reserva memoria para la matriz
        //inicializar matriz
        for (int i = 0; i < tamano * tamano; i++) {
            vector[i] = 0;
        }
        cout << "El area es: " << getAreaTotal(tamano) * 10000 << " mts" << endl;
        cout << "El area en numero de hectareas es: " << getAreaTotal(tamano) << endl;
        cout << "Numero de cuadrantes: " << getCuadrantes(maxdiv) << endl;
        cout << "Total de hectareas por cuadrante: " << numHectareas << endl;

        //genera la matriz
        /*data = (int *) malloc(sizeof(int) * tamano * tamano);
        matriz = (int **) malloc(sizeof(int *) * tamano);
        for (int i = 0; i < tamano; i++) {
            matriz[i] = &(data[i * tamano]);
        }*/


        cout << "Generando la matriz..." << endl;

        fillMatrix(vector, tamano * tamano);
        //numCerosG, numUnosG, numDosG = 0;

        printMatrix(vector, tamano * tamano, tamano);
        /*for (int i = 0; i < tamano; i++) {
            for (int j = 0; j < tamano; j++) {
                cout << matriz[i][j] << " ";
            }
            cout << endl;
        }*/

        //int numCuad = getCuadrantes(maxdiv);


        /*for(int i=0; i<numCuad; i++){
            for(int j = 0; j<3; j++){
                cout << datosLocales[i][j] << " ";
            }
            cout << endl;
        }*/
    }

    vectorLocal = (int *) malloc((tamano * maxdiv) * (sizeof(int)));
    for (int i = 0; i < tamano * maxdiv; i++) {
        vectorLocal[i] = 0;
    }

    int nc = getCuadrantes(maxdiv);
    resultadosParciales = (int *) malloc((nc * 3) * (sizeof(int)));
    for (int i = 0; i < nc * 3; i++) {
        resultadosParciales[i] = 0;
    }

    /*if(myid == 0){
        cout << "Resultados parciales " << endl;
        for(int i=0; i<nc*3; i++){
            if(i!=0 && i%3==0){
                cout << endl;
            }
            cout << resultadosParciales[i] << " ";
        }
        cout << endl;
    }*/

    results = (int *) malloc((nc * 3) * (sizeof(int)));
    for (int i = 0; i < nc * 3; i++) {
        results[i] = 0;
    }

    /*if(myid == 0){
        cout << "vector local" << endl;
        for(int i=0; i<nc*3; i++){
            if(i%3 == 0 && i != 0){
                cout << endl;
            }
            cout << vectorLocal[i] << " ";
        }
        cout << endl;
    }*/

    MPI_Scatter(vector, stripSize, MPI_INT, vectorLocal, stripSize, MPI_INT, 0, MPI_COMM_WORLD);

    /*if(myid == 0) {
        cout << "vector local " << endl;
        for (int i = 0; i < stripSize; i++) {
            if (i != 0 && i % 18 == 0) {
                cout << endl;
            }
            cout << vectorLocal[i] << " ";
        }
        cout << endl;
    }*/

    int pos = 0;
    int temp[maxdiv][tamano];

    for (int i = 0; i < maxdiv; i++) {
        for (int j = 0; j < tamano; j++) {
            temp[i][j] = vectorLocal[pos];
            pos++;
        }
    }

    /*if(myid == 5){
        cout << "temporal" << endl;
        for(int i=0; i< maxdiv; i++){
            for(int j=0; j<tamano; j++){
                cout << temp[i][j] << " ";
            }
            cout << endl;
        }
    }*/



    int c = myid * ((numprocs * maxdiv * numprocs * maxdiv) / numprocs) /
            (((numprocs * maxdiv * numprocs * maxdiv)) / maxdiv);
    c = c * maxdiv;
    /*if (myid == 5) {
        cout << "c: " << c  << endl;
    }*/
    // cout << "c: " << c << endl;
    int cp = 0;
    for (int i = 0; i < stripSize; i++) {
        cp = (i % tamano);
        cp = cp / numprocs;
        cp = (cp + c) * 3;
        /*if (myid == 5) {
            cout << "c: " << c << " cp: " << cp  << " y i: " << i << " y hay un: " << vectorLocal[i] << endl;
        }*/
        //cout << "cp : " << cp << endl;
        if (vectorLocal[i] == 0) {
            resultadosParciales[cp] += 1;
        }
        if (vectorLocal[i] == 1) {
            resultadosParciales[cp + 1] += 1;
        }
        if (vectorLocal[i] == 2) {
            resultadosParciales[cp + 2] += 1;
        }
    }

    MPI_Reduce(resultadosParciales, results, nc * 3, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myid == 0) {
        cout << "Resultados finales " << endl;
        for (int i = 0; i < nc * 3; i++) {
            if (i != 0 && i % 3 == 0) {
                cout << endl;
            }
            cout << results[i] << " ";
        }
        cout << endl;
    }

    /*if (myid == 5) {
        cout << "Resultados parciales " << endl;
        for (int i = 0; i < nc * 3; i++) {
            if (i != 0 && i % 3 == 0) {
                cout << endl;
            }
            cout << resultadosParciales[i] << " ";
        }
        cout << endl;
    }*/

    /*if(myid == 0){
        cout << "vector local 0 " << endl;
        for(int i=0; i<tamano*3; i++){
            if(i%tamano == 0 && i != 0){
                cout << endl;
            }
            cout << vectorLocal[i] << " ";
        }
        cout << endl;
    }*/
    /*if(myid == 1){
        cout << "vector local 1 " << endl;
        for(int i=0; i<tamano*3; i++){
            if(i%tamano == 0 && i != 0){
                cout << endl;
            }
            cout << vectorLocal[i] << " ";
        }
        cout << endl;
    }*/

    /*for(int i = 0; i < stripSize; i++) {
        if(i == 0) {
            cout << "rank = " << myid << endl;
        }
        for(int j = 0; j < tamano; j++) {
            cout << filaLocal[i][j] << " ";
        }
        printf("\n");
    }*/


    /*cout << "La cantidad de ceros es: " << numCerosG << " , aprox " << (double)(100 * numCerosG)/numHecTotales << "%" << endl;
    cout << "La cantidad de unos es: " << numUnosG << " , aprox " << (double)(100 * numUnosG)/numHecTotales << "%" << endl;
    cout << "La cantidad de dos es: " << numDosG << " , aprox " <<  (double)(100 * numDosG)/numHecTotales << "%" << endl;

    if((double)(100 * numCerosG)/numHecTotales > 50){
        cout << "El costo por reforestar es muy alto. No se obtiene el beneficio" << endl;
    } else if((double)(100 * numDosG)/numHecTotales == 100){
        cout << "El costo por reforestar el bosque será gratuito." << endl;
    } else {
        cout << "Se obtiene el beneficio, pero no será gratuito." << endl;
    }

}*/

    MPI_Finalize();
    return 0;
}