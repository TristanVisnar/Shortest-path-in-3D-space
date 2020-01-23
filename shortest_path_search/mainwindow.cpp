#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "resultwindow.h"
#include "generatespacedialog.h"
#include "ispacegenerator.h"
#include <QFileDialog>
#include <QDir>

#include "djikstra.h"
#include "astar.h"
#include "dstarlite.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    setupSettingsWidget();

    settings = new Settings();
    ui->openGLWidget->result = (Result*)settings;

}

MainWindow::~MainWindow()
{
    delete ui;
}


/*
 *  MENU -> FILE: ACTIONS
 *
 */

void MainWindow::on_actionGenerate_random_triggered()
{
    GenerateSpaceDialog gsd(this);
    if(gsd.exec()){
        settings->scene = ISpaceGenerator::getGenerator(gsd.selectedGenerator)->run(gsd.sizeX, gsd.sizeY, gsd.sizeZ, gsd.obsticleOccuranceRate, gsd.hiddenObsticleOccuranceRate);
    }

    ui->openGLWidget->result = (Result*)new Settings(*settings);
    ui->openGLWidget->firstDrawSetup();
}

void MainWindow::on_actionLoad_space_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load scene", QDir::currentPath(), "Scene files (*.scn)");
    if(fileName == "")
        return;

    settings->scene.load(fileName);

    ui->openGLWidget->result = (Result*)new Settings(*settings);
    ui->openGLWidget->firstDrawSetup();

}

void MainWindow::on_actionSave_space_triggered()
{

    QString fileName = QFileDialog::getSaveFileName(this,"Save scene", (settings->scene.path!="")? settings->scene.path:QDir::currentPath(),"Scene files (*.scn)");
    if(fileName == "")
        return;

    settings->scene.save(fileName);
}

void MainWindow::on_actionOpen_result_triggered()
{
    rw = new ResultWindow(this);
    // send data to rw
    rw->show();
}

/*
 * SETTINGS
 *
 */

void MainWindow::setupSettingsWidget()
{
    ui->comboBox->addItem("Djikstra algorithm");
    ui->comboBox->addItem("A* algorithm");
    ui->comboBox->addItem("D* algorithm");
}

void MainWindow::updateSettings()
{
    settings->runMode = (ui->useHiddenWallsCbx->isChecked())? RunModes::HIDDEN : RunModes::NORMAL;
    settings->neighberhood = (ui->radioButton->isChecked())? Neighberhoods::NEIGHBERHOOD6 : Neighberhoods::NEIGHBERHOOD26;
}


SearchAlgorithmNames::SearchAlgorithmNames MainWindow::getSelectedAlgorithmType()
{
    switch(ui->comboBox->currentIndex()){
        case 0:
            return SearchAlgorithmNames::DJIKSTRA_ALGORITHM;
        case 1:
            return SearchAlgorithmNames::A_STAR_ALGORITHM;
        case 2:
            return  SearchAlgorithmNames::D_STAR_ALGORITHM;
        default:{
            // error
            return SearchAlgorithmNames::DJIKSTRA_ALGORITHM;
        }
    }

}

void MainWindow::on_runAlgorithmBtn_released()
{
    updateSettings();
    ISearchAlgorithm* searchAlgorithm = nullptr;
    switch(getSelectedAlgorithmType()){

    case SearchAlgorithmNames::DJIKSTRA_ALGORITHM:
        searchAlgorithm = new Djikstra(*settings);
        break;
    case SearchAlgorithmNames::A_STAR_ALGORITHM:
        searchAlgorithm = new AStar(*settings);
        break;
    case SearchAlgorithmNames::D_STAR_ALGORITHM:
        searchAlgorithm = new DStarLite(*settings);
        break;
    }


    if(searchAlgorithm){
        rw = new ResultWindow(this);
        searchAlgorithm->run(rw->result);
        rw->show();
    }

}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ui->openGLWidget->keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    ui->openGLWidget->keyReleaseEvent(event);
}
