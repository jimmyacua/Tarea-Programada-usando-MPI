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
                if (0 < dv && dv <= f0) {
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
    int namelen, tamano = 0;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int stripSize = 0;
    int numHectareas = 0;
    int maxdiv = 0;
    int *vector = 0;
    int *vectorLocal = 0;
    int *results = 0;
    int *resultadosParciales = 0;
    double f0 = 0, f1 = 0, f2 = 0;
    int numHectareasAReforestar = 0;

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
        vector = (int *) malloc((tamano * tamano) * (sizeof(int))); //reserva memoria para la matriz
        //inicializar matriz
        for (int i = 0; i < tamano * tamano; i++) {
            vector[i] = 0;
        }

        string s = "";
        cout << "¿Desea ingresar una distribución para generar la matriz (S/N)?" << endl;
        cin >> s;
        if (s != "s" && s != "S") {
            fillMatrix(vector, tamano * tamano);
        } else {
            bool continuar = false;
            while (!continuar) {
                cout << "Ingrese el primer valor entre 0 y 1. ADVERTENCIA: los tres valores deben sumar 1." << endl;
                cin >> f0;
                cout << "Ingrese el segundo valor entre 0 y 1. ADVERTENCIA: los tres valores deben sumar 1." << endl;
                cin >> f1;
                cout << "Ingrese el tercer valor entre 0 y 1. ADVERTENCIA: los tres valores deben sumar 1." << endl;
                cin >> f2;
                if (f0 + f1 + f2 == 1.0) {
                    continuar = true;
                } else {
                    cout << "Los tres valores deben sumar 1. Intente otra vez." << endl;
                }
            }
            fillMatrix(vector, tamano * tamano, f0, f1, f2);
        }

        cout << "Generando la matriz..." << endl;
        printMatrix(vector, tamano * tamano, tamano);

        cout << "El area es: " << numHectareas * 10000 << " mts" << endl;
        cout << "El area en numero de hectareas es: " << getAreaTotal(tamano) << endl;
        cout << "Numero de cuadrantes: " << getCuadrantes(maxdiv) << endl;
        cout << "Total de hectareas por cuadrante: " << numHectareas << endl;
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


    results = (int *) malloc((nc * 3) * (sizeof(int)));
    for (int i = 0; i < nc * 3; i++) {
        results[i] = 0;
    }

    MPI_Scatter(vector, stripSize, MPI_INT, vectorLocal, stripSize, MPI_INT, 0, MPI_COMM_WORLD);

    int pos = 0;
    int temp[maxdiv][tamano];

    for (int i = 0; i < maxdiv; i++) {
        for (int j = 0; j < tamano; j++) {
            temp[i][j] = vectorLocal[pos];
            pos++;
        }
    }

    int c = myid * ((numprocs * maxdiv * numprocs * maxdiv) / numprocs) /
            (((numprocs * maxdiv * numprocs * maxdiv)) / maxdiv);
    c = c * maxdiv;

    int cp = 0;
    for (int i = 0; i < stripSize; i++) {
        cp = (i % tamano);
        cp = cp / numprocs;
        cp = (cp + c) * 3;

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
        int i = 0;
        int j = 0;
        while (i < nc && j < stripSize) {
            int n0 = 0, n1 = 0, n2 = 0;
            n0 = results[j];
            n1 = results[j + 1];
            n2 = results[j + 2];
            cout << "Para el cuadrante numero " << i << " hay " << (n0 * 100) / numHectareas
                 << "% de area que representa una gran inversión para reforestar, tiene "
                 << (n1 * 100) / numHectareas
                 << "% de area con un costo alto y " << (n2 * 100) / numHectareas
                 << "% de area con un costo razonable. ";
            if ((n0 * 100) / numHectareas < 20 && (n2 * 100) / numHectareas >= 50) {
                cout << "Este cuadrante se puede reforestar.\n" << endl;
            } else {
                cout << "Este cuadrante no se puede reforestar.\n" << endl;
            }

            i++;
            j += 3;
        }
        for(int r=0; r<tamano*tamano; r++){
            if(vector[r] == 2){ //hectarea que se puede reforestar a un costo razonable
                numHectareasAReforestar +=1;
            }
        }

        double porcHectRef = (100*numHectareasAReforestar)/getAreaTotal(tamano);
        cout << "Numero de hectareas que se pueden reforestar: " << numHectareasAReforestar <<
        ", lo que representa el " << porcHectRef << "% de las hectareas totales" << endl;
        if(porcHectRef == 100){
            cout << "Por lo tanto, el costo de la reforestacion es 0." << endl;
        } else if(porcHectRef >= 50){
            cout << "Por lo tanto, se obtiene un beneficio en el costo." << endl;
        } else {
            cout << "Por lo tanto, no se obtiene el beneficio." << endl;
        }
    }

    MPI_Finalize();
    return 0;
}