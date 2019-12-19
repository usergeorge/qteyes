#include "qteyes.h"
#include "ui_qteyes.h"

QtEyes::QtEyes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtEyes)
{
    ui->setupUi(this);
}

QtEyes::~QtEyes()
{
    delete ui;
}
