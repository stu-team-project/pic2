#pragma once

// This file contains
// 1. class rework and procedures' declarations
// 2. class for filters and compatibility functions (immediately definition)

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

class Filters
{
public:

    //Comptibility with older functions

    static QVector<QVector<QColor>> imageToVector(const QImage& image) {
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

    static QImage vectorToImage(const QVector<QVector<QColor>>& colorVector) {
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

    // Janci filters unchanged:

    static void blackAndWhite(QVector<QVector<QColor>>* VecOfPixelsColor2D, int height, int width) {
        int gray, red, green, blue;
        QVector<QColor>tmpVec;
        QColor grayTmpColor;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                red = VecOfPixelsColor2D->at(i).at(j).red();
                green = VecOfPixelsColor2D->at(i).at(j).green();
                blue = VecOfPixelsColor2D->at(i).at(j).blue();

                gray = (red + green + blue) / 3;

                grayTmpColor.setRed(gray);
                grayTmpColor.setGreen(gray);
                grayTmpColor.setBlue(gray);

                tmpVec.append(grayTmpColor);
            }
            VecOfPixelsColor2D->replace(i, tmpVec);
            tmpVec.clear();
        }
    }
    static void inverse(QVector<QVector<QColor>>* VecOfPixelsColor2D, int height, int width)
    {
        int diff;
        const int maxValue = 255;
        const int minValue = 0;
        QVector<QColor>tmpVec;
        QColor inverseColor;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                diff = maxValue - VecOfPixelsColor2D->at(i).at(j).red();
                inverseColor.setRed(minValue + diff);
                diff = maxValue - VecOfPixelsColor2D->at(i).at(j).green();
                inverseColor.setGreen(minValue + diff);
                diff = maxValue - VecOfPixelsColor2D->at(i).at(j).blue();
                inverseColor.setBlue(minValue + diff);

                tmpVec.append(inverseColor);
            }
            VecOfPixelsColor2D->replace(i, tmpVec);
            tmpVec.clear();
        }
    }
    static void redFilter(QVector<QVector<QColor>>* VecOfPixelsColor2D, int height, int width)
    {
        int red, green, blue, average;
        QVector<QColor>tmpVec;
        QColor redTmpColor;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                red = VecOfPixelsColor2D->at(i).at(j).red();
                green = VecOfPixelsColor2D->at(i).at(j).green();
                blue = VecOfPixelsColor2D->at(i).at(j).blue();

                average = (red + green + blue) / 3;

                if (red + 100 > 255)
                {
                    red = 255;
                }
                else
                {
                    red += 100;
                }

                redTmpColor.setRed(red);
                redTmpColor.setGreen(green);
                redTmpColor.setBlue(blue);

                tmpVec.append(redTmpColor);
            }
            VecOfPixelsColor2D->replace(i, tmpVec);
            tmpVec.clear();
        }
    }
};