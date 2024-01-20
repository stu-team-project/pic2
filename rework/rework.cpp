#include "rework.h"

// This file contains application procedures.

rework::rework(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
   
    ui.saveButton->setEnabled(false);
    ui.comboBox->setEnabled(false);
    //fill combobox here with new filters
    ui.comboBox->addItem("Sparse-cmyk"); //0
    ui.comboBox->addItem("Dense-cmyk");
    ui.comboBox->addItem("Closest-cmyk");
    ui.comboBox->addItem("Dithering-cmyk");
    ui.comboBox->addItem("Strong edge");
    ui.comboBox->addItem("Kuwahara"); //5
    ui.comboBox->addItem("ClosestRandColor");
    ui.comboBox->addItem("Comix");
    ui.comboBox->addItem("Black and white"); //8
    ui.comboBox->addItem("Inverse");
    ui.comboBox->addItem("RedFilter");
    ui.comboBox->addItem("FindingEdge"); //11
    ui.comboBox->addItem("Blur 3x3");
    ui.comboBox->addItem("Blur 5x5");
    ui.comboBox->addItem("Blur 7x7");

}
rework::~rework(){}
void rework::on_comboBox_activated(int index) {
    ui.spinBox->setEnabled(false);
    ui.spinBox_2->setEnabled(false);
    ui.imageLabel2->clear();
    image1 = image0;
    QVector<QVector<QColor>> to_modify;
    switch (index)
    {
    case 0:
        to_modify = Filters::imageToVector(image1);
        Filters::sparsecmyk(&to_modify);
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 1:
        to_modify = Filters::imageToVector(image1);
        Filters::densecmyk(&to_modify);
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 2: Filters::closestcmyk(image1); break;
    case 3: Filters::dithercmyk(image1); break;
    case 4:
        ui.spinBox->setEnabled(true);
        ui.spinBox_2->setEnabled(true);
        if (ui.spinBox->value() < 1) { ui.spinBox->setValue(1); }
        if (ui.spinBox_2->value() < 10) { ui.spinBox_2->setValue(10); }
        Filters::edgemask(image0, image1, ui.spinBox->value(), ui.spinBox_2->value());
        break;
    case 5: 
        ui.spinBox->setEnabled(true);
        if (ui.spinBox->value() < 3) { ui.spinBox->setValue(3); }
        if (ui.spinBox->value() > 5 ) { ui.spinBox->setValue(5); }
        Filters::kuwahara(image0, image1, ui.spinBox->value());
        break;
    case 6:
        //randcolors
        ui.spinBox->setEnabled(true);
        if (ui.spinBox->value() < 1) { ui.spinBox->setValue(1); }
        Filters::closestRandColor(image0, image1, ui.spinBox->value());
        break;
    case 7:
        image2 = image0;
        image3 = image0;
        ui.spinBox->setEnabled(true);
        if (ui.spinBox->value() == 0 ) {
            Filters::kuwahara(image0, image2, 5);
            Filters::edgemask(image2, image3, 2, 8000);
        }
        else if (ui.spinBox->value() > 0) {
            Filters::closestRandColor(image0, image2, ui.spinBox->value());
            Filters::edgemask(image0, image3, 2, 5000);
        }
        else if (ui.spinBox->value() < 0) {
            Filters::dithercmyk(image2);
            Filters::edgemask(image0, image3, 2, 5000);
        }
        Filters::comix(image2, image3, image1);
        break;
    case 8:
        to_modify = Filters::imageToVector(image1);
        Filters::blackAndWhite(&to_modify,image1.height(),image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 9:
        to_modify = Filters::imageToVector(image1);
        Filters::inverse(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 10:
        to_modify = Filters::imageToVector(image1);
        Filters::redFilter(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 11:
        to_modify = Filters::imageToVector(image1);
        Filters::findingEdge(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 12:
        to_modify = Filters::imageToVector(image1);
        Filters::blur3x3(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 13:
        to_modify = Filters::imageToVector(image1);
        Filters::blur5x5(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 14:
        to_modify = Filters::imageToVector(image1);
        Filters::blur7x7(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    default:
        break;
    }
    ui.imageLabel2->setPixmap(QPixmap::fromImage(image1));
    ui.saveButton->setEnabled(true);
}

void rework::on_openButton_clicked() {
	try{
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Images (*.png *.xpm *.jpg *.ppm)"));
        if (!fileName.isEmpty()) {
            image0.load(fileName);
            if (!image0.isNull()) {
                ui.imageLabel1->setPixmap(QPixmap::fromImage(image0));
                ui.comboBox->setEnabled(true);
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

