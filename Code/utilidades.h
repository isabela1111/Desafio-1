#ifndef UTILIDADES_H
#define UTILIDADES_H
#include "QString"


// Declaración de las funciones

// Función para cargar los píxeles de una imagen BMP
unsigned char* loadPixels(QString input, int &width, int &height);

// Función para exportar una imagen a un archivo BMP
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

// Función para cargar la máscara de enmascaramiento desde un archivo
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

#endif // UTILIDADES_H
