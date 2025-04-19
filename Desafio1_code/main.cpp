#include <QCoreApplication>
#include <QImage>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <cstring>
#include <QDebug>
#include "utilidades.h"
#include "transformaciones.h"
#include "enmascaramiento.h"

using namespace std;

int main() {
    // Archivos de entrada
    QString archivoTransformada = "C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/I_D.bmp"; //I_D.bmp
    QString archivoMascara = "C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/M.bmp"; //M.bmp
    QString archivoSalida = "C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/I_O-f.bmp";

    // Variables para ancho y alto de la imagen
    int width = 0;
    int height = 0;

    // Cargar imagen transformada y máscara
    unsigned char* imagenTransformada = loadPixels(archivoTransformada, width, height);
    unsigned char* mascara = loadPixels(archivoMascara, width, height);

    // Verificación de carga de imágenes
    if (!imagenTransformada) {
        qDebug() << "Error al cargar la imagen transformada: " << archivoTransformada;
        return -1;
    }
    if (!mascara) {
        qDebug() << "Error al cargar la máscara: " << archivoMascara;
        return -1;
    }

    qDebug() << "Imagen transformada y máscara cargadas exitosamente.";

    // Tamaño total de la imagen (ancho * alto * 3 canales RGB)
    int totalSize = width * height * 3;

    // Detectar archivos .txt con pistas
    int cantidadTxt = 0;
    char** archivosTxt = detectarArchivosDeEnmascaramiento("C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2", cantidadTxt);

    // Verificación de archivos .txt detectados
    if (cantidadTxt == 0) {
        qDebug() << "No se encontraron archivos de enmascaramiento (.txt) en la carpeta.";
        return -1;
    }

    // Crear una lista de QString para manejar los archivos
    QStringList archivosTxtList;
    for (int i = 0; i < cantidadTxt; ++i) {
        archivosTxtList.append(QString::fromLocal8Bit(archivosTxt[i]));
    }

    // Iterar sobre los archivos de enmascaramiento
    for (int i = archivosTxtList.size() - 1; i >= 0; --i) {
        QString archivoTxtQString = archivosTxtList[i];

        int seed = 0;
        int n_pixels = 0;
        unsigned int* datos = loadSeedMasking(archivosTxtList[i].toLocal8Bit().data(), seed, n_pixels);

        // Verificación de carga de semillas
        if (!datos) {
            qDebug() << "No se pudieron cargar los datos de enmascaramiento desde el archivo: " << archivoTxtQString;
            continue;
        }

        qDebug() << "Archivo de semillas cargado exitosamente: " << archivoTxtQString;

        int mascaraSize = n_pixels * 3;

        // Usar la función para probar transformaciones
        probarTransformaciones(imagenTransformada, mascara, width, height, totalSize,
                               datos, seed, mascaraSize, archivosTxtList[i].toLocal8Bit().data());

        // Exportar la imagen intermedia después de cada transformación
        QString archivoIntermedio = QString("C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/Paso%1.bmp").arg(i + 1);
        if (exportImage(imagenTransformada, width, height, archivoIntermedio)) {
            qDebug() << "Imagen intermedia guardada en: " << archivoIntermedio;
        } else {
            qDebug() << "Error al guardar la imagen intermedia en: " << archivoIntermedio;
        }

        delete[] datos;
    }

    // Guardar la imagen final
    if (exportImage(imagenTransformada, width, height, archivoSalida)) {
        qDebug() << "Imagen exportada exitosamente a: " << archivoSalida;
    } else {
        qDebug() << "Error al exportar la imagen a: " << archivoSalida;
    }

    // Liberar recursos
    for (int i = 0; i < cantidadTxt; ++i)
        delete[] archivosTxt[i];
    delete[] archivosTxt;
    delete[] imagenTransformada;
    delete[] mascara;

    return 0;
}
