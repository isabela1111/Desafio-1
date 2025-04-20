#include "enmascaramiento.h"
#include <QFile>
#include <QString>
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
// Verificación del enmascaramiento S(k) = ID(k+s) + M(k) mod 256
bool verificarEnmascaramiento(
    unsigned char* imagenTransformada, // Arreglo de los bytes RGB de la imagen después de ser transformada (XOR, Desplazamiento o Rotación)
    unsigned char* mascara, // Arreglo de los bytes RGB de la máscara M.bmp
    unsigned int* datosEnmascarados, // Arreglo con los valores RGB del .txt a comparar
    int seed, // Semilla S que está en la primera línea del .txt que se está comparando
    int mascaraSize, // Cantidad total de bytes que hay en la máscara M.bmp
    int totalSizeImagen) // Tamaño total de la imagen transformada (en bytes)
{
    // Verifica que no se intente acceder fuera del arreglo de la imagen transformada
    if (seed + mascaraSize > totalSizeImagen) {
        return false; // Si la semilla y la máscara exceden el tamaño de la imagen, se devuelve false
    }

    // Compara byte por byte: (imagenTransformada[k + seed] + mascara[k]) mod 256 debe ser igual al dato esperado
    for (int i = 0; i < mascaraSize; i++) {
        // Aplica la fórmula (imagenTransformada + mascara) mod 256
        int suma = (imagenTransformada[i + seed] + mascara[i]) & 0xFF;  // ← mod 256

        // Si la suma no es igual a los datos enmascarados esperados, no es la transformación correcta
        if (suma != (int)datosEnmascarados[i]) {
            return false; // Si un solo valor no coincide, la transformación no es correcta
        }
    }

    return true; // Si todos los valores coinciden, la transformación es correcta
}


unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels){
    /*
     * @brief Carga la semilla y los resultados del enmascaramiento desde un archivo de texto.
     *
     * Esta función abre un archivo de texto que contiene una semilla en la primera línea y,
     * a continuación, una lista de valores RGB resultantes del proceso de enmascaramiento.
     * Primero cuenta cuántos tripletes de píxeles hay, luego reserva memoria dinámica
     * y finalmente carga los valores en un arreglo de enteros.
     *
     * @param nombreArchivo Ruta del archivo de texto que contiene la semilla y los valores RGB.
     * @param seed Variable de referencia donde se almacenará el valor entero de la semilla.
     * @param n_pixels Variable de referencia donde se almacenará la cantidad de píxeles leídos
     *                 (equivalente al número de líneas después de la semilla).
     *
     * @return Puntero a un arreglo dinámico de enteros que contiene los valores RGB
     *         en orden secuencial (R, G, B, R, G, B, ...). Devuelve nullptr si ocurre un error al abrir el archivo.
     *
     * @note Es responsabilidad del usuario liberar la memoria reservada con delete[].
     */

    // Abrir el archivo que contiene la semilla y los valores RGB
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        // Verificar si el archivo pudo abrirse correctamente
        cout << "No se pudo abrir el archivo." << endl;
        return nullptr;
    }

    // Leer la semilla desde la primera línea del archivo
    archivo >> seed;

    int r, g, b;

    // Contar cuántos grupos de valores RGB hay en el archivo
    // Se asume que cada línea después de la semilla tiene tres valores (r, g, b)
    while (archivo >> r >> g >> b) {
        n_pixels++;  // Contamos la cantidad de píxeles
    }

    // Cerrar el archivo para volver a abrirlo desde el inicio
    archivo.close();
    archivo.open(nombreArchivo);

    // Verificar que se pudo reabrir el archivo correctamente
    if (!archivo.is_open()) {
        cout << "Error al reabrir el archivo." << endl;
        return nullptr;
    }

    // Reservar memoria dinámica para guardar todos los valores RGB
    // Cada píxel tiene 3 componentes: R, G y B
    unsigned int* RGB = new unsigned int[n_pixels * 3];

    // Leer nuevamente la semilla desde el archivo (se descarta su valor porque ya se cargó antes)
    archivo >> seed;

    // Leer y almacenar los valores RGB uno por uno en el arreglo dinámico
    for (int i = 0; i < n_pixels * 3; i += 3) {
        archivo >> r >> g >> b;
        RGB[i] = r;
        RGB[i + 1] = g;
        RGB[i + 2] = b;
    }

    // Cerrar el archivo después de terminar la lectura
    archivo.close();

    // Mostrar información de control en consola
    cout << "Semilla: " << seed << endl;
    cout << "Cantidad de píxeles leídos: " << n_pixels << endl;

    // Retornar el puntero al arreglo con los datos RGB
    return RGB;
}


// Función que detecta los archivos de enmascaramiento y saber la cantidad que hay
char** detectarArchivosDeEnmascaramiento(const QString& carpeta, int& cantidad) {
    int capacidad = 10;  // Capacidad inicial del arreglo de nombres de archivos
    char** archivos = new char*[capacidad]; // Arreglo dinámico para almacenar nombres de archivos
    cantidad = 0; // Contador de archivos encontrados

    // Bucle infinito que va probando con M1.txt, M2.txt, etc... ya que tenemos el patrón de que las pistas son nombradas como M junto al número de su orden
    for (int i = 0;; ++i) {
        // Crea el nombre del archivo como QString: "M1.txt", "M2.txt", etc...
        QString nombre = QString("M%1.txt").arg(i);

        // Crea la ruta completa del archivo combinando la carpeta y el nombre
        QString rutaCompleta = carpeta + "/" + nombre;

        // Verifica si el archivo existe en el sistema
        QFile archivo(rutaCompleta);
        if (!archivo.exists()) break; // Si no existe nos detenemos ahí

        // Convierte el QString a arreglo de caracteres (char*) usando QByteArray
        QByteArray nombreBytes = rutaCompleta.toLocal8Bit();

        // Reservamos espacio y copiamos el nombre al arreglo de nombres
        archivos[cantidad] = new char[nombreBytes.size() + 1]; // +1 para el carácter nulo
        strncpy(archivos[cantidad], nombreBytes.constData(), nombreBytes.size());
        archivos[cantidad][nombreBytes.size()] = '\0';  // Asegurarse de que el string termine en '\0'

        cantidad++; // Aumentamos el contador de archivos detectados

        // Si llegamos al límite de capacidad significando que hay más de una decena de pistas, duplicamos el tamaño del arreglo
        if (cantidad == capacidad) {
            int nuevaCapacidad = capacidad * 2;
            char** nuevo = new char*[nuevaCapacidad]; // Nuevo arreglo más grande

            // Copiamos todos los nombres anteriores al nuevo arreglo
            for (int j = 0; j < cantidad; ++j) {
                nuevo[j] = archivos[j];
            }

            // Liberamos el arreglo anterior y reemplazamos por el nuevo
            delete[] archivos;
            archivos = nuevo;
            capacidad = nuevaCapacidad;
        }
    }

    return archivos; // Finalmente retornamos el arreglo con los nombres detectados
}
