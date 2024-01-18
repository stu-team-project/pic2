#pragma once

// This file contains
// 1. class rework and procedures' definitions
// 2. compatibility functions
// 3. filters

#include <QtWidgets/QMainWindow>
#include "ui_rework.h"

#include <QImage>
//https://doc.qt.io/qt-6/qimage.html#reading-and-writing-image-files

#include <QFileDialog>

#include <QVector>
#include <QColor>

class rework : public QMainWindow
{
    Q_OBJECT

public:
    rework(QWidget *parent = nullptr);
    ~rework();

private:
    Ui::reworkClass ui;
    QImage start_image, new_image, new_image2;

private slots:
    void on_openButton_clicked();
};

//Comptibility with older functions?
QVector<QVector<QColor>> imageToVector(const QImage& image) {
    QVector<QVector<QColor>> result;
    for (int y = 0; y < image.height(); ++y) {
        QVector<QColor> row;
        for (int x = 0; x < image.width(); ++x) {
            row.append(image.pixelColor(x, y));
        }
        result.append(row);
    }
    return result;
}

QImage vectorToImage(const QVector<QVector<QColor>>& colorVector) {
    int width = colorVector.isEmpty() ? 0 : colorVector[0].size();
    int height = colorVector.size();
    QImage result(width, height, QImage::Format_ARGB32); // not sure about the format
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            result.setPixelColor(x, y, colorVector[y][x]);
        }
    }
    return result;
}
