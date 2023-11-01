#include <stdio.h>

int calcularArea(int longitud, int altura);
int calcularPerimetro(int longitud, int altura);
int imprimirResultados(int area, int perimetro);

int main(){
    int altura, longitud;
    printf("Ingrese la longitud del rectángulo: ");
    scanf(" %d", &longitud);
    printf("Ingrese el altura del rectángulo: ");
    scanf(" %d", &altura);

    imprimirResultados(calcularArea(longitud, altura), calcularPerimetro(longitud, altura));
    return 0;
}

int calcularArea(int longitud, int altura){
    int area = longitud * altura;
    return area;
}

int calcularPerimetro(int longitud, int altura){
    int perimetro = (longitud * 2) + (altura * 2);
    return perimetro;
}

int imprimirResultados(int area, int perimetro){
    printf("\nEl área del rectángulo es: %d\nEl perímetro del rectangulo es: %d", area, perimetro);
}