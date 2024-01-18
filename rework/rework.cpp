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
            }
            else {

            }
        }
	}
	catch (const std::exception&)
	{

	}
}