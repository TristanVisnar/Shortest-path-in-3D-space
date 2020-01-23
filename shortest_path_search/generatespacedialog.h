#ifndef GENARATESPECEDIALOG_H
#define GENARATESPECEDIALOG_H

#include <QDialog>

#define MIN_SIZE 4
#define MAX_SIZE 10000

namespace Ui {
class GenerateSpaceDialog;
}

class GenerateSpaceDialog : public QDialog
{
    Q_OBJECT
public:
    int sizeX, sizeY, sizeZ;
    int selectedGenerator = 0;
    float obsticleOccuranceRate = 0.15f;
    float hiddenObsticleOccuranceRate = 0.15f;

public:
    explicit GenerateSpaceDialog(QWidget *parent = nullptr);
    ~GenerateSpaceDialog();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::GenerateSpaceDialog *ui;
    void populateComboBox();
    bool setSize();
    bool setGenerator();
    bool setObsticleOccuranceRate();
    bool setHiddenObsticleOccuranceRate();
};

#endif // GENARATESPECEDIALOG_H
