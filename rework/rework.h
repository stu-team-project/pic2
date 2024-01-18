#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_rework.h"

class rework : public QMainWindow
{
    Q_OBJECT

public:
    rework(QWidget *parent = nullptr);
    ~rework();

private:
    Ui::reworkClass ui;
};
