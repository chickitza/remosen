#pragma once

#include <QtWidgets/QMainWindow>
#include<QPushButton>
#include "ui_remosen.h"

class remosen : public QMainWindow
{
    Q_OBJECT

public:
    remosen(QWidget *parent = nullptr);
    ~remosen();

private:
    Ui::remosenClass ui;
    QPushButton button_for_test;
    int flag = 0;

public:
    void change_text();
};
