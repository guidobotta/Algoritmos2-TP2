#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "strutil.h"

#include <stdio.h>
#include <stdbool.h>

/*
    Para cada archivo nuevo:
    *Guardar en ABB con clave "date"
    *Guardar en hash con clave "flight_number"
    *Guardar en heap de maximos struct vuelo_heap
*/

//SON TODOS CHAR*, VEMOS MAS ADELANTE QUE TIPO DE DATO CONVIENE PARA CADA UNO
//REEMPLAZAR VUELO POR FLIGHT? PASAR TODO A INGLES O A ESPAÑOL? WTF CON "TODO" JAJA

typedef vuelo_heap{         //Guardamos cada struct vuelo_heap en el heap y con el conseguimos la
    char* priority;         //clave para el hash
    char* flight_number;
}vuelo_heap_t;

typedef struct vuelo{
    char* flight_number;
    char* airline;
    char* origin_airport;
    char* destination_airport;
    char* tail_number;
    char* priority;
    char* date;
    char* departure_delay;
    char* air_time;
    char* cancelled;
}vuelo_t;

vuelo_t *crear_vuelo(){
    vuelo_t *vuelo = malloc(sizeof(vuelo_t));
    if(!vuelo) return NULL;
    //Pedir memoria para cada uno y luego asignar NULL o 0.
    //Me parece mejor que asignemos los valores a cada campo fuera de esta funcion
    vuelo->flight_number;
    vuelo->airline;
    vuelo->origin_airport;
    vuelo->destination_airport;
    vuelo->tail_number;
    vuelo->priority;
    vuelo->date;
    vuelo->departure_delay;
    vuelo->air_time;
    vuelo->cancelled;
    return vuelo;
}

bool agregar_archivo(char* nombre_archivo){
    
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo) return false;
    
    fscanf(archivo,
        FORMATO_ARCHIVO, //<--- Es una constante que tiene un char* con el formato del .csv
        vuelo->flight_number,
        vuelo->airline
        vuelo->origin_airport
        vuelo->destination_airport
        vuelo->tail_number
        vuelo->priority
        vuelo->date
        vuelo->departure_delay
        vuelo->air_time
        vuelo->cancelled);

    vuelo_heap->priority = vuelo->priority;
    vuelo_heap->flight_number = vuelo->flight_number;
    //fscanf se encarga de meter cada valor del archivo en su correspondiente lugar.
    hash_guardar(vuelo->flight_number, vuelo);
    abb_guardar(vuelo->date, vuelo);
    heap_encolar(vuelo->date, vuelo_heap);
    //Parsear cada linea con getline o fscanf y meter dentro del struct
    fclose(archivo);
}

///
// EJECUTA LA OPERACIÓN
///
void ejecucion(char* linea){
    char** comando = split(linea, ' ');
    if (!strcmp(comando[0], "agregar_archivo")); //EJECUTAR AGREGAR_ARCHIVO
    else if (!strcmp(comando[0], "ver_tablero")); //EJECUTAR VER_TABLERO
    else if (!strcmp(comando[0], "info_vuelo")); //EJECUTAR INFO_VUELO
    else if (!strcmp(comando[0], "prioridad_vuelos")); //EJECUTAR PRIORIDAD_VUELOS
    else if (!strcmp(comando[0], "borrar")); //EJECUTAR BORRAR
}

/*
Ejecuta el main.c y se queda esperando input del usuario

#agregar_archivo <nombre_archivo>
Llama a la funcion agregar_archivo(nombre_archivo);
#ver_tablero <k cantidad vuelo> <modo: asc/desc> <desde> <hasta>
Llama a la funcion ver_tablero();
#info_vuelo <codigo vuelo>
Llama a la funcion info_vuelo();
#prioridad_vuelos <k cantidad vuelos>
Llama a la funcion prioridad_vuelos();
#borrar <desde> <hasta>
Llama a la funcion borrar();
*/
int main(){

    hash_t* hash = hash_crear(free);
    if(!hash){
        return 1;
    }

    hash_t* abb = abb_crear(free, funcion_comparacion);
    if(!abb){
        free(hash);
        return 1;
    }

    size_t tam = 0;
    char* linea = NULL;
    while (getline(&linea, &tam, stdin) != -1){
        ejecucion(linea);
    }

    return 0;
}
