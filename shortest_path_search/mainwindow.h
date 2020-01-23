#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include "settings.h"
#include "isearchalgorithm.h"
#include "resultwindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Settings* settings;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:
    void setupSettingsWidget();
    void updateSettings();
    SearchAlgorithmNames::SearchAlgorithmNames getSelectedAlgorithmType();

private slots:
    // Menu -> File: Actions
    void on_actionGenerate_random_triggered();
    void on_actionLoad_space_triggered();
    void on_actionSave_space_triggered();
    void on_actionOpen_result_triggered();

    void on_runAlgorithmBtn_released();


    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    ResultWindow *rw;
};

#endif // MAINWINDOW_H
