#ifndef ENMASCARAMIENTO_H
#define ENMASCARAMIENTO_H
#include"QString"
// Función para cargar la máscara de enmascaramiento desde un archivo
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

// Función que detecta los archivos M0.txt, M1.txt,....
char** detectarArchivosDeEnmascaramiento(const QString& carpeta, int& cantidad);


// Verifica si una transformación es válida comparando con los datos del .txt
bool verificarEnmascaramiento(
    unsigned char* imagenTransformada,
    unsigned char* mascara,
    unsigned int* datosEnmascarados,
    int seed,
    int mascaraSize,
    int totalSizeImagen
    );

#endif // ENMASCARAMIENTO_H
