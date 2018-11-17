#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo{
    void* dato;
    struct nodo* proximo;
}nodo_t;

struct lista{
    nodo_t* primero;
    nodo_t* ultimo;
    size_t cantidad;
};

/* ******************************************************************
 *                    PRIMITIVAS DEL NODO
 * *****************************************************************/

// Crea un nodo.
// Post: devuelve el nodo con su valor asignado.
nodo_t* crear_nodo_lista(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->dato = valor;
    nodo->proximo = NULL;
    return nodo;
}

// Asigna nodo_prox como proximo al nodo. Puede recibir NULL.
// Pre: los nodos fueron creados.
// Post: el proximo al nodo es nodo_prox.
void asignar_proximo_nodo(nodo_t *nodo, nodo_t *nodo_prox){
    nodo->proximo = nodo_prox;
}

// Devuelve el siguiente nodo asignado
// Pre: el nodo fue creado.
// Post: se devolvio el proximo nodo.
nodo_t* obtener_proximo(nodo_t* nodo){
    return nodo->proximo;
}

// Devuelve el dato asignado al nodo.
// Pre: el nodo fue creado.
// Post: se devolvio el valor guardado en el nodo.
void* obtener_dato(nodo_t *nodo){
    return nodo->dato;
}

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));
    if (!lista) return NULL;

    lista->primero = NULL;
    lista->ultimo = NULL;
    lista->cantidad = 0;
    return lista;
}

size_t lista_largo(const lista_t *lista){
    return lista->cantidad;
}

bool lista_esta_vacia(const lista_t *lista){
    return (lista_largo(lista) == 0);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nodo = crear_nodo_lista(dato);
    if (!nodo) return false;

    if (lista_esta_vacia(lista)) lista->ultimo = nodo;
    else asignar_proximo_nodo(nodo, lista->primero);
    lista->primero = nodo;
    lista->cantidad += 1;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo = crear_nodo_lista(dato);
    if (!nodo) return false;

    if (lista_esta_vacia(lista)) lista->primero = nodo;
    else asignar_proximo_nodo(lista->ultimo, nodo);
    lista->ultimo = nodo;
    lista->cantidad += 1;
    return true;
}

void *lista_borrar_primero(lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;
    
    nodo_t* nodo_aux = obtener_proximo(lista->primero);
    void* dato = obtener_dato(lista->primero);
    free(lista->primero);
    lista->primero = nodo_aux;
    lista->cantidad -= 1;
    if (lista_esta_vacia(lista)) lista->ultimo = NULL;
    return dato;
}

void *lista_ver_primero(const lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;
    return obtener_dato(lista->primero);
}

void *lista_ver_ultimo(const lista_t* lista){
    if (lista_esta_vacia(lista)) return NULL;
    return obtener_dato(lista->ultimo);
}

void lista_destruir(lista_t *lista, void destruir_dato(void*)){
    while (!lista_esta_vacia(lista)){
        //Libero nodos
        void* dato = lista_borrar_primero(lista);
        //Aplico destruir_dato (si es distinto de NULL) a cada dato
        if (destruir_dato) destruir_dato(dato);
    }
    free(lista);
}

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    nodo_t* nodo_aux = lista->primero;
    while (nodo_aux && visitar(obtener_dato(nodo_aux), extra)){
        nodo_aux = obtener_proximo(nodo_aux);
    }
}

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

struct lista_iter{
    nodo_t* actual;
    nodo_t* anterior;
    lista_t* lista;
};

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if (!iter) return NULL;

    iter->lista = lista;
    iter->actual = lista->primero;
    iter->anterior = NULL;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if (lista_iter_al_final(iter)) return false;

    iter->anterior = iter->actual;
    iter->actual = obtener_proximo(iter->actual);
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    if (lista_iter_al_final(iter)) return NULL;
    return (obtener_dato(iter->actual));
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return (!iter->actual);
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    nodo_t* nodo_aux = crear_nodo_lista(dato);
    if (!nodo_aux) return false;

    if (!iter->anterior){
        iter->lista->primero = nodo_aux;
    }else asignar_proximo_nodo(iter->anterior, nodo_aux);
    if (!iter->actual){
        iter->lista->ultimo = nodo_aux;
    }else asignar_proximo_nodo(nodo_aux, iter->actual);
    iter->actual = nodo_aux;
    iter->lista->cantidad += 1;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
    if (lista_iter_al_final(iter)) return NULL;

    nodo_t* nodo_a_borrar = iter->actual;
    void* dato = obtener_dato(nodo_a_borrar);

    iter->actual = obtener_proximo(nodo_a_borrar);
    if (lista_iter_al_final(iter)) iter->lista->ultimo = iter->anterior;
    if (!iter->anterior) iter->lista->primero = iter->actual;
    else asignar_proximo_nodo(iter->anterior, iter->actual);
    iter->lista->cantidad -= 1;

    free(nodo_a_borrar);
    return dato;
}