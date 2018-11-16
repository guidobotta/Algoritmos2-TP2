#include "hash.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAM 37
#define FACTOR_AUMENTO 0.671
#define FACTOR_DISMINUCION 0.2
#define VACIO 0
#define OCUPADO 1
#define BORRADO -1
#define AUMENTAR true
#define DISMINUIR false

/* ******************************************************************+
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct hash_campo{
    char *clave;
    void *valor;
    char estado;
}hash_campo_t;

struct hash{
    size_t cantidad;
    size_t borrados;
    size_t largo;
    float carga;
    hash_campo_t *tabla;
    hash_destruir_dato_t destruir_dato;
};

 /* ******************************************************************
  *                      FUNCIONES PRIVADAS
  * *****************************************************************/

//Declaro las funciones para evitar conflictos con las demas funciones

unsigned int hashing(const char *key, const hash_t* hash);
int calcular_posicion(hash_campo_t* tabla, int fact, int posicion, const char* clave, int* vacio, size_t largo);
size_t nuevo_largo(hash_t *hash, bool aumentar);
bool redimensionar_hash(hash_t *hash, bool aumentar);
void modificar_campo(hash_t* hash, void *dato, const char *clave, int posicion);

////
//FUNCION DE HASHING
////
//Funcion de hash K&R obtenida de internet.
size_t funcion_hash(unsigned char *s)
{
    size_t hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31*hashval;
    return hashval;
}

unsigned int hashing(const char *key, const hash_t* hash){
    return (unsigned int)(funcion_hash((unsigned char*)key) % hash->largo);
}

////
//FUNCION DE CALCULADO DE POSICION
////

// Devuelve -1 si no se encuentra la clave o la posicion de la clave.
// Asigna la posicion del primer borrado, si hay, a borrado.
// Asigna la posicion del vacio en caso de no estar el elemento.

int calcular_posicion(hash_campo_t* tabla, int fact, int posicion, const char* clave, int* vacio, size_t largo){
    
    if(!tabla[posicion].estado){ //Estado VACIO
        *vacio = posicion;
        return -1;
    }
    else if(tabla[posicion].clave){
        if(!strcmp(tabla[posicion].clave,clave)) return posicion;
    }
    fact++;

    return calcular_posicion(tabla, fact, (int)((posicion+(fact*fact))%largo), clave, vacio, largo);
}

// Redimensionar Hash

bool redimensionar_hash(hash_t *hash, bool aumentar){

    size_t tam_nuevo = 0;
    if(aumentar) tam_nuevo = (size_t)((double)hash->largo*(2.7)); //Multiplico el tamaño por 2.7
    else{
       tam_nuevo = hash->largo/4; //Divido entre 4
    }

    hash_campo_t* tabla_nueva = (hash_campo_t*) calloc(tam_nuevo,sizeof(hash_campo_t));
    size_t tope = hash->largo;
    if(!tabla_nueva) return false;

    hash_campo_t* tabla_vieja = hash->tabla;
    hash->tabla = tabla_nueva;
    hash->largo = tam_nuevo;
    hash->borrados = 0;

    for(int i=0; i<tope; i++){
        if(tabla_vieja[i].estado == OCUPADO){
            int indice = hashing(tabla_vieja[i].clave, hash);
            int vacio = -1;
            calcular_posicion(hash->tabla, 0, indice, tabla_vieja[i].clave, &vacio, hash->largo);
            modificar_campo(hash, tabla_vieja[i].valor, tabla_vieja[i].clave, vacio);
        }
    }

    hash->carga = ((float)(hash->cantidad))/(float)hash->largo;
    free(tabla_vieja);

    return true;
}

// Modificar campo

void modificar_campo(hash_t* hash, void *dato, const char *clave, int posicion){
    hash->tabla[posicion].clave = (char*)clave;
    hash->tabla[posicion].valor = dato;
    hash->tabla[posicion].estado = OCUPADO;
}

/* ******************************************************************
 *                      PRIMITIVAS DEL HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){

    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;

    hash_campo_t* tabla = (hash_campo_t*)calloc(TAM,sizeof(hash_campo_t));

    if (!tabla){
        free(hash);
        return NULL;
    }

    hash->cantidad = 0;
    hash->largo = TAM;
    hash->carga = 0;
    hash->borrados = 0;
    hash->tabla = tabla;
    hash->destruir_dato = destruir_dato;

    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){

    if(hash->carga >= FACTOR_AUMENTO){
        if(!redimensionar_hash(hash, AUMENTAR)) return false;//Le pasamos DISMINUIR si hay que achicar
    }

    int indice = hashing(clave, hash);
    int vacio = -1;
    int posicion = calcular_posicion(hash->tabla, 0, indice, clave, &vacio, hash->largo);

    if(posicion == -1){
        //ASIGNAR CLAVE EN VACIO
        char *clave_dinamica = malloc(sizeof(char)*strlen(clave)+1);
        modificar_campo(hash, dato, strcpy(clave_dinamica, clave), vacio);
    }
    else{
        if(hash->destruir_dato){
            hash->destruir_dato(hash->tabla[posicion].valor);
        }
        //free(hash->tabla[posicion].clave);
        hash->tabla[posicion].valor = dato;
        return true;
    } //PISAR CLAVE ANTERIOR

    hash->cantidad++;
    hash->carga = ((float)(hash->cantidad + hash->borrados))/(float)hash->largo;

    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){

    if(hash->carga <= FACTOR_DISMINUCION && hash->largo > TAM){
        if(!redimensionar_hash(hash, DISMINUIR)) return false;
    }
    
    int posicion = 0;
    int vacio = 0;
    int indice = hashing(clave, hash);

    if((posicion = calcular_posicion(hash->tabla, 0, indice, clave, &vacio, hash->largo)) == -1) return NULL;
    free(hash->tabla[posicion].clave);
    void* dato = hash->tabla[posicion].valor;

    hash->tabla[posicion].clave = NULL;
    hash->tabla[posicion].valor = NULL;
    hash->tabla[posicion].estado = BORRADO;
    
    hash->cantidad--;
    hash->borrados++;
    hash->carga = ((float)(hash->cantidad + hash->borrados))/(float)hash->largo;

    return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    
    int indice = hashing(clave, hash);
    int vacio = -1;
    int posicion = calcular_posicion(hash->tabla, 0, indice, clave, &vacio, hash->largo);

    if (posicion == -1) return NULL;
    return hash->tabla[posicion].valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){

    int indice = hashing(clave, hash);
    int vacio = -1;
    
    if(calcular_posicion(hash->tabla, 0, indice, clave, &vacio, hash->largo) != -1) return true;
    return false;
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
}

void hash_destruir(hash_t *hash){

    for(int i=0; i < hash->largo; i++){
        //Si el estado no es ocupado no hago nada
        if(hash->tabla[i].estado == OCUPADO){
            free(hash->tabla[i].clave);
            if(hash->destruir_dato) hash->destruir_dato(hash->tabla[i].valor);
        }
    }

    free(hash->tabla);
    free(hash);
}

/* ******************************************************************
 *                DEFINICION DEL STRUCT HASH
 * *****************************************************************/

 struct hash_iter{
    hash_t* hash;
    size_t posicion;
 };

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR HASH
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* hash_iter = malloc(sizeof(hash_iter_t));
    if (!hash_iter) return NULL;

    hash_iter->hash = (hash_t*)hash;
    hash_iter->posicion = 0;
    if(!hash_iter_ver_actual(hash_iter)) hash_iter_avanzar(hash_iter);

    return hash_iter;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return(iter->posicion >= iter->hash->largo); //Chequear
}

bool hash_iter_avanzar(hash_iter_t *iter){
    iter->posicion++;
    while(!hash_iter_al_final(iter)){
        if(iter->hash->tabla[iter->posicion].estado == OCUPADO) return true;
        //Devuelve true si encuentra un campo ocupado
        iter->posicion++;
    }
    //Devuelve false si llegó al final del hash
    return false;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if(hash_iter_al_final(iter)) return NULL;
    return iter->hash->tabla[iter->posicion].clave;
}

void hash_iter_destruir(hash_iter_t* iter){
    free(iter);
}
