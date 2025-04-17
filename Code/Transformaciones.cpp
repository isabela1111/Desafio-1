#include "transformaciones.h"
#include "utilidades.h"
#include <iostream>
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

// Esta función aplica las operaciones bit a bit, verifica si las transformaciones son correctas y exporta las imágenes resultantes si lo son.
// Se usa una máscara y otros parámetros para la verificación de las transformaciones.
// Parámetros:
// - imagenTransformada: Imagen original transformada.
// - mascara: Máscara para la verificación de transformaciones.
// - width, height: Dimensiones de la imagen.
// - totalSize: Tamaño total de la imagen en bytes.
// - datos: Datos de referencia para la verificación.
// - seed: Semilla para la verificación.
// - mascaraSize: Tamaño de la máscara.
// - nombreArchivoTXT: Nombre del archivo de la máscara.
void probarTransformaciones(unsigned char* imagenTransformada, unsigned char* mascara, int width, int height,
                            int totalSize,unsigned int* datos, int seed, int mascaraSize, const char* nombreArchivoTXT){

    unsigned char* temp = new unsigned char[totalSize];
    unsigned char* resultadoXOR = new unsigned char[totalSize];
    QString archivoSalida = "I_O.bmp";

    cout << "\nProbando con: " << nombreArchivoTXT << " | Seed: " << seed << endl;

    // XOR
    aplicarXOR(imagenTransformada, mascara, resultadoXOR, totalSize);
    if (verificarTransformacionCorrecta(resultadoXOR, mascara, datos, seed, mascaraSize, totalSize)) {
        exportImage(resultadoXOR, width, height, archivoSalida);
        cout << "Transformación detectada: XOR" << endl;
    }

    // Rotación izquierda
    memcpy(temp, imagenTransformada, totalSize);
    aplicarRotacionIzquierda(temp, 3, totalSize);
    if (verificarTransformacionCorrecta(temp, mascara, datos, seed, mascaraSize, totalSize)) {
        exportImage(temp, width, height, archivoSalida);
        cout << "Transformación detectada: Rotación Izquierda" << endl;
    }

    // Rotación derecha
    memcpy(temp, imagenTransformada, totalSize);
    aplicarRotacionDerecha(temp, 3, totalSize);
    if (verificarTransformacionCorrecta(temp, mascara, datos, seed, mascaraSize, totalSize)) {
        exportImage(temp, width, height, archivoSalida);
        cout << " Transformación detectada: Rotación Derecha" << endl;
    }

    // Desplazamiento izquierda
    memcpy(temp, imagenTransformada, totalSize);
    aplicarDesplaIzquierda(temp, 2, totalSize);
    if (verificarTransformacionCorrecta(temp, mascara, datos, seed, mascaraSize, totalSize)) {
        exportImage(temp, width, height, archivoSalida);
        cout << "Transformación detectada: Desplazamiento Izquierda" << endl;
    }

    // Desplazamiento derecha
    memcpy(temp, imagenTransformada, totalSize);
    aplicarDesplaDerecha(temp, 2, totalSize);
    if (verificarTransformacionCorrecta(temp, mascara, datos, seed, mascaraSize, totalSize)) {
        exportImage(temp, width, height, archivoSalida);
        cout << " Transformación detectada: Desplazamiento Derecha" << endl;
    }

    delete[] temp;
    delete[] resultadoXOR;
}



