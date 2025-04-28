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
    QString archivoIM = "C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/I_M.bmp";
    QString archivoSalida = "C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2/I_O_F.bmp";

    int widthIM = 0 ;
    int heightIM = 0;
    unsigned char* imagenIM = loadPixels(archivoIM, widthIM, heightIM);
    if (!imagenIM) {
        qDebug() << "Error al cargar IM.bmp";
        return -1;
    }

    int width = 0;
    int height = 0;

    // Cargar imagen transformada
    unsigned char* imagenTransformada = loadPixels(archivoTransformada, width, height);
    if (!imagenTransformada) {
        qDebug() << "Error al cargar la imagen transformada.";
        delete[] imagenIM;
        return -1;
    }

    qDebug() << "Imagen transformada cargada con tamaño:" << width << "x" << height;

    // Cargar máscara
    int widthM = 0, heightM = 0;
    unsigned char* mascaraOriginal = loadPixels(archivoMascara, widthM, heightM);
    if (!mascaraOriginal) {
        qDebug() << "Error al cargar la máscara.";
        delete[] imagenTransformada;
        delete[] imagenIM;
        return -1;
    }

    qDebug() << "Máscara cargada con tamaño:" << widthM << "x" << heightM;

    // Redimensionar máscara si es necesario usando la función redimensionarMascara
    unsigned char* mascara = mascaraOriginal;
    if (width != widthM || height != heightM) {
        qDebug() << "Redimensionando la máscara para que coincida con la imagen";
        mascara = redimensionarMascara(mascaraOriginal, widthM, heightM, width, height);
        delete[] mascaraOriginal;  // Liberar la memoria de la máscara original
    }

    int totalSize = width * height * 3;

    // Detectar archivos .txt
    int cantidadTxt = 0;
    char** archivosTxt = detectarArchivosDeEnmascaramiento("C:/Users/Isabela/OneDrive/Documents/Informática 2/GitHub/Desafio-1/Caso 2", cantidadTxt);
    if (cantidadTxt == 0) {
        qDebug() << "No se encontraron archivos de enmascaramiento (.txt).";
        delete[] imagenTransformada;
        delete[] imagenIM;
        delete[] mascara;
        return -1;
    }

    // Procesar cada archivo .txt
    for (int i = 0; i < cantidadTxt; ++i) {
        QString rutaTxt = QString::fromLocal8Bit(archivosTxt[i]);
        qDebug() << "\nProcesando" << rutaTxt;

        int seed = 0;
        int n_pixels = 0;
        unsigned int* datos = loadSeedMasking(rutaTxt.toLocal8Bit().data(), seed, n_pixels);
        if (!datos) {
            qDebug() << "No se pudieron cargar los datos desde:" << rutaTxt;
            continue;
        }

        int mascaraSize = n_pixels * 3;

        if (seed + mascaraSize > totalSize) {
            qDebug() << "La máscara y semilla exceden el tamaño de la imagen. Archivo omitido:" << rutaTxt;
            delete[] datos;
            continue;
        }

        // Aplicar transformación
        probarTransformaciones(imagenTransformada, imagenIM, mascara,width, height, totalSize,
                               datos, seed, mascaraSize,rutaTxt.toLocal8Bit().data());


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

    // Guardar imagen final
    if (exportImage(imagenTransformada, width, height, archivoSalida)) {
        qDebug() << "\nImagen final exportada correctamente en:" << archivoSalida;
    } else {
        qDebug() << "Error al exportar la imagen final.";
    }

    // Liberar memoria
    for (int i = 0; i < cantidadTxt; ++i)
        delete[] archivosTxt[i];
    delete[] archivosTxt;
    delete[] imagenTransformada;
    delete[] imagenIM;
    delete[] mascara;

    return 0;
}
