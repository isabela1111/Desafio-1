#include "transformaciones.h"
#include "utilidades.h"

void realizarExperimentos(unsigned char* imagenOriginal, unsigned char* imagenMascarada, int width, int height) {
    int size = width * height * 3;  // Tamaño de la imagen en bytes (RGB)

    unsigned char* resultadoXOR = new unsigned char[size];
    unsigned char* imagenRotada = new unsigned char[size];

    // XOR entre la imagen original y la imagen mascarada
    aplicarXOR(imagenOriginal, imagenMascarada, resultadoXOR, size);
    exportImage(resultadoXOR, width, height, "P1.bmp");

    // Rotación 3 bits a la derecha
    aplicarRotacionDerecha(resultadoXOR, 3, size);
    exportImage(resultadoXOR, width, height, "P2.bmp");

    // XOR nuevamente entre el resultado de la rotación y la IM
    aplicarXOR(resultadoXOR, imagenMascarada, imagenRotada, size);
    exportImage(imagenRotada, width, height, "P3.bmp");

    // Limpiar la memoria usada
    delete[] resultadoXOR;
    delete[] imagenRotada;
}
