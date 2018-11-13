#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct lista lista_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve la cantidad de elementos de la lista
// Pre: la lista fue creada
size_t lista_largo(const lista_t *lista);

// Devuelve true si la lista está vacía, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento en la lista y lo ubica en el primer lugar.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista que se encuentra en
// el primer lugar.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento en la lista y lo ubica en el ultimo lugar.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista que se encuentra en
// el último lugar.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Elimina el primer elemento de la lista y devuelve su valor. En caso
// de que la lista este vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: Se devolvió el valor del primer elemento de la lista o NULL.
// La lista contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Si la lista tiene elementos, devuelve el valor del primer elemento.
// Si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: Se devolvió el valor del primer elemento de la lista o NULL,
// si estaba vacía.
void *lista_ver_primero(const lista_t *lista);

// Si la lista tiene elementos, devuelve el valor del último elemento.
// Si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento de la lista o NULL,
// si estaba vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Destruye la lista. Recibe una función destruir_dato por parámetro,
// que se le aplica a cada uno de los elementos de la lista. Si
// destruir_dato es NULL, no hace nada.
// Pre: la lista fue creada.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void*));

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Itera la lista y ejecuta la función visitar con el dato correspondiente
// a cada iteración. La iteración se ejecutara hasta que la función visitar
// devuelva NULL o hasta que llegue al final de la lista.
// Pre: la lista fue creada. La función visitar fue creada.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

typedef struct lista_iter lista_iter_t;

// Crea el iterador externo de la lista.
// Pre: la lista fue creada.
// Post: devuelve el iterador externo.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posición con el iterador.
// Pre: la lista y el iterador fueron creados
bool lista_iter_avanzar(lista_iter_t *iter);

// Si la lista tiene elementos, y el iterador no se encuentra al final,
// devuelve el valor del elemento que se encuentra en la posición del iterador.
// Pre: la lista y el iterador fueron creados.
// Post: se devolvió el valor del elemento que se encontraba en la posición del
// iterador o NULL.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si el iterador se encuentra al final de la lista o false
// en otro caso.
// Pre: la lista y el iterador fueron creados.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: la lista y el iterador fueron creados.
// Post: el iterador fue destruído.
void lista_iter_destruir(lista_iter_t *iter);

// Agrega un nuevo elemento en la lista en la posición que se encuentra eliterador.
// Pre: la lista y el iterador fueron creados.
// Post: Se agregó un nuevo elemento a la lista en la posición donde se encontraba
// el iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el elemento en el que se encuentra el iterador.
// Pre: la lista y el iterador fueron creados.
// Post: en caso de que el iterador estuviera en una posición válida, se eliminó el
// elemento donde se encontraba y devolvió su valor. Si el iterador se encuentra al
// final de la lista o la lista está vacía devuelve NULL.
void *lista_iter_borrar(lista_iter_t *iter);