# TRABAJO PRACTICO 2

## Sistema de Consulta de Vuelos

---

### **Funciones**

- **Agregar_archivo:**

El sistema que actualmente tiene algueiza genera continuamente archivos en formato .csv que contienen la información de cada vuelo. Nuestro sistema deberá procesar estos archivos y responder a las consultas que los operarios necesiten realizar. El sistema tiene que ser capaz de recibir en cualquier momento un nuevo archivo conteniendo el detalle de vuelos (nuevos o viejos) y actualizar sus datos.

El comando se acompaña del nombre de un archivo csv, accesible desde el mismo directorio donde se ejecuta el programa.

**Ejemplo:** agregar_archivo vuelos-algueiza-parte-01.csv

**Salida esperada:**
Si el comando se ejecuta correctamente deberá imprimir *OK* por salida estándar después de ser ejecutado. Si se produce un error al procesar el archivo se imprime *Error en comando \<comando>* por *stderr* y se continua la ejecución.

Al ejecutarse, el programa deberá leer el archivo, parsear cada línea sabiendo que tiene el formato csv y guardar cada línea en la/las estructuras auxiliares correspondientes para ser capaz de ejecutar los otros comandos. En caso que dos líneas tengan el mismo código de vuelo, el sistema se deberá quedar solamente con la información de la última aparición.
Este comando se puede ejecutar más de una vez durante el ciclo de uso de nuestro programa, y por lo tanto puede recibir nuevos archivos. Si un nuevo archivo procesado contiene un código de vuelo que ya se encuentra en nuestro sistema, nuestro programa deberá quedarse solamente con la información de la última aparición.

- **Ver_tablero:**
  
El primer requerimiento que mencionaron los operarios fue que nuestro sistema permita ordenar los registros de despegues por fecha. Esto debe realizarse de forma ascendente o descendente según el criterio del usuario. Adicionalmente nos mencionaron que no están interesados en visualizar todo el listado, por lo que también es necesario que puedan ingresar un intervalo de fechas de despegue (min / max) y una cantidad de vuelos a listar.

**El comando recibe varios parámetros:**

1. K cantidad vuelos: Numero entero mayor a 0, que indica la cantidad K de vuelos a mostrar.
2. Modo: asc/desc : cadena con “asc” o “desc” como únicos posibles valores, indican el orden a elegir utilizando el campo fecha de despegue.
3. Desde: cadena en formato YYYY-MM-DDTHH:MM:SS que indica el tiempo desde que se tienen que mostrar los vuelos, los vuelos con una fecha de despegue anteriores al tiempo ingresado no se tienen que mostrar.
4. Hasta: cadena en formato YYYY-MM-DDTHH:MM:SS que indica el tiempo hasta que se tienen que mostrar los vuelos, los vuelos con una fecha de despegue posteriores al tiempo ingresado no se tienen que mostrar.

**Ejemplo:** ver_tablero 5 asc 2018-10-10T00:01:00 2018-10-19T00:12:00

**Salida esperada:**

    2018-10-10T08:51:32 - 1234
    2018-10-11T10:20:12 - 11123
    2018-10-11T10:20:22 - 9113
    2018-10-12T17:20:12 - 991223
    2018-10-18T21:32:59 - 98123
    OK

Al ejecutarse deberán imprimir por salida estandar las fechas de despegue y los códigos de los vuelos que cumplan con la lógica anteriomente mencionada, cada vuelo se imprime en una nueva línea. El formato de cada línea tiene que ser YYYY-MM-DDTHH:MM:SS - \<código de vuelo>. En el caso que dos vuelos tengan la misma fecha de despegue, se deberá mostrar los vuelos comparando por número de vuelo (tomado como cadena).

**Casos en los que el sistema tiene que devolver error (con el formato anteriormente mencionado):**

1. Recibir un K igual o menor a 0
2. Recibir un modo que no sea asc o desc
3. Recibir una fecha hasta que sea anterior a la fecha desde

- **Info_vuelo:**

Existen ocasiones en las cuales un operario necesita ver información detallada de un vuelo. Por este motivo es necesario que ingresando el código de vuelo, el sistema devuelva toda la información posible asociada al mismo.

Este comando debe mostrar toda la informacion del vuelo cuyo código de vuelo coincida con el que fue pasado por parámetro, el formato que tiene que mostrar de la información es exactamente igual a la línea leída del archivo original, salvo que en vez de comas se tienen espacios.

**Ejemplo:** info_vuelo 4608

**Salida esperada:**

    4608 OO PDX SEA N812SK 08 2018-04-10T23:22:55 05 43 0
    OK

En caso de ingresar un código de vuelo que no exista dentro de los códigos procesados, se deberá devolver un mensaje de error con el formato anteriormente mencionado.

- **Prioridad_vuelos:**

Algueiza tiene un sistema de prioridades sobre los vuelos que necesitan atención especial, los operarios necesitan saber cuales son esos vuelos. Nuestro sistema deberá ser capaz de devolver los K vuelos que tienen prioridad más importante.

Este comando deberá mostrar de mayor a menor, los K vuelos con mayor prioridad que hayan sido cargados en el sistema, de la forma \<prioridad> - \<código de vuelo>. Si dos vuelos tienen la misma prioridad, se desempatará por el código de vuelo mostrándolos de menor a mayor.

**Ejemplo:** prioridad_vuelos 3

**Salida esperada:**

    10 - 1234
    3 - 324
    3 - 325
    OK

- **Borrar:**

Finalmente los operarios nos mencionaron que la información que maneja el sistema pierde validez con el paso del tiempo. Esta información carece de importancia y debe ser eliminada del sistema.

Con este fin se debe incorporar la posibilidad de eliminar información de vuelos que estén en un rango de fechas de despegue (min / max).

**El comando recibe dos argumentos:**

1. Desde: cadena en formato YYYY-MM-DDTHH:MM:SS
2. Hasta: cadena en formato YYYY-MM-DDTHH:MM:SS

**Ejemplo:** borrar 2018-10-04T02:01:01 2018-11-01T03:00:00

**Salida esperada:**

    4701 EV EWR CMH N11150 12 2018-10-04T04:19:24 -10 55 0
    2807 MQ SGF DFW N604MQ 00 2018-10-05T13:57:14 00 107 0
    OK

Al ejecutarse, todos los vuelos cuya fecha de despegue que sean mayores a < desde > y menores a < hasta > tienen que ser borrados del sistema y mostrados por salida estandar. En caso de que se reciba una fecha < hasta > que sea menor a < dede >, se deberá devolver un mensaje de error con el formato anteriormente mencionado.

---

### **Tiempos de ejecución**

1. **Agregar_archivo:** El mantenimiento para actualizar los vuelos debe ser O( V log(n) ) siendo V la cantidad de vuelos en el nuevo archivo y n la cantidad total de vuelos en el sistema.
2. **Ver_tablero:** debe ser O(v) en el peor caso (en el que se tenga que mostrar todos los vuelos del sistema), O(logv) en un caso promedio (en el caso en el que no se pidan mostrar demasiados visitantes). V es la cantidad de vuelos.
3. **Info_vuelo:** debe ser O(1).
4. Prioridad_vuelos:** debe ser O( n log(K) ) siendo K la cantidad de vuelos a mostrar y n la cantidad de vuelos en el sistema.
5. **Borrar:** debe ser O( K log (n) ) siendo K la cantidad de vuelos que hay en el rango de fechas ingresado y n la cantidad de vuelos en todo el sistema.

---

### **Consideraciones adicionales**

- **Archivos entrantes:**

Cada línea de los archivos entrantes .csv tienen la siguiente estructura:

    FLIGHT_NUMBER: 4608
    AIRLINE: OO
    ORIGIN_AIRPORT: PDX
    DESTINATION_AIRPORT: SEA
    TAIL_NUMBER: N812SK
    PRIORITY: 08
    DATE: 2018-04-10T23:22:55
    DEPARTURE_DELAY: 05
    AIR_TIME: 43
    CANCELLED: 0

Cada dato de una línea de log está separado por una coma.

**Ejemplo:**

4608,OO,PDX,SEA,N812SK,08,2018-04-10T23:22:55,05,43,0

---

### **Criterios de aprobación**

El código entregado debe ser claro y legible y ajustarse a las especificaciones de la consigna. Debe compilar sin advertencias y correr sin errores de memoria.

La entrega incluye, obligatoriamente, los siguientes archivos de código:

+ El código de la solución del TP.
+ El código de los TDAs programados en la cursada que se requieran, con las modificaciones que sean necesarias.
+ Un archivo deps.mk con las dependencias del proyecto en formato make. Este deberá contener sólamente una línea que indique qué objetos necesita para compilar el ejecutable algueiza, por ejemplo:
        
        algueiza: tp2.o hash.o abb.o heap.o strutil.o

El corrector automático va a interpretar ese archivo de dependencias y va a compilar todos los .o especificados a partir de los .h y .c que deberán enviar, con los siguientes flags de GCC:

    -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion
