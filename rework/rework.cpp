#include "rework.h"

// This file contains application procedures.

rework::rework(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //fill combobox here with new filter

}

rework::~rework()
{}

void rework::on_openButton_clicked() {
	try{
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.xpm *.jpg *.ppm)"));
        if (!fileName.isEmpty()) {
            QImage image0(fileName);
            if (!image0.isNull()) {
                ui.imageLabel1->setPixmap(QPixmap::fromImage(image0));

                //testing compatibility
                /*QVector<QVector<QColor>> to_modify = Filters::imageToVector(image);
                Filters::OLDdensecmykFilter(&to_modify);
                QImage image2 = Filters::vectorToImage(to_modify);
                ui.imageLabel2->setPixmap(QPixmap::fromImage(image2));*/

                image1 = image0;
                Filters::dithercmyk(image1);
                ui.imageLabel2->setPixmap(QPixmap::fromImage(image1));
            }
            else {

            }
        }
	}
	catch (const std::exception&)
	{

	}
}

void rework::on_saveButton_clicked() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.jpg *.bmp *.ppm);;All Files (*)"));
    if (filePath.isEmpty()) {
        ui.errorLabel->setText("filepath empty");
        return;
    }
    QImageWriter imageWriter(filePath);
    imageWriter.setFormat("png");
    //imageWriter.setQuality(80);
    if (imageWriter.format().isEmpty()) {
        ui.errorLabel->setText("format empty");
        return;
    }
    if (imageWriter.write(image1)) {
        // Image saved successfully
        ui.errorLabel->setText("saved");
    }
    else {
        ui.errorLabel->setText("Writer failed: " + imageWriter.errorString());
    }
}