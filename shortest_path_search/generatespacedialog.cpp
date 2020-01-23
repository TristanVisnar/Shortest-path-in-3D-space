#include "generatespacedialog.h"
#include "ui_generatespacedialog.h"

GenerateSpaceDialog::GenerateSpaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateSpaceDialog)
{
    ui->setupUi(this);
    populateComboBox();
}

GenerateSpaceDialog::~GenerateSpaceDialog()
{
    delete ui;
}

void GenerateSpaceDialog::populateComboBox()
{
    ui->comboBox->addItem("Randome space");
    ui->comboBox->addItem("Randome space - weights are set to 1.0f");
}

bool isValidSize(int size){
    if(size<MIN_SIZE || size>MAX_SIZE)
        return false;
    return true;
}

bool GenerateSpaceDialog::setSize()
{
    sizeX = ui->spinBoxX->value();
    sizeY = ui->spinBoxY->value();
    sizeZ = ui->spinBoxZ->value();
    if(!isValidSize(sizeX) && !isValidSize(sizeY) && !isValidSize(sizeZ)){
        // size is wrong
        return false;
    }
    return true;
}

bool GenerateSpaceDialog::setGenerator()
{
    selectedGenerator = ui->comboBox->currentIndex();
    if(selectedGenerator<0)
        return false;
    return true;
}

bool GenerateSpaceDialog::setObsticleOccuranceRate()
{
    obsticleOccuranceRate = ui->doubleSpinBox->value();
    return true;
}

bool GenerateSpaceDialog::setHiddenObsticleOccuranceRate()
{
    hiddenObsticleOccuranceRate = ui->doubleSpinBox_2->value();
    return true;
}


void GenerateSpaceDialog::on_buttonBox_accepted()
{
    if(!setSize() || !setGenerator() || !setObsticleOccuranceRate() || !setHiddenObsticleOccuranceRate())
        reject();
    accept();
}
