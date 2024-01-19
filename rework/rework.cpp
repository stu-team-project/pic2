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
            QImage image(fileName);
            if (!image.isNull()) {
                ui.imageLabel1->setPixmap(QPixmap::fromImage(image));

                //testing compatibility
                QVector<QVector<QColor>> to_modify = Filters::imageToVector(image);
                Filters::redFilter(&to_modify, image.height(), image.width());
                QImage image2 = Filters::vectorToImage(to_modify);
                ui.imageLabel2->setPixmap(QPixmap::fromImage(image2));
            }
            else {

            }
        }
	}
	catch (const std::exception&)
	{

	}
}