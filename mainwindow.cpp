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
    // Iterator for listing .png's
//    this->iter = new QDirIterator("imgs/", QStringList() << "*.png", QDir::Files);
    this->iter = new QDirIterator("imgs/", QDir::Files);
    // Show first img
    if(this->iter->hasNext()){
        const QPixmap pix(this->iter->next());
        ui->plate_img->setPixmap(pix.scaled(250,250,Qt::KeepAspectRatio));
    } else {
        this->fin_imagenes();
        QMessageBox::warning(this, tr("Patente Labeler"),
                                       tr("Asegura que hay imagenes en imgs/.\n"
                                          "No se encontro ninguna"),
                                       QMessageBox::Ok);
        return;
    }
    this->list_count = 1;
    // Load pantente_freq con las del .txt
    this->cargar_freq_txt();
    // Load las predicciones (pseudo-labeling)
    this->cargar_pred_txt();
    // Mostrar primera prediccion
    this->mostrar_pred();
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
  if ((e->key()==Qt::Key_Enter) || (e->key()==Qt::Key_Return))
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
    const QString input_patente = ui->txt_patente->text().trimmed().toUpper();
    // valid plate
    if(!this->valid_plate(input_patente))
    {
        QMessageBox::warning(this, tr("Patente Labeler"),
                                       tr("La patente es incorrecta.\n"
                                          "Requiere longitud de 6 o 7 caracteres"),
                                       QMessageBox::Ok);
        return;
    }
    // Checkear si esta registrada mas de 2 veces
    if(this->plate_frequency.contains(input_patente))
    {
        if(this->plate_frequency[input_patente] > 2)
        {
            int ret = QMessageBox::question(this, tr("Patente Labeler"),
                                           QString("%1 fue registrada mas de 2 veces\nSeguro desea ingresar?")
                                            .arg(input_patente),
                                           QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::Yes);
            if(ret == QMessageBox::No)
            {
                return;
            }
        }
    }
    // Write to file
    if(file_result.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
         QTextStream stream(&file_result);
         stream << "imgs/" << this->iter->fileName() << '\t' << input_patente << '\n';
         file_result.close();
    }
    // Add to map
    if(!this->plate_frequency.contains(input_patente))
    {
        this->plate_frequency[input_patente] = 1;
    } else {
        this->plate_frequency[input_patente] += 1;
    }
    // Show only last 10 plates
    if(list_count % 30 == 0)
    {
        ui->list_last_plates->clear();
    }
    ui->list_last_plates->addItem(input_patente.toUpper());
    // Move to next image
    this->next_image();
}

void MainWindow::next_image()
{
    // Clean input text
    ui->txt_patente->setText("");
    // Increase image count
    this->list_count += 1;
    // Move image to 'done_imgs/'
    QDir dir;
    dir.rename("imgs/" + this->iter->fileName(), "done_imgs/" + this->iter->fileName());
    // Move to next img
    if(this->iter->hasNext()){
        const QPixmap pix(this->iter->next());
        ui->plate_img->setPixmap(pix.scaled(250,250,Qt::KeepAspectRatio));
        this->mostrar_pred();
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

void MainWindow::cargar_freq_txt()
{
    if(!file_result.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Asegurar que existe el archivo results.txt", file_result.errorString());
        return;
    }

    QTextStream in(&file_result);

    while(!in.atEnd()) {
        const QString linea = in.readLine();
        const QString patente = linea.split("\t")[1].trimmed();
        if(!this->plate_frequency.contains(patente))
        {
            this->plate_frequency[patente] = 1;
        } else {
            this->plate_frequency[patente] += 1;
        }
    }

    file_result.close();
}

void MainWindow::cargar_pred_txt()
{
    QFile file_preds("img_pred.txt");
    if(!file_preds.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "Plate Labeler", "No se van a cargar las predicciones (No Pseudo-Labeling)");
        return;
    }

    QTextStream in(&file_preds);
    in.readLine();
    while(!in.atEnd()) {
        const QString linea = in.readLine();
        QStringList entry = linea.split("\t");
        const QString plate_filename = entry[0].split("\\").constLast();
        // Remove filename (first element)
        entry.removeFirst();
        this->plate_prediction[plate_filename] = entry;
    }

    file_result.close();
}

void MainWindow::mostrar_pred()
{
    const QString current_filename = this->iter->fileName();
    // Display prediction for current image
    if(!this->plate_prediction.empty() && this->plate_prediction.contains(current_filename))
    {
        const QStringList plate_conf = this->plate_prediction[current_filename];
        ui->txt_patente->setText(plate_conf[0]);
        ui->lbl_confianza->setText("Confianza " + plate_conf[1] + " %");
    }
}


void MainWindow::on_btn_freq_clicked()
{
    const QString patente = ui->txt_patente->text().trimmed().toUpper();
    const int freq_num = this->plate_frequency[patente];
    ui->txt_freq->setText(QString("Frequency %1").arg(freq_num));
}
