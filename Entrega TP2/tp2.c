#define _POSIX_C_SOURCE 200809L //Sino falla strdup
#include "heap.h"
#include "hash.h"
#include "abb.h"
#include "lista.h"
#include "strutil.h"
#include <ctype.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define ASCENDENTE "asc"
#define DESCENDENTE "desc"


/*
    Para cada archivo nuevo:
    *Guardar en ABB con clave "date|flight_number"
    *Guardar en hash con clave "flight_number"
*/


/* ******************************************************************
 *                     ESTRUCTURAS AUXILIARES
 * *****************************************************************/

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
}

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
lista_t* crear_lista_vuelos(abb_iter_t* iter, abb_t* abb, char* fecha_final, char* modo){
    lista_t* lista_vuelos = lista_crear();
    if (!lista_vuelos) return NULL;

    vuelo_t* vuelo = NULL;
    bool ascendente = !strcmp(modo, ASCENDENTE);

    while(!abb_iter_in_al_final(iter)){
        vuelo = abb_obtener(abb, abb_iter_in_ver_actual(iter));
        if(date_comp(vuelo->date, fecha_final) > 0) break;
        if(ascendente) lista_insertar_ultimo(lista_vuelos, vuelo);
        else lista_insertar_primero(lista_vuelos, vuelo);
        abb_iter_in_avanzar(iter);
    }

    return lista_vuelos;
}

//Imprime la lista de vuelos de ver_tablero (ver_tablero)
void imprimir_lista_vuelos(lista_t* lista_vuelos, int cantidad_vuelos){
    lista_iter_t* lista_iter = lista_iter_crear(lista_vuelos);
    if(!lista_iter) return;
    int tope = 0;
    while(!lista_iter_al_final(lista_iter) && ((tope < cantidad_vuelos) || (cantidad_vuelos == -1))){
        vuelo_t* vuelo_actual = (vuelo_t*)lista_iter_ver_actual(lista_iter);
        printf("%s - %s\n", vuelo_actual->date, vuelo_actual->flight_number);
        tope++;
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
    const vuelo_t* vuelo1 = vuelo_1;
    const vuelo_t* vuelo2 = vuelo_2;
    if (atoi(vuelo1->priority) > atoi(vuelo2->priority)) return -1;
    else if (atoi(vuelo1->priority) < atoi(vuelo2->priority)) return 1;
    else if (strcmp(vuelo1->flight_number, vuelo2->flight_number) < 0) return -1;
    else if (strcmp(vuelo1->flight_number, vuelo2->flight_number) > 0) return 1;

    return 0;
}

//Imprime y luego borra a los elementos (borrar)
void borrar_e_imprimir_elementos(lista_t* lista_vuelos, abb_t* abb, hash_t* hash){
    lista_iter_t* lista_iter = lista_iter_crear(lista_vuelos);
    if(!lista_iter) return;

    vuelo_t* vuelo = NULL;

    while(!lista_iter_al_final(lista_iter)){
        vuelo = lista_iter_ver_actual(lista_iter);
        imprimir_vuelo(vuelo);

        hash_borrar(hash, vuelo->flight_number);

        char* date_fnumber[3];
        date_fnumber[0] = vuelo->date;
        date_fnumber[1] = vuelo->flight_number;
        date_fnumber[2] = NULL;
        char* clave_abb = join(date_fnumber, '|');
        abb_borrar(abb, clave_abb);

        destruir_vuelo(vuelo);
        lista_iter_avanzar(lista_iter);
        free(clave_abb);
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

    if(!comando[1] || comando[2]) return false;

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

        if(hash_pertenece(hash, vuelo->flight_number)){
            char* _date_fnumber[3];
            _date_fnumber[0] = ((vuelo_t*)hash_obtener(hash, vuelo->flight_number))->date;
            _date_fnumber[1] = vuelo->flight_number;
            //El flight_number es el mismo, pero el date puede cambiar
            _date_fnumber[2] = NULL;
            char* _clave_abb = join(_date_fnumber, '|');
            abb_borrar(abb, _clave_abb);
            free(_clave_abb);
        }

        char* date_fnumber[3];
        date_fnumber[0] = vuelo->date;
        date_fnumber[1] = vuelo->flight_number;
        date_fnumber[2] = NULL;
        char* clave_abb = join(date_fnumber, '|');

        if(!abb_guardar(abb, clave_abb, vuelo)){
            destruir_vuelo(vuelo);
            fclose(archivo);
            return false;
        }

        if(!hash_guardar(hash, vuelo->flight_number, vuelo)){
            destruir_vuelo(vuelo);
            fclose(archivo);
            return false;
        }

        free(clave_abb);
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

    if(!comando[1] || !comando[2] || !comando[3] ||
    !comando[4] || comando[5] || !isdigit(comando[1][0])) return false;

    int cantidad_vuelos = atoi(comando[1]);
    if(cantidad_vuelos <= 0) return false;
    char* modo = comando[2];
    if(strcmp(modo, ASCENDENTE) && strcmp(modo, DESCENDENTE)){
        free_strv(comando);
        return false;
    }

    char* fecha_inicial = comando[3];
    char* fecha_final = comando[4];
    if(date_comp(fecha_inicial, fecha_final) > 0) return false;

    abb_iter_t* iter = abb_buscar_clave_e_iterar(abb, fecha_inicial);
    //Aca le pasamos fecha_inicial en vez de la clave correcta, pero funciona
    if(!iter){
        free_strv(comando);
        return false;
    }

    lista_t* lista_vuelos = crear_lista_vuelos(iter, abb, fecha_final, modo);
    if(!lista_vuelos){
        abb_iter_in_destruir(iter);
        return false;
    }
    imprimir_lista_vuelos(lista_vuelos, cantidad_vuelos);

    lista_destruir(lista_vuelos, NULL);
    abb_iter_in_destruir(iter);
    return true;
}

///
// INFO VUELO
///

bool info_vuelo(char** comando, hash_t* hash){
    //info_vuelo <numero de vuelo>

    if(!comando[1] || comando[2]) return false;

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

    if(!comando[1] || comando[2] || !isdigit(comando[1][0])) return false;
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
        vuelo_t* vuelo_heap = hash_obtener(hash, clave_vuelo);
        if (contador < cantidad){
            heap_encolar(heap, vuelo_heap);
            contador++;
        }
        else if (priority_comp(vuelo_heap, (vuelo_t*)heap_ver_max(heap)) < 0){
            heap_desencolar(heap);
            heap_encolar(heap, vuelo_heap);
        }
        hash_iter_avanzar(hash_iter);
    }

    while(!heap_esta_vacio(heap)){
        lista_insertar_primero(lista, heap_desencolar(heap));
    }

    while(!lista_esta_vacia(lista)){
        vuelo_t* vuelo_actual = lista_borrar_primero(lista);
        printf("%s - %s\n", vuelo_actual->priority, vuelo_actual->flight_number);
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

    if(!comando[1] || !comando[2] || comando[3]) return false;

    char* fecha_inicial = comando[1];
    char* fecha_final = comando[2];
    if(date_comp(fecha_inicial, fecha_final) > 0) return false;

    abb_iter_t* iter = abb_buscar_clave_e_iterar(abb, fecha_inicial);
    //Aca le pasamos fecha_inicial en vez de la clave correcta, pero funciona
    if(!iter) return false;

    lista_t* lista_vuelos = crear_lista_vuelos(iter, abb, fecha_final, ASCENDENTE);
    if(!lista_vuelos){
        abb_iter_in_destruir(iter);
        return false;
    }
    borrar_e_imprimir_elementos(lista_vuelos, abb, hash);

    lista_destruir(lista_vuelos, NULL);
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

    if(!exito) fprintf(stderr, "Error en comando %s\n", comando[0]);
    else printf("OK\n");

    free_strv(comando);
}


int main(){

    hash_t* hash = hash_crear(destruir_vuelo);
    if(!hash){
        return 1;
    }

    abb_t* abb = abb_crear(date_comp, NULL);
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
