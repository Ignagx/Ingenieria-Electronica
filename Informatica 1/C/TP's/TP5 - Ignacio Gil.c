#include <stdio.h>

int main(void)
{
    int cantidad;
    float notaAlta = 0, notaBaja = 100, promedio = 0;

    printf("Ingrese la cantidad de alumnos: ");

    while (1)
    {
        scanf(" %d", &cantidad);                                         // Se Ingresa la cantidad de alumnos
        if (cantidad >= 0)                                               // Se verifica si la nota esta entre 0 y 100
            break;                                                       // Se sale del loop y termina el proceso si esta bien
        printf("Numero invalido, porfavor ingrese un numero positivo: "); // Pide que vuelva a ingresar si esta mal
    }

    for (int i = 0; i < cantidad; i++) // El codigo se repite segun la cantidad de alumnos
    {
        float nota = 0;
        printf("Ingrese la nota del alumno N° %d (Debe estar entre 0 y 100): ", i + 1);
        while (1)
        {
            scanf(" %f", &nota);
            if (nota >= 0 && nota <= 100)                                       // Se verifica si la nota esta entre 0 y 100
                break;                                                          // Se sale del loop y termina el proceso si esta bien
            printf("Nota Invalida, porfavor ingrese una nota entre 0 y 100: "); // Pide que vuelva a ingresar si esta mal
        }

        promedio += nota; // Se le suma la nota al promedio total

        if (nota > notaAlta) // Se verifica si es la nota mas alta
            notaAlta = nota;

        if (nota < notaBaja) // Se verifica si es la nota mas baja
            notaBaja = nota;
    }

    printf("\nEl promedio de notas es: %.2f\n", (promedio / cantidad)); // Se calcula e imprime el promedio
    printf("La nota mas alta es: %.2f\n", notaAlta);                    // Se imprime la nota mas alta
    printf("La nota mas baja es: %.2f", notaBaja);                      // Se imprime la nota mas baja

    return 0;
}