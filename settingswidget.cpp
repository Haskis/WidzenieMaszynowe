#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SettingsWidget)
{
  ui->setupUi(this);

  //Signal Slots
  connect(ui->horizontalSliderXMax,SIGNAL(valueChanged(int)),this,SLOT(xMaxValueChanged(int)));
  connect(ui->horizontalSliderXMin,SIGNAL(valueChanged(int)),this,SLOT(xMinValueChanged(int)));

  connect(ui->horizontalSliderYMax,SIGNAL(valueChanged(int)),this,SLOT(yMaxValueChanged(int)));
  connect(ui->horizontalSliderYMin,SIGNAL(valueChanged(int)),this,SLOT(yMinValueChanged(int)));

  connect(ui->horizontalSliderZMax,SIGNAL(valueChanged(int)),this,SLOT(zMaxValueChanged(int)));
  connect(ui->horizontalSliderZMin,SIGNAL(valueChanged(int)),this,SLOT(zMinValueChanged(int)));
}

SettingsWidget::~SettingsWidget()
{
  delete ui;
}


void SettingsWidget::xMaxValueChanged(int value){
  emit sliderValueChanged(X_MAX,value);
}

void SettingsWidget::xMinValueChanged(int value){
  emit sliderValueChanged(X_MIN,value);
}

void SettingsWidget::yMaxValueChanged(int value){
  emit sliderValueChanged(Y_MAX,value);
}

void SettingsWidget::yMinValueChanged(int value){
  emit sliderValueChanged(Y_MIN,value);
}

void SettingsWidget::zMaxValueChanged(int value){
  emit sliderValueChanged(Z_MAX,value);
}

void SettingsWidget::zMinValueChanged(int value){
  emit sliderValueChanged(Z_MIN,value);
}
