#ifndef UTILIDADES_H
#define UTILIDADES_H
#include "QString"

// Función para cargar los píxeles de una imagen BMP
unsigned char* loadPixels(QString input, int &width, int &height);

// Función para exportar una imagen a un archivo BMP
bool exportImage(unsigned char* pixelData, int width, int height, const QString& archivoSalida);

// Declaración de la función que redimensiona la máscara
unsigned char* redimensionarMascara(unsigned char* mascara, int widthM, int heightM, int width, int height);

#endif // UTILIDADES_H
