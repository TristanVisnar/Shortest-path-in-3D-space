#ifndef LOGGERWIDGET_H
#define LOGGERWIDGET_H

#include <QWidget>

class LoggerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoggerWidget(QWidget *parent = nullptr);

signals:
public slots:
    void info(const QString info) const;
    void error(const QString error) const;
    void warning(const QString warning) const;

public:

};

#endif // LOGGERWIDGET_H
