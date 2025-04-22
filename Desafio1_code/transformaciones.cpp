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


// Verifica si la transformación aplicada es correcta comparando el resultado con los datos esperados del .txt
bool verificarTransformacionCorrecta(
    unsigned char* imagenTransformada,  // Imagen luego de aplicar una operaccion bit a bit
    unsigned char* mascara,             // Imagen máscara (M.bmp), misma dimensión que la original
    unsigned int* datosEnmascarados,    // Datos RGB esperados del .txt (después de aplicar S(k) = ID(k+s) + M(k))
    int seed,                           // Semilla que indica desde dónde empezar en la imagen transformada
    int mascaraSize,                    // Cantidad total de bytes (RGB) en la máscara
    int totalSizeImagen                 // Tamaño total de la imagen transformada (en bytes)
    ) {
    // Verifica que no se intente acceder fuera del arreglo de la imagen transformada
    if (seed + mascaraSize > totalSizeImagen) {
        return false;
    }

    // Compara byte por byte: (imagenTransformada[k + seed] + mascara[k]) debe ser igual al dato esperado
    for (int i = 0; i < mascaraSize; i++) {
        int suma = imagenTransformada[i + seed] + mascara[i];

        if (suma != (int)datosEnmascarados[i]) {
            // Si un solo valor no coincide, no es la transformación correcta
            return false;
        }
    }

    return true;
}

void probarTransformaciones(unsigned char* imagenTransformada, unsigned char* mascara,
                            int width, int height, int totalSize,
                            unsigned int* datos, int seed, int mascaraSize,
                            const char* nombreArchivoTXT)
{
    unsigned char* copia = new unsigned char[totalSize];

    // XOR
    memcpy(copia, imagenTransformada, totalSize);
    aplicarXOR(copia, mascara, copia, totalSize);
    if (verificarTransformacionCorrecta(copia, mascara, datos, seed, mascaraSize, totalSize)) {
        memcpy(imagenTransformada, copia, totalSize);
        delete[] copia;
        return;
    }

    //Se crea un bucle para que analice a cuantos bits se desplazo o se roto la imagen de acuerdo .txt
    for (int bits = 1; bits <= 8; ++bits) {
        memcpy(copia, imagenTransformada, totalSize);
        aplicarRotacionDerecha(copia, bits, totalSize);
        if (verificarEnmascaramiento(copia, mascara, datos, seed, mascaraSize, totalSize)) {
            memcpy(imagenTransformada, copia, totalSize);
            delete[] copia;
            break;
        }
        memcpy(copia, imagenTransformada, totalSize);
        aplicarRotacionIzquierda(copia, bits, totalSize);
        if (verificarEnmascaramiento(copia, mascara, datos, seed, mascaraSize, totalSize)) {
            memcpy(imagenTransformada, copia, totalSize);
            delete[] copia;
            break;
        }
        memcpy(copia, imagenTransformada, totalSize);
        aplicarDesplaDerecha(copia, bits, totalSize);
        if (verificarEnmascaramiento(copia, mascara, datos, seed, mascaraSize, totalSize)) {
            memcpy(imagenTransformada, copia, totalSize);
            delete[] copia;
            break;
        }
        memcpy(copia, imagenTransformada, totalSize);
        aplicarDesplaIzquierda(copia, bits, totalSize);
        if (verificarEnmascaramiento(copia, mascara, datos, seed, mascaraSize, totalSize)) {
            memcpy(imagenTransformada, copia, totalSize);
            delete[] copia;
            break;
        }
    }

    qDebug() << "Ninguna transformación válida encontrada para" << nombreArchivoTXT;
    delete[] copia;
}
