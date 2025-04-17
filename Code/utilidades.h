#ifndef UTILIDADES_H
#define UTILIDADES_H
#include "QString"

// Función para cargar los píxeles de una imagen BMP
unsigned char* loadPixels(QString input, int &width, int &height);

// Función para exportar una imagen a un archivo BMP
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

#endif // UTILIDADES_H
