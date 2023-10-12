#include "choice.h"
#include "ui_choice.h"
#include "mainwindow.h"
#include "figure.h"
#include "a_figure.h"
#include "attack.h"

choice::choice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::choice)
{
    ui->setupUi(this);
}

choice::~choice()
{
    delete ui;
}

void choice::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        if(e->x()>345&&e->x()<345+270&&e->y()>110&&e->y()<110+80){
            this->close();
            figure *f=new figure();
            f->show();
        }
        if(e->x()>335&&e->x()<335+290&&e->y()>190&&e->y()<190+80){
            this->close();
            A_figure *f=new A_figure();
            f->show();
        }
        if(e->x()>325&&e->x()<325+310&&e->y()>270&&e->y()<270+80){
            this->close();
            attack *f=new attack();
            f->show();
        }
        if(e->x()>850&&e->x()<850+100&&e->y()>10&&e->y()<10+80){
            this->close();
            MainWindow *f=new MainWindow();
            f->show();
        }
    }
}
