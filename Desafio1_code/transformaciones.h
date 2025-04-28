#ifndef TRANSFORMACIONES_H
#define TRANSFORMACIONES_H

// Realiza una operación XOR bit a bit entre dos imágenes
void aplicarXOR(unsigned char* imagen1, unsigned char* imagen2, unsigned char* resultado, int size);
// Desplaza los bits de los píxeles de una imagen hacia la izquierda
void aplicarDesplaIzquierda(unsigned char* imagen, int bits, int size);
// Desplaza los bits de los píxeles de una imagen hacia la derecha
void aplicarDesplaDerecha(unsigned char* imagen, int bits, int size);
// Rota los bits de los píxeles de una imagen a la derecha
void aplicarRotacionDerecha(unsigned char* imagen, int bits, int size);
// Rota los bits de los píxeles de una imagen a la izquierda
void aplicarRotacionIzquierda(unsigned char* imagen, int bits, int size);

// Compara una imagen transformada con una máscara para verificar si la transformación es correcta
bool verificarEnmascaramiento(unsigned char* imagenTransformada, unsigned char* mascara, unsigned int* datosEnmascarados, int seed, int mascaraSize, int totalSizeImagen);

// Aplica transformaciones (XOR, rotación, desplazamiento) a una imagen y verifica si alguna coincide con los datos de enmascaramiento, exportando la imagen si es correcta
void probarTransformaciones(unsigned char* imagenTransformada, unsigned char* imagenIM, unsigned char* mascara,
                            int width, int height, int totalSize,unsigned int* datos, int seed, int mascaraSize,
                            const char* nombreArchivoTXT);

#endif
