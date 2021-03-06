#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  //Default Y(x) Cr(y) Cb(z) values
  xMin  = 0;
  xMax  = 255;
  yMin = 95;
  yMax = 146;
  zMin= 133;
  zMax = 218;

  mySettingsWidget = new SettingsWidget();
  ui->setupUi(this);

  //Check if video is opened and display it on Main Window
  if(!myVideoCapture.open(0)){
    ui->labelVideoStatus->setText("Could Not Open Video");
  }
  else
    ui->labelVideoStatus->setText("Video device opened");


  cv::namedWindow("OryginalImage");
  cv::namedWindow("ProcessedImage");

  //Get frame every refreshTime miliseconds
  frameRefreshTimer = new QTimer(this);
  refreshTime = 150;
  frameRefreshTimer->start(refreshTime);

  connect(frameRefreshTimer,SIGNAL(timeout()),this,SLOT(getAndProceedFrame()));
  connect(mySettingsWidget,SIGNAL(sliderValueChanged(int,int)),this,SLOT(valuesChanged(int,int)));
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::getAndProceedFrame(){
  cv::vector<cv::vector<cv::Point> > countursArray;
  cv::vector<cv::vector<cv::Point> > countursArrayHand;
  cv::vector<cv::vector<cv::Point> > countursArrayOutside;
  cv::vector<cv::vector<cv::Point> > countursArrayInside;
  cv::vector<cv::Point> handCountur;
  cv::vector<cv::Point> hullCounturAfter;
  cv::vector<int> hullCounturIndexes;
  cv::vector<cv::Point> hullCountur;

  cv::vector<cv::Vec4i> defects;
  cv::vector<cv::Point> defectsCountur;
  cv::vector<cv::Mat> channels;

  //Go into YCrCb base
  myVideoCapture >> oryginalFrame;

  cv::blur(oryginalFrame,oryginalFrame,cv::Size(3,3));
  cv::blur(oryginalFrame,oryginalFrame,cv::Size(3,3));
//  qDebug()<<"xMax"<<xMax;
//  qDebug()<<"yMax"<<yMax;
//  qDebug()<<"zMax"<<zMax;
//  qDebug()<<"xMin"<<xMin;
//  qDebug()<<"yMin"<<yMin;
//  qDebug()<<"zMin"<<zMin;

  oryginalFrame.copyTo(processedFrame);
  //Get only part of image
  processedFrame = processedFrame(cv::Rect(0, 0, 250, 300));

//Wersja z YCrCb
  if(ui->spinBoxVersion->value() == 0){

  cv::cvtColor(processedFrame,processedFrame,CV_RGB2YCrCb);
  cv::inRange(processedFrame,cv::Scalar(xMin,yMin,zMin),cv::Scalar(xMax,yMax,zMax),processedFrame);
    }

//Wersja z kanałami
  else{
  //Background removal,
  if(backgroundFrame.data != NULL){
      cv::absdiff(processedFrame,backgroundFrame,processedFrame);
    }
  cv::split(processedFrame,channels);
  processedFrame = channels[ui->spinBoxChannelChoose->value()];
  //Binearyzacja na podstawie wartości suwaka
  cv::threshold(processedFrame,processedFrame,ui->horizontalSliderThreshold->value(),255,CV_THRESH_BINARY);
  //cv::erode(processedFrame,processedFrame,cv::Mat());
  //cv::dilate(processedFrame,processedFrame,cv::Mat());
    }
//Koniec róznych wersji

//Narysuj obraz zainteresowania na obrazie oryginalnym
  cv::rectangle(oryginalFrame,cv::Rect(0, 0, 250, 300),cv::Scalar(0,50,255),3);

//Znajdz wszystkie kontury
  cv::findContours(processedFrame,countursArray,CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

//Zamien kodowanie obrazu, aby mozna bylo rysowac kolorowo
  cv::cvtColor(processedFrame,processedFrame,CV_GRAY2RGB);

//Znajdz pierwszy kontur ktory ma powierzchnie wieksza od 5000 ( wyznaczone eksperymentalnie)
  for(int i=0; i<countursArray.size(); i++)
    if(cv::contourArea(countursArray[i])>5000){
      handCountur = countursArray[i];
     //countursArray.clear();
      countursArrayHand.push_back(handCountur);

      //Znajdz punkty zewnetrzne dłoni
      cv::convexHull(handCountur,hullCounturIndexes);
      for(int i=0;i<hullCounturIndexes.size();i++){
          hullCountur.push_back(handCountur[hullCounturIndexes[i]]);
      }
      //Usuń punkty zewnetrzne zbyt blisko siebie
//      for(int i=0;i<hullCountur.size()-1;i++){

//          float distance = sqrt((hullCountur[i].x-hullCountur[i+1].x)*
//                                (hullCountur[i].x-hullCountur[i+1].x)+
//                                (hullCountur[i].y-hullCountur[i+1].y)*
//                                (hullCountur[i].y-hullCountur[i+1].y));
//          if(distance<0){

//            }
//          else
            hullCounturAfter.push_back(hullCountur[i]);
//        }


      cv::Point2f circleCenter;
      float radious;
      //cv::Moments moment;
      //moment = cv::moments(hullCountur);
      cv::minEnclosingCircle(hullCountur,circleCenter, radious);

//      circleCenter.x = int(moment.m10/moment.m00);
//      circleCenter.y = int(moment.m01/moment.m00);

//      radious /= 1.2;
      cv::circle(processedFrame,circleCenter,radious,cv::Scalar(255,0,255));

      countursArrayOutside.push_back(hullCounturAfter);

      //Znajdz punkty wewnetrzne dłoni
      cv::convexityDefects(handCountur,hullCounturIndexes,defects);
      for(int i=0;i<defects.size();i++){
          if(defects[i][3]>20*256.0)
            if(handCountur[defects[i][2]].y<circleCenter.y+0.5*radious)
            defectsCountur.push_back(handCountur[defects[i][2]]);
      }

      countursArrayInside.push_back(defectsCountur);
      cv::drawContours(processedFrame,countursArrayHand,-1,cv::Scalar(255,0,0),2);
      cv::drawContours(processedFrame,countursArrayOutside,-1,cv::Scalar(0,255,0),2);
      cv::drawContours(processedFrame,countursArrayInside,-1,cv::Scalar(0,0,255),2);

      break;
    }


  std::string number = QString::number(defectsCountur.size()).toStdString();

  cv::putText(oryginalFrame,number,cv::Point(20,20),1,1,cv::Scalar(0,0,255));

  cv::imshow("OryginalImage",oryginalFrame);
  cv::imshow("ProcessedImage",processedFrame);
}

void MainWindow::on_actionSettings_triggered()
{
    mySettingsWidget->show();
}

void MainWindow::valuesChanged(int id, int value){
  switch (id){
    case X_MIN:
      qDebug()<<"X_MIN";
      xMin=value;
      return;
    case X_MAX:
      qDebug()<<"X_MAX";
      xMax=value;
      return;
    case Y_MIN:
      qDebug()<<"Y_MIN";
      yMin=value;
      return;
    case Y_MAX:
      qDebug()<<"Y_MAX";
      yMax=value;
      return;
    case Z_MIN:
      qDebug()<<"Z_MIN";
      zMin=value;
      return;
    case Z_MAX:
      qDebug()<<"Z_MAX";
      zMax=value;
      return;
    }
}

float pointsDistance(cv::Point x, cv::Point y){
  return sqrt((x.x-y.x)*(x.x-y.x)+(x.y-y.y)*(x.y-y.y));
}

void MainWindow::on_pushButton_clicked()
{
  qDebug()<<"BACK";
    myVideoCapture>>backgroundFrame;

    backgroundFrame = backgroundFrame(cv::Rect(0, 0, 250, 300));
}
