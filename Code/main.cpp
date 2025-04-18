/*
 * Programa demostrativo de manipulaciónprocesamiento de imágenes BMP en C++ usando Qt.
 *
 * Descripción:
 * Este programa realiza las siguientes tareas:
 * 1. Carga una imagen BMP desde un archivo (formato RGB sin usar estructuras ni STL).
 * 2. Modifica los valores RGB de los píxeles asignando un degradado artificial basado en su posición.
 * 3. Exporta la imagen modificada a un nuevo archivo BMP.
 * 4. Carga un archivo de texto que contiene una semilla (offset) y los resultados del enmascaramiento
 *    aplicados a una versión transformada de la imagen, en forma de tripletas RGB.
 * 5. Muestra en consola los valores cargados desde el archivo de enmascaramiento.
 * 6. Gestiona la memoria dinámicamente, liberando los recursos utilizados.
 *
 * Entradas:
 * - Archivo de imagen BMP de entrada ("I_O.bmp").
 * - Archivo de salida para guardar la imagen modificada ("I_D.bmp").
 * - Archivo de texto ("M1.txt") que contiene:
 *     • Una línea con la semilla inicial (offset).
 *     • Varias líneas con tripletas RGB resultantes del enmascaramiento.
 *
 * Salidas:
 * - Imagen BMP modificada ("I_D.bmp").
 * - Datos RGB leídos desde el archivo de enmascaramiento impresos por consola.
 *
 * Requiere:
 * - Librerías Qt para manejo de imágenes (QImage, QString).
 * - No utiliza estructuras ni STL. Solo arreglos dinámicos y memoria básica de C++.
 *
 * Autores: Augusto Salazar Y Aníbal Guerra
 * Fecha: 06/04/2025
 * Asistencia de ChatGPT para mejorar la forma y presentación del código fuente
 */

#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include <QString>
#include <QString>
#include <QString>
#include <QFile>
#include <QString>
#include <QByteArray>
#include <cstring>
#include "utilidades.h"
#include "transformaciones.h"
#include "enmascaramiento.h"
#include "experimentos.h"

using namespace std;


//Sugerengia de main
int main() {

    // Archivos de entrada
    QString archivoTransformada = "I_D.bmp";  // Imagen enmascarada
    QString archivoMascara = "M.bmp";         // Máscara
    QString archivoSalida = "I_O.bmp";        // Archivo final

    // Variables para ancho y alto de la imagen
    int width = 0;
    int height = 0;

    // Cargar imagen transformada y máscara
    unsigned char* imagenTransformada = loadPixels(archivoTransformada, width, height);
    unsigned char* mascara = loadPixels(archivoMascara, width, height);
    if (!imagenTransformada || !mascara) return -1;

    // Tamaño total de la imagen (ancho * alto * 3 canales RGB)
    int totalSize = width * height * 3;

    // Detectar archivos .txt con pistas
    int cantidadTxt = 0;
    char** archivosTxt = detectarArchivosDeEnmascaramiento(cantidadTxt);

    // Iterar sobre los archivos de enmascaramiento
    for (int i = cantidadTxt - 1; i >= 0; --i) {
        int seed = 0;
        int n_pixels = 0;
        unsigned int* datos = loadSeedMasking(archivosTxt[i], seed, n_pixels);
        if (!datos) continue;

        int mascaraSize = n_pixels * 3;

        // Usar la función para probar transformaciones
        probarTransformaciones(imagenTransformada, mascara, width, height, totalSize,
                               datos, seed, mascaraSize, archivosTxt[i]);

        delete[] datos;
    }

    // Liberar recursos
    for (int i = 0; i < cantidadTxt; ++i)
        delete[] archivosTxt[i];
    delete[] archivosTxt;
    delete[] imagenTransformada;
    delete[] mascara;

    return 0;

}


/*
int main(){
    // Definición de rutas de archivo de entrada (imagen original) y salida (imagen modificada)
    QString archivoEntrada = "I_O.bmp";
    QString archivoSalida = "I_D.bmp";

    // Variables para almacenar las dimensiones de la imagen
    int height = 0;
    int width = 0;

    // Carga la imagen BMP en memoria dinámica y obtiene ancho y alto
    unsigned char *pixelData = loadPixels(archivoEntrada, width, height);

    // Identificación de archivos .txt que se proporcionaron para este caso
    int cantidadTxt = 0;
    char** archivosTxt = detectarArchivosDeEnmascaramiento(cantidadTxt);

    // Se inicia bucle para ir observando el contenido del .txt actual y comparando cada transformación
    // con el hasta hallar la correspondiente y pasar al siguiente .txt
    for (int i = cantidadTxt - 1; i >= 0; --i) {
        int seed = 0;
        int n_pixels = 0;

        unsigned int* datos = loadSeedMasking(archivosTxt[i], seed, n_pixels);
        // Muestra los primeros píxeles enmascarados, hasta 5 píxeles máximo
        cout << "\n[" << archivosTxt[i] << "] Seed: " << seed << ", Píxeles: " << n_pixels << endl;
        for (int j = 0; j < min(n_pixels * 3, 15); j += 3) {
            cout << "Pixel " << j / 3 << ": ("
                 << datos[j] << ", "
                 << datos[j + 1] << ", "
                 << datos[j + 2] << ")" << endl;
        }
        // Libera los datos de este archivo
        delete[] datos;
    }

    // Se borra y libera toda la memoria relacionada a los .txts una vez se está terminando el programa
    // (parte final de programa "temporal")
    for (int i = 0; i < cantidadTxt; ++i) {
        delete[] archivosTxt[i];
    }
    delete[] archivosTxt;

    // Simula una modificación de la imagen asignando valores RGB incrementales
    // (Esto es solo un ejemplo de manipulación artificial)
    for (int i = 0; i < width * height * 3; i += 3) {
        pixelData[i] = i;     // Canal rojo
        pixelData[i + 1] = i; // Canal verde
        pixelData[i + 2] = i; // Canal azul
    }

    // Exporta la imagen modificada a un nuevo archivo BMP
    bool exportI = exportImage(pixelData, width, height, archivoSalida);

    // Muestra si la exportación fue exitosa (true o false)
    cout << exportI << endl;

    // Libera la memoria usada para los píxeles
    delete[] pixelData;
    pixelData = nullptr;

    // Variables para almacenar la semilla y el número de píxeles leídos del archivo de enmascaramiento
    int seed = 0;
    int n_pixels = 0;

    // Carga los datos de enmascaramiento desde un archivo .txt (semilla + valores RGB)
    unsigned int *maskingData = loadSeedMasking("M1.txt", seed, n_pixels);

    // Muestra en consola los primeros valores RGB leídos desde el archivo de enmascaramiento
    for (int i = 0; i < n_pixels * 3; i += 3) {
        cout << "Pixel " << i / 3 << ": ("
             << maskingData[i] << ", "
             << maskingData[i + 1] << ", "
             << maskingData[i + 2] << ")" << endl;
    }

    // Libera la memoria usada para los datos de enmascaramiento
    if (maskingData != nullptr){
        delete[] maskingData;
        maskingData = nullptr;
    }

    // Llamada a la función realizarExperimentos
    // Descomentar las siguientes líneas si se desea realizar los experimentos de transformación (XOR, rotación, desplazamiento).
    // Comentario: La función realizarExperimentos toma la imagen original y la imagen enmascarada y realiza transformaciones.

    // unsigned char* imagenOriginal = loadPixels("I_O.bmp", width, height);
    // unsigned char* imagenMascarada = loadPixels("imagenMascarada.bmp", width, height);
    // realizarExperimentos(imagenOriginal, imagenMascarada, width, height);
    // delete[] imagenOriginal;
    // delete[] imagenMascarada;

    return 0; // Fin del programa
}
*/

