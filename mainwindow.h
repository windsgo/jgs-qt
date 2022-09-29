#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "draggerwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public DraggerWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_choosepath_clicked();

private:
    void _check_jgs_and_insert(const QString& filename);
    void _handle_one_file(const QString& filename);


private:
    Ui::MainWindow *ui;
    QString _current_path;
    QList<QString> _jgs_file_list;
    QTextStream ts;
};
#endif // MAINWINDOW_H
