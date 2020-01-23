#include "resultwindow.h"
#include "ui_resultwindow.h"

ResultWindow::ResultWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    result = *new Result();
    ui->openGLWidget->result = &this->result;
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::show()
{
    ui->textEdit->append(QString("Scena: " + result.scene.path + "\n"));
    QString str;
    &str << result.scene.startPoint;
    ui->textEdit->append(QString("Zacetek: %1\n").arg(str));
    str.clear();
    &str << result.scene.endPoint;
    ui->textEdit->append(QString("Konec: %1\n").arg(str));

    ui->textEdit->append(QString("Algoritem: %1\n").arg(result.algorithmName));
    ui->textEdit->append(QString("Sosedstvo: %1\n").arg(result.getNeighberhoodName()));
    ui->textEdit->append(QString("Način zagona: %1\n").arg(result.getRunModeName()));

    ui->textEdit->append(QString("Stevilo razvejanj: %1\n").arg(result.getNumberOfExpandedStates()));
    ui->textEdit->append(QString("Cas izvajanja algoritma: %1ms\n").arg(result.runTime));

    ui->textEdit->append("Path: ");
    for (int i = 0; i<result.paths.last().size(); i++) {
        ui->textEdit->append(QString("%1: %2,%3,%4").arg(i)
                                                    .arg(result.paths.last()[i].x())
                                                    .arg(result.paths.last()[i].y())
                                                    .arg(result.paths.last()[i].z()));
    }

    QMainWindow::show();
}


void ResultWindow::keyPressEvent(QKeyEvent *event)
{
    ui->openGLWidget->keyPressEvent(event);
}

void ResultWindow::keyReleaseEvent(QKeyEvent *event)
{
    ui->openGLWidget->keyReleaseEvent(event);
}

void ResultWindow::on_endStateButton_released()
{
    if(!result.isDataSet()) return;
    ui->openGLWidget->toEndStep();
}

void ResultWindow::on_nextStateButton_released()
{
    if(!result.isDataSet()) return;
    ui->openGLWidget->toNextStep();
}

void ResultWindow::on_previousStateButton_released()
{
    if(!result.isDataSet()) return;
    ui->openGLWidget->toPreviousStep();
}

void ResultWindow::on_shortestPathOnlyButton_released()
{
    if(!result.isDataSet()) return;
    ui->openGLWidget->showOnlyEndPath();
}

void ResultWindow::on_startStateButton_released()
{
    if(!result.isDataSet()) return;
    ui->openGLWidget->toStartStep();
}
