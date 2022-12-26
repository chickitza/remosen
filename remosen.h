#pragma once

#include <QtWidgets/QMainWindow>
#include <qdebug>
#include <QString>
#include <QFileDialog>
#include <windows.h>
#include "ui_remosen.h"
#include "gdal_priv.h"
#include <armadillo>

#include <QMessageBox>

using namespace arma;

class remosen : public QMainWindow
{
    Q_OBJECT

public:
    remosen(QWidget *parent = nullptr);
    ~remosen();

public slots:
    void on_inPushButton_clicked();
    void on_outPushButton_clicked();
    void on_proPushButton_clicked();

private:
    Ui::remosenClass ui;

    // 输入输出目录
    QString inFilePath;
    QString outFilePath;


};
