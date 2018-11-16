#define _POSIX_C_SOURCE 200809L //Sino falla strdup
#include "strutil.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char** split(const char* str, char sep){
    int contador = 2;
    for(int i=0; i < strlen(str); i++){ //Cuento apariciones de sep
        if (str[i] == sep) contador++;
    }
    
    char** vector_cadenas = calloc(contador, sizeof(char*));
    if (!vector_cadenas) return NULL;

    char* cadena = calloc(strlen(str)+1, sizeof(char));
    if (!cadena){
        free(vector_cadenas);
        return NULL;
    } 

    int contador_1 = 0;
    int contador_2 = 0;
    for(int i=0; i < strlen(str); i++){
        if (str[i] == sep){
            vector_cadenas[contador_1] = strdup(cadena);
            if (!vector_cadenas[contador_1]){
                free_strv(vector_cadenas);
                free(cadena);
            }
            contador_1++;
            cadena[0] = '\0'; //"Reseteo" cadena
            contador_2 = 0;
        }else {
            cadena[contador_2] = str[i];
            contador_2++;
            cadena[contador_2] = '\0';
        }
    }
    vector_cadenas[contador_1] = strdup(cadena);
    if (!vector_cadenas[contador_1]) free_strv(vector_cadenas);
    free(cadena);
    return vector_cadenas;
}

char* join(char** strv, char sep){
    int cantidad_elementos = 0;
    size_t longitud_cadena = 0;
    while(strv[cantidad_elementos] != NULL){
        longitud_cadena += strlen(strv[cantidad_elementos]) + 1;
        cantidad_elementos++;
    }

    char* cadena = calloc(longitud_cadena+1, sizeof(char));
    if (!cadena) return NULL;

    int posicion_original = 0;
    int posicion_nueva = 0;

    for (int i=0; i<cantidad_elementos; i++){
        while(strv[i][posicion_original] != '\0'){
            cadena[posicion_nueva] = strv[i][posicion_original];
            posicion_original++;
            posicion_nueva++;
        }
        cadena[posicion_nueva] = sep;
        posicion_nueva++;
        posicion_original = 0;
    }
    if (posicion_nueva > 0) cadena[posicion_nueva-1] = '\0';
    return cadena;
}

void free_strv(char* strv[]){
    int posicion = 0;
    while(strv[posicion] != NULL){
        free(strv[posicion]);
        posicion++;
    }
    free(strv);
}