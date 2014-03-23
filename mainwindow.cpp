#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  hMin=0;
  sMin=100;
  vMin=60;
  hMax=20;
  sMax=150;
  vMax=255;

  mySettingsWidget = new SettingsWidget();
  ui->setupUi(this);
  if(!myVideoCapture.open(0)){
    ui->labelVideoStatus->setText("Could Not Open Video");
  }
  else
    ui->labelVideoStatus->setText("Video device opened");

  cv::namedWindow("OryginalImage");
  cv::namedWindow("ProcessedImage");

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

//Zgarnij klatkę i rozbij ją na kanały, wykorzystaj tylko jeden kanał
  myVideoCapture >> oryginalFrame;
  cv::split(oryginalFrame,channels);
  processedFrame = channels[ui->spinBoxChannelChoose->value()];

//Ogranicz obraz zainteresowania i narysuj go na obrazie oryginalnym
  processedFrame = processedFrame(cv::Rect(0, 0, 200, 200));
  cv::rectangle(oryginalFrame,cv::Rect(0, 0, 200, 200),cv::Scalar(0,50,255),3);

//Binearyzacja na podstawie wartości suwaka
  cv::threshold(processedFrame,processedFrame,ui->horizontalSliderThreshold->value(),255,CV_THRESH_BINARY_INV);
  //cv::erode(processedFrame,processedFrame,cv::Mat());
  //cv::dilate(processedFrame,processedFrame,cv::Mat());

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
      for(int i=0;i<hullCountur.size()-1;i++){

          float distance = sqrt((hullCountur[i].x-hullCountur[i+1].x)*
                                (hullCountur[i].x-hullCountur[i+1].x)+
                                (hullCountur[i].y-hullCountur[i+1].y)*
                                (hullCountur[i].y-hullCountur[i+1].y));
          if(distance<25){
              ;//i++;
            }
          else
            hullCounturAfter.push_back(hullCountur[i]);
        }
      countursArrayOutside.push_back(hullCounturAfter);

      //Znajdz punkty wewnetrzne dłoni
      cv::convexityDefects(handCountur,hullCounturIndexes,defects);
      for(int i=0;i<defects.size();i++){
          qDebug()<<"I "<<i<<" "<<defects[i][3];
          if(defects[i][3]>20*256.0)
            defectsCountur.push_back(handCountur[defects[i][2]]);
      }

      countursArrayInside.push_back(defectsCountur);
      cv::drawContours(processedFrame,countursArrayHand,-1,cv::Scalar(255,0,0),2);
      cv::drawContours(processedFrame,countursArrayOutside,-1,cv::Scalar(0,255,0),2);
      cv::drawContours(processedFrame,countursArrayInside,-1,cv::Scalar(0,0,255),2);


      break;
    }

   // qDebug()<<countur.size()<<hullPoints.size()<<convexPoints.size();

    //cv::convexityDefects(countur,hullPoints,convexPoints);
    //convexPointsArray.push_back(convexPoints);
    //cv::drawContours(processedFrame,convexPointsArray,-1,cv::Scalar(255,0,0),2);




  cv::imshow("OryginalImage",oryginalFrame);
  cv::imshow("ProcessedImage",processedFrame);

  qDebug()<<"HMax:"<<hMax;
  qDebug()<<"HMin:"<<hMin;
  qDebug()<<"SMax:"<<sMax;
  qDebug()<<"SMin:"<<sMin;
  qDebug()<<"VMax:"<<vMax;
  qDebug()<<"VMin:"<<vMin;
}

void MainWindow::on_actionSettings_triggered()
{
    mySettingsWidget->show();
}

void MainWindow::valuesChanged(int id, int value){
  switch (id){
    case H_MAX:
      hMax=value;
      return;
    case H_MIN:
      hMin=value;
      return;
    case S_MAX:
      sMax=value;
      return;
    case S_MIN:
      sMin=value;
      return;
    case V_MAX:
      vMax=value;
      return;
    case V_MIN:
      vMin=value;
      return;
    }
}

float pointsDistance(cv::Point x, cv::Point y){
  return sqrt((x.x-y.x)*(x.x-y.x)+(x.y-y.y)*(x.y-y.y));
}
