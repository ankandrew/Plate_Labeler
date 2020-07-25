#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QKeyEvent>

// Debugging
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    file_result("results.txt")
{
    ui->setupUi(this);
    // Iterator for listing .jpg's
    this->iter = new QDirIterator("imgs/", QStringList() << "*.png", QDir::Files);
    // Show first img
    if(this->iter->hasNext()){
        const QPixmap pix(this->iter->next());
        ui->plate_img->setPixmap(pix.scaled(250,250,Qt::KeepAspectRatio));
    } else {
        this->fin_imagenes();
    }
    this->list_count = 1;

}

MainWindow::~MainWindow()
{
    delete ui;
    delete iter;
}

// ------ SLOTS ------

// Shortcuts
void MainWindow::keyPressEvent(QKeyEvent *e)
{
  if ((e->key()==Qt::Key_Return))
  {
    this->on_btn_siguiente_clicked();
  }

  if ((e->key()==Qt::Key_Escape))
  {
    this->on_btn_eliminar_clicked();
  }

}


void MainWindow::on_btn_siguiente_clicked()
{
    const QString input_patente = ui->txt_patente->toPlainText().trimmed();
    // valid plate
    if(!this->valid_plate(input_patente))
    {
        QMessageBox::warning(this, tr("Patente Labeler"),
                                       tr("La patente es incorrecta.\n"
                                          "Requiere longitud de 6 o 7 caracteres"),
                                       QMessageBox::Ok);
        return;
    }
    // Write to file
    if(file_result.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
         QTextStream stream(&file_result);
         stream << "imgs/" << this->iter->fileName() << '\t' << input_patente.toUpper() << '\n';
         file_result.close();
    }
    // Show only last 10 plates
    if(list_count % 10 == 0)
    {
        ui->list_last_plates->clear();
    }
    ui->list_last_plates->addItem(input_patente.toUpper());
    // Move to next image
    this->next_image();
}

void MainWindow::next_image()
{
    // Increase image count
    this->list_count += 1;
    // Move image to 'done_imgs/'
    QDir dir;
    dir.rename("imgs/" + this->iter->fileName(), "done_imgs/" + this->iter->fileName());
    // Move to next img
    if(this->iter->hasNext()){
        const QPixmap pix(this->iter->next());
        ui->plate_img->setPixmap(pix.scaled(250,250,Qt::KeepAspectRatio));
    } else {
        this->fin_imagenes();
        // Quit app.
        int ret = QMessageBox::question(this, tr("Patente Labeler"),
                                       tr("Se teriminaron las fotos\n"
                                          "Deseas salir de la app?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes);

        if(ret == QMessageBox::Yes)
        {
            QCoreApplication::quit();
        }
    }
    // Clean input text
    ui->txt_patente->setText("");
}

bool MainWindow::valid_plate(const QString input_text){
    const int len = input_text.length();
    return (len < 6 || len > 7) ? false : true;
}

void MainWindow::on_btn_eliminar_clicked()
{
    int ret = QMessageBox::question(this, tr("Patente Labeler"),
                                   tr("Estas seguro que queres borrar la foto?\n"
                                      "(no hay camino atras)"),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::Yes);
    if(ret == QMessageBox::Yes)
    {
        // Delete img
        QDir dir;
        dir.remove("imgs/" + this->iter->fileName());
        // Move to next img
        this->next_image();
    }
}

void MainWindow::fin_imagenes()
{
    ui->plate_img->setText("Fin de imagenes");
    // disable input text
    ui->txt_patente->setReadOnly(true);
    ui->btn_siguiente->setEnabled(false);
    ui->btn_eliminar->setEnabled(false);

}

