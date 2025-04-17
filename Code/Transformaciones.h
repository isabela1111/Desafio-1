#ifndef TRANSFORMACIONES_H
#define TRANSFORMACIONES_H

// Declaraciones de funciones implementadas en transformaciones.cpp
void aplicarXOR(unsigned char* imagen1, unsigned char* imagen2, unsigned char* resultado, int size);
void aplicarDesplaIzquierda(unsigned char* imagen, int bits, int size);
void aplicarDesplaDerecha(unsigned char* imagen, int bits, int size);
void aplicarRotacionDerecha(unsigned char* imagen, int bits, int size);
void aplicarRotacionIzquierda(unsigned char* imagen, int bits, int size);

bool verificarEnmascaramiento(unsigned char* imagenTransformada, unsigned char* mascara, unsigned int* datosEnmascarados, int seed, int mascaraSize, int totalSizeImagen);
void probarTransformaciones(unsigned char* imagenTransformada, unsigned char* mascara,int width, int height, int totalSize,unsigned int* datos, int seed, int mascaraSize,const char* nombreArchivoTXT);

#endif
