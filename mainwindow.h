#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <settingswidget.h>
#include <codes.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QSpinBox>
namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:

  quint8 xMin;
  quint8 yMin;
  quint8 zMin;
  quint8 xMax;
  quint8 yMax;
  quint8 zMax;


  SettingsWidget *mySettingsWidget;
  cv::VideoCapture myVideoCapture;
  Ui::MainWindow *ui;
  cv::Mat oryginalFrame;
  cv::Mat processedFrame;
  cv::Mat backgroundFrame;
  QTimer *frameRefreshTimer;
  int refreshTime;

  float pointsDistance(cv::Point x, cv::Point y);
  private slots:

  void getAndProceedFrame();
  void on_actionSettings_triggered();
  void valuesChanged(int id, int value);
  void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
