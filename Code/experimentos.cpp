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
