#include <QCoreApplication>
#include <QImage>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include "utilidades.h"
#include "transformaciones.h"
#include "enmascaramiento.h"

int main() {
    // Archivos de entrada
    QString archivoTransformada = "C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/I_D.bmp";
    QString archivoMascara = "C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/M.bmp";
    QString archivoSalida = "C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/I_O-f.bmp";

    int width = 0;
    int height = 0;

    // Cargar imagen transformada
    unsigned char* imagenTransformada = loadPixels(archivoTransformada, width, height);
    if (!imagenTransformada) {
        qDebug() << "Error al cargar la imagen transformada.";
        return -1;
    }

    qDebug() << "Imagen transformada cargada con tamaño:" << width << "x" << height;

    // Cargar imagen máscara
    int widthM = 0, heightM = 0;
    unsigned char* mascara = loadPixels(archivoMascara, widthM, heightM);
    if (!mascara) {
        qDebug() << "Error al cargar la máscara.";
        delete[] imagenTransformada;
        return -1;
    }

    qDebug() << "Máscara cargada con tamaño:" << widthM << "x" << heightM;

    // Redimensionar la máscara si es necesario
    if (width != widthM || height != heightM) {
        qDebug() << "Redimensionando la máscara para que coincida con la imagen";
        unsigned char* mascaraRedimensionada = new unsigned char[width * height * 3];

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int srcIndex = ((y % heightM) * widthM + (x % widthM)) * 3;
                int destIndex = (y * width + x) * 3;

                mascaraRedimensionada[destIndex] = mascara[srcIndex];
                mascaraRedimensionada[destIndex + 1] = mascara[srcIndex + 1];
                mascaraRedimensionada[destIndex + 2] = mascara[srcIndex + 2];
            }
        }

        delete[] mascara;
        mascara = mascaraRedimensionada;
    }

    int totalSize = width * height * 3;

    // Detectar archivos .txt con pistas
    int cantidadTxt = 0;
    char** archivosTxt = detectarArchivosDeEnmascaramiento("C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2", cantidadTxt);

    if (cantidadTxt == 0) {
        qDebug() << "No se encontraron archivos de enmascaramiento (.txt).";
        delete[] imagenTransformada;
        delete[] mascara;
        return -1;
    }

    // Procesar cada archivo .txt
    for (int i = 0; i < cantidadTxt; ++i) {
        QString rutaTxt = QString::fromLocal8Bit(archivosTxt[i]);

        int seed = 0;
        int n_pixels = 0;
        unsigned int* datos = loadSeedMasking(rutaTxt.toLocal8Bit().data(), seed, n_pixels);
        if (!datos) {
            qDebug() << "No se pudieron cargar los datos de enmascaramiento desde:" << rutaTxt;
            continue;
        }

        int mascaraSize = n_pixels * 3;

        // Aplicar transformación
        probarTransformaciones(imagenTransformada, mascara, width, height, totalSize,
                               datos, seed, mascaraSize, rutaTxt.toLocal8Bit().data());

        // Guardar imagen intermedia solo si NO es el último archivo
        if (i < cantidadTxt - 1) {
            QString nombrePaso = QString("C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/Paso%1.bmp").arg(i + 1);
            if (exportImage(imagenTransformada, width, height, nombrePaso)) {
                qDebug() << "Imagen intermedia guardada en:" << nombrePaso;
            } else {
                qDebug() << "Error al guardar imagen intermedia.";
            }
        }

        delete[] datos;
    }

    // Guardar imagen final después de aplicar TODAS las transformaciones
    if (exportImage(imagenTransformada, width, height, archivoSalida)) {
        qDebug() << "Imagen final exportada correctamente.";
    } else {
        qDebug() << "Error al exportar la imagen final.";
    }

    // Liberar memoria
    for (int i = 0; i < cantidadTxt; ++i)
        delete[] archivosTxt[i];
    delete[] archivosTxt;
    delete[] imagenTransformada;
    delete[] mascara;

    return 0;
}
