#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDirIterator>
#include <QHash>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_siguiente_clicked();

    void on_btn_eliminar_clicked();

    void keyPressEvent(QKeyEvent *e);

    void on_btn_freq_clicked();

private:
    Ui::MainWindow *ui;
    QDirIterator *iter;
    QFile file_result;

    void next_image();
    bool valid_plate(const QString input_text);
    void fin_imagenes();

    int list_count;

    QHash<QString, int> plate_frequency;
    void cargar_freq_txt();

    QHash<QString, QStringList> plate_prediction;
    void cargar_pred_txt();

    void mostrar_pred();
};

#endif // MAINWINDOW_H
