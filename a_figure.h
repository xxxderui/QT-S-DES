#ifndef A_FIGURE_H
#define A_FIGURE_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <QString>
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>
#include <sstream>
#include <iterator>
#include <string>
#include <QMouseEvent>

namespace Ui {
class A_figure;
}

class A_figure : public QMainWindow
{
    Q_OBJECT

public:
    explicit A_figure(QWidget *parent = nullptr);
    ~A_figure();
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();

private:
    Ui::A_figure *ui;
};

#endif // A_FIGURE_H
