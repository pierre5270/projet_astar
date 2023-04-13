#include "vertexitem.h"
#include "mainwindow.h"
#include "view.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "graph.h"
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include  <cstdio>
#include <string>
#include <QInputDialog>
#include <QPushButton>
#include <QTreeWidgetItem>
MainWindow::MainWindow(QWidget */*parent*/)
    : scene_(new QGraphicsScene(this))
{
    scene_->setBackgroundBrush(Qt::black);
    populateScene();

    View *view = new View("Main Map View");
    view->view()->setScene(scene_);

    setCentralWidget(view);

    setWindowTitle(tr("Projet Map"));
}

void MainWindow::populateScene()
{
QPainter* paint;
 Graph graph("./graph_dc_area.txt");
    std::string file_name{"./graph_dc_area.txt"};

      std::ifstream fin{file_name};
     std::string line;
     size_t cnt{0};
  while (std::getline(fin, line)) {

      double lon,lat;
      int ide;
    vector<double>tab;
    ++cnt;
      if(line[0]!='#')
      { if(line[0]=='V'){
           std::stringstream ss(line);
            double x1,y1;
          char col, op, eq,vir;
          std::string var;

          ss >> col >>vir>>ide>>vir>> lon >> op >> lat >> eq >> var;
         x1=graph.to_cartesianX(ide);
         y1=graph.to_cartesianY(ide);

         // cout<<tab<<endl;
          string gr=to_string(ide)+ " "+to_string(x1)+ " "+to_string(y1);
          QGraphicsItem *item = new VertexItem(gr);

          scene_->addItem(item);


     }
          else if(line[0]=='E'){
             std::stringstream ss(line);
             double x1,y1,x2,y2;
             char col, eq,vir;
             int sid,did;
             std::string var;

             ss >> col >>vir>>sid>>vir>> did ;
            // cout<<sid<<"  "<<did<<endl;
             x1=graph.to_cartesianX(sid);
             y1=graph.to_cartesianY(sid);
             x2=graph.to_cartesianX(did);
             y2=graph.to_cartesianY(did);

             QGraphicsRectItem *_item;
            _item->setPen(QColor(102, 102, 102));

            auto *item2=new QGraphicsLineItem(x1,y1,x2,y2);
            QPen pen1;
            pen1.setWidth(20);
            pen1.setColor(QColor(128, 109, 52));
            item2->setLine(x1+20,y1+20,x2+20,y2+20);
            item2->setPen(pen1 );
            item2->setZValue(-1);
            scene_->addItem(item2);



          }
      }


    }
    fin.close();
  //}
  qDebug() << "Created " << cnt << " vertex object";
}
