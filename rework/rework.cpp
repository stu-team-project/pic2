#include "rework.h"

// This file contains application procedures.

rework::rework(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
   
    ui.saveButton->setEnabled(false);
    ui.comboBox->setEnabled(false);
    //fill combobox here with new filters
    ui.comboBox->addItem("Sparse-cmyk"); 
    ui.comboBox->addItem("Dense-cmyk");
    ui.comboBox->addItem("Closest-cmyk");
    ui.comboBox->addItem("Dithering-cmyk");
    ui.comboBox->addItem("Strong edge");
    ui.comboBox->addItem("Kuwahara"); //5
    ui.comboBox->addItem("ClosestRandColor");
    ui.comboBox->addItem("Comix");
    ui.comboBox->addItem("Black and white"); 
    ui.comboBox->addItem("Inverse");
    ui.comboBox->addItem("RedFilter"); //10
    ui.comboBox->addItem("FindingEdge"); 
    ui.comboBox->addItem("Blur 3x3");
    ui.comboBox->addItem("Blur 5x5");
    ui.comboBox->addItem("Blur 7x7");
    ui.comboBox->addItem("Fisheye"); //15
    ui.comboBox->addItem("Green Filter"); //16
    ui.comboBox->addItem("Blue Filter"); //17

}
rework::~rework(){}
void rework::on_comboBox_activated(int index) {
    ui.spinBox->setEnabled(false);
    ui.spinBox_2->setEnabled(false);
    ui.spinBox_2->setVisible(false);
    ui.imageLabel2->clear();
    image1 = image0;
    QVector<QVector<QColor>> to_modify;
    switch (index)
    {
    case 0:
        ui.errorLabel->setText("Nikita's first attempt at CMYK");
        to_modify = Filters::imageToVector(image1);
        Filters::sparsecmyk(&to_modify);
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 1:
        ui.errorLabel->setText("Nikita's second attempt at CMYK");
        to_modify = Filters::imageToVector(image1);
        Filters::densecmyk(&to_modify);
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 2: 
        ui.errorLabel->setText("Nikita's closest CMYK color");
        Filters::closestcmyk(image1); 
        break;
    case 3: 
        ui.errorLabel->setText("Nikita's third attempt at CMYK");
        Filters::dithercmyk(image1); 
        break;
    case 4:
        ui.errorLabel->setText("Nikita's strong edge. Recommended values: 1-10 for the first parameter aka linewidth, 100-90000 for the second parameter aka sensitivity.");
        ui.spinBox->setEnabled(true);
        ui.spinBox_2->setEnabled(true);
        ui.spinBox->setMaximum(10);
        ui.spinBox->setMinimum(1);

        ui.spinBox_2->setVisible(true);
        ui.spinBox_2->setMaximum(90000);
        ui.spinBox_2->setMinimum(100);
        Filters::edgemask(image0, image1, ui.spinBox->value(), ui.spinBox_2->value());
        break;
    case 5: 
        ui.errorLabel->setText("Nikita's Kuwahara filter. Recommended values 2..5");
        ui.spinBox->setEnabled(true);
        if (ui.spinBox->value() < 2) { ui.spinBox->setValue(2); }
        if (ui.spinBox->value() > 5 ) { ui.spinBox->setValue(5); }
        Filters::kuwahara(image0, image1, ui.spinBox->value());
        break;
    case 6:
        ui.errorLabel->setText("Nikita's continuation in ClosestColor. Click again for new  random result. Recommended values 16-30. If set 1 uses a preprepared color collection.");
        ui.spinBox->setEnabled(true);
        ui.spinBox->setMaximum(255);
        ui.spinBox->setMinimum(1);
        if (ui.spinBox->value() < 1) { ui.spinBox->setValue(1); }
        Filters::closestRandColor(image0, image1, ui.spinBox->value());
        break;
    case 7:
        ui.errorLabel->setText("Nikita's Comix. =0 based on Kuwahara, >0 based on closestColor, <0 based on dithercmyk");
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
        ui.errorLabel->setText("Jan's Black and white filter");
        to_modify = Filters::imageToVector(image1);
        Filters::blackAndWhite(&to_modify,image1.height(),image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 9:
        ui.errorLabel->setText("Jan's Inverse filter");
        to_modify = Filters::imageToVector(image1);
        Filters::inverse(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 10:
        ui.errorLabel->setText("Jan's Red filter");
        to_modify = Filters::imageToVector(image1);
        Filters::redFilter(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 11:
        ui.errorLabel->setText("Jan's finding edge filter");
        to_modify = Filters::imageToVector(image1);
        Filters::findingEdge(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 12:
        ui.errorLabel->setText("Jan's blur 3x3 filter");
        to_modify = Filters::imageToVector(image1);
        Filters::blur3x3(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 13:
        ui.errorLabel->setText("Jan's blur 5x5 filter");
        to_modify = Filters::imageToVector(image1);
        Filters::blur5x5(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 14:
        ui.errorLabel->setText("Jan's blur 7x7 filter");
        to_modify = Filters::imageToVector(image1);
        Filters::blur7x7(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;

    case 15:
        ui.errorLabel->setText("Nikita's Fisheye filter. Recommended values 0..2");
        ui.spinBox->setEnabled(true);
        ui.spinBox->setMaximum(2);
        ui.spinBox->setMinimum(0);
        Filters::fisheye(image0, image1, ui.spinBox->value());
        break;
    case 16:
        ui.errorLabel->setText("Jan's Green filter");
        to_modify = Filters::imageToVector(image1);
        Filters::greenFilter(&to_modify, image1.height(), image1.width());
        image1 = Filters::vectorToImage(to_modify);
        break;
    case 17:
        ui.errorLabel->setText("Jan's Blue filter");
        to_modify = Filters::imageToVector(image1);
        Filters::blueFilter(&to_modify, image1.height(), image1.width());
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
        ui.errorLabel->setText("File path is empty");
        return;
    }
    QString fileExtension = QFileInfo(filePath).suffix().toLower();

    QImageWriter imageWriter(filePath);

    if (!fileExtension.isEmpty()) {
        imageWriter.setFormat(fileExtension.toUtf8());
    }
    else {
        ui.errorLabel->setText("Unknown format");
        return;
    }

    if (!image1.isNull()) {
        if (imageWriter.write(image1)) {
            // Image saved successfully
            ui.errorLabel->setText("Saved");
        }
        else {
            ui.errorLabel->setText("Failed to write image: " + imageWriter.errorString());
        }
    }
    else {
        ui.errorLabel->setText("Image is null");
    }
}

