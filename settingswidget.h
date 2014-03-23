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
  void xMaxValueChanged(int value);
  void xMinValueChanged(int value);

  void yMaxValueChanged(int value);
  void yMinValueChanged(int value);

  void zMaxValueChanged(int value);
  void zMinValueChanged(int value);

};

#endif // SETTINGSWIDGET_H
