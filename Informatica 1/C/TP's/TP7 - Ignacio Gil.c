#include <stdio.h>

#define TAM 10

void ingresar(int[], float[], int);
void ordenar(int[], float[], int);
void imprimir(int[], float[], int);

int main()
{
    int codBarra[TAM];
    float precio[TAM];

    printf("Se debe ingresar %d producto(s).\n", TAM);

    ingresar(codBarra, precio, TAM);
    ordenar(codBarra, precio, TAM);
    imprimir(codBarra, precio, TAM);
}

void ingresar(int barra[], float precio[], int n)
{
    for (int i = 0; i < n; i++)
    {
        do
        {
            printf("Ingrese el codigo de barra del producto (debe ser positivo): ");
            scanf(" %d", &barra[i]);
        } while (barra[i] < 0); // Solo se puede ingresar codigos de barra positivos, esto lo verifica

        do
        {
            printf("Ingrese su precio (mayor a 0): ");
            scanf(" %f", &precio[i]);
        } while (precio[i] <= 0); // Solo se puede ingresar precios positivos, esto lo verifica
    }
}

void ordenar(int barra[], float precio[], int n) // Metodo de la burbuja
{
    for (int i = 0; i < 3; i++) // Solo se hace 3 veces, ya que solo hay que hacer 3 ordenamientos (3 mayores y 3 menores)
    {
        for (int j = 0; j < n - 1; j++) // Se hacen los 3 ordenamientos mayores
        {
            if (precio[j] > precio[j + 1])
            {
                int cambioPrecio;
                int cambioBarra;

                cambioPrecio = precio[j + 1];
                precio[j + 1] = precio[j];
                precio[j] = cambioPrecio;

                cambioBarra = barra[j + 1];
                barra[j + 1] = barra[j];
                barra[j] = cambioBarra;
            }
        }
        for (int j = n - 4; j > 1; j--) // Se hacen los 3 ordenamientos menores
        {
            printf("%d ", j);
            if (precio[j] < precio[j - 1])
            {
                int cambioPrecio;
                int cambioBarra;

                cambioPrecio = precio[j - 1];
                precio[j - 1] = precio[j];
                precio[j] = cambioPrecio;

                cambioBarra = barra[j - 1];
                barra[j - 1] = barra[j];
                barra[j] = cambioBarra;
            }
        }
    }
}

void imprimir(int barra[], float precio[], int n)
{
    printf("\nProducto mas caro:\nCódigo: %d\nPrecio: %.2f\n", barra[TAM - 1], precio[TAM - 1]); // Se imprime el mas caro
    printf("\nProducto mas barato:\nCódigo: %d\nPrecio: %.2f\n", barra[0], precio[0]);           // Se imprime el mas barato

    printf("\nProductos mas caros:\n");
    for (int i = TAM - 1; i > (TAM - 4); i--)
    {
        printf("-----\nCódigo: %d\nPrecio: %.2f\n", barra[i], precio[i]); // Se imprimen los 3 mas caros
    }

    printf("\nProductos mas baratos:\n");
    for (int i = 0; i < 3; i++)
    {
        printf("-----\nCódigo: %d\nPrecio: %.2f\n", barra[i], precio[i]); // Se imprimen los 3 mas baratos
    }
}