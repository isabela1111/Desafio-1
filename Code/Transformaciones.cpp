#include <iostream>
#include "transformaciones.h"
using namespace std;

// XOR
// Realiza la operación bit a bit XOR entre dos arreglos de bytes.
// Entrada: imagen1, imagen2 (arreglos de tamaño 'size')
// Salida: resultado[i] = imagen1[i] ^ imagen2[i]
void aplicarXOR(unsigned char* imagen1, unsigned char* imagen2, unsigned char* resultado, int size) {
    for (int i = 0; i < size; i++) {
        resultado[i] = imagen1[i] ^ imagen2[i];
    }
}

//DESPLAZAMIENTOS
// Desplaza bits a la izquierda (agrega ceros a la derecha).
unsigned char desplazarIzquierda(unsigned char byte, int bits) {
    return byte << bits;
}

// Aplica el desplazamiento a la izquierda a todos los bytes del arreglo.
void aplicarDesplaIzquierda(unsigned char* imagen, int bits, int size) {
    for (int i = 0; i < size; i++) {
        imagen[i] = desplazarIzquierda(imagen[i], bits);
    }
}

// Desplaza bits a la derecha (agrega ceros a la izquierda).
unsigned char desplazarDerecha(unsigned char byte, int bits) {
    return byte >> bits;
}

// Aplica el desplazamiento a la derecha a todos los bytes del arreglo.
void aplicarDesplaDerecha(unsigned char* imagen, int bits, int size) {
    for (int i = 0; i < size; i++) {
        imagen[i] = desplazarDerecha(imagen[i], bits);
    }
}

// ROTACIONES
// Rota bits hacia la derecha. Los bits que salen por la derecha entran por la izquierda.
unsigned char rotarDerecha(unsigned char byte, int bits) {
    return (byte >> bits) | (byte << (8 - bits));
}

// Aplica rotación a la derecha a todos los bytes del arreglo.
void aplicarRotacionDerecha(unsigned char* imagen, int bits, int size) {
    for (int i = 0; i < size; i++) {
        imagen[i] = rotarDerecha(imagen[i], bits);
    }
}

// Rota bits hacia la izquierda. Los bits que salen por la izquierda entran por la derecha.
unsigned char rotarIzquierda(unsigned char byte, int bits) {
    return (byte << bits) | (byte >> (8 - bits));
}

// Aplica rotación a la izquierda a todos los bytes del arreglo.
void aplicarRotacionIzquierda(unsigned char* imagen, int bits, int size) {
    for (int i = 0; i < size; i++) {
        imagen[i] = rotarIzquierda(imagen[i], bits);
    }
}


// Verificación del enmascaramiento S(k) = ID(k+s) + M(k)
bool verificarEnmascaramiento(
    unsigned char* imagenTransformada, //Arreglo de los bytes RGB de la imagen despues de ser transformada (XOR,Desplazamiento o Rotacion)
    unsigned char* mascara, //Arreglo de los bytes RGB de la mascara M.bmp
    unsigned int* datosEnmascarados,//Arreglo con los valores RGB del .txt a comparar
    int seed, //semilla S que esta en la primera linea del .txt que se esta comparando
    int mascaraSize, //cantidad total de bytes que hay en la mascara M.bmp
    int totalSizeImagen)
{
    if (seed + mascaraSize > totalSizeImagen) {
        return false; //Verifica que no esten fuera del rango
    }

    for (int i = 0; i < mascaraSize; i++) {
        int suma = imagenTransformada[i + seed] + mascara[i];
        if (suma != (int)datosEnmascarados[i]) {
            return false; //Si el enmascaramiento no es parecido al .txt, se regresa que esa no es la transformacion correcta
        }
    }

    return true; //Si todo es parecido, regresa que la transformacion es correcta
}
