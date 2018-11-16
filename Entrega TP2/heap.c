#include "heap.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define TAM 30
#define AUMENTAR true
#define DISMINUIR false

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct heap{
	void** tabla;
	size_t cantidad;
	size_t capacidad;
	cmp_func_t cmp;
};

 /* ******************************************************************
  *                      PRIMITIVAS PRIVADAS
  * *****************************************************************/

bool heap_redimensionar(heap_t *heap, bool aumentar){
	size_t nuevo_tam = 0;
	if(aumentar){
		nuevo_tam = heap->capacidad*2;
	}else{
		nuevo_tam = heap->capacidad/2;
	}
	void** aux = realloc(heap->tabla, sizeof(void*)*nuevo_tam);
	if(!aux) return false;
	heap->tabla = aux;
	heap->capacidad = nuevo_tam;
	return true;
}

void swap(void** elem_1, void** elem_2){
	void* aux = *elem_1;
	*elem_1 = *elem_2;
	*elem_2 = aux;
}

void up_heap(heap_t *heap, int pos){
	if(!pos) return;
	int padre = (pos-1)/2;
	if((padre < 0) || (heap->cmp(heap->tabla[pos], heap->tabla[padre])<0)) return;

	swap(&heap->tabla[pos], &heap->tabla[padre]);
	up_heap(heap, padre);
}

void down_heap(heap_t* heap, int pos){
	int hijo_izq = (2*pos) + 1;
	int hijo_der = (2*pos) + 2;

	if((hijo_der < heap->cantidad) && (heap->cmp(heap->tabla[pos], heap->tabla[hijo_der]) < 0)){
		if(heap->cmp(heap->tabla[hijo_izq], heap->tabla[hijo_der]) > 0){
			swap(&heap->tabla[pos], &heap->tabla[hijo_izq]);
			down_heap(heap, hijo_izq);
		}
		else{
			swap(&heap->tabla[pos], &heap->tabla[hijo_der]);
			down_heap(heap, hijo_der);
		}
	}
	else if ((hijo_izq < heap->cantidad) && (heap->cmp(heap->tabla[pos], heap->tabla[hijo_izq]) < 0)){
		swap(&heap->tabla[pos], &heap->tabla[hijo_izq]);
		down_heap(heap, hijo_izq);
	}

	return;
}

 /* ******************************************************************
  *                      PRIMITIVAS DEL HEAP
  * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp){
	if(!cmp) return NULL;

	heap_t * heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;

	void** tabla = malloc(sizeof(void*)*TAM);
	if(!tabla){
		free(heap);
		return NULL;
	}

	heap->tabla = tabla;
	heap->cmp = cmp;
	heap->cantidad = 0;
	heap->capacidad = TAM;

	return heap;
}

// Heapify

heap_t *heapify(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;

	heap->tabla = arreglo;
	heap->cmp = cmp;
	heap->cantidad = n;
	heap->capacidad = n+1;

	for(int i=(int)n-1; i >= 0; i--){
		down_heap(heap, i); //Aplico desde las hojas (ultimas pos del vector) hasta la raiz (primera pos)
	}
	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){

	void** tabla = malloc(sizeof(void*)*n);
	if(!tabla) return NULL;

	for(int i=0; i<n; i++){
		tabla[i] = arreglo[i];
	}

	heap_t* heap = heapify(tabla, n, cmp);
	return heap;
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	return !heap->cantidad;
}

bool heap_encolar(heap_t *heap, void *elem){
	if(heap->cantidad >= heap->capacidad-1){
		if(!heap_redimensionar(heap, AUMENTAR)) return false;
	}
	heap->tabla[heap->cantidad] = elem;

	up_heap(heap,(int)heap->cantidad);
	heap->cantidad++;

	return true;
}

void *heap_ver_max(const heap_t *heap){
	if (heap_esta_vacio(heap)) return NULL;
	return heap->tabla[0];
}

void *heap_desencolar(heap_t *heap){

	if(heap_esta_vacio(heap)) return NULL;
	void* dato = heap->tabla[0];
	swap(&heap->tabla[0], &heap->tabla[heap->cantidad-1]);

	heap->cantidad--;
	if(heap->cantidad <= heap->capacidad/4){
		if(!heap_redimensionar(heap, DISMINUIR)) return NULL;
	}
	down_heap(heap, 0);

	return dato;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){

	if(destruir_elemento){
		for(size_t i=0; i<heap->cantidad; i++){
			destruir_elemento(heap->tabla[i]);
		}
	}

	free(heap->tabla);
	free(heap);
}

///
// Heap Sort
///
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heap_t *heap = heapify(elementos, cant, cmp);
	if(!heap) return;
	for(int i=(int)cant-1; i>=0; i--){
		swap(&heap->tabla[0], &heap->tabla[i]);
		heap->cantidad--;
		down_heap(heap, 0);
	}
	free(heap);
}
