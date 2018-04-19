/* 4882701 */

#include "../include/cadena.h"
#include "../include/info.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>

struct nodo {
  info_t dato;
  nodo *anterior;
  nodo *siguiente;
};

struct rep_cadena {
  nodo *inicio;
  nodo *final;
};

/* Constructoras */

/*  Devuelve la cadena_t vacía (sin elementos). */
cadena_t crear_cadena() {
  cadena_t res = new rep_cadena;
  res->inicio = res->final = NULL;
  return res;
}

/*
  Se inserta `i' como último elemento de `cad'.
  Si es_vacia_cadena (cad) `i' se inserta como único elemento de `cad'.
 */
void insertar_al_final(info_t i, cadena_t &cad){
  nodo *nuevo = new nodo;
  nuevo->dato = i;
  nuevo->siguiente = NULL;
  nuevo->anterior = cad->final;
  if (cad->final == NULL) {
    assert(cad->inicio == NULL);
    cad->inicio = nuevo;
  } else {
    assert(cad->inicio != NULL);
    cad->final->siguiente = nuevo;
  }
  cad->final = nuevo;
}

/*
  Se inserta `i' como un nuevo elemento inmediatamente antes de `loc'.
  Precondición: localizador_en_cadena(loc, cad).
 */
void insertar_antes(info_t i, localizador_t loc, cadena_t &cad) {
  nodo *nuevo = new nodo;
  nuevo->dato = i;
  nuevo->anterior = anterior(loc,cad);
  nuevo->siguiente = loc;
  if(es_inicio_cadena(loc,cad)){
    cad->inicio = nuevo;
  } else if(!es_inicio_cadena(loc,cad)){
    loc->anterior->siguiente = nuevo;
  }
  loc->anterior = nuevo;
}

/*
  Se inserta la cadena_t `sgm' inmediatamente después de `loc' en `cad',
  manteniendo los elementos originales y el orden relativo entre ellos.
  No se debe obtener memoria (los nodos de `sgm' pasan a ser parte de `cad').
  Al terminar, `sgm' queda vacía.
  Si es_vacia_cadena(cad) `loc' es ignorado y el segmento queda insertado.
  Precondición: es_vacia_cadena(cad) o localizador_en_cadena(loc, cad).
 */
void insertar_segmento_despues(cadena_t &sgm, localizador_t loc, cadena_t &cad) {
  assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
  if (es_vacia_cadena(cad)) {
    cad->inicio = sgm->inicio;
    cad->final = sgm->final;
  } else {
    if (!es_vacia_cadena(sgm)) {
      sgm->inicio->anterior = loc;
      sgm->final->siguiente = loc->siguiente;
      if (es_final_cadena(loc, cad))
        cad->final = sgm->final;
      else
        loc->siguiente->anterior = sgm->final;
      loc->siguiente = sgm->inicio;
    }
  }
  sgm->inicio = sgm->final = NULL;
}

/*
  Devuelve una cadena_t con los elementos de `cad' que se encuentran entre
  `desde' y `hasta', incluidos.
  La cadena_t resultado no comparte memoria con `cad'.
  Si es_vacia_cadena(cad) devuelve la cadena_t vacia.
  Precondición: es_vacia_cadena(cad) o precede_en_cadena(desde, hasta, cad).
 */
cadena_t segmento_cadena(localizador_t desde, localizador_t hasta, cadena_t cad){
  assert(es_vacia_cadena(cad) || precede_en_cadena(desde, hasta, cad));
  cadena_t res = crear_cadena();
  if (!es_vacia_cadena(cad)) {
    localizador_t loc = desde;
    while (loc != siguiente(hasta, cad)) {
      info_t info = copia_info((loc->dato));
      insertar_al_final(info, res);
      loc = siguiente(loc, cad);
    }
  }
  return res;
}

/*
  Devuelve una cadena_t con los elementos de `cad' que se encuentran
  entre `desde' y `hasta', incluidos. Además, quita de `cad' dichos elementos,
  manteniendo el orden de los restantes.
  Si es_vacia_cadena(cad) devuelve la cadena_t vacía.
  No se debe obtener ni devolver memoria de nodos ni de elementos.
  La cadena_t resultado no comparte memoria con `cad'.
  Con `desde' y `hasta' se pasa a acceder al inicio y final respectivamente
  de la cadena_t resultado.
  Precondición: es_vacia_cadena(cad) o precede_en_cadena(desde, hasta, cad).
 */
cadena_t separar_segmento(localizador_t desde, localizador_t hasta, cadena_t &cad){
  cadena_t resultado = crear_cadena();
  if (!es_vacia_cadena(cad)){
    resultado->inicio = desde;
    resultado->final = hasta;
    if (es_inicio_cadena(desde,cad) && es_final_cadena(hasta,cad)){
      cad->inicio = NULL;
      cad->final = NULL;
    } else if (es_inicio_cadena(desde,cad)){
      cad->inicio = siguiente(hasta,cad);
      hasta->siguiente->anterior = NULL;
    } else if (es_final_cadena(hasta,cad)){
      cad->final = anterior(desde,cad);
      desde->anterior->siguiente = NULL;
    }else{
      desde->anterior->siguiente = hasta->siguiente;
      hasta->siguiente->anterior = desde->anterior;
    }
    desde->anterior = NULL;
    hasta->siguiente = NULL;
  }
  return resultado;
}

/*
  Devuelve una cadena_t con todos los elementos de `c1' y `c2' ordenada de
  manera
  creciente segun sus datos numéricos.
  Si hay elementos cuyos datos numéricos son iguales los elementos de `c1'
  quedan antes de los de `c2'.
  La cadena_t resultado no comparte memoria ni con `c1' ni con `c2'.
  Precondición: esta_ordenada (c1) y esta_ordenada (c2).
 */
cadena_t mezcla(cadena_t c1, cadena_t c2) {
  cadena_t resultado = crear_cadena();
  localizador_t loc1 = inicio_cadena(c1);
  localizador_t loc2 = inicio_cadena(c2);
  while (es_localizador(loc1) || es_localizador(loc2)){
    if(!es_localizador(loc2)) {
      insertar_al_final(copia_info(info_cadena(loc1,c1)),resultado);
      loc1 = siguiente(loc1,c1);
    }
    else if(!es_localizador(loc1)){
      insertar_al_final(copia_info(info_cadena(loc2,c2)),resultado);
      loc2 = siguiente(loc2,c2);
    }
    else {
      if (numero_info(info_cadena(loc1,c1)) <= numero_info(info_cadena(loc2,c2))){
        insertar_al_final(copia_info(info_cadena(loc1,c1)),resultado);
        loc1 = siguiente(loc1,c1);
      } else {
        insertar_al_final(copia_info(info_cadena(loc2,c2)),resultado);
        loc2 = siguiente(loc2,c2);
      }
    }
  }
  return resultado;
}

/* Destructoras */

/*
  Se quita el elemento referenciado en `loc' y se libera la memoria asignada
  al mismo y al nodo apuntado por el localizador.
  El valor de `loc' queda indeterminado.
  Precondición: localizador_en_cadena(loc, cad).
 */
void remover_de_cadena(localizador_t &loc, cadena_t &cad){
  if(!es_vacia_cadena(cad)){
    if(es_final_cadena(loc,cad) && es_inicio_cadena(loc,cad))
      cad->inicio = cad->final = NULL;
    else if(es_inicio_cadena(loc,cad)){
      cad->inicio = loc->siguiente;
      loc->siguiente->anterior = NULL;
    } else if (es_final_cadena(loc,cad)) {
      cad->final = loc->anterior;
      loc->anterior->siguiente = NULL;
    } else {
      loc->anterior->siguiente = loc->siguiente;
      loc->siguiente->anterior = loc->anterior;
    }
    liberar_info(loc->dato);
    delete loc;
  }
}

/*  Libera la memoria asignada a `cad' y la de todos sus elementos. */
void liberar_cadena(cadena_t &cad) {
  nodo *a_borrar;
  while (cad->inicio != NULL) {
    a_borrar = cad->inicio;
    cad->inicio = cad->inicio->siguiente;
    liberar_info(a_borrar->dato);
    delete a_borrar;
  }
  delete cad;
}

/* Predicados */

/*
  Devuelve `true' si y sólo si `loc' es un localizador_t válido.
  En cadenas enlazadas un localizador_t no es válido si es `NULL'.
*/
bool es_localizador(localizador_t loc){
  return loc != NULL;
}

/* Devuelve `true' si y sólo si `cad' es vacía (no tiene elementos). */
bool es_vacia_cadena(cadena_t cad) {
  return (cad->inicio == NULL) && (cad->final == NULL);
}

/*
  Devuelve `true' si y sólo si `cad' está ordenada de forma no decreciente
  (= creciente de manera no estricta) según los datos numéricos de sus
  elementos.
  Si es_vacia_cadena (cad) devuelve `true'.
 */
bool esta_ordenada(cadena_t cad) {
  bool res = true;
  if (!es_vacia_cadena(cad)) {
    localizador_t loc = inicio_cadena(cad);
    while (res && es_localizador(siguiente(loc, cad))) {
      localizador_t sig_loc = siguiente(loc, cad);
      if (numero_info(info_cadena(loc, cad)) > numero_info(info_cadena(sig_loc, cad)))
        res = false;
      else
        loc = siguiente(loc, cad);
    }
  }
  return res;
}

/*
  Devuelve `true' si y sólo si con `loc' se accede al último elemento de `cad'.
  Si es_vacia_cadena (cad) devuelve `false'.
 */
bool es_final_cadena(localizador_t loc, cadena_t cad) {
  if (es_vacia_cadena(cad))
    return false;
  else
    return loc == cad->final;
}

/*
  Devuelve `true' si y sólo si con `loc' se accede al primer elemento de `cad'.
  Si es_vacia_cadena (cad) devuelve `false'.
 */
bool es_inicio_cadena(localizador_t loc, cadena_t cad) {
  if (es_vacia_cadena(cad))
    return false;
  else
    return loc == cad->inicio;
}

/*
  Devuelve `true' si y sólo si con `loc' se accede a un elemento de `cad'.
  Si es_vacia_cadena (cad) devuelve `false'.
 */
bool localizador_en_cadena(localizador_t loc, cadena_t cad) {
  if(es_vacia_cadena(cad))
    return false;
  else {
    localizador_t indice = inicio_cadena(cad);
    while (es_localizador(indice) && indice != loc)
      indice = siguiente(indice,cad);
    return es_localizador(indice);
  }
}

/*
  Devuelve `true' si y sólo si `loc1' es igual o precede a `loc2` en `cad`.
  Si es_vacia_cadena (cad) devuelve `false'.
 */
bool precede_en_cadena(localizador_t loc1, localizador_t loc2, cadena_t cad) {
  localizador_t cursor = loc1;
  while (es_localizador(cursor) && (cursor != loc2))
    cursor = siguiente(cursor, cad);
  return ((cursor == loc2) && (localizador_en_cadena(loc1,cad)));
}

/* Posicionamiento */

/*
  Devuelve el localizador_t con el que se accede al inicio de `cad`.
  Si es_vacia_cadena(cad) devuelve un localizador_t no válido.
 */
localizador_t inicio_cadena(cadena_t cad){
  localizador_t res;
  if(es_vacia_cadena(cad))
    res = NULL;
  else{
    res = cad->inicio;
  }
  return res;
}

/*
  Devuelve el localizador_t con el que se accede al final de `cad'.
  Si es_vacia_cadena(cad) devuelve un localizador_t no válido.
 */
localizador_t final_cadena(cadena_t cad){
  localizador_t res;
  if(es_vacia_cadena(cad))
    res = NULL;
  else
    res = cad->final;
  return res;
}

/*
  Devuelve el localizador_t con el que se accede al k-esimo elemento de `cad'.
  Si `k' es 0 o mayor a la cantidad de elementos de `cad' devuelve un localizdor
  no válido.
 */
localizador_t kesimo(nat k, cadena_t cad){
  localizador_t resultado = NULL;
  if (es_vacia_cadena(cad) || k == 0)
    return resultado;
  else {
    nat i = 1;
    resultado = inicio_cadena(cad);
    while (es_localizador(resultado) && i<k) {
      i++;
      resultado = siguiente(resultado,cad);
    }
    if (resultado == NULL)
      return resultado;
    else {
      return resultado;
    }
  }
}

/*
  Devuelve el localizador_t con el que se accede al elemento de `cad'
  inmediatamente  siguiente a `loc'.
  Si es_final_cadena(loc, cad) devuelve un localizador_t no válido.
  Precondición: localizador_en_cadena(loc, cad).
 */
localizador_t siguiente(localizador_t loc, cadena_t cad) {
  localizador_t res;
  if (es_final_cadena(loc,cad))
    res = NULL;
  else if (!es_final_cadena(loc,cad))
    res = loc->siguiente;
  return res;
}

/*
  Devuelve el localizador_t con el que se accede al elemento de `cad'
  inmediatamente  anterior a `loc'.
  Si es_inicio_cadena(loc, cad) devuelve un localizador_t no válido.
  Precondición: localizador_en_cadena(loc, cad).
 */
localizador_t anterior(localizador_t loc, cadena_t cad){
  localizador_t res;
  if (es_inicio_cadena(loc,cad))
    res = NULL;
  else
    res = loc->anterior;
  return res;
}

/*
  Devuelve el localizador_t con el que se accede al elemento cuyo dato numérico
  es el menor en el segmento que va desde `loc' hasta el final_cadena(cad).
  Si hay más de un elemento cuyo valor es el menor el resultado accede al que
  precede a los otros.
  Precondición: localizador_en_cadena(loc, cad).
 */
localizador_t menor_en_cadena(localizador_t loc, cadena_t cad){
  assert(localizador_en_cadena(loc,cad));
  localizador_t res = loc;
  while (es_localizador(siguiente(loc, cad))) {
    loc = siguiente(loc, cad);
    if (numero_info(info_cadena(loc, cad)) < numero_info(info_cadena(res, cad)))
      res = loc;
  }
  return res;
}

/*
  Devuelve el primer localizador_t con el que se accede a un elemento cuyo dato
  numérico es igual a `clave', buscando desde `loc' (inclusive) hacia el final
  de `cad'.
  Si no se encuentra o `cad' es vacía devuelve un localizador_t no válido.
  Precondición: es_vacia_cadena(cad) o localizador_en_cadena(loc, cad).
 */
localizador_t siguiente_clave(int clave, localizador_t loc, cadena_t cad){
  assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
  localizador_t res = loc;
  if (es_vacia_cadena(cad))
    res = NULL;
  else {
    while (es_localizador(res) && numero_info(info_cadena(res, cad)) != clave)
      res = siguiente(res, cad);
  }
  return res;
}

/*
  Devuelve el primer localizador_t con el que se accede a un elemento cuyo dato
  numérico es igual a `clave', buscando desde `loc' (inclusive) hacia el inicio
  de `cad'.
  Si no se encuentra o `cad' es vacia devuelve un localizador_t no válido.
  Precondición: es_vacia_cadena(cad) o localizador_en_cadena(loc, cad).
 */
localizador_t anterior_clave(int clave, localizador_t loc, cadena_t cad){
  localizador_t res = loc;
  if(es_vacia_cadena(cad))
    res = NULL;
  else {
    while (es_localizador(res) && numero_info(info_cadena(res, cad)) != clave)
      res = anterior(res, cad);
  }
  return res;
}

/* Selectoras */

/*
  Devuelve el elemento de `cad' al que se accede con `loc'.
  Precondición: localizador_en_cadena(loc, cad).
 */
info_t info_cadena(localizador_t loc, cadena_t cad){
  return loc->dato;
}

/* Modificadoras */

/*
  Sustituye con `i' el elemento de `cad' al que se accede con `loc'.
  No destruye el elemento al que antes se accedía con `loc'.
  Precondición: localizador_en_cadena(loc, cad).
 */
void cambiar_en_cadena(info_t i, localizador_t loc, cadena_t &cad){
  assert(localizador_en_cadena(loc, cad));
  loc->dato = i;
}

/*
  Intercambia los elementos a los que se accede con `loc1' y `loc2'.
  `loc1' y `loc2' mantienen su relación de precedencia.
  Precondición:
    localizador_en_cadena (loc1, cad)
  y localizador_en_cadena (loc2, cad).
 */
void intercambiar(localizador_t loc1, localizador_t loc2, cadena_t &cad){
  info_t aux = loc1->dato;
  loc1->dato = loc2->dato;
  loc2->dato = aux;
}

/* Salida */
/*
  Imprime los elementos de `cad` de la siguiente forma:
  (dn de pos1,fr de pos1)(dn de pos2,fr de pos2) ...
  donce `dn` es el dato numérico y `fr` es la frase.
  Antes de terminar, se debe imprimir un fin de linea.
  Si es_vacia_cadena(cad) sólo se imprime el fin de línea.
 */
void imprimir_cadena(cadena_t cad){
  if(es_vacia_cadena(cad))
    printf("\n");
  else{
    localizador_t loc = inicio_cadena(cad);
    while(es_localizador(loc)){
      int numero = numero_info(info_cadena(loc,cad));
      char *frase = frase_info(info_cadena(loc,cad));
      printf("(%i,%s)", numero, frase);
      loc = siguiente(loc,cad);
    }
    printf("\n");
  }
}
