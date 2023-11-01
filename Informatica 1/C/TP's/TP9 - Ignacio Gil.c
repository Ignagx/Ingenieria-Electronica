#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ENTERO 1
#define FLOTANTE 0

// This is a file change, I do not want to merge this in main yet
// Now, I will merge the branch
// This is really good
// I did this from my notebook
// HAHA You Lose

typedef struct {
  char nombre[40];
  int  anio_nacimiento;
} autor_t;

typedef struct libro {
  char    nombre[40];
  int     cant_paginas;
  int     cant_capitulos;
  int    *paginas_por_capitulo;
  float   precio;
  autor_t datos_autor;
} libro_t;

char *error[] = {                                                 // Se definen los errores por el codigo
    "Entrada Invalida!\n",                                        // error[0]
    "No se aceptan numeros negativos!\n",                         // error[1]
    "El numero ingresado debe ser mayor a 0!\n",                  // error[2]
    "No debe ingresar mas del tamaño de piezas especificados!\n", // error[3]
    "Memory Allocation Error\n"};                                 // error[4]

int  prueba_ingreso(const char *); // Funcion que verifica el correcto ingreso de numeros
void ingresar_numero(void *, int); // Funcion que solo permite el ingreso de Numeros
void ingresar_nombre(char *);
void ingresar_capitulos(int *);
void ingresar_paginas(libro_t *);
void ingresar_precio(float *);
void ingresar_datos_autor(autor_t *);
void cargar_biblioteca(libro_t *bib, int cant);
void imprimir_biblioteca(libro_t *bib, int cant);
void swap(int *, int *);
void swapFloat(float *, float *);
void swapChar(char *, char *);
void quicksort(libro_t *, int, int);
void ordenar_por_precio_menor_mayor(libro_t *bib, int cant);

int main(void) {

  int cantidad = 0;

  do {
    if (cantidad != 0)
      printf("%s", error[0]);
    printf("Ingrese la cantidad de libros a cargar: ");
    ingresar_numero(&cantidad, ENTERO);
  } while (cantidad < 0);

  libro_t *biblioteca = NULL;

  biblioteca = calloc(cantidad * sizeof(*biblioteca), sizeof(*biblioteca));

  if (biblioteca != NULL) {
    cargar_biblioteca(biblioteca, cantidad);

    printf("===============================================\n");
    printf("Imprimr biblioteca desordenada: \n");
    imprimir_biblioteca(biblioteca, cantidad);

    printf("Imprimir biblioteca ordenada: \n");
    ordenar_por_precio_menor_mayor(biblioteca, cantidad);
    imprimir_biblioteca(biblioteca, cantidad);

    free(biblioteca->paginas_por_capitulo);
    free(biblioteca);
  } else
    printf("%s", error[4]);

  return 0;
}

void ingresar_numero(void *pV, const int tipo) {

  char string[1000];
  int  correcto;

  do {
    scanf(" %1000s", string); // Se ingresa la entrada a un arreglo

    correcto = prueba_ingreso(string);

  } while (correcto == 0);

  switch (tipo) {
  case FLOTANTE:
    *(float *)pV = atof(string);
    break;

  case ENTERO:
    *(int *)pV = atoi(string);
    break;
  }
}

int prueba_ingreso(const char *string) {
  int valido = 1;

  if (strtof(string, NULL) == 0 && strcmp(string, "0") != 0) { // Se verifica si el arreglo es un float con strtof y se verifica que no sea un 0 genuino para ser un error
    printf("%s", error[0]);
    valido = 0;
  }

  return valido;
}

void cargar_biblioteca(libro_t *bib, int cant) {
  for (int i = 0; i < cant; i++) {
    printf("Ingrese el nombre del libro N° %d: ", i + 1);
    ingresar_nombre((bib + i)->nombre);
    printf("Ingrese la cantidad de capitulos de %s: ", (bib + i)->nombre);
    ingresar_capitulos(&(bib + i)->cant_capitulos);
    ingresar_paginas(bib + i);
    printf("Ingrese el precio de %s: ", (bib + i)->nombre);
    ingresar_precio(&(bib + i)->precio);
    printf("Ingrese el autor %s: ", (bib + i)->nombre);
    ingresar_datos_autor(&(bib + i)->datos_autor);
  }
}

void ingresar_nombre(char *nombre) {
  char ch;
  scanf(" %40[^\n]s", nombre);
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;
}

void ingresar_capitulos(int *capitulos) {
  ingresar_numero(capitulos, ENTERO);
}

void ingresar_paginas(libro_t *bib) {
  bib->paginas_por_capitulo = malloc(bib->cant_capitulos * sizeof(*(bib->paginas_por_capitulo)));
  for (int i = 0; i < bib->cant_capitulos; i++) {
    printf("Ingrese la cantidad de paginas del capitulo N° %d: ", i + 1);
    ingresar_numero(&bib->paginas_por_capitulo[i], ENTERO);
    (bib->cant_paginas) += (bib->paginas_por_capitulo[i]);
  }
}

void ingresar_precio(float *precio) {
  ingresar_numero(precio, FLOTANTE);
}

void ingresar_datos_autor(autor_t *autor) {
  char ch;

  scanf(" %40[^\n]s", autor->nombre);
  while ((ch = getchar()) != '\n' && ch != EOF)
    ;
  printf("Ingrese el año en el que nació %s: ", autor->nombre);

  ingresar_numero(&autor->anio_nacimiento, ENTERO);
}

void imprimir_biblioteca(libro_t *bib, int cant) {
  for (int i = 0; i < cant; i++) {
    printf("Nombre del libro N: %s\n", i + 1, (bib + i)->nombre);
    printf("Cantidad de Paginas: %d\n", (bib + i)->cant_paginas);
    printf("Cantidad de capitulos: %d\n", (bib + i)->cant_capitulos);
    for (int j = 0; j < (bib + i)->cant_capitulos; j++)
      printf("Cantidad de paginas del capitulo %d: %d\n", j + 1, (bib + i)->paginas_por_capitulo[j]);
    printf("Precio: %.2f\n", (bib + i)->precio);
    printf("Nombre del autor: %s\n", (bib + i)->datos_autor.nombre);
    printf("Año de nacimiento del autor: %d\n", (bib + i)->datos_autor.anio_nacimiento);
    printf("===============================================\n");
  }
}

void swap(int *p, int *q) {
  int tmp = *p;
  *p = *q;
  *q = tmp;
}

void swapFloat(float *p, float *q) {
  float tmp = *p;
  *p = *q;
  *q = tmp;
}

void swapDatos(autor_t *p, autor_t *q) {
  autor_t tmp = *p;
  *p = *q;
  *q = tmp;
}

void swapChar(char *p, char *q) {
  char tmp[40] = {0};
  strcpy(tmp, p);
  strcpy(p, q);
  strcpy(q, tmp);
}

void swapArr(int **p, int **q) {
  int *tmp = *p;
  *p = *q;
  *q = tmp;
}

void quicksort(libro_t *bib, int left, int right) {
  int i, j;

  i = left;
  j = right;

  do {
    while (((bib + i)->precio < (bib + j)->precio) && i < j)
      j--;
    if (i < j) {
      swapFloat(&(bib + i)->precio, &(bib + j)->precio);
      swapChar((bib + i)->nombre, (bib + j)->nombre);
      swapArr(&(bib + i)->paginas_por_capitulo, &(bib + j)->paginas_por_capitulo);
      swap(&(bib + i)->cant_capitulos, &(bib + j)->cant_capitulos);
      swapDatos(&(bib + i)->datos_autor, &(bib + j)->datos_autor);
      swap(&(bib + i)->cant_paginas, &(bib + j)->cant_paginas);
      i++;
    }
    while (((bib + i)->precio < (bib + j)->precio) && i < j)
      i++;
    if (i < j) {
      swapFloat(&(bib + i)->precio, &(bib + j)->precio);
      swapChar((bib + i)->nombre, (bib + j)->nombre);
      swapArr(&(bib + i)->paginas_por_capitulo, &(bib + j)->paginas_por_capitulo);
      swap(&(bib + i)->cant_capitulos, &(bib + j)->cant_capitulos);
      swapDatos(&(bib + i)->datos_autor, &(bib + j)->datos_autor);
      swap(&(bib + i)->cant_paginas, &(bib + j)->cant_paginas);
      j--;
    }
  } while (i < j);

  if (left < j)
    quicksort(bib, left, j - 1);
  if (i < right) {
    quicksort(bib, i + 1, right);
  }
}

void ordenar_por_precio_menor_mayor(libro_t *bib, int cant) {
  quicksort(bib, 0, cant - 1);
}