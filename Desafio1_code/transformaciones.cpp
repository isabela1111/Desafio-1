#include "transformaciones.h"
#include "utilidades.h"
#include"enmascaramiento.h"
#include <QDebug>
#include <cstring>
#include <QString>


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
// Rota bits hacia la derecha, Los bits que salen por la derecha entran por la izquierda.
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

bool verificarTransformacionCorrecta(
    unsigned char* imagenTransformada,
    unsigned char* mascara,
    unsigned int* datosEnmascarados,
    int seed,
    int mascaraSize,
    int totalSizeImagen
    ) {
    if (seed + mascaraSize > totalSizeImagen) {
        return false;
    }

    for (int i = 0; i < mascaraSize; i++) {
        int suma = (imagenTransformada[i + seed] + mascara[i]) & 0xFF;
        if (suma != (int)datosEnmascarados[i]) {
            return false;
        }
    }

    return true;
}

void probarTransformaciones(unsigned char* imagenTransformada, unsigned char* imagenIM, unsigned char* mascara,
                            int width, int height, int totalSize,unsigned int* datos, int seed, int mascaraSize,
                            const char* nombreArchivoTXT)
{
    unsigned char* copia = new unsigned char[totalSize];

    // 1. Probar XOR
    memcpy(copia, imagenTransformada, totalSize);
    aplicarXOR(copia, imagenIM, copia, totalSize);
    if (verificarEnmascaramiento(copia, mascara, datos, seed, mascaraSize, totalSize)) {
        memcpy(imagenTransformada, copia, totalSize);
        delete[] copia;
        qDebug() << "Transformación encontrada: XOR para" << nombreArchivoTXT;
        return;
    }

    // 2. Probar Rotaciones y Desplazamientos
    for (int bits = 1; bits <= 7; ++bits) {

        // Rotación Izquierda
        memcpy(copia, imagenTransformada, totalSize);
        aplicarRotacionIzquierda(copia, bits, totalSize);
        if (verificarEnmascaramiento(copia, mascara, datos, seed, mascaraSize, totalSize)) {
            memcpy(imagenTransformada, copia, totalSize);
            delete[] copia;
            qDebug() << "Transformación encontrada: Rotación Izquierda" << bits << "bits para" << nombreArchivoTXT;
            return;
        }

        // Rotación Derecha
        memcpy(copia, imagenTransformada, totalSize);
        aplicarRotacionDerecha(copia, bits, totalSize);
        if (verificarEnmascaramiento(copia, mascara, datos, seed, mascaraSize, totalSize)) {
            memcpy(imagenTransformada, copia, totalSize);
            delete[] copia;
            qDebug() << "Transformación encontrada: Rotación Derecha" << bits << "bits para" << nombreArchivoTXT;
            return;
        }

        // Desplazamiento Izquierda
        memcpy(copia, imagenTransformada, totalSize);
        aplicarDesplaIzquierda(copia, bits, totalSize);
        if (verificarEnmascaramiento(copia, mascara, datos, seed, mascaraSize, totalSize)) {
            memcpy(imagenTransformada, copia, totalSize);
            delete[] copia;
            qDebug() << "Transformación encontrada: Desplazamiento Izquierda" << bits << "bits para" << nombreArchivoTXT;
            return;
        }

        // Desplazamiento Derecha
        memcpy(copia, imagenTransformada, totalSize);
        aplicarDesplaDerecha(copia, bits, totalSize);
        if (verificarEnmascaramiento(copia, mascara, datos, seed, mascaraSize, totalSize)) {
            memcpy(imagenTransformada, copia, totalSize);
            delete[] copia;
            qDebug() << "Transformación encontrada: Desplazamiento Derecha" << bits << "bits para" << nombreArchivoTXT;
            return;
        }
    }

    // Si ninguna transformación funcionó
    qDebug() << " Ninguna transformación válida encontrada para" << nombreArchivoTXT;
    delete[] copia;
}
