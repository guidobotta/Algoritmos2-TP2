#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "lista.h"
#include "strutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define FORMATO_ARCHIVO "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n"
#define NUMERO_PARAMETROS 10

/*
    Para cada archivo nuevo:
    *Guardar en ABB con clave "date"
    *Guardar en hash con clave "flight_number"
    *Guardar en heap de maximos struct vuelo_heap
*/

//SON TODOS CHAR*, VEMOS MAS ADELANTE QUE TIPO DE DATO CONVIENE PARA CADA UNO
//REEMPLAZAR VUELO POR FLIGHT? PASAR TODO A INGLES O A ESPAÑOL? WTF CON "TODO" JAJA

typedef vuelo_resumido{         //Guardamos cada struct vuelo_heap en el heap y con el conseguimos la
    char* priority;         //clave para el hash
    char* flight_number;
    char* date;
}vuelo_resumen_t;

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

vuelo_resumen_t *resumir_vuelo(vuelo_t* vuelo){
    vuelo_resumen_t *vuelo_resumen = malloc(sizeof(vuelo_resumen_t));
    if(!vuelo_resumen) return NULL;

    vuelo_resumen->priority = vuelo->priority;
    vuelo_resumen->flight_number = vuelo->flight_number;
    vuelo_resumen->date = vuelo->date;

    return vuelo_resumen;
}

void destruir_vuelo(vuelo_t *vuelo){
    free(vuelo->flight_number);
    free(vuelo->airplane);
    free(vuelo->origin_airport);
    free(vuelo->destination_airport);
    free(vuelo->tail_number);
    free(vuelo->priority);
    free(vuelo->date);
    free(vuelo->cancelled);
    free(vuelo);
}

vuelo_t *vuelo_crear(char* linea){
    vuelo_t *vuelo = malloc(sizeof(vuelo_t));
    if(!vuelo) return NULL;
    //Pedir memoria para cada uno y luego asignar NULL o 0.

    char **cadenas = split(linea, ',');
    vuelo->flight_number = strdup(cadenas[0]);
    if(!vuelo->flight_number) return NULL;

    vuelo->airline = strdup(cadenas[0]);
    if(!vuelo->airplane){
        free(vuelo->flight_number);
        return NULL;
    }
    vuelo->origin_airport = strdup(cadenas[1]);
    if(!vuelo->origin_airport){
        free(vuelo->flight_number);
        free(vuelo->airplane);
        return NULL;
    }
    vuelo->destination_airport = strdup(cadenas[2]);
    if(!vuelo->destination_airport){
        free(vuelo->flight_number);
        free(vuelo->airplane);
        free(vuelo->origin_airport);
        return NULL;
    }
    vuelo->tail_number = strdup(cadenas[3]);
    if(!vuelo->tail_number){
        free(vuelo->flight_number);
        free(vuelo->airplane);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        return NULL;
    }
    vuelo->priority = strdup(cadenas[4]);
    if(!vuelo->priority){
        free(vuelo->flight_number);
        free(vuelo->airplane);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        return NULL;
    }
    vuelo->date = strdup(cadenas[5]);
    if(!vuelo->date){
        free(vuelo->flight_number);
        free(vuelo->airplane);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        free(vuelo->priority);
        return NULL;
    }
    vuelo->departure_delay = strdup(cadenas[6]);
    if(!vuelo->departure_delay){
        free(vuelo->flight_number);
        free(vuelo->airplane);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        free(vuelo->priority);
        free(vuelo->date);
        return NULL;
    }
    vuelo->air_time = strdup(cadenas[7]);
    if(!vuelo->air_time){
        free(vuelo->flight_number);
        free(vuelo->airplane);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        free(vuelo->priority);
        free(vuelo->date);
        free(vuelo->departure_delay);
        return NULL;
    }
    vuelo->cancelled = strdup(cadenas[8]);
    if(!vuelo->cancelled){
        free(vuelo->flight_number);
        free(vuelo->airplane);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        free(vuelo->priority);
        free(vuelo->date);
        free(vuelo->departure_delay);
        free(vuelo->air_time)
        return NULL;
    }

    return vuelo;
}

///
// AGREGAR ARCHIVO
///

bool agregar_archivo(char** comando, hash_t *hash, abb_t* abb){


    FILE* archivo = fopen(comando[1], "r");
    if(!archivo) return false;
    char *linea = NULL;
    size_t n = 0;
    while(getline(&linea, &n, archivo)){

        vuelo_t *vuelo = vuelo_crear(linea);
        if(!vuelo){
            fclose(archivo);
            return false;
        }
        vuelo_resumido_t *vuelo_resumen = resumir_vuelo(vuelo);
        if(!vuelo_resumen){
            destruir_vuelo();
            fclose(archivo);
            return false;
        }
        //Si falla alguna de estas operaciones, destruimos el abb y hash? Perderíamos los vuelos anteriores
        if(!abb_guardar(abb, vuelo->date, vuelo_resumen)){
            destruir_vuelo(vuelo);
            fclose(archivo);
            return false;
        }

        if(!hash_guardar(hash, vuelo->flight_number, vuelo)){
            destruir_vuelo(vuelo);
            fclose(archivo);
            return false;
        }
    }
    fclose(archivo);

    return true;
}

///
// VER TABLERO
///

void ver_tablero(char** comando);

///
// INFO VUELO
///

bool info_vuelo(char** comando, hash_t* hash){

    if(!comando[1]) return false;

    if(hash_pertenece(hash, comando[1])){
        vuelo_t* vuelo = hash_obtener(hash, comando[1]);
        printf("%s %s %s %s %s %s %s\n", vuelo->flight_number, vuelo->airline, vuelo->origin_airport, 
        vuelo->destination_airport, vuelo->tail_number, vuelo->priority, vuelo->date, vuelo->departure_delay,
        vuelo->air_time, vuelo->cancelled);
    }else return false;

    return true;
}

///
// PRIORIDAD VUELOS
///

int priority_comp(const vuelo_resumen_t* vuelo_1, const vuelo_resumen_t* vuelo_2){
    if (atoi(vuelo_1->priority) > atoi(vuelo_2->priority)) return -1;
    else if (atoi(vuelo_1->priority) < atoi(vuelo_2->priority)) return 1;

    else if (atoi(vuelo_1->flight_number) > atoi(vuelo_2->flight_number)) return -1;
    else if (atoi(vuelo_1->flight_number) < atoi(vuelo_2->flight_number)) return 1;

    return 0;
}

bool prioridad_vuelos(char** comando, hash_t* hash){

    if(!comando[1]) return false;

    heap_t* heap = heap_crear(priority_comp);
    //Debe ser un Heap de Mínimos, por lo que la funcion de comparacion debe estar al reves
    if (!heap) return false;

    int cantidad = atoi(comando[1]);
    int contador = 0;

    hash_iter_t* hash_iter = hash_iter_crear(hash);
    if (!hash_iter){
        heap_destruir(heap);
        return false;
    }

    lista_t* lista = lista_crear();
    if(!lista){
        hash_iter_destruir(hash_iter);
        heap_destruir(heap);
        return false; 
    }

    while (!hash_iter_al_final(hash_iter)){
        vuelo_resumen_t* vuelo_heap = resumir_vuelo((vuelo_t*)hash_iter_ver_actual(hash_iter));
        if (contador < cantidad){
            heap_encolar(heap, vuelo_heap);
            contador++;
        }
        else if (priority_comp(vuelo_heap, (vuelo_t*)hash_iter_ver_actual(hash_iter)) < 0){
            free(heap_desencolar(heap));
            heap_encolar(heap, vuelo_heap);
        }
        else free(vuelo_heap);
        hash_iter_avanzar(hash_iter);
    }

    while(!heap_esta_vacio(heap)){
        lista_insertar_primero(lista, heap_desencolar(heap));
    }

    while(!lista_esta_vacia(lista)){
        vuelo_resumen_t* vuelo_actual = (vuelo_resumen_t*)lista_borrar_primero(lista);
        printf("%s - %s\n", vuelo_actual->priority, vuelo_actual->flight_number);
    }

    hash_iter_destruir(hash_iter);
    heap_destruir(heap);
    lista_destruir(lista, NULL);

    return true;
}

///
// BORRAR
///

void borrar(char** comando);

///
// EJECUTADOR
///

void ejecucion(char* linea, hash_t* hash, abb_t* abb){
    char** comando = split(linea, ' ');
    if (!strcmp(comando[0], "agregar_archivo")){
        if(!agregar_archivo(comando, hash, abb)) fprintf(stderr, "Error en el comando %s\n", comando[0]); //EJECUTAR AGREGAR_ARCHIVO
    }
    else if (!strcmp(comando[0], "ver_tablero")){
        if(!ver_tablero(comando)) fprintf(stderr, "Error en el comando %s\n", comando[0]); //EJECUTAR VER_TABLERO
    }
    else if (!strcmp(comando[0], "info_vuelo")){
        if(!info_vuelo(comando, hash)) fprintf(stderr, "Error en el comando %s\n", comando[0]); //EJECUTAR INFO_VUELO
    }
    else if (!strcmp(comando[0], "prioridad_vuelos")){
        if(!prioridad_vuelos(comando, hash)) fprintf(stderr, "Error en el comando %s\n", comando[0]); //EJECUTAR PRIORIDAD_VUELOS
    }
    else if (!strcmp(comando[0], "borrar")){
        if(!borrar(comando)) fprintf(stderr, "Error en el comando %s\n", comando[0]); //EJECUTAR BORRAR
    }
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
        ejecucion(linea, hash, abb);
    }
    return 0;
}
