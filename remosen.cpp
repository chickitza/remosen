#include "remosen.h"

remosen::remosen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    // ��ʼ������
    this->setWindowTitle("RSSoftware");
    this->setWindowIcon(QIcon("icon.png"));
    resize(500, 500);

    // ��ʼ����ť
    button_for_test.setParent(this);
    button_for_test.setText("kmjyy");
    button_for_test.resize(100, 30);
    button_for_test.move(200, 200);

    // �����ť�¼�����
    connect(&button_for_test, &QPushButton::clicked, this, &remosen::change_text);
}

remosen::~remosen()
{}

void remosen::change_text()
{
    if (flag == 0)
    {
        button_for_test.setText("kmjwl");
        flag = 1;
    }
    else
    {
        button_for_test.setText("kmjyy");
        flag = 0;
    }
}
