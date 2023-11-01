#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// I wil create a new branch

#define TAM 20
#define MIN 20
#define MAX 200
#define ENTERO 1
#define FLOTANTE 0

char *error[] = {                                                 // Se definen los errores por el codigo
    "Entrada Invalida!\n",                                        // error[0]
    "No se aceptan numeros negativos!\n",                         // error[1]
    "El numero ingresado debe ser mayor a 0!\n",                  // error[2]
    "No debe ingresar mas del tamaño de piezas especificados!\n", // error[3]
    "Memory Allocation Error\n"};                                 // error[4]

void ingresarNumero(void *, int);                                       // Funcion que solo permite el ingreso de Numeros
int  pruebaIngreso(const char *);                                       // Funcion que verifica el correcto ingreso de numeros
int  ingresarValidarTam(void);                                          // Funcion que pide al usuario ingresar la cantidad de piezas, no mayoresque TAM
void cargarArreglo(float *, const int);                                 // Funcion que pide al usuario cargar la calidad de las piezas
int  calcularValidas(const float *, const int);                         // Funcion que calcula la cantidad de piezas validas a partir de su calidad
int  calcularDefectuosas(const float *, const int);                     // Funcion que calcula la cantidad de piezas defectuosas a partir de su calidad
void mostrarPiezas(const float *, const int);                           // Funcion que muestra las piezas ingresadas en pantalla
void swap(int *, int *);                                                // Funcion que intercambia dos valores de un arreglo
void ordenar(const float *, int *, int);                                // Funcion para ordenar el arreglo con quick sort
void mostrarPiezasValidasOrdenadasMenorMayor(const float *, int const); // Funcion que muestra las piezas validas de manera ordenada

int main() {
  int    piezas;
  float *calidad = NULL;
  int    validas;
  int    defectuosas;

  piezas = ingresarValidarTam();

  calidad = malloc(piezas * sizeof(float));

  if (calidad == NULL) {
    printf("%s", error[4]);
    return -1;
  }

  cargarArreglo(calidad, piezas);

  validas = calcularValidas(calidad, piezas);

  printf("Numero de piezas validas: %d\n", validas);

  defectuosas = calcularDefectuosas(calidad, piezas);

  printf("Numero de piezas defectuosas: %d\n", defectuosas);

  mostrarPiezas(calidad, piezas);

  mostrarPiezasValidasOrdenadasMenorMayor(calidad, piezas);

  free(calidad);

  return 0;
}

int pruebaIngreso(const char *string) {
  int valido = 1;

  if (strtof(string, NULL) == 0 && strcmp(string, "0") != 0) { // Se verifica si el arreglo es un float con strtof y se verifica que no sea un 0 genuino para ser un error
    printf("%s", error[0]);
    valido = 0;
  }

  return valido;
}

void ingresarNumero(void *pV, const int tipo) {

  char string[1000];
  int  correcto;

  do {
    scanf(" %1000s", string); // Se ingresa la entrada a un arreglo

    correcto = pruebaIngreso(string);

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

int ingresarValidarTam(void) {

  int n;

  printf("Ingrese la cantidad de piezas como numero entero mayor a 0(no mas que %d):", TAM);

  do {

    ingresarNumero(&n, ENTERO);

    if (n <= 0)
      printf("%s", error[2]);

    if (n > TAM)
      printf("%s", error[3]);

  } while ((n <= 0) || (n > TAM));

  return n;
}

void cargarArreglo(float *arr, const int cantidad_piezas) {

  printf("Ingrese la calidad de las %d piezas (No debe ser negativo)\n", cantidad_piezas);

  for (int i = 0; i < cantidad_piezas; i++) {
    do {
      printf("Pieza N° %d: ", i + 1);

      ingresarNumero((arr + i), FLOTANTE);

      if (*(arr + i) < 0)
        printf("%s", error[1]);

    } while (*(arr + i) < 0);
  }
}

int calcularValidas(const float *arr, const int cantidad_piezas) {
  int n = 0;

  for (int i = 0; i < cantidad_piezas; i++)
    if (*(arr + i) >= MIN && *(arr + i) <= MAX)
      n++;

  return n;
}

int calcularDefectuosas(const float *arr, const int cantidad_piezas) {
  int n = 0;

  for (int i = 0; i < cantidad_piezas; i++)
    if (*(arr + i) < MIN || *(arr + i) > MAX)
      n++;

  return n;
}

void mostrarPiezas(const float *arr, const int cantidad_piezas) {
  printf("Arreglo desordenado: \n");
  for (int i = 0; i < cantidad_piezas; i++)
    printf("%d: %.2f\n", i + 1, *(arr + i));
}

void swap(int *p, int *q) {
  int tmp = *p;
  *p = *q;
  *q = tmp;
}

void ordenar(const float *arr, int *pArr, int cantidad_piezas) {
  int validas = 0;

  for (int i = 0; i < cantidad_piezas; i++) // Se iguala los indices solo de los elementos validos
    if (*(arr + i) >= MIN && *(arr + i) <= MAX) {
      static int n = 0;
      *(pArr + n++) = i;
      validas++;
    }

  for (int j = 0; j < validas - 1; j++) // Ordenamiento de Burbuja de los indices
    for (int i = 0; i < validas - j - 1; i++)
      if (*(arr + *(pArr + i)) > *(arr + *(pArr + i + 1)))
        swap(pArr + i, pArr + i + 1);
}

void mostrarPiezasValidasOrdenadasMenorMayor(const float *arr, int const cantidad_piezas) {

  int validas = calcularValidas(arr, cantidad_piezas);

  if (validas > 0) // Solo se hace si hay piezas validas
  {
    int *pArr = NULL; // Se crea un arreglo de indices nuevo asi no se modifica el original
    pArr = malloc(validas * sizeof(int));

    if (pArr == NULL) {
      printf("%s", error[4]);
      return;
    }

    ordenar(arr, pArr, cantidad_piezas); // Se ordenan los indices, para que queden los validos de menor a mayor

    printf("Piezas Validas Ordenadas: \n");

    for (int j = 0; j < validas; j++)
      printf("Pieza N° %d: %.2f\n", (*(pArr + j) + 1), *(arr + (*(pArr + j))));

    free(pArr);
  }
}