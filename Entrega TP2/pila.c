#include "pila.h"
#include <stdlib.h>

#define TAM_INICIAL 20
#define LIMITE_REDUCCION 4
#define FACTOR_AUMENTO 20
#define FACTOR_REDUCCION 5

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));
    if(!pila) return NULL;

    pila->datos = malloc(sizeof(void*)*TAM_INICIAL);
    if(!pila->datos){
        free(pila);
        return NULL;
    }
    pila->capacidad = TAM_INICIAL;
    pila->cantidad = 0;
    return pila;
}

bool pila_redimensionar(pila_t *pila, int factor){
    void* auxiliar = realloc(pila->datos, sizeof(void*)*pila->capacidad*factor/10);
    if(!auxiliar) return false;
    pila->capacidad = pila->capacidad*factor/10; //Si hago pila->capacidad *= factor/10 se rompe con factor = 5
    pila->datos = auxiliar;
    return true;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}
bool pila_esta_vacia(const pila_t *pila){
    return !pila->cantidad;
}
bool pila_apilar(pila_t *pila, void* valor){
    if(pila->cantidad == pila->capacidad-1){
        if(!pila_redimensionar(pila, FACTOR_AUMENTO)) return NULL;
    }
    pila->cantidad++;
    pila->datos[pila->cantidad] = valor;
    return true;
}
void* pila_ver_tope(const pila_t *pila){
    if(pila_esta_vacia(pila)) return NULL;

    return pila->datos[pila->cantidad];
}
void* pila_desapilar(pila_t *pila){

    if(pila_esta_vacia(pila)) return NULL;

    void* resultado = pila->datos[pila->cantidad];
    pila->cantidad--;
    if((pila->cantidad <= (pila->capacidad/LIMITE_REDUCCION))&&(pila->capacidad > TAM_INICIAL)){
        pila_redimensionar(pila, FACTOR_REDUCCION);
    }
    return resultado;
}
