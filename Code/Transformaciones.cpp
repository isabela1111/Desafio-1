#include <iostream>
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
