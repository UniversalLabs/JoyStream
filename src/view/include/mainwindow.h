#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller/include/Controller.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void startView(Controller * controller);

private slots:
    void on_pushButton_clicked();

    Controller * controller_;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
