#include "rework.h"

// This file contains application procedures.

rework::rework(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    //fill combobox here with new filter
    ui.comboBox->addItem("Sparse-cmyk");
    ui.comboBox->addItem("Dense-cmyk");
    ui.comboBox->addItem("Closest-cmyk");
    ui.comboBox->addItem("Dithering-cmyk");
    ui.comboBox->addItem("Black & White");
    ui.comboBox->addItem("Densecmyk");
    ui.comboBox->addItem("Densecmyk");

}

rework::~rework()
{}

void rework::on_openButton_clicked() {
	try{
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.xpm *.jpg *.ppm)"));
        if (!fileName.isEmpty()) {
            image0.load(fileName);
            if (!image0.isNull()) {
                ui.imageLabel1->setPixmap(QPixmap::fromImage(image0));
            }
            else {

            }
        }
	}
	catch (const std::exception&)
	{

	}
}


////testing compatibility
//QVector<QVector<QColor>> to_modify = Filters::imageToVector(image0);
//Filters::densecmyk(&to_modify);
//image2 = Filters::vectorToImage(to_modify);
//ui.imageLabel2->setPixmap(QPixmap::fromImage(image2));

/*image1 = image0;
Filters::dithercmyk(image1);
ui.imageLabel2->setPixmap(QPixmap::fromImage(image1));*/


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

void rework::on_comboBox_activated(int index) {
    ui.imageLabel2->clear();
    image1 = image0;
    QVector<QVector<QColor>> to_modify;
    switch (index)
    {case 0:
        to_modify = Filters::imageToVector(image1);
        Filters::sparsecmyk(&to_modify);
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 1:
        to_modify = Filters::imageToVector(image1);
        Filters::densecmyk(&to_modify);
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 2:
        Filters::closestcmyk(image1);
        break;
    case 3:
        Filters::dithercmyk(image1);
        break;
    default:
        break;
    }
    ui.imageLabel2->setPixmap(QPixmap::fromImage(image1));
}