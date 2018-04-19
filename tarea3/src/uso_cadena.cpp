/* 4882701 */

#include "../include/cadena.h"
#include "../include/info.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h> // strlen, strcpy, strcmp, strcat

/*
  Devuelve `true' si y sólo si en `cad' hay un elemento cuyo dato numérico es
  `i'.
 */
bool pertenece(int i, cadena_t cad) {
  if (es_vacia_cadena(cad))
    return false;
  else {
    localizador_t navegador = inicio_cadena(cad);
    return siguiente_clave(i,navegador,cad) != NULL;
  }
}

/*  Devuelve la cantidad de elementos de `cad'. */
nat longitud(cadena_t cad){
  localizador_t loc = inicio_cadena(cad);
  nat cantidad = 0;
  while (es_localizador(loc)){
    cantidad++;
    loc = siguiente(loc,cad);
  }
  return cantidad;
}

/*
  Devuelve `true' si y sólo si `c1' y `c2' son iguales (es decir, si los
  elementos son iguales y en el mismo orden).
  Si es_vacia_cadenat(c1) y es_vacia_cadena(c2) devuelve `true'.
 */
bool son_iguales(cadena_t c1, cadena_t c2){
  if(es_vacia_cadena(c1) && es_vacia_cadena(c2))
    return true;
  else{
    if(es_vacia_cadena(c1) || es_vacia_cadena(c2))
      return false;
    else {
      localizador_t cadena1 = inicio_cadena(c1);
      localizador_t cadena2 = inicio_cadena(c2);
      while (cadena1 != NULL && cadena2 != NULL && son_iguales(info_cadena(cadena1,c1),info_cadena(cadena2,c2))){
        cadena1 = siguiente(cadena1,c1);
        cadena2 = siguiente(cadena2,c2);
      }
      return cadena1 == NULL && cadena2 == NULL;
    }
  }
}

/*
  Devuelve el resultado de concatenar `c2' después de `c1'.
  La cadena_t resultado no comparte memoria ni con `c1' ni con `c2'.
 */
cadena_t concatenar(cadena_t c1, cadena_t c2) {
  cadena_t resultado = crear_cadena();
  if(es_vacia_cadena(c1) && es_vacia_cadena(c2))
    return resultado;
  else{
    localizador_t loc1 = inicio_cadena(c1);
    localizador_t loc2 = inicio_cadena(c2);
    if(es_localizador(loc1))
      while(es_localizador(loc1)){
        insertar_al_final(copia_info(info_cadena(loc1,c1)),resultado);
        loc1 = siguiente(loc1,c1);
      }
    if(es_localizador(loc2))
      while(es_localizador(loc2)){
        insertar_al_final(copia_info(info_cadena(loc2,c2)),resultado);
        loc2 = siguiente(loc2,c2);
      }
    return resultado;
  }
}

/*
  Se ordena `cad' de manera creciente según los datos numéricos de sus
  elementos.
  No se debe obtener ni devolver memoria de manera dinámica.
  Si es_vacia_cadena (cad) no hace nada.
 */
void ordenar(cadena_t &cad){
  if(!es_vacia_cadena(cad) && !esta_ordenada(cad)){
    localizador_t loc = inicio_cadena(cad);
    while (es_localizador(loc) && es_localizador(siguiente(loc,cad))){
      intercambiar(loc,menor_en_cadena(loc,cad),cad);
      loc = siguiente(loc,cad);
    }
  }
}

/*
  Cambia todas las ocurrencias de `original' por `nuevo' en los elementos
   de `cad'. No debe quedar memoria inaccesible.
 */
void cambiar_todos(int original, int nuevo, cadena_t &cad){
  localizador_t loc = inicio_cadena(cad);
  while(es_localizador(loc)){
    if (numero_info(info_cadena(loc,cad)) == original){
      char *frase = new char[strlen(frase_info(info_cadena(loc,cad))) + 1];
      strcpy(frase, frase_info(info_cadena(loc,cad)));
      info_t dato = crear_info(nuevo,frase);
      insertar_antes(dato,loc,cad);
      loc = anterior(loc,cad);
      localizador_t aRemover = siguiente(loc,cad);
      remover_de_cadena(aRemover,cad);
    }
    loc = siguiente(loc,cad);
  }
}

/*
  Devuelve la cadena_t de elementos de `cad' que cumplen
  "menor <= numero_info (elemento) <= mayor".
  El orden relativo de los elementos en la cadena_t resultado debe ser el mismo
  que en `cad'.
  La cadena_t resultado no comparte memoria con `cad'.
  Precondición: esta_ordenada (cad), `menor' <= `mayor',
  pertenece (menor, cad), pertenece (mayor, cad).
 */
cadena_t subcadena(int menor, int mayor, cadena_t cad) {
  cadena_t resultado = crear_cadena();
  localizador_t loc = inicio_cadena(cad);
  while (es_localizador(loc)){
    if(numero_info(info_cadena(loc,cad)) >= menor && numero_info(info_cadena(loc,cad)) <= mayor)
      insertar_al_final(copia_info(info_cadena(loc,cad)),resultado);
    loc = siguiente(loc,cad);
  }
  return resultado;
}
