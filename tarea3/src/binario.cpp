/*
  Módulo de definición de `binario_t'.

  Se definen los árboles binarios de búsqueda de elementos de tipo `info_t'.
  La propiedad de orden de los árboles es definida por el dato de texto de sus
  elementos.

  Laboratorio de Programación 2.
  InCo-FIng-UDELAR
 */

#include "../include/info.h"
#include "../include/cadena.h"
#include "../include/binario.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct rep_binario {
  info_t dato;
  rep_binario *izq;
  rep_binario *der;
};

/*
  Devuelve 1 si la frase de i es menor que la del info_t de nodo
  Devuelve 0 si son iguales
  Devuelve -1 si es menor
  Precondición: !es_vacio_binario
*/
int orden_elemento(info_t i, binario_t b){
  return strcmp(frase_info(b->dato),frase_info(i));
}

/* Constructoras */

/*  Devuelve un binario_t vacío (sin elementos). */
binario_t crear_binario() { return NULL; }

/*
  Inserta `i' en `b' respetando la propiedad de orden definida.
  Si en `b' ya hay un elemento cuyo dato de texto es frase_info(i) no se hace
  nada.
  Devuelve `true' si se insertó `i', o `false' en otro caso.
 */
bool insertar_en_binario(info_t i, binario_t &b) {
  if (es_vacio_binario(b)) {
    b->dato = i;
    b->izq = NULL;
    b->der = NULL;
    return true;
  } else {
    if (orden_elemento(i,b) < 0)
      return insertar_en_binario(i,b->izq);
    else if (orden_elemento(i,b) > 0)
      return insertar_en_binario(i,b->der);
    else
      return false;
  }
}

/* Destructoras */

/*
  Devuelve el elemento mayor (según la propiedad de orden definida) de `b'.
  Remueve el nodo en el que se localiza ese elemento (libera la memoria
  asignada al nodo, pero no la del elemento).
  Precondición: ! es_vacio_binario(b).
 */
info_t remover_mayor(binario_t &b) {
  info_t res;
  if (b->der == NULL) {
    res = b->dato;
    binario_t izq = b->izq;
    delete (b);
    b = izq;
  } else {
    res = remover_mayor(b->der);
  }
  return res;
}

/*
  Remueve de `b' el nodo en el que el dato de texto de su elemento es `t'.
  Si en ningún nodo se cumple esa condición no se hace nada.
  Devuelve `true' si se removió el nodo, o `false' en otro caso.
  Si los dos subárboles del nodo a remover son no vacíos, en sustitución del
  elemento removido debe quedar el que es el mayor (segun la propiedad de orden
  definida) en el subárbol izquierdo.
  Libera la memoria del nodo y del elemento.
 */
bool remover_de_binario(const char *t, binario_t &b);

/* Libera la memoria asociada a `b' y todos sus elementos. */
void liberar_binario(binario_t &b);

/* Predicados */

nat max(int i, int j){
  if (i<=j)
    return j;
  else
    return i;
}

/* Devuelve `true' si y sólo si `b' es vacío (no tiene elementos). */
bool es_vacio_binario(binario_t b) {
  return b == NULL;
}

/*
  Devuelve `true' si y sólo si cada nodo de `b' cumple la condición de balanceo
  AVL. El árbol vacío cumple la condición.
  Un nodo cumple la condición de balanceo AVL si el valor absoluto de la
  diferencia de las alturas de sus subárboles izquierdo y derecho en menor o
  igual a 1.
  Cada nodo se puede visitar una sola vez. */
bool es_AVL(binario_t b);

/* Selectoras */

/*
  Devuelve el elemento asociado a la raíz de `b'.
  Precondición: ! es_vacio_binario(b).
 */
info_t raiz(binario_t b) {
  return b->dato;
}

/*
  Devuelve el subárbol izquierdo de  `b'.
  Precondición: ! es_vacio_binario(b).
 */
binario_t izquierdo(binario_t b) {
  return b->izq;
}

/*
  Devuelve el subárbol derecho de  `b'.
  Precondición: ! es_vacio_binario(b).
 */
binario_t derecho(binario_t b) {
  return b->der;
}

/*
  Devuelve el subárbol que tiene como raíz al nodo con el elemento cuyo dato de
  texto es `t'.
  Si `t' no pertenece a `b', devuelve el árbol vacío.
 */
binario_t buscar_subarbol(const char *t, binario_t b);

/*
  Devuelve la altura de `b'.
  La altura de un árbol vacío es 0.
 */
nat altura_binario(binario_t b) {
  if (es_vacio_binario(b))
    return 0;
  else
    return 1 + max(altura_binario(izquierdo(b)),altura_binario(derecho(b)));
}

/*  Devuelve la cantidad de elementos de `b'. */
nat cantidad_binario(binario_t b);

/*
  Devuelve el elemento que, según la propiedad de orden de los árboles
  `binario_t', está en el k-ésimo lugar de `b'.
  No se deben crear estructuras auxiliares.
  No se deben visitar nuevos nodos después que se encontró el kesimo.
  Precondición: 1 <= k <= cantidad_binario(b).
 */
info_t kesimo_en_binario(nat k, binario_t b);

/*
  Devuelve una cadena_t con los elementos de `b' en orden lexicográfico
  creciente
  según sus datos de texto.
  La cadena_t devuelta no comparte memoria con `b'.
 */
cadena_t linealizacion(binario_t b);

/*
  Devuelve un árbol con copias de los elementos de `b' que cumplen la condición
  "numero_info(elemento) < clave".
  La estructura del árbol resultado debe ser análoga a la de `b'. Esto
  significa que dados dos nodos `U' y `V' de `b' en los que se cumple la
  condición y tal que `U' es ancestro de `V', en el árbol resultado la copia de
  `U' debe ser ancestro de la copia de `V' cuando sea posible. Esto no siempre
  se puede lograr y al mismo tiempo mantener la propiedad de orden del árbol
  en el caso en el que en un nodo `V' no se cumple la condición y en sus dos
  subárboles hay nodos en los que se cumple. En este caso, la copia del nodo
  cuyo elemento es el mayor (según la propiedad de orden definida) de los que
  cumplen la condición en el subárbol izquierdo de `V' deberá ser ancestro de
  las copias de todos los descendientes de `V' que cumplen la condición.
  (Ver ejemplos en LetraTarea3.pdf).
  El árbol resultado no comparte memoria con `b'. *)
 */
binario_t filtrado(int clave, binario_t b);

/* Salida */

/*
  Imprime los elementos de `b', uno por línea, en orden descendente de la
  propiedad de orden de los árboles `binario_t'.
  Antes del elemento imprime una cantidad de guiones igual a su nivel.
  El elemento se imprime según la especificación dada en `info_a_texto'.
  El nivel de la raíz es 0.
  Antes de terminar, se debe imprimir un fin de linea.
  Si es_vacio_binario(b) sólo se imprime el fin de línea.
 */
void imprimir_binario(binario_t b);
