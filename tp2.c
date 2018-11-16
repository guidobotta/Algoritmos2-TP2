#define _POSIX_C_SOURCE 200809L //Sino falla strdup
#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "lista.h"
#include "strutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
    Para cada archivo nuevo:
    *Guardar en ABB con clave "date"
    *Guardar en hash con clave "flight_number"
    *Guardar en heap de maximos struct vuelo_heap
*/

//SON TODOS CHAR*, VEMOS MAS ADELANTE QUE TIPO DE DATO CONVIENE PARA CADA UNO
//REEMPLAZAR VUELO POR FLIGHT? PASAR TODO A INGLES O A ESPAÑOL? WTF CON "TODO" JAJA

/* ******************************************************************
 *                     ESTRUCTURAS AUXILIARES
 * *****************************************************************/

//Estructura auxiliar para usar en Heap y Abb
typedef struct vuelo_resumido{
    char* priority;
    char* flight_number;
    char* date;
}vuelo_resumen_t;

//Estructura completa para usar en Hash
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

/* ******************************************************************
 *                     FUNCIONES AUXILIARES
 * *****************************************************************/

//Crea un vuelo completo
vuelo_t *vuelo_crear(char* linea){
    vuelo_t *vuelo = malloc(sizeof(vuelo_t));
    if(!vuelo) return NULL;

    char **cadenas = split(linea, ',');
    if (!cadenas) return NULL;

    vuelo->flight_number = strdup(cadenas[0]);
    if(!vuelo->flight_number) return NULL;

    vuelo->airline = strdup(cadenas[1]);
    if(!vuelo->airline){
        free(vuelo->flight_number);
        return NULL;
    }
    vuelo->origin_airport = strdup(cadenas[2]);
    if(!vuelo->origin_airport){
        free(vuelo->flight_number);
        free(vuelo->airline);
        return NULL;
    }
    vuelo->destination_airport = strdup(cadenas[3]);
    if(!vuelo->destination_airport){
        free(vuelo->flight_number);
        free(vuelo->airline);
        free(vuelo->origin_airport);
        return NULL;
    }
    vuelo->tail_number = strdup(cadenas[4]);
    if(!vuelo->tail_number){
        free(vuelo->flight_number);
        free(vuelo->airline);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        return NULL;
    }
    vuelo->priority = strdup(cadenas[5]);
    if(!vuelo->priority){
        free(vuelo->flight_number);
        free(vuelo->airline);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        return NULL;
    }
    vuelo->date = strdup(cadenas[6]);
    if(!vuelo->date){
        free(vuelo->flight_number);
        free(vuelo->airline);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        free(vuelo->priority);
        return NULL;
    }
    vuelo->departure_delay = strdup(cadenas[7]);
    if(!vuelo->departure_delay){
        free(vuelo->flight_number);
        free(vuelo->airline);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        free(vuelo->priority);
        free(vuelo->date);
        return NULL;
    }
    vuelo->air_time = strdup(cadenas[8]);
    if(!vuelo->air_time){
        free(vuelo->flight_number);
        free(vuelo->airline);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        free(vuelo->priority);
        free(vuelo->date);
        free(vuelo->departure_delay);
        return NULL;
    }
    vuelo->cancelled = strdup(cadenas[9]);
    if(!vuelo->cancelled){
        free(vuelo->flight_number);
        free(vuelo->airline);
        free(vuelo->origin_airport);
        free(vuelo->destination_airport);
        free(vuelo->tail_number);
        free(vuelo->priority);
        free(vuelo->date);
        free(vuelo->departure_delay);
        free(vuelo->air_time);
        return NULL;
    }

    free_strv(cadenas);
    return vuelo;
}

//Crea un vuelo resumido
vuelo_resumen_t *crear_vuelo_resumen(vuelo_t* vuelo){
    vuelo_resumen_t *vuelo_resumen = malloc(sizeof(vuelo_resumen_t));
    if(!vuelo_resumen) return NULL;

    vuelo_resumen->priority = vuelo->priority;
    vuelo_resumen->flight_number = vuelo->flight_number;
    vuelo_resumen->date = vuelo->date;

    return vuelo_resumen;
}

//Destruye el vuelo completo
void destruir_vuelo(void* _vuelo_){
    vuelo_t* vuelo = _vuelo_;
    free(vuelo->flight_number);
    free(vuelo->airline);
    free(vuelo->origin_airport);
    free(vuelo->destination_airport);
    free(vuelo->tail_number);
    free(vuelo->priority);
    free(vuelo->date);
    free(vuelo->departure_delay);
    free(vuelo->air_time);
    free(vuelo->cancelled);
    free(vuelo);
}

//Devuelve el mayor de los valores
int buscar_mayor(const char fecha1, const char fecha2){
    if(fecha1 > fecha2) return 1;
    return -1;
} // ¡¡REVISAR ESTA FUNCIÓN | NO LA VEO NECESARIA, SON CHAR LOS QUE RECIBE!!

//Comparación por date y flight_number (ver_tablero, borrar y main)
int date_comp(const char* fecha1, const char* fecha2){
    size_t tope = strlen(fecha1)+1;
    for(int i=0; i<tope; i++){
        if(fecha1[i] != fecha2[i]){
            return buscar_mayor(fecha1[i], fecha2[i]);
        }
    }
    return 0;
}

//Crea una lista de vuelos ordenados segun el modo y lo devuelve (ver_tablero y borrar)
lista_t* crear_lista_vuelos(abb_iter_t* iter, abb_t* abb, char* fecha_final, int cantidad_vuelos, char* modo){
    lista_t* lista_vuelos = lista_crear();
    if (!lista_vuelos) return NULL;

    vuelo_resumen_t* vuelo = NULL;
    bool ascendente = !strcmp(modo, "asc");
    int tope = 0;

    while( (!abb_iter_in_al_final(iter)) && ( (tope < cantidad_vuelos) || (cantidad_vuelos == -1)) ){
        vuelo = abb_obtener(abb, abb_iter_in_ver_actual(iter));
        if(date_comp(vuelo->date, fecha_final) > 0) break;
        if(ascendente) lista_insertar_ultimo(lista_vuelos, vuelo);
        else lista_insertar_primero(lista_vuelos, vuelo);
        tope++;
        abb_iter_in_avanzar(iter);
    }

    return lista_vuelos;
}

//Imprime la lista de vuelos de ver_tablero (ver_tablero)
void imprimir_lista_vuelos(lista_t* lista_vuelos){
    lista_iter_t* lista_iter = lista_iter_crear(lista_vuelos);
    if(!lista_iter) return;

    while(!lista_iter_al_final(lista_iter)){
        vuelo_resumen_t* vuelo_actual = (vuelo_resumen_t*)lista_iter_ver_actual(lista_iter);
        printf("%s - %s\n", vuelo_actual->date, vuelo_actual->flight_number);
        lista_iter_avanzar(lista_iter);
    }

    lista_iter_destruir(lista_iter);
}

//Imprime el vuelo completo (info_vuelo, borrar)
void imprimir_vuelo(vuelo_t* vuelo){
    printf("%s %s %s %s %s %s %s %s %s %s\n", vuelo->flight_number, vuelo->airline, vuelo->origin_airport,
    vuelo->destination_airport, vuelo->tail_number, vuelo->priority, vuelo->date, vuelo->departure_delay,
    vuelo->air_time, vuelo->cancelled);
}

//Comparación por priority y flight_number (prioridad_vuelo)
int priority_comp(const void* vuelo_1, const void* vuelo_2){
    if (atoi(((const vuelo_resumen_t*)vuelo_1)->priority) > atoi(((const vuelo_resumen_t*)vuelo_2)->priority)) return -1;
    else if (atoi(((const vuelo_resumen_t*)vuelo_1)->priority) < atoi(((const vuelo_resumen_t*)vuelo_2)->priority)) return 1;

    else if (atoi(((const vuelo_resumen_t*)vuelo_1)->flight_number) > atoi(((const vuelo_resumen_t*)vuelo_2)->flight_number)) return -1;
    else if (atoi(((const vuelo_resumen_t*)vuelo_1)->flight_number) < atoi(((const vuelo_resumen_t*)vuelo_2)->flight_number)) return 1;

    return 0;
}

//Imprime y luego borra a los elementos (borrar)
void borrar_e_imprimir_elementos(lista_t* lista_vuelos, abb_t* abb, hash_t* hash){
    lista_iter_t* lista_iter = lista_iter_crear(lista_vuelos);
    if(!lista_iter) return;

    vuelo_resumen_t* vuelo_resumen = NULL;
    vuelo_t* vuelo_completo = NULL;

    while(!lista_iter_al_final(lista_iter)){
        vuelo_resumen = lista_iter_ver_actual(lista_iter);
        vuelo_completo = hash_borrar(hash, vuelo_resumen->flight_number);
        abb_borrar(abb, vuelo_resumen->date);
        imprimir_vuelo(vuelo_completo);
        destruir_vuelo(vuelo_completo);
        //free(vuelo_resumen);
        lista_iter_avanzar(lista_iter);
    }
    lista_iter_destruir(lista_iter);
}

/* ******************************************************************
 *                    FUNCIONES PRINCIPALES
 * *****************************************************************/

///
// AGREGAR ARCHIVO
///

bool agregar_archivo(char** comando, hash_t *hash, abb_t* abb){
    //agregar_archivo <archivo>

    if(!comando[1]) return false;

    FILE* archivo = fopen(comando[1], "r");
    if(!archivo) return false;
    char *linea = NULL;
    size_t n = 0;
    int leidos = 0;
    while((leidos = (int)getline(&linea, &n, archivo)) !=-1){
        if (linea[leidos-1] == '\n') linea[leidos-1] = '\0';
        vuelo_t *vuelo = vuelo_crear(linea);
        if(!vuelo){
            fclose(archivo);
            return false;
        }
        vuelo_resumen_t *vuelo_resumen = crear_vuelo_resumen(vuelo);
        if(!vuelo_resumen){
            destruir_vuelo(vuelo);
            fclose(archivo);
            return false;
        }

        char* date_fnumber[3];
        date_fnumber[0] = vuelo->date;
        date_fnumber[1] = vuelo->flight_number;
        date_fnumber[2] = NULL;
        char* clave_abb = join(date_fnumber, '|'); //Hay que liberarlo
        if(!abb_guardar(abb, clave_abb, vuelo_resumen)){
            destruir_vuelo(vuelo);
            fclose(archivo);
            return false;
        }
        free(clave_abb);
        if(!hash_guardar(hash, vuelo->flight_number, vuelo)){
            destruir_vuelo(vuelo);
            fclose(archivo);
            return false;
        }
    }
    free(linea);
    fclose(archivo);
    return true;
}

///
// VER TABLERO
///

bool ver_tablero(char** comando, hash_t* hash, abb_t* abb){
    //ver_tablero <K cantidad vuelos> <modo: asc/desc> <desde> <hasta>

    if(!comando[1] || !comando[2] || !comando[3] || !comando[4]) return false;

    int cantidad_vuelos = atoi(comando[1]);
    if(cantidad_vuelos <= 0) return false;
    char* modo = comando[2];
    if(strcmp(modo, "asc") && strcmp(modo, "desc")){
        free_strv(comando);
        return false;
    }

    char* fecha_inicial = comando[3];
    char* fecha_final = comando[4];
    if(date_comp(fecha_inicial, fecha_final) > 0) return false;

    abb_iter_t* iter = abb_buscar_clave_e_iterar(abb, fecha_inicial);
    if(!iter){
        free_strv(comando);
        return false;
    }

    lista_t* lista_vuelos = crear_lista_vuelos(iter, abb, fecha_final, cantidad_vuelos, modo);
    if(!lista_vuelos){
        abb_iter_in_destruir(iter);
        return false;
    }
    imprimir_lista_vuelos(lista_vuelos);

    lista_destruir(lista_vuelos, NULL); //Chequear si no hay que pasarle destruir_vuelo
    abb_iter_in_destruir(iter);
    return true;
}

///
// INFO VUELO
///

bool info_vuelo(char** comando, hash_t* hash){
    //info_vuelo <numero de vuelo>

    if(!comando[1]) return false;

    if(hash_pertenece(hash, comando[1])){
        vuelo_t* vuelo = hash_obtener(hash, comando[1]);
        imprimir_vuelo(vuelo);
    }else return false;

    return true;
}

///
// PRIORIDAD VUELOS
///


bool prioridad_vuelos(char** comando, hash_t* hash){
    //prioridad_vuelos <K cantidad vuelos>

    if(!comando[1]) return false;

    heap_t* heap = heap_crear(priority_comp);
    //Debe ser un Heap de Mínimos, por lo que la funcion de comparacion debe estar al reves
    if (!heap) return false;

    int cantidad = atoi(comando[1]);
    int contador = 0;

    hash_iter_t* hash_iter = hash_iter_crear(hash);
    if (!hash_iter){
        heap_destruir(heap, NULL);
        return false;
    }

    lista_t* lista = lista_crear();
    if(!lista){
        hash_iter_destruir(hash_iter);
        heap_destruir(heap, NULL);
        return false;
    }

    while (!hash_iter_al_final(hash_iter)){
        const char* clave_vuelo = hash_iter_ver_actual(hash_iter);
        vuelo_resumen_t* vuelo_heap = crear_vuelo_resumen((vuelo_t*)hash_obtener(hash, clave_vuelo));
        if (contador < cantidad){
            heap_encolar(heap, vuelo_heap);
            contador++;
        }
        else if (priority_comp(vuelo_heap, (vuelo_resumen_t*)heap_ver_max(heap)) < 0){
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
        free(vuelo_actual);
    }

    hash_iter_destruir(hash_iter);
    heap_destruir(heap, NULL);
    lista_destruir(lista, NULL);

    return true;
}

///
// BORRAR
///

bool borrar(char** comando, hash_t* hash, abb_t* abb){
    //borrar <desde> <hasta>

    if(!comando[1] || !comando[2]) return false;

    char* fecha_inicial = comando[1];
    char* fecha_final = comando[2];
    if(date_comp(fecha_inicial, fecha_final) > 0) return false;

    abb_iter_t* iter = abb_buscar_clave_e_iterar(abb, fecha_inicial);
    if(!iter) return false;

    lista_t* lista_vuelos = crear_lista_vuelos(iter, abb, fecha_final, -1, "asc"); //HACER UN DEFINE CON ASCENDENTE Y DESCENDENTE
    if(!lista_vuelos){
        abb_iter_in_destruir(iter);
        return false;
    }
    borrar_e_imprimir_elementos(lista_vuelos, abb, hash);

    lista_destruir(lista_vuelos, NULL); //Chequear si no hay que pasarle destruir_vuelo
    abb_iter_in_destruir(iter);
    return true;

}

/* ******************************************************************
 *                     ESTRUCTURA PRINCIPAL
 * *****************************************************************/

///
// EJECUTADOR
///

void ejecucion(char* linea, hash_t* hash, abb_t* abb){
    char** comando = split(linea, ' ');
    bool exito = true;

    if (!strcmp(comando[0], "agregar_archivo")){
        exito = agregar_archivo(comando, hash, abb); //EJECUTAR AGREGAR_ARCHIVO
    }
    else if (!strcmp(comando[0], "ver_tablero")){
        exito = ver_tablero(comando, hash, abb); //EJECUTAR VER_TABLERO
    }
    else if (!strcmp(comando[0], "info_vuelo")){
        exito = info_vuelo(comando, hash); //EJECUTAR INFO_VUELO
    }
    else if (!strcmp(comando[0], "prioridad_vuelos")){
        exito = prioridad_vuelos(comando, hash); //EJECUTAR PRIORIDAD_VUELOS
    }
    else if (!strcmp(comando[0], "borrar")){
        exito = borrar(comando, hash, abb); //EJECUTAR BORRAR
    }
    else exito = false;

    if(!exito) fprintf(stderr, "Error en el comando %s\n", comando[0]);
    else printf("OK\n");

    free_strv(comando);
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

    hash_t* hash = hash_crear(destruir_vuelo);
    if(!hash){
        return 1;
    }

    abb_t* abb = abb_crear(date_comp, free);
    if(!abb){
        free(hash);
        return 1;
    }

    size_t tam = 0;
    char* linea = NULL;
    int leidos = 0;
    while ((leidos = (int)getline(&linea, &tam, stdin)) != -1){
        if (linea[leidos-1] == '\n') linea[leidos-1] = '\0';
        ejecucion(linea, hash, abb);
    }

    free(linea);
    hash_destruir(hash);
    abb_destruir(abb);
    return 0;
}

/*
FALTA CHEQUEAR QUE LOS COMANDOS QUE PASEN SEAN CORRECTOR
POR EJEMPLO QUE LOS NUMEROS SEAN NUMEROS ANTES DE CONVERTIRLOS
*/
