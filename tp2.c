



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
    //QUIZAS CONVIENE SEPARAR TODO ESTO EN DISTINTAS FUNCIONES, HEAP, ABB, HASH.
    FILE* archivo = fopen(nombre_archivo, "r");
    if(!archivo) return false;
    vuelo_t *vuelo = crear_vuelo();
    if(!vuelo){
        fclose(archivo);
        return false;
    }
    heap_t *heap = heap_crear(funcion_comparacion);
    if(!heap){
        fclose(archivo);
        free(vuelo);
        return false;
    }
    hash_t* hash = hash_crear(free);
    if(!hash){
        fclose(archivo);
        free(vuelo);
        free(heap);
        return false;
    }
    hash_t* abb = abb_crear(free, funcion_comparacion);
    if(!abb){
        fclose(archivo);
        free(vuelo);
        free(heap);
        free(hash);
        return false;
    }
    vuelo_heap_t *vuelo_heap = malloc(sizeof(vuelo_heap_t));
    if(!vuelo_heap){
        fclose(archivo);
        free(vuelo);
        free(heap);
        free(abb);
        free(hash);
        return false;
    }
    fscanf(FORMATO_ARCHIVO, //<--- Es una constante que tiene un char* con el formato del .csv
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

int main(){
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
}
