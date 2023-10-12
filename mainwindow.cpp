#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "choice.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        if(e->x()>560&&e->x()<560+110&&e->y()>230&&e->y()<230+60){
            this->close();
            choice *f=new choice();
            f->show();
        }
        if(e->x()>280&&e->x()<280+110&&e->y()>230&&e->y()<230+60){
            this->close();
        }
    }
}
