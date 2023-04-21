# Laboratorio3_Paralela

# Instrucciones de compilación y ejecución de archivos en C
Este documento proporciona instrucciones para compilar y ejecutar tres archivos en C: `mpi_vector_add2.c` , `mpi_vector_add3.c` y `vector_add2.c.`

## Requisitos previos

Antes de continuar con las instrucciones de compilación y ejecución, es necesario asegurarse de que se cumplan los siguientes requisitos previos:

- Tener instalado un compilador de C (por ejemplo, gcc)
- Tener instalado MPI (Message Passing Interface)

## Compilación

A continuación se muestran las instrucciones para compilar cada uno de los archivos en C:

## vector_add2.c y mpi_vector_add2.c correspondientes al inciso C

Para compilar estos archivos, es necesario utilizar el compilador de MPI. Se puede utilizar el siguiente comando para compilar ambos archivos:

```sh
gcc -o vector_add vector_add2.c -lm

mpicc -o mpi_vector_add2 mpi_vector_add2.c
```

## mpi_vector_add3.c Correspondiente al inciso D
Para compilar este archivo, es necesario utilizar el compilador de MPI. Se puede utilizar el siguiente comando para compilar el archivo:

```sh
mpicc -o mpi_vector_add3 mpi_vector_add3.c
```

## Ejecución

A continuación se muestran las instrucciones para ejecutar cada uno de los archivos compilados anteriormente:

## vector_add2.c y mpi_vector_add2.c correspondientes al inciso C

Para ejecutar estos archivos, es necesario utilizar el comando "mpiexec" seguido del número de procesos que se desean ejecutar para cada archivo. El siguiente comando se puede utilizar para ejecutar ambos archivos:

```sh
mpiexec -np <numero_de_procesos> ./mpi_vector_add2
mpiexec -np <numero_de_procesos> ./mpi_vector_add3
```

