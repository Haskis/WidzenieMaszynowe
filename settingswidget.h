#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QSlider>
#include <codes.h>
namespace Ui {
  class SettingsWidget;
}

class SettingsWidget : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsWidget(QWidget *parent = 0);
  ~SettingsWidget();

private:
  Ui::SettingsWidget *ui;

signals:
  void sliderValueChanged(int id, int value);

public slots:
  void hMaxValueChanged(int value);
  void hMinValueChanged(int value);

  void sMaxValueChanged(int value);
  void sMinValueChanged(int value);

  void vMaxValueChanged(int value);
  void vMinValueChanged(int value);

};

#endif // SETTINGSWIDGET_H
