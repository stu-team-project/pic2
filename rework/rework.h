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
    static void blur(QVector<QVector<QColor>>* VecOfPixelsColor2D, int height, int width, int variance)
    {
        const QVector<QVector<QColor>> tmpVecOfPixelsColor2D = *VecOfPixelsColor2D;
        QVector<QColor>boundaryOfPixel;
        QVector<QColor>tmpVec;
        QVector<int> RGB;
        QColor blurColor, averageColor;

        averageColor = getAveragePixel(VecOfPixelsColor2D);

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                boundaryOfPixel = fillMask(&tmpVecOfPixelsColor2D, averageColor, i, j, height, width, variance);

                RGB = gaussMask(&boundaryOfPixel, variance);
                blurColor.setRed(RGB.at(0));
                blurColor.setGreen(RGB.at(1));
                blurColor.setBlue(RGB.at(2));

                tmpVec.append(blurColor);
                boundaryOfPixel.clear();
            }
            VecOfPixelsColor2D->replace(i, tmpVec);
            tmpVec.clear();
        }
    }
    static QVector<QColor> fillMask(const QVector<QVector<QColor>>* tmpVecOfPixelsColor2D, QColor averageColor, int i, int j, int height, int width, int variance)
    {
        QVector<QColor> vecOfMask;

        for (int x = i - variance; x <= i + variance; x++)
        {
            for (int y = j - variance; y <= j + variance; y++)
            {
                if (x < 0 || y < 0 || x >= height || y >= width)
                {
                    vecOfMask.append(averageColor);
                }
                else
                {
                    vecOfMask.append(tmpVecOfPixelsColor2D->at(x).at(y));
                }
            }
        }

        return vecOfMask;
    }
    static QColor getAveragePixel(const QVector<QVector<QColor>>* VecOfPixelsColor2D)
    {
        QColor averageColor;
        int red(0), green(0), blue(0), sum(0);
        int height = VecOfPixelsColor2D->size();
        int width = VecOfPixelsColor2D->at(0).size();
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                red += VecOfPixelsColor2D->at(i).at(j).red();
                green += VecOfPixelsColor2D->at(i).at(j).green();
                blue += VecOfPixelsColor2D->at(i).at(j).blue();
                sum++;
            }
        }
        averageColor.setRed(red / sum);
        averageColor.setGreen(green / sum);
        averageColor.setBlue(blue / sum);

        return averageColor;
    }
    static QVector<int> gaussMask(QVector<QColor>* mask, int variance)
    {
        if (variance > 3 or variance < 1) { variance = 3; };
        QVector<int> returnRGB;
        QVector<int> valueOfMask;
        if (variance == 1)
        {
            valueOfMask = { 1,2,1,2,4,2,1,2,1 };
        }
        else if (variance == 2)
        {
            valueOfMask = { 1,4,7,4,1,4,16,26,16,4,7,26,41,26,7,4,16,26,16,4,1,4,7,4,1 };
        }
        else if (variance == 3)
        {
            valueOfMask = { 0,0,1,2,1,0,0,0,3,13,22,13,3,0,1,13,59,97,59,13,1,2,22,97,159,97,22,2,1,13,59,97,59,13,1,0,3,13,22,13,3,0,0,0,1,2,1,0,0 };
        }
        else
        {
            qDebug() << "incorrect value of Variance in function gaussMask";
        }

        int red(0), green(0), blue(0), sumOfMaskValues(0);

        for (int i = 0; i < valueOfMask.size(); i++)
        {
            red += mask->at(i).red() * valueOfMask.at(i);
            green += mask->at(i).green() * valueOfMask.at(i);
            blue += mask->at(i).blue() * valueOfMask.at(i);
            sumOfMaskValues += valueOfMask.at(i);
        }

        red = red / sumOfMaskValues;
        returnRGB.append(red);

        green = green / sumOfMaskValues;
        returnRGB.append(green);

        blue = blue / sumOfMaskValues;
        returnRGB.append(blue);

        return returnRGB;
    }
    static QVector<int> SobelOperatorX(QVector<QColor>* mask)
    {
        QVector<int> returnRGB;
        QVector<int> valueOfMask{ -1, 0, 1, -2, 0, 2, -1, 0, 1 };
        int red(0), green(0), blue(0);

        for (int i = 0; i < valueOfMask.size(); i++)
        {
            red += mask->at(i).red() * valueOfMask.at(i);
            green += mask->at(i).green() * valueOfMask.at(i);
            blue += mask->at(i).blue() * valueOfMask.at(i);
        }

        red = red / 16;
        returnRGB.append(red);

        green = green / 16;
        returnRGB.append(green);

        blue = blue / 16;
        returnRGB.append(blue);

        return returnRGB;
    }
    static QVector<int> SobelOperatorY(QVector<QColor>* mask)
    {
        QVector<int> returnRGB;
        QVector<int> valueOfMask{ -1, -2, -1, 0, 0, 0, 1, 2, 1 };
        int red(0), green(0), blue(0);

        for (int i = 0; i < valueOfMask.size(); i++)
        {
            red += mask->at(i).red() * valueOfMask.at(i);
            green += mask->at(i).green() * valueOfMask.at(i);
            blue += mask->at(i).blue() * valueOfMask.at(i);
        }

        red = red / 16;
        returnRGB.append(red);

        green = green / 16;
        returnRGB.append(green);

        blue = blue / 16;
        returnRGB.append(blue);

        return returnRGB;
    }

};