#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QMainWindow>
#include "result.h"

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow();

    void show();

    Result result;

private:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


private slots:


    void on_endStateButton_released();

    void on_nextStateButton_released();

    void on_previousStateButton_released();

    void on_shortestPathOnlyButton_released();

    void on_startStateButton_released();

private:
    Ui::ResultWindow *ui;
};

#endif // RESULTWINDOW_H
