#include <stdio.h>

int main(void)
{
    int nota;

    printf("Ingrese la calificación: ");
    scanf(" %d", &nota);
    printf("Calificación: ");

    if (nota >= 90)
        printf("A");
    else if (nota >= 80)
        printf("B");
    else if (nota >= 70)
        printf("C");
    else if (nota >= 60)
        printf("D");
    else
        printf("F");

    return 0;
}