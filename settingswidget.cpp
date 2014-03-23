#include "settingswidget.h"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SettingsWidget)
{
  ui->setupUi(this);

  //Signal Slots
  connect(ui->horizontalSliderHMax,SIGNAL(valueChanged(int)),this,SLOT(hMaxValueChanged(int)));
  connect(ui->horizontalSliderHMin,SIGNAL(valueChanged(int)),this,SLOT(hMinValueChanged(int)));

  connect(ui->horizontalSliderSMax,SIGNAL(valueChanged(int)),this,SLOT(sMaxValueChanged(int)));
  connect(ui->horizontalSliderSMin,SIGNAL(valueChanged(int)),this,SLOT(sMinValueChanged(int)));

  connect(ui->horizontalSliderVMax,SIGNAL(valueChanged(int)),this,SLOT(vMaxValueChanged(int)));
  connect(ui->horizontalSliderVMin,SIGNAL(valueChanged(int)),this,SLOT(vMinValueChanged(int)));
}

SettingsWidget::~SettingsWidget()
{
  delete ui;
}


void SettingsWidget::hMaxValueChanged(int value){
  emit sliderValueChanged(H_MAX,value);
}

void SettingsWidget::hMinValueChanged(int value){
  emit sliderValueChanged(H_MIN,value);
}

void SettingsWidget::sMaxValueChanged(int value){
  emit sliderValueChanged(S_MAX,value);
}

void SettingsWidget::sMinValueChanged(int value){
  emit sliderValueChanged(S_MIN,value);
}

void SettingsWidget::vMaxValueChanged(int value){
  emit sliderValueChanged(V_MAX,value);
}

void SettingsWidget::vMinValueChanged(int value){
  emit sliderValueChanged(V_MIN,value);
}
