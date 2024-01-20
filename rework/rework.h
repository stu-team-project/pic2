#pragma once

// This file contains
// 0. libraries
// 1. class rework and procedures' declarations
// 2. class for filters and compatibility functions (immediately definition)

#include <QtWidgets/QMainWindow>
#include "ui_rework.h"

#include <QImage>
//https://doc.qt.io/qt-6/qimage.html#reading-and-writing-image-files

#include <QFileDialog>
#include <QImageWriter>

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
    QImage image0, image1, image2, image3; //0 original, 1 output, 2 and 3 helpers

private slots:
    void on_openButton_clicked();
    void on_saveButton_clicked();
    void on_comboBox_activated(int index);
};

class Filters
{
public:

    //Comptibility with older functions
    static QVector<QVector<QColor>> imageToVector(QImage& image) {
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
    static QImage vectorToImage(QVector<QVector<QColor>>& colorVector) {
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

    // Nikita's old filters:
    static int randlimit(int limit) { //run """srand((unsigned int)time(NULL));""" first
        return rand() % limit;
    }
    static void sparsecmyk(QVector<QVector<QColor>>* VecOfPixelsColor2D)
    {
        //no halftone
        //use 4pixel dithering
        srand((unsigned int)time(NULL));
        QVector<QColor> tmpVec;
        QColor newColor;
        for (int i = 0; i < VecOfPixelsColor2D->size(); i++) {
            for (int j = 0; j < VecOfPixelsColor2D->at(i).size(); j++) {
                int c, m, y, k;
                VecOfPixelsColor2D->at(i).at(j).getCmyk(&c, &m, &y, &k);
                newColor.setCmyk(0, 0, 0, 0);
                if (i % 2 == 0 && j % 2 == 0) {
                    if (c > randlimit(255)) { newColor.setCmyk(255, 0, 0, 0); }
                }
                else if (i % 2 == 1 && j % 2 == 0) {
                    if (m > randlimit(255)) { newColor.setCmyk(0, 255, 0, 0); }
                }
                else if (i % 2 == 0 && j % 2 == 1) {
                    if (y > randlimit(255)) { newColor.setCmyk(0, 0, 255, 0); }
                }
                else if (i % 2 == 1 && j % 2 == 1) {
                    if (k > randlimit(255)) { newColor.setCmyk(0, 0, 0, 255); }
                }
                tmpVec.append(newColor);
            }
            VecOfPixelsColor2D->replace(i, tmpVec);
            tmpVec.clear();
        }
    }
    static void densecmyk(QVector<QVector<QColor>>* VecOfPixelsColor2D)
    {
        srand((unsigned int)time(NULL));
        QVector<QColor> tmpVec;
        QColor newColor;
        int decider;
        int pixel;
        for (int i = 0; i < VecOfPixelsColor2D->size(); i++) {
            for (int j = 0; j < VecOfPixelsColor2D->at(i).size(); j++) {
                int c, m, y, k;
                VecOfPixelsColor2D->at(i).at(j).getCmyk(&c, &m, &y, &k);
                pixel = (j % 2) + 2 * (i % 2);
                newColor.setCmyk(0, 0, 0, 0);
                switch (pixel)
                {
                case 0:
                    decider = randlimit(m + y + k + 1);
                    if (decider < m) { newColor.setCmyk(0, 255, 0, 0); }
                    else if (decider < m + y) { newColor.setCmyk(0, 0, 255, 0); }
                    else { newColor.setCmyk(0, 0, 0, 255); }
                    break;
                case 1:
                    decider = randlimit(c + y + k + 1);
                    if (decider < c) { newColor.setCmyk(255, 0, 0, 0); }
                    else if (decider < c + y) { newColor.setCmyk(0, 0, 255, 0); }
                    else { newColor.setCmyk(0, 0, 0, 255); }
                    break;
                case 2:
                    decider = randlimit(c + m + k + 1);
                    if (decider < c) { newColor.setCmyk(255, 0, 0, 0); }
                    else if (decider < c + m) { newColor.setCmyk(0, 255, 0, 0); }
                    else { newColor.setCmyk(0, 0, 0, 255); }
                    break;
                case 3:
                    decider = randlimit(c + m + y + 1);
                    if (decider < c) { newColor.setCmyk(255, 0, 0, 0); }
                    else if (decider < c + m) { newColor.setCmyk(0, 255, 0, 0); }
                    else { newColor.setCmyk(0, 0, 255, 0); }
                    break;
                default:
                    break;
                }
                tmpVec.append(newColor);
            }
            VecOfPixelsColor2D->replace(i, tmpVec);
            tmpVec.clear();
        }
    }
    //Nikita's new filters
    static int bounds(int a, int lb, int rb){
        int result = a < lb ? lb : a;
        result = a < rb ? a : rb;
        return result;
    }
    static QColor modifyColor(QColor& color, int errorR, int errorG, int errorB, int weight) { 
        QColor newColor;
        newColor.setRed(bounds(color.red() + errorR * weight / 16, 0, 255));
        newColor.setGreen(bounds( color.green() + errorG * weight / 16, 0, 255));
        newColor.setBlue(bounds( color.blue() + errorB * weight / 16, 0, 255));
        return newColor;
    }
    static QColor findClosestColor(QColor& color, QVector<QColor>& colorset) {
        int minDist = INT_MAX;
        QColor closestColor;
        for (const QColor& col : colorset) {
            int dist = qPow(col.red() - color.red(), 2) +
                qPow(col.green() - color.green(), 2) +
                qPow(col.blue() - color.blue(), 2);
            if (dist < minDist) {
                minDist = dist;
                closestColor = col;
            }
        }
        return closestColor;
    }
    static void closestcmyk(QImage& image) {
        QVector<QColor> colorset = {
            QColor::fromRgb(0, 0, 0),      // Black
            QColor::fromRgb(255, 255, 255),// White
            QColor::fromRgb(0, 255, 255),  // Cyan
            QColor::fromRgb(255, 0, 255),  // Magenta
            QColor::fromRgb(255, 255, 0)   // Yellow
        };
        int width = image.width();
        int height = image.height();
        QColor oldColor, newColor;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                QColor oldColor = image.pixelColor(x, y);
                QColor newColor = findClosestColor(oldColor, colorset);
                image.setPixelColor(x, y, newColor);
            }
        }
    }
    static void dithercmyk(QImage& image) {
        // https://en.wikipedia.org/wiki/Floyd%E2%80%93Steinberg_dithering
        QVector<QColor> colorset = {
            QColor::fromRgb(0, 0, 0),      // Black
            QColor::fromRgb(255, 255, 255),// White
            QColor::fromRgb(0, 255, 255),  // Cyan
            QColor::fromRgb(255, 0, 255),  // Magenta
            QColor::fromRgb(255, 255, 0)   // Yellow
        };
        int width = image.width();
        int height = image.height();
        QColor oldColor, newColor;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                QColor oldColor = image.pixelColor(x,y);
                QColor newColor = findClosestColor(oldColor, colorset);
                image.setPixelColor(x,y,newColor);

                int errorR = oldColor.red() - newColor.red();
                int errorG = oldColor.green() - newColor.green();
                int errorB = oldColor.blue() - newColor.blue();

                //and now about neighbours
                //I intentionally set coeffs to 7,3,3,3 instead of 7,3,5,1. Looks better
                QColor neighbourColor;
                if (x + 1 < width) {
                    oldColor = image.pixelColor(x + 1, y);
                    newColor = modifyColor(oldColor, errorR, errorG, errorB, 7);
                    image.setPixelColor(x+1, y, newColor);
                }
                if (y + 1 < height) {
                    if (x - 1 >= 0) {
                        oldColor = image.pixelColor(x -1, y+1);
                        newColor = modifyColor(oldColor, errorR, errorG, errorB, 3);
                        image.setPixelColor(x-1, y+1, newColor);
                    }
                    oldColor = image.pixelColor(x, y + 1);
                    newColor = modifyColor(oldColor, errorR, errorG, errorB, 3);
                    image.setPixelColor(x, y + 1, newColor);
                    if (x + 1 < width) {
                        oldColor = image.pixelColor(x + 1, y + 1);
                        newColor = modifyColor(oldColor, errorR, errorG, errorB, 3);
                        image.setPixelColor(x + 1, y + 1, newColor);
                    }
                }
            }
        }
    }
    static QVector<QColor> zone3x3(QImage& input, int x, int y, int lr, int ud) {
        // lr can be 0 for right or -1 for left
        // ud can be 0 for down or -1 for up
        QVector<QColor> result;
        for (int i = 0 + lr*2; i < 3 + lr*2; i++) {
            for (int j = 0 + ud*2; j < 3 + ud*2; j++) {
                if (x + i < input.width() && x + i >= 0 &&
                    y + j < input.width() && y + j >= 0) {
                    result.append(input.pixelColor(x + i, y + j));
                }
            }
        }
        return result;
    }
    static void varstat(QVector<QColor> zone, QColor& average, int& variance) {
        int red, gree, blue;
        
    }
    static void kuwahara(QImage& input, QImage& output) {

    }


    // Janci filters:

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
    static void findingEdge(QVector<QVector<QColor>>* VecOfPixelsColor2D, int height, int width)
    {
        const QVector<QVector<QColor>> tmpVecOfPixelsColor2D = *VecOfPixelsColor2D;
        QVector<QColor>boundaryOfPixel;
        QVector<QColor>tmpVec;
        QVector<int> RGBForXDirection, RGBForYDirection;
        QColor blurColor, averageColor;
        int red(0), green(0), blue(0), sum(0);

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

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                boundaryOfPixel = fillMask(&tmpVecOfPixelsColor2D, averageColor, i, j, height, width, 1);

                RGBForXDirection = SobelOperatorX(&boundaryOfPixel);
                RGBForYDirection = SobelOperatorY(&boundaryOfPixel);

                red = (RGBForXDirection.at(0) * RGBForXDirection.at(0) + RGBForYDirection.at(0) * RGBForYDirection.at(0));
                red = pow(red, 0.5);
                green = (RGBForXDirection.at(1) * RGBForXDirection.at(1) + RGBForYDirection.at(1) * RGBForYDirection.at(1));
                green = pow(green, 0.5);
                blue = (RGBForXDirection.at(2) * RGBForXDirection.at(2) + RGBForYDirection.at(2) * RGBForYDirection.at(2));
                blue = pow(blue, 0.5);


                blurColor.setRed(red);
                blurColor.setGreen(green);
                blurColor.setBlue(blue);

                tmpVec.append(blurColor);
                boundaryOfPixel.clear();
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
    static void blur3x3(QVector<QVector<QColor>>* VecOfPixelsColor2D, int height, int width)
    {
        blur(VecOfPixelsColor2D, height, width, 1);
    }
    static void blur5x5(QVector<QVector<QColor>>* VecOfPixelsColor2D, int height, int width)
    {
        blur(VecOfPixelsColor2D, height, width, 2);
    }
    static void blur7x7(QVector<QVector<QColor>>* VecOfPixelsColor2D, int height, int width)
    {
        blur(VecOfPixelsColor2D, height, width, 3);
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