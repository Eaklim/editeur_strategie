#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QList>
#include<iostream>
#include <QDebug>
#include <QtCore>
#include <QtGui>

#define ROBOTCENTRE robot1->pos().x() + GLOBALOFFSETX - 25,robot1->pos().y() + GLOBALOFFSETY + 15
#define ROBOTCENTREX robot1->pos().x() + GLOBALOFFSETX - 25
#define ROBOTCENTREY robot1->pos().y() + GLOBALOFFSETY + 15
#define NOMBRECOUCHE 36

double PosXrob = 0, PosYrob = 0, PosRotrob = 0, PosXrobPres = 90, PosYrobPres =0 , PosRotrobPres = 0,
        PosCXrob= 0, PosCYrob= 0, rayonCourbe, rayonCourbeVentouse, angleCourbe,angleRotation;


unsigned int coordGateau[NOMBRECOUCHE][5], Etage_ascenseur=0;

const QStringList MainWindow::dataCol1 = {"Debut Match",
                                          "Ligne Droite",
                                          "Rotation",
                                          "Courbe",
                                          "Action",
                                          "Recalage",
                                          "XYT",
                                          /*"toto"*/};

const QStringList MainWindow::dataEquipe = {"Bleu",
                                            "Vert"};


const QStringList MainWindow::dataAction = { "Evitement"
                                             ,"Set_odo"
                                             ,"Wait"
                                             ,"Wait_other_bot"
                                             ,"Check_pos_adversaire"
                                             ,"Fin_de_match"
                                             ,"Ajouter_point"
                                             ,"Prise_gateau"
                                             ,"Relacher_gateau"
                                             ,"Etage_gateau"
                                             ,"Pose_cerise"
                                             ,"Attrape_cerise"
                                             ,"Range_Attrape_cerise"
                                             ,"Tir_cerise"
                                             ,"Deguisement"
                                             ,"Barillet"};

int carreFlag = 1 , robot2Flag = 0;
double distanceLigneDroite;

void setItemImagePositionAsRobot(QGraphicsPixmapItem* item, QGraphicsPixmapItem* robot) {
    item->setPos(robot->pos());
}

/**********************************************************************************
*      Appel de la fonction pour les différent cases du setHeaderComboBox         *
*********************************************************************************/

void setTableHeaders_deploiement(QTableView *tableView, QAbstractItemDelegate *cbd) {
    tableView->model()->setHeaderData(2, Qt::Horizontal, QObject::tr("Equipe"),Qt::DisplayRole);
    tableView->model()->setHeaderData(3, Qt::Horizontal, QObject::tr("Angle"),Qt::DisplayRole);
    tableView->model()->setHeaderData(4, Qt::Horizontal, QObject::tr("Abs:x"),Qt::DisplayRole);
    tableView->model()->setHeaderData(5, Qt::Horizontal, QObject::tr("Ord:y"),Qt::DisplayRole);
    //tableView->model()->setHeaderData(6, Qt::Horizontal, QObject::tr("Case"),Qt::DisplayRole);
   //tableView->model()->setHeaderData(8, Qt::Horizontal, QObject::tr("Précis?"),Qt::DisplayRole);
   //tableView->model()->setHeaderData(9, Qt::Horizontal, QObject::tr("Enchainement"),Qt::DisplayRole);
    tableView->setItemDelegateForColumn(2, cbd);
    tableView->setItemDelegateForColumn(6, cbd);
    //tableView->setItemDelegateForColumn(8, cbd);
    //tableView->setItemDelegateForColumn(9, cbd);
}

void setTableHeaders_ligne_droite(QTableView *tableView, QAbstractItemDelegate *cbd) {
    tableView->model()->setHeaderData(2, Qt::Horizontal, QObject::tr("Dist"), Qt::DisplayRole);
    tableView->model()->setHeaderData(3, Qt::Horizontal, QObject::tr(" "), Qt::DisplayRole);
    tableView->model()->setHeaderData(4, Qt::Horizontal, QObject::tr(" "), Qt::DisplayRole);
    tableView->model()->setHeaderData(6, Qt::Horizontal, QObject::tr(" "),Qt::DisplayRole);
    tableView->model()->setHeaderData(5, Qt::Horizontal, QObject::tr(" "), Qt::DisplayRole);
    tableView->model()->setHeaderData(6, Qt::Horizontal, QObject::tr("Précis?"), Qt::DisplayRole);
    tableView->model()->setHeaderData(7, Qt::Horizontal, QObject::tr("Enchainmnt"), Qt::DisplayRole);
    //tableView->setItemDelegateForColumn(7, cbd);
    tableView->setItemDelegateForColumn(6, cbd);
    tableView->setItemDelegateForColumn(7, cbd);
}

void setTableHeaders_rotation(QTableView *tableView, QAbstractItemDelegate *cbd) {
    tableView->model()->setHeaderData(2, Qt::Horizontal, QObject::tr("Angle"),Qt::DisplayRole);
    tableView->model()->setHeaderData(3, Qt::Horizontal, QObject::tr("Relatif"),Qt::DisplayRole);
    tableView->model()->setHeaderData(4, Qt::Horizontal, QObject::tr(" "),Qt::DisplayRole);
    tableView->model()->setHeaderData(5, Qt::Horizontal, QObject::tr(" "),Qt::DisplayRole);
    tableView->model()->setHeaderData(6, Qt::Horizontal, QObject::tr("Précis?"),Qt::DisplayRole);
    tableView->model()->setHeaderData(7, Qt::Horizontal, QObject::tr("Enchainement"),Qt::DisplayRole);
    tableView->setItemDelegateForColumn(3,cbd);
    tableView->setItemDelegateForColumn(6,cbd);
    tableView->setItemDelegateForColumn(7,cbd);
}

void setTableHeaders_courbe(QTableView *tableView, QAbstractItemDelegate *cbd) {
    tableView->model()->setHeaderData(2, Qt::Horizontal, QObject::tr("Rayon"),Qt::DisplayRole);
    tableView->model()->setHeaderData(3, Qt::Horizontal, QObject::tr("Angle"),Qt::DisplayRole);
    tableView->model()->setHeaderData(4, Qt::Horizontal, QObject::tr("Sens"),Qt::DisplayRole);
    tableView->model()->setHeaderData(5, Qt::Horizontal, QObject::tr(" "),Qt::DisplayRole);
    tableView->model()->setHeaderData(6, Qt::Horizontal, QObject::tr("Précis?"),Qt::DisplayRole);
    tableView->model()->setHeaderData(7, Qt::Horizontal, QObject::tr("Enchainement"),Qt::DisplayRole);
    tableView->setItemDelegateForColumn(4,cbd);
    tableView->setItemDelegateForColumn(6,cbd);
    tableView->setItemDelegateForColumn(7,cbd);
}
void setTableHeaders_action(QTableView *tableView, QAbstractItemDelegate *cbd) {
    tableView->model()->setHeaderData(2, Qt::Horizontal, QObject::tr("Action"),Qt::DisplayRole);
    tableView->model()->setHeaderData(3, Qt::Horizontal, QObject::tr("Par1"),Qt::DisplayRole);
    tableView->model()->setHeaderData(4, Qt::Horizontal, QObject::tr("Par2"),Qt::DisplayRole);
    tableView->model()->setHeaderData(5, Qt::Horizontal, QObject::tr("Par3"),Qt::DisplayRole);
    tableView->model()->setHeaderData(6, Qt::Horizontal, QObject::tr("Précis?"),Qt::DisplayRole);
    tableView->model()->setHeaderData(7, Qt::Horizontal, QObject::tr("Enchainmnt"),Qt::DisplayRole);
    tableView->setItemDelegateForColumn(2,cbd); //change la fonction de la fenêtre, le delegate indique qu'il change en combobox
    tableView->setItemDelegateForColumn(3,cbd);
    tableView->setItemDelegateForColumn(6,cbd);
    tableView->setItemDelegateForColumn(7,cbd);
}

void setTableHeaders_recalage(QTableView *tableView, QAbstractItemDelegate *cbd) {
    tableView->model()->setHeaderData(2, Qt::Horizontal, QObject::tr("Sens"),Qt::DisplayRole);
    tableView->model()->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"),Qt::DisplayRole);
    tableView->model()->setHeaderData(4, Qt::Horizontal, QObject::tr("Axe"),Qt::DisplayRole);
    tableView->model()->setHeaderData(5, Qt::Horizontal, QObject::tr("Para"),Qt::DisplayRole);
    tableView->model()->setHeaderData(6, Qt::Horizontal, QObject::tr("Précis?"),Qt::DisplayRole);
    tableView->model()->setHeaderData(7, Qt::Horizontal, QObject::tr("Enchainmnt"),Qt::DisplayRole);
    tableView->setItemDelegateForColumn(2,cbd);
    tableView->setItemDelegateForColumn(3,cbd);
    tableView->setItemDelegateForColumn(4,cbd);
    tableView->setItemDelegateForColumn(5,cbd);
    tableView->setItemDelegateForColumn(6,cbd);
    tableView->setItemDelegateForColumn(7,cbd);
}

void setTableHeaders_XYT(QTableView *tableView, QAbstractItemDelegate *cbd) {
    tableView->model()->setHeaderData(2, Qt::Horizontal, QObject::tr("Sens"),Qt::DisplayRole);
    tableView->model()->setHeaderData(3, Qt::Horizontal, QObject::tr("Abs:x"),Qt::DisplayRole);
    tableView->model()->setHeaderData(4, Qt::Horizontal, QObject::tr("Ord:y"),Qt::DisplayRole);
    tableView->model()->setHeaderData(5, Qt::Horizontal, QObject::tr("Angle"),Qt::DisplayRole);
    tableView->model()->setHeaderData(6, Qt::Horizontal, QObject::tr("Précis?"),Qt::DisplayRole);
    tableView->model()->setHeaderData(7, Qt::Horizontal, QObject::tr("Enchainmnt"),Qt::DisplayRole);
    tableView->setItemDelegateForColumn(2,cbd);
    tableView->setItemDelegateForColumn(6,cbd);
    tableView->setItemDelegateForColumn(7,cbd);

    tableView->update();
}
////Affichage des couches de gateaux***********************************************************************************************************************

unsigned int coordonneesBase[NOMBRECOUCHE][5]={ // coordonnées des échantillons {x,y,COULEUR,pas_prise=0 + niveau 0 à 9/prise_avant=10 + niveau /prise_arrière=20 + prise, caché=0/visible=1}
                                    {   230,    575,    ROSE,0,1},{   230,    575,    ROSE,1,1},{   230,    575,    ROSE,2,1}, // en haut à gauche
                                    {   230,    775,  YELLOW,0,1},{   230,    775,  YELLOW,1,1},{   230,    775,  YELLOW,2,1},
                                    {   230,   2425,    ROSE,0,1},{   230,   2425,    ROSE,1,1},{   230,   2425,    ROSE,2,1},// en haut à droite
                                    {   230,   2225,  YELLOW,0,1},{   230,   2225,  YELLOW,1,1},{   230,   2225,  YELLOW,2,1},
                                    {   1775,   575,    ROSE,0,1},{   1775,   575,    ROSE,1,1},{   1775,   575,    ROSE,2,1},// en bas à gauche
                                    {   1775,   775,  YELLOW,0,1},{   1775,   775,  YELLOW,1,1},{   1775,   775,  YELLOW,2,1},
                                    {   1775,  2425,    ROSE,0,1},{   1775,  2425,    ROSE,1,1},{   1775,  2425,    ROSE,2,1},// en bas à droite
                                    {   1775,  2225,  YELLOW,0,1},{   1775,  2225,  YELLOW,1,1},{   1775,  2225,  YELLOW,2,1},
                                    {   730,   1125,   BROWN,0,1},{   730,   1125,   BROWN,1,1},{   730,   1125,   BROWN,2,1},//milieu haut gauche
                                    {   730,   1875,   BROWN,0,1},{   730,   1875,   BROWN,1,1},{   730,   1875,   BROWN,2,1}, //milieu haut droite
                                    {   1280,  1125,   BROWN,0,1},{   1280,  1125,   BROWN,1,1},{   1280,  1125,   BROWN,2,1}, //milieu bas gauche
                                    {   1280,  1875,   BROWN,0,1},{   1280,  1875,   BROWN,1,1},{   1280,  1875,   BROWN,2,1}}; //milieu bas droite

unsigned int xOffset_cake,yOffset_cake;

//position actuelle des elements du jeu à faire modifer à chaque action



//Affichage cérise****************************************************************************************************************************************
static QGraphicsItem *item[7],*Cerise[50];
    QPen redline(Qt::red,10,Qt::SolidLine);
    QPen blacklinedot(Qt::black,10,Qt::DotLine);
    QPen ellipsered(Qt::red,1,Qt::SolidLine);
    QBrush ellipsebrushred(Qt::red,Qt::SolidPattern);
    QPen ellipsegreen(Qt::green,1,Qt::SolidLine);
    QBrush ellipsebrushgreen(Qt::green,Qt::SolidPattern);
    QPen Pen[50];
    QBrush Brush;
    QPainterPath path,path1,pathArcVentouse[6];
    QModelIndex j;
    QModelIndex testindex;
    QString data;
    QFont font;
    QPainterPath pathtext[6];
    unsigned int Cerise_coord[4][2]={{15,1365},{2000-15,1365},{1000,15},{1000,2715}},Xoffset,Yoffset;
    unsigned char cerise_size=22;
    unsigned char Ecart_cerise=30;

void MainWindow::create_Gateau()
{
    for (int i = 0; i < NOMBRECOUCHE; i++) {
        for (int j = 0; j < 5; j++) {
            coordGateau[i][j] = coordonneesBase[i][j];
        }
    }

    for(int i=0;i<NOMBRECOUCHE;i++) // cree tous les Gateaux sur la table
    {
        QPixmap pix(determinerCouleur(coordGateau[i][2]));
        pix = pix.scaled(pix.width() * 0.27, pix.height() * 0.27, Qt::KeepAspectRatio);
        ptrGateau[i] = scene->addPixmap(pix);
        ptrGateau[i]->setOffset(-ptrGateau[i]->boundingRect().center().x(),
                                -ptrGateau[i]->boundingRect().center().y());

    }
}


void create_cerise(QGraphicsScene *scene)
{


    for(int i=0;i<10;i++) // cree tous les Cerises sur la table
    {

            Cerise[i] = scene->addEllipse(0,0,cerise_size,cerise_size,ellipsered,ellipsebrushred);
            Cerise[i]->setPos(Cerise_coord[0][1]+i*Ecart_cerise-cerise_size/2,Cerise_coord[0][0]-cerise_size/2);
            Cerise[i+10] = scene->addEllipse(0,0,cerise_size,cerise_size,ellipsered,ellipsebrushred);
            Cerise[i+10]->setPos(Cerise_coord[1][1]+i*Ecart_cerise-cerise_size/2,Cerise_coord[1][0]-cerise_size/2);
            Cerise[i+20] = scene->addEllipse(0,0,cerise_size,cerise_size,ellipsered,ellipsebrushred);
            Cerise[i+20]->setPos(Cerise_coord[2][1]+i*Ecart_cerise-cerise_size/2,Cerise_coord[2][0]-cerise_size/2);
            Cerise[i+30] = scene->addEllipse(0,0,cerise_size,cerise_size,ellipsered,ellipsebrushred);
            Cerise[i+30]->setPos(Cerise_coord[3][1]+i*Ecart_cerise-cerise_size/2,Cerise_coord[3][0]-cerise_size/2);
    }
    Cerise[40] =  scene->addEllipse(0,0,cerise_size,cerise_size,ellipsered,ellipsebrushred);
    Xoffset=-100*cos(((PosRotrob) * M_PI)/180)-0*sin(((PosRotrob) * M_PI)/180);
    Yoffset=-100*sin(((PosRotrob) * M_PI)/180)+0*cos(((PosRotrob) * M_PI)/180);
    Cerise[40]->setPos(PosYrob+Yoffset-cerise_size/2,PosXrob+Xoffset-cerise_size/2);// faire suivre la cérise au robot
//    Gobelet_coord[numero_Gobelet][0]=uint(PosXrob + Xoffset-36);
//    Gobelet_coord[numero_Gobelet][1]=uint(PosYrob + Yoffset-36);
//    Gobelet[numero_Gobelet]->setPos(Gobelet_coord[numero_Gobelet][1], Gobelet_coord[numero_Gobelet][0]);
//    Xoffset=Ventouse_coo[1+avt_arr][0]*cos(((PosRotrob) * M_PI)/180)-Ventouse_coo[1+avt_arr][1]*sin(((PosRotrob) * M_PI)/180);
//    Yoffset=Ventouse_coo[1+avt_arr][0]*sin(((PosRotrob) * M_PI)/180)+Ventouse_coo[1+avt_arr][1]*cos(((PosRotrob) * M_PI)/180);


}
//void gateau_pris(QGraphicsScene *scene)
//{
//    for (int i=0; i<12; i++)
//    {
//        if (coordonneesBase[i][3]==1)
//        {
//            Xoffset=-100*cos(((PosRotrob) * M_PI)/180)-0*sin(((PosRotrob) * M_PI)/180);
//            Yoffset=-100*sin(((PosRotrob) * M_PI)/180)+0*cos(((PosRotrob) * M_PI)/180);
//            coordonneesBase[40]->setPos(PosYrob+Yoffset-cerise_size/2,PosXrob+Xoffset-cerise_size/2);// faire suivre la cérise au robot
//        }
//    }

//}
/*************************************************************************************************/

/*************************************************************************************************/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,ptrGateau{new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,
                    new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,
                    new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,
                    new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,
                    new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,
                    new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem}
    ,ptrCarre{new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,
              new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem}
    ,brasMesure{new QGraphicsRectItem,new QGraphicsRectItem}
    ,ventouse{new QGraphicsEllipseItem ,new QGraphicsEllipseItem,new QGraphicsEllipseItem ,new QGraphicsEllipseItem,new QGraphicsEllipseItem ,
              new QGraphicsEllipseItem ,new QGraphicsEllipseItem,new QGraphicsEllipseItem ,new QGraphicsEllipseItem,new QGraphicsEllipseItem ,
              new QGraphicsEllipseItem ,new QGraphicsEllipseItem,new QGraphicsEllipseItem ,new QGraphicsEllipseItem,new QGraphicsEllipseItem ,
              new QGraphicsEllipseItem ,new QGraphicsEllipseItem}
    ,collisionLine{new QGraphicsLineItem,new QGraphicsLineItem,new QGraphicsLineItem,new QGraphicsLineItem}
    ,brasDistrib{new QGraphicsLineItem , new QGraphicsLineItem}

{
    initVisu();
    initModel();




    MyComboBoxDelegate *cbd = new MyComboBoxDelegate(ui->tableView, this);

//    connect(cbd, SIGNAL(updateHeaders(const QModelIndex &)), this, SLOT(updateHeader(const QModelIndex &)));

    ui->tableView->setItemDelegateForColumn(1, cbd);
    //ui->tableView->setItemDelegateForColumn(10, cbd);
    //ui->tableView->setItemDelegateForColumn(11, cbd);

    initHeaderData();
    setFocusOnButton();
    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tableViewIsClicked(const QModelIndex &))); //affichage dès le lancement de l'éditeur
    PosXrob=300;
    PosYrob=1125;
    ui->tableView->model()->insertRow(ui->tableView->model()->rowCount());
    ui->tableView->model()->setData(ui->tableView->model()->index(0,0),0);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,1),dataCol1.at(0));
    ui->tableView->model()->setData(ui->tableView->model()->index(0,2),dataEquipe.at(0));
    ui->tableView->model()->setData(ui->tableView->model()->index(0,3),PosRotrob);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,4),PosXrob);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,5),PosYrob);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,6)," ");
    ui->tableView->model()->setData(ui->tableView->model()->index(0,7)," ");

    ui->tableView->model()->setData(ui->tableView->model()->index(0,10),1);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,11),1);
    QModelIndex newIndex  = ui->tableView->model()->index(0,0);
    ui->tableView->setCurrentIndex(newIndex);

    robot1->setPos(PosYrob,PosXrob);
    robot1->setRotation(90-PosRotrob);
    robotdep->setPos(PosYrob,PosXrob);
    robotdep->setRotation(90-PosRotrob);
    ui->lcdPosX->display(PosXrob);
    ui->lcdPosY->display(PosYrob);
    ui->lcdPosT->display(PosRotrob);

    ui->graphicsView->setMouseTracking(true);

//    connect(ui->graphicsView, SIGNAL(mouseMoveEvent(QMouseEvent *event)),this, SLOT(setMousePoint(QMouseEvent *event)));
}


void MainWindow::initVisu()//**********************************************************************
{
    qDebug() << "init visu";
    ui->setupUi(this); //L'user Interface démarre

    // positions initiales des splitter en fixant les zones
    QList<int> size;
    size << 0 << 0;
    size[0] = 0.5 * width();
    size[1] = width() - size[0];
    ui->splitterHoriz->setSizes(size);
    size[0] = 0.75 * height();
    size[1] = height() - size[0];
    ui->splitterVerti->setSizes(size);

    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,3000,2000);//???????????
    ui->graphicsView->setScene(scene);
    //Intégration des images pour la scene
    QImage tapis(":/Images/Cake2023/table2023.png");
    QPixmap scaledTapis = QPixmap::fromImage(tapis.scaled(3000, 2000, Qt::KeepAspectRatio)); //conserver le bon mise en forme

    image = scene->addPixmap(scaledTapis);
    image->setOffset(0,0);

//visu du robot***************************************************************************************
    QPixmap robot(":/Images/Cake2023/ROB2023.png");

    robot1 = scene->addPixmap(robot);
    robot1->setPixmap(robot.scaled(LARGEUR_ROBOT,LARGEUR_ROBOT,Qt::KeepAspectRatio));
    robot1->setOffset(-robot1->boundingRect().center().x(),
                      -robot1->boundingRect().center().y());
    robot1->setPos(0,0); //Le robot est positionné
    robot1->setRotation(90);

    robotdep = scene->addPixmap(robot);
    robotdep->setPixmap(robot.scaled(LARGEUR_ROBOT,LARGEUR_ROBOT,Qt::KeepAspectRatio));
    robotdep->setOffset(-robotdep->boundingRect().center().x(),
                        -robotdep->boundingRect().center().y());
    robotdep->setPos(1000,1000); //Le robot est positionné
    robotdep->setRotation(90);
    robotdep->hide();


    //Création des bordures virtuelles
    QPen redline(Qt::red);
    QPen blackline(Qt::black);
    redline.setStyle(Qt::SolidLine);
    redline.setWidth(3);
    redline.setBrush(Qt::red);
    lineUp = scene->addLine(ORIGINE_X,ORIGINE_Y,LONGUEUR_TABLE,ORIGINE_Y,redline);
    lineDown = scene->addLine(ORIGINE_X,LARGEUR_TABLE,LONGUEUR_TABLE,LARGEUR_TABLE,redline);
    lineRight = scene->addLine(LONGUEUR_TABLE,ORIGINE_Y,LONGUEUR_TABLE,LARGEUR_TABLE,redline);
    lineLeft = scene->addLine(ORIGINE_X,ORIGINE_Y,ORIGINE_X,LARGEUR_TABLE,redline);
    lineMap1 = scene->addLine(OBSTACLE_1_X,OBSTACLE_1_Y,OBSTACLE_1_X,LARGEUR_TABLE,redline);
    lineMap2 = scene->addLine(OBSTACLE_2_X,OBSTACLE_2_Y,OBSTACLE_2_X,LARGEUR_TABLE,redline);
    lineMap3 = scene->addLine(OBSTACLE_3_X,OBSTACLE_3_Y,OBSTACLE_3_X,LARGEUR_TABLE,redline);
    lineUp->hide();
    lineDown->hide();
    lineRight->hide();
    lineLeft->hide();
    lineMap1->hide();
    lineMap2->hide();
    lineMap3->hide();

    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    qDebug() << scene->sceneRect();

    create_cerise(scene);
    create_Gateau();
    afficher_Gateau();
}
//********************************************************************************************************************
QPixmap MainWindow::determinerCouleur(int couleur){
    QPixmap pixReturn;

    //on vérifie d'abord si l'échantillon doit être face caché
    //if (coordonnees[i][3] == 0){
        switch(couleur){
        case BROWN :
            pixReturn.load(":/Images/Cake2023/face_couche_brun.png");
             qDebug("test couche marron");
            break;
        case ROSE :
            pixReturn.load(":/Images/Cake2023/face_couche_rose.png");
            qDebug("test couche rose");
            break;
        case YELLOW  :
            pixReturn.load(":/Images/Cake2023/face_couche_jaune.png");
            qDebug("test couche jaune");
            break;
       }
    return pixReturn;
}

//********************************************************************************************************************
void MainWindow::SetView()
{
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event); //Détection de l'évènement
    SetView(); //Actualise la vue (taille des images dans le "graphics view")
}

/**********************************************************************************
 *      Fonction setHeaderComboBox(int index) : Modifie les headers en fonction   *
 *                            de l'index de la ComboBox                           *
 *********************************************************************************/
void MainWindow::setHeaderComboBox(int index)
{
    MyComboBoxDelegate *cbd = dynamic_cast<MyComboBoxDelegate *>(ui->tableView->itemDelegateForColumn(1));//ajoute les différentes options pour le column 1
    resetComboBox();
    switch (index) {

    case 0 : //Début match
       setTableHeaders_deploiement(ui->tableView, cbd);
       break;

    case 1 : //Ligne droite
      setTableHeaders_ligne_droite(ui->tableView, cbd);
        break;

    case 2 : //Rotation
        setTableHeaders_rotation(ui->tableView, cbd);
        break;

    case 3 : //Courbe
        setTableHeaders_courbe(ui->tableView, cbd);
        break;

    case 4 : //Action
        setTableHeaders_action(ui->tableView, cbd);
        break;

    case 5 : //Recalage
        setTableHeaders_recalage(ui->tableView, cbd);
        break;

    case 6 : //XYT
        setTableHeaders_XYT(ui->tableView, cbd);
        break;
    }

    ui->tableView->viewport()->update();
}


/**********************************************************************************
 *               Fonction initHeaderData() : Initialise les headers               *
 *********************************************************************************/
void MainWindow::initHeaderData()
{
    ui->tableView->model()->setHeaderData(1, Qt::Horizontal, QObject::tr("Inst"),Qt::DisplayRole);
    ui->tableView->model()->setHeaderData(8, Qt::Horizontal, QObject::tr(" "),Qt::DisplayRole);
    ui->tableView->model()->setHeaderData(9, Qt::Horizontal, QObject::tr(" "),Qt::DisplayRole);
    ui->tableView->model()->setHeaderData(10, Qt::Horizontal, QObject::tr("L.S."),Qt::DisplayRole);// c'est quoi LS??????
    ui->tableView->model()->setHeaderData(11, Qt::Horizontal, QObject::tr("SiInter"),Qt::DisplayRole);

//    ui->tableView->resizeColumnsToContents();
}

/**********************************************************************************
 *                  Fonction initModel() : Initialise le modèle                   *
 *********************************************************************************/
void MainWindow::initModel()//quelle modèle??????
{
    MyModel *myModel = new MyModel(this);
    ui->tableView->setModel(myModel);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

/**********************************************************************************
 *   Fonction tableViewIsClicked(const QModelIndex &index) : A chaque fois que    *
 *          l'utilisateur clique sur le tableau, actualise les headers            *
 *********************************************************************************/
void MainWindow::tableViewIsClicked(const QModelIndex &index)
{
    updateHeader(index);
    updateVisu(index);
}

void MainWindow::updateHeader(const QModelIndex &index)
{
    QModelIndex i = index.model()->index(index.row(), 1);
    QString data = index.model()->data(i).toString();
    setHeaderComboBox(dataCol1.indexOf(data));
}

/***********************************************************************************/
// Fonction qui calcul le trajectoire des lignes pour l'avancé ligne droite         *
//***********************************************************************************

void drawRobotPath(int table_ligne, QModelIndex index, QGraphicsScene* scene, QGraphicsItem* item[3], QPen redline, double PosXrobPres, double PosYrobPres, double PosRotrobPres, double largeur_robot, double distanceLigneDroite)
{


    if(table_ligne == index.row())
    {
        double PosXrob = PosXrobPres + distanceLigneDroite * cos(((PosRotrobPres) * M_PI)/180);
        double PosYrob = PosYrobPres + distanceLigneDroite * sin((PosRotrobPres * M_PI)/180);
        // Tracé Ligne centre
        item[0] = scene->addLine(int(PosYrobPres), int(PosXrobPres),int(PosYrob),int(PosXrob),redline);
        // Tracé Ligne gauche
        item[1] = scene->addLine(int(PosYrobPres + largeur_robot/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrobPres + largeur_robot/2*cos(((PosRotrobPres-90) * M_PI)/180))
        ,int(PosYrob + largeur_robot/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrob + largeur_robot/2*cos(((PosRotrobPres-90) * M_PI)/180)),redline);
        // Tracé Ligne de droite
        item[2] = scene->addLine(int(PosYrobPres - largeur_robot/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrobPres - largeur_robot/2*cos(((PosRotrobPres-90) * M_PI)/180))
                ,int(PosYrob - largeur_robot/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrob - largeur_robot/2*cos(((PosRotrobPres-90) * M_PI)/180)),redline);
    }
}

//**********************************************************************************************************

//void calculcentrerotationgauche(double PosXrobPres, double PosYrobPres, double PosRotrobPres, double rayonCourbe, double angleCourbe, double &PosXrob, double &PosYrob, double &PosRotrob) {
//    double PosCXrob = PosXrobPres - rayonCourbe * sin((PosRotrobPres * M_PI)/180);
//    double PosCYrob = PosYrobPres + rayonCourbe * cos((PosRotrobPres * M_PI)/180);
//
//    PosXrob = PosCXrob + rayonCourbe * sin(((PosRotrobPres+angleCourbe) * M_PI)/180);
//    PosYrob = PosCYrob - rayonCourbe * cos(((PosRotrobPres+angleCourbe) * M_PI)/180);
//    PosRotrob = (PosRotrobPres) + angleCourbe;
//}

//**********************************************************************************************************





// Update position of game objects during the strat
void MainWindow::updateVisu(const QModelIndex &index)
{
     qDebug("table view clicked");
    //à remplir pour mettre à jour les mouvements et avoir les tracés rouges

    //int i = index.row();
     static QGraphicsItem *item[7] = {0};
    int angle, rayon;
    int Pen_Random[6]={1,2,2,1,1,2};
    double facteur=1;
    //    unsigned int Gobelet_robot[6]={100,100,100,100,100,100};
    int Manche_coord[2][2]={{50,-75},{-15,75}};
    double rayonCourbeVent[6];
    int Xoffset,Yoffset,avt_arr,incr,numero_Gobelet,valeur=1;
    double m,p,Xdepart,Xarrivee,Ydepart,Yarrivee,x,y,xc,yc,R,calcul;
    unsigned int numero_action,numero_manche,numero_ligne,numero_ligne_courbe;
    int centaine,dizaine,unite;
    QPen redline(Qt::red,10,Qt::SolidLine);
    QPen greenline(Qt::green,10,Qt::SolidLine);
    QPen blacklinedot(Qt::black,10,Qt::DotLine);
    QPen ellipsered(Qt::red,1,Qt::SolidLine);
    QBrush ellipsebrushred(Qt::red,Qt::SolidPattern);
    QPen ellipsedarkred(Qt::darkRed,1,Qt::SolidLine);
    QBrush ellipsebrushdarkred(Qt::darkRed,Qt::SolidPattern);
    QPen ellipsegreen(Qt::green,1,Qt::SolidLine);
    QBrush ellipsebrushgreen(Qt::green,Qt::SolidPattern);
    QPen ellipsedarkgreen(Qt::darkGreen,1,Qt::SolidLine);
    QBrush ellipsebrushdarkgreen(Qt::darkGreen,Qt::SolidPattern);
    QPen Pen[50];
    QBrush Brush;
    QPainterPath path,path1,pathArcVentouse[6];
    QModelIndex j;
    QModelIndex testindex;
    QString data;
    QFont font;
    QPainterPath pathtext[6];
    font.setPixelSize(50);
    font.setBold(false);
    font.setFamily("Times");
    QPixmap tapis(":/Images/Cake2023/table2023.png");
    QPainter painter;
    //QPixmap tapisFlipped(":/Images/Cake2023/tableFlipped.png");
    static int equipe, old_equipe, start_update=0,futur_i=0;
    bool check,check_blocage;
    int indexComboBox,flagStop = 0,table_ligne = 0;
    int newValue;
    bool resDeploye[2] {false,false};


    for (int i = 0; i < NOMBRECOUCHE; i++) {
        for (int j = 0; j < 5; j++) {
            coordGateau[i][j] = coordonneesBase[i][j];
        }
    }
    check = ui->checkBox->isChecked();
    setWindowTitle("éditeur de stratégie 2023 - The cherry on the cake ");


    qDebug() << nbUpdateVisu << "______________________________________________________________________________________________________";

    int cpt_boucle=0;
    Etage_ascenseur=0;

    while((table_ligne<index.row()+1)&&(cpt_boucle<(2*(ui->tableView->model()->rowCount())))) // on suit les numero de lignes et on bloque la boucle infini à 2 occurences
    {
        cpt_boucle++;
        check_blocage = ui->checkBox_blocage->isChecked();
        if((table_ligne==ui->Blocage_ligne->value())&&(check_blocage==true))
        {
            facteur=ui->Blocage_pourcentage->value()/100.0;
            futur_i=ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,11)).toInt();
        }
        else
        {
            facteur=1.0;
            futur_i=ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,10)).toInt();
        }
        j = ui->tableView->model()->index(table_ligne,1);
        data = ui->tableView->model()->data(j).toString();
        indexComboBox = dataCol1.indexOf(data);

        PosXrobPres = PosXrob;
        PosYrobPres = PosYrob;
        PosRotrobPres = PosRotrob;

        if((indexComboBox!=2)&&(indexComboBox!=6))
        {
                robotdep->setPos(PosYrob,PosXrob);
                robotdep->setRotation(90-PosRotrob);
        }
        qDebug() << "Rotation robot = " << robot1->rotation();

        // remove toutes les lignes de déplacement et les lignes des ventouses

//        for(int i=0;i<4;i++)
//            scene->removeItem(collisionLine[i]);
//        for(int i=0;i<6;i++){// a enlever????
//            scene->removeItem(ventouse[i]);// a enlever????
//        }
//        for(int i=0;i<2;i++){// a enlever????
//            scene->removeItem(brasDistrib[i]);// a enlever????
//        }

        Affichage_Debug_Coord_Gateau();
        qDebug("efface item");
        for(int i=0;i<7;i++)
        {
            if (item[i])
                scene->removeItem(item[i]);
        }

        switch(indexComboBox)
        {
        case 0: //Début match
            qDebug("Début match");
            old_equipe = equipe;
                       testindex = ui->tableView->model()->index(table_ligne,2);
                       if(ui->tableView->model()->data(testindex).toString() == "Bleu")
                       {
                           testindex = ui->tableView->model()->index(table_ligne,4);
                           PosXrob = ui->tableView->model()->data(testindex).toDouble();
                           testindex = ui->tableView->model()->index(table_ligne,5);
                           PosYrob = ui->tableView->model()->data(testindex).toDouble();
                           testindex = ui->tableView->model()->index(table_ligne,3);
                           PosRotrob = (ui->tableView->model()->data(testindex).toDouble());
                           PosRotrobPres=PosRotrob;
                           PosXrobPres=PosXrob;
                           PosYrobPres=PosYrob;
                           equipe = 0;
                       }
                       else if(ui->tableView->model()->data(testindex).toString() == "Vert")
                       {
                           testindex = ui->tableView->model()->index(table_ligne,4);
                           PosXrob = 2000- (ui->tableView->model()->data(testindex).toDouble()) ;
                           testindex = ui->tableView->model()->index(table_ligne,5);
                           PosYrob = (ui->tableView->model()->data(testindex).toDouble());
                           testindex = ui->tableView->model()->index(table_ligne,3);
                           PosRotrob = (ui->tableView->model()->data(testindex).toDouble()) +180 ;
                           PosRotrobPres=PosRotrob;
                           PosXrobPres=PosXrob;
                           PosYrobPres=PosYrob;
                           equipe = 1;
                       }
                       robot1->setPos(PosYrob,PosXrob);

            break;

        case 1: //Ligne Droite
            // effacer les trassé d'avant
            qDebug("Ligne Droite");

            if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="")

            {
                for(int j=2;j<12;j++)
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,j),"");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,0),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,0)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,2),200);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,6),"Non");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,7),"Attendre");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,10),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,10)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,11),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,11)).toInt()+1);
            }
            testindex = ui->tableView->model()->index(table_ligne,2);
            /*
             * x = distance * cos (angle)
             * y = distance * sin (angle)
             */
            distanceLigneDroite = facteur*(ui->tableView->model()->data(testindex).toDouble());
            PosXrob = PosXrobPres + distanceLigneDroite * cos(((PosRotrobPres) * M_PI)/180);
            PosYrob = PosYrobPres + distanceLigneDroite * sin((PosRotrobPres * M_PI)/180);
            if(table_ligne==index.row())
            {
               //// Tracé Ligne centre
               //item[0]= scene->addLine(int(PosYrobPres), int(PosXrobPres),int(PosYrob),int(PosXrob),redline);
               //// Tracé Ligne gauche
               //item[1] = scene->addLine(int(PosYrobPres + LARGEUR_ROBOT/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrobPres + LARGEUR_ROBOT/2*cos(((PosRotrobPres-90) * M_PI)/180))
               //,int(PosYrob + LARGEUR_ROBOT/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrob + LARGEUR_ROBOT/2*cos(((PosRotrobPres-90) * M_PI)/180)),redline);
               //// Tracé Ligne de droite
               //item[2] = scene->addLine(int(PosYrobPres - LARGEUR_ROBOT/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrobPres - LARGEUR_ROBOT/2*cos(((PosRotrobPres-90) * M_PI)/180))
               //,int(PosYrob - LARGEUR_ROBOT/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrob - LARGEUR_ROBOT/2*cos(((PosRotrobPres-90) * M_PI)/180)),redline);

               //appel de la fonction drawRobotPath qui permet de calcuer le tracé de ligne de "Ligne Droite"
               drawRobotPath(table_ligne, index, scene, item, redline, PosXrobPres, PosYrobPres, PosRotrobPres, (float)LARGEUR_ROBOT, distanceLigneDroite);


            }

            break;

        case 2: //Rotation
            qDebug("Rotation");

            if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="")
            {
                for(int j=2;j<12;j++)
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,j),"");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,0),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,0)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,2),0);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),"Absolu");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,6),"Non");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,7),"Attendre");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,10),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,10)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,11),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,11)).toInt()+1);
            }

            if(equipe == 0)
            {
                testindex = ui->tableView->model()->index(table_ligne,2);
                angleRotation=ui->tableView->model()->data(testindex).toDouble();
                if(ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString()=="Relatif")
                    PosRotrob = (PosRotrobPres + angleRotation);
                else
                    PosRotrob = angleRotation;
            }
            if(equipe == 1)
            {
                testindex = ui->tableView->model()->index(table_ligne,2);
                angleRotation=ui->tableView->model()->data(testindex).toDouble();
                if(ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString()=="Relatif")
                    PosRotrob = (PosRotrobPres - angleRotation);
                else
                    PosRotrob = -angleRotation;
            }
            break;

        case 3: //Courbe
            qDebug("courbe");
            if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="")
            {
                for(int j=2;j<12;j++)
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,j),"");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,0),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,0)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,2),500);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),45);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,4),"Gauche");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,6),"Non");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,7),"Attendre");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,10),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,10)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,11),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,11)).toInt()+1);
            }
            testindex = ui->tableView->model()->index(table_ligne,2);
            rayonCourbe = ui->tableView->model()->data(testindex).toDouble();
            testindex = ui->tableView->model()->index(table_ligne,3);
            angleCourbe = facteur*(ui->tableView->model()->data(testindex).toDouble());

            //Inversion de sens de courbe si il y a un changement d'équipe
            testindex = ui->tableView->model()->index(table_ligne,4);
            if(equipe==0 && old_equipe==1 && (ui->tableView->model()->data(testindex).toString()) == "Gauche")
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,4),"Droite");
            }
            else if(equipe==0 && old_equipe==1 && (ui->tableView->model()->data(testindex).toString()) == "Droite")
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,4),"Gauche");
            }
            else if(equipe==1 && old_equipe==0 && (ui->tableView->model()->data(testindex).toString()) == "Gauche")
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,4),"Droite");
            }
            else if(equipe==1 && old_equipe==0 && (ui->tableView->model()->data(testindex).toString()) == "Droite")
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,4),"Gauche");
            }

            if(ui->tableView->model()->data(testindex).toString() == "Gauche")
            {
                // calcul du centre de rotation
               PosCXrob = PosXrobPres - rayonCourbe * sin((PosRotrob * M_PI)/180);
               PosCYrob = PosYrobPres + rayonCourbe * cos((PosRotrob * M_PI)/180);

               PosXrob = PosCXrob + rayonCourbe * sin(((PosRotrob+angleCourbe) * M_PI)/180);
               PosYrob = PosCYrob - rayonCourbe * cos(((PosRotrob+angleCourbe) * M_PI)/180);
               PosRotrob = (PosRotrobPres) + angleCourbe;

                //calculcentrerotationgauche(PosXrobPres, PosYrobPres, PosRotrobPres, rayonCourbe, angleCourbe, PosXrob, PosYrob, PosRotrob);
                //******************************************************************************************
                // tracé de la trajectoire
                if(table_ligne==index.row())
                {
                    path.moveTo(PosYrobPres,PosXrobPres);
                    path.arcTo(PosCYrob-rayonCourbe,PosCXrob-rayonCourbe,2*rayonCourbe,2*rayonCourbe,180+PosRotrobPres,angleCourbe);
                    item[0]=scene->addPath(path,redline);
                    path.moveTo(int(PosYrobPres - LARGEUR_ROBOT/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrobPres - LARGEUR_ROBOT/2*cos(((PosRotrobPres-90) * M_PI)/180)));
                    path.arcTo(PosCYrob-rayonCourbe+LARGEUR_ROBOT/2,PosCXrob-rayonCourbe+LARGEUR_ROBOT/2,2*(rayonCourbe-LARGEUR_ROBOT/2),2*(rayonCourbe-LARGEUR_ROBOT/2)
                               ,180+PosRotrobPres,angleCourbe);
                    item[1]=scene->addPath(path,redline);
                    path.moveTo(int(PosYrobPres + LARGEUR_ROBOT/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrobPres + LARGEUR_ROBOT/2*cos(((PosRotrobPres-90) * M_PI)/180)));
                    path.arcTo(PosCYrob-rayonCourbe-LARGEUR_ROBOT/2,PosCXrob-rayonCourbe-LARGEUR_ROBOT/2,2*(rayonCourbe+LARGEUR_ROBOT/2),2*(rayonCourbe+LARGEUR_ROBOT/2)
                               ,180+PosRotrobPres,angleCourbe);
                    item[2]=scene->addPath(path,redline);
                    unsigned int diag=DIAG_ROBOT1;
                    for(int i=0;i<6;i++)
                    path1.moveTo((PosYrob+(diag/2)*cos(((90-PosRotrobPres) * M_PI)/180)),PosXrob+(diag/2)*sin(((90-PosRotrobPres) * M_PI)/180));
                    path1.arcTo(PosYrob-diag/2,PosXrob-diag/2,diag,diag,PosRotrobPres-90,360);
                    item[3]=scene->addPath(path1,blacklinedot);
                    path.moveTo((PosYrob+(diag/2)*cos(((90-PosRotrobPres) * M_PI)/180)),PosXrob+(diag/2)*sin(((90-PosRotrobPres) * M_PI)/180));
                    path.arcTo(PosYrob-diag/2,PosXrob-diag/2,diag,diag,PosRotrobPres-90,PosRotrobPres-PosRotrob);
                    item[4]=scene->addPath(path,redline);
                }
            }
            else
            {
                // calcul du centre de rotation
                PosCXrob = PosXrobPres + rayonCourbe * sin((PosRotrob * M_PI)/180);
                PosCYrob = PosYrobPres - rayonCourbe * cos((PosRotrob * M_PI)/180);

                PosXrob = PosCXrob - rayonCourbe * sin(((PosRotrob-angleCourbe) * M_PI)/180);
                PosYrob = PosCYrob + rayonCourbe * cos(((PosRotrob-angleCourbe) * M_PI)/180);
                PosRotrob = (PosRotrobPres) - angleCourbe;
                if(table_ligne==index.row())
                {
                    // tracé de la trajectoire
                    if (item[0]) scene->removeItem(item[0]);
                    if (item[1]) scene->removeItem(item[1]);
                    if (item[2]) scene->removeItem(item[2]);
                    path.moveTo(PosYrobPres,PosXrobPres);
                    path.arcTo(PosCYrob-rayonCourbe,PosCXrob-rayonCourbe,2*rayonCourbe,2*rayonCourbe,PosRotrobPres,-angleCourbe);
                    item[0]=scene->addPath(path,redline);
                    path.moveTo(int(PosYrobPres + LARGEUR_ROBOT/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrobPres + LARGEUR_ROBOT/2*cos(((PosRotrobPres-90) * M_PI)/180)));
                    path.arcTo(PosCYrob-rayonCourbe+LARGEUR_ROBOT/2,PosCXrob-rayonCourbe+LARGEUR_ROBOT/2,2*(rayonCourbe-LARGEUR_ROBOT/2),2*(rayonCourbe-LARGEUR_ROBOT/2)
                               ,PosRotrobPres,-angleCourbe);
                    item[1]=scene->addPath(path,redline);
                    path.moveTo(int(PosYrobPres - LARGEUR_ROBOT/2*sin(((PosRotrobPres-90) * M_PI)/180)), int(PosXrobPres - LARGEUR_ROBOT/2*cos(((PosRotrobPres-90) * M_PI)/180)));
                    path.arcTo(PosCYrob-rayonCourbe-LARGEUR_ROBOT/2,PosCXrob-rayonCourbe-LARGEUR_ROBOT/2,2*(rayonCourbe+LARGEUR_ROBOT/2),2*(rayonCourbe+LARGEUR_ROBOT/2)
                               ,PosRotrobPres,-angleCourbe);
                    item[2]=scene->addPath(path,redline);

                }
            }


            break;

        case 4: //Action
            qDebug("Action");
            if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="")
            {
                /*if (item[0]) scene->removeItem(item[0]);
                item[0] = 0;
                if (item[1]) scene->removeItem(item[1]);
                item[1] = 0;
                if (item[2]) scene->removeItem(item[2]);
                item[2] = 0;*/
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,0),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,0)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,2),"Prise_gateau");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),"Avant");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,10),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,10)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,11),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,11)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,6),"Non");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,7),"Enchainement");
            }



//Prise_gateau_______________________________________________________________________________________________________________
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Prise_gateau")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Avant")){
                qDebug("Test prise avant");
                // Pick up the cake at the front of the robot
                 detecterCollisionGateau(0);
            }
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Prise_gateau")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Arrière")){

                qDebug("Test prise arriere");
                  // Pick up the cake at the back of the robot
                 detecterCollisionGateau(1);
            }
//Relacher gateau _________________________________________________________________________________________________________________________________
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Relacher_gateau")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Avant")){
                qDebug("Test relacher avant");
                //qDebug() << "Cake picked up with front end";
                // Release the last layer of cake that has been collected from the front
                relacherGateau(0);
            }
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Relacher_gateau")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Arrière")){

                qDebug("Test relacher arriere");
                // Release the last layer of cake that has been collected from the front
                relacherGateau(1);
            }

// niveau ascenseur
            if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Etage_gateau")
            {
                qDebug("Etage ascenseur %d", Etage_ascenseur );
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Etage 0")
                    Etage_ascenseur=0;
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Etage 1")
                    Etage_ascenseur=1;
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Etage 2")
                    Etage_ascenseur=2;
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Etage 3")
                    Etage_ascenseur=3;
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Etage 4")
                    Etage_ascenseur=4;
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Etage 5")
                    Etage_ascenseur=5;
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Etage 6")
                    Etage_ascenseur=6;
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Etage 7")
                    Etage_ascenseur=7;
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Etage 8")
                    Etage_ascenseur=8;
                qDebug("Etage ascenseur %d", Etage_ascenseur );
            }
            break;
//**********************************************************************************************************************


        case 5: //Recalage
            qDebug("Recalage");
            if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="")
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,0),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,0)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,2),"Arrière");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),"Mécanique");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,4),"Abscisse");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,5),"Rob-");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,10),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,10)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,11),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,11)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,6),"Non");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,7),"Enchainement");
            }

            if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,5)).toString())=="Rob-") newValue = 115;
            else if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,5)).toString())=="Rob+") newValue = 1885;
            else if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,5)).toString())=="Gob-") newValue = 168;
            else if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,5)).toString())=="Gob+") newValue = 1832;
            else newValue=115;

            //Recalage Abscisse
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,4)).toString())=="Abscisse") && ((PosRotrob == 90.)||(PosRotrob == 270.)))
            {
                //arrière abscisse
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Arrière" && (PosRotrob == 90.))
                {
                    PosYrob= newValue;
                    robot1->setPos(PosYrob,PosXrob);
                }
                else if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Arrière" && (PosRotrob == 270.))
                {
                    PosYrob = newValue;
                    robot1->setPos(PosYrob,PosXrob);
                }
                //avant abscisse
                else if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Avant" && (PosRotrob == 90.))
                {
                    PosYrob = newValue;
                    robot1->setPos(PosYrob,PosXrob);
                }
                else if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Avant" && (PosRotrob == 270.))
                {
                    PosYrob = newValue;
                    robot1->setPos(PosYrob,PosXrob);
                }
            }

            //Recalage Ordonnée
            else if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,4)).toString())=="Ordonnée") && ((PosRotrob == 0.)||(PosRotrob == 180.)))
            {
                //Arrière Ordonnée
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Arrière" && (PosRotrob == 0.))
                {
                    PosXrob = newValue;
                    robot1->setPos(PosYrob,PosXrob);
                }
                else if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Arrière" && (PosRotrob == 180.))
                {
                    PosXrob = newValue;
                    robot1->setPos(PosYrob,PosXrob);
                }

                //Avant Ordonnée
                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Avant" && (PosRotrob == 180.))
                {
                    PosXrob = newValue;
                    robot1->setPos(PosYrob,PosXrob);
                }
                else if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Avant" && (PosRotrob == 0.))
                {
                    PosXrob = newValue;
                    robot1->setPos(PosYrob,PosXrob);
                }
            }
            if(table_ligne==index.row())
            {
            }
            break;

        case 6: //XYT
            qDebug("XYT");
            int diag=DIAG_ROBOT1;
            double anglefin=0;

            if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="")
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,0),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,0)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,2),"Avant");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),PosXrob);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,4),PosYrob);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,5),PosRotrob);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,6),"Non");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,7),"Attendre");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,10),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,10)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,11),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,11)).toInt()+1);
            }

            testindex = ui->tableView->model()->index(table_ligne,3);
            PosXrob = ui->tableView->model()->data(testindex).toDouble();
            testindex = ui->tableView->model()->index(table_ligne,4);
            PosYrob = ui->tableView->model()->data(testindex).toDouble();
            testindex = ui->tableView->model()->index(table_ligne,5);
            PosRotrob = (ui->tableView->model()->data(testindex).toDouble());

            if(equipe==0 && old_equipe==1) //Jaune à bleu
            {
                PosYrob = 3000-PosYrob;
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,4),PosYrob);
                PosRotrob = -PosRotrob;
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,5),PosRotrob);
            }
            else if(equipe==1 && old_equipe==0) //bleu à jaune
            {
                PosYrob = 3000-PosYrob;
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,4),PosYrob);
                PosRotrob = -PosRotrob;
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,5),PosRotrob);
            }
            // calculer l'angle à faire pour se positionner sur la ligne droite
            double angle=abs(qAtan2(PosYrob-PosYrobPres,PosXrob-PosXrobPres)* 180)/M_PI;
            avt_arr=0;
            if(PosYrob-PosYrobPres<0)
            {
                angle=-angle;
            }
            if(table_ligne==index.row())
            {
                for (int i=0; i<6; i++)
                    if (item[i])
                        scene->removeItem(item[i]);

                // Tracé Ligne centre trajectoire du robot
                item[0]= scene->addLine(int(PosYrobPres), int(PosXrobPres),int(PosYrob),int(PosXrob),redline);

                ui->lcdPosArcOffset->display(angle);

                if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Arrière")
                {
                    angle+=180;
                    avt_arr=3;
                }
                ui->lcdPosArcDebut->display(angle);
                // Tracé Ligne gauche trajectoire du robot
                item[1] = scene->addLine(int(PosYrobPres + LARGEUR_ROBOT/2*sin(((angle-90) * M_PI)/180)), int(PosXrobPres + LARGEUR_ROBOT/2*cos(((angle-90) * M_PI)/180))
                ,int(PosYrob + LARGEUR_ROBOT/2*sin(((angle-90) * M_PI)/180)), int(PosXrob + LARGEUR_ROBOT/2*cos(((angle-90) * M_PI)/180)),redline);
                // Tracé Ligne de droite trajectoire du robot
                item[2] = scene->addLine(int(PosYrobPres - LARGEUR_ROBOT/2*sin(((angle-90) * M_PI)/180)), int(PosXrobPres - LARGEUR_ROBOT/2*cos(((angle-90) * M_PI)/180))
                ,int(PosYrob - LARGEUR_ROBOT/2*sin(((angle-90) * M_PI)/180)), int(PosXrob - LARGEUR_ROBOT/2*cos(((angle-90) * M_PI)/180)),redline);
                // arc depart
                for(int i=0;i<6;i++)
                path1.moveTo((PosYrobPres+(diag/2)*cos(((90-PosRotrobPres) * M_PI)/180)),PosXrobPres+(diag/2)*sin(((90-PosRotrobPres) * M_PI)/180));
                path1.arcTo(PosYrobPres-diag/2,PosXrobPres-diag/2,diag,diag,PosRotrobPres-90,360);
                item[3]=scene->addPath(path1,blacklinedot);
                path.moveTo((PosYrobPres+(diag/2)*cos(((90-PosRotrobPres) * M_PI)/180)),PosXrobPres+(diag/2)*sin(((90-PosRotrobPres) * M_PI)/180));
                ui->lcdPosArcFin->display(angle-PosRotrobPres);
                robotdep->setRotation(90-angle);
                if(angle-PosRotrobPres<-180)
                    anglefin=360+angle-PosRotrobPres;
                else if(angle-PosRotrobPres>180)
                    anglefin=angle-PosRotrobPres-360;
                else
                    anglefin=angle-PosRotrobPres;
                path.arcTo(PosYrobPres-diag/2,PosXrobPres-diag/2,diag,diag,PosRotrobPres-90,anglefin);
                item[4]=scene->addPath(path,redline);
            }

            if(table_ligne==index.row()) // si ligne actuel on trace les arc de cercle pour représenter les rotations
            {
                if (item[5]) scene->removeItem(item[5]);
                item[5] = 0;
                if (item[6]) scene->removeItem(item[6]);
                item[6] = 0;

                // arc d arrivee
                path1.moveTo((PosYrob+(diag/2)*cos(((90-PosRotrobPres) * M_PI)/180)),PosXrob+(diag/2)*sin(((90-PosRotrobPres) * M_PI)/180));
                path1.arcTo(PosYrob-diag/2,PosXrob-diag/2,diag,diag,PosRotrobPres-90,360);
                item[5]=scene->addPath(path1,blacklinedot);
                path.moveTo((PosYrob+(diag/2)*cos(((PosRotrobPres-angle) * M_PI)/180)),PosXrob+(diag/2)*sin(((PosRotrobPres-angle) * M_PI)/180));
                path.arcTo(PosYrob-diag/2,PosXrob-diag/2,diag,diag,anglefin,PosRotrob-anglefin-90);
                item[6]=scene->addPath(path,redline);

            }
            break;
        }// sortie du switch action et deplacement

        //placement du theta entre 0 et 360°
        if(PosRotrob<0)
        {
            PosRotrob=PosRotrob+360;
        }
        else if(PosRotrob>360)
        {
             PosRotrob=PosRotrob-360;
        }
        if((indexComboBox==2)||(indexComboBox==6))
        {
                robotdep->setPos(PosYrob,PosXrob);
                robotdep->setRotation(90-PosRotrob);
        }
        robot1->setPos(PosYrob,PosXrob);
        robot1->setRotation(90-PosRotrob);

        // Affichage de la position en X,Y,T

        table_ligne=futur_i;
/*        for(int i = 0; i < 30; ++i)
        {
          if(coordonnees[i][5] > 10) coordonnees[i][4] = PosRotrob - 90 ;
        }*/
        Mise_a_jour_coord_gateau();
        afficher_Gateau();

    }
     Xoffset=-100*cos(((PosRotrob) * M_PI)/180)-0*sin(((PosRotrob) * M_PI)/180);
    Yoffset=-100*sin(((PosRotrob) * M_PI)/180)+0*cos(((PosRotrob) * M_PI)/180);

    Cerise[40]->setPos(PosYrob+Yoffset-cerise_size/2,PosXrob+Xoffset-cerise_size/2);
    qDebug("Cerise suit robot");
//------------------------------------------------------------------------------------------------

    Affichage_Debug_Coord_Gateau();


    ui->lcdPosX->display(PosXrob);
    ui->lcdPosY->display(PosYrob);
    ui->lcdPosT->display(PosRotrob);

}

/**********************************************************************************
 *             Fonction resetComboBox() : Supprime les combobox                   *
 *********************************************************************************/
void MainWindow::resetComboBox()
{
    ui->tableView->setItemDelegateForColumn(2,nullptr);
    ui->tableView->setItemDelegateForColumn(3,nullptr);
    ui->tableView->setItemDelegateForColumn(4,nullptr);
    ui->tableView->setItemDelegateForColumn(5,nullptr);
    ui->tableView->setItemDelegateForColumn(6,nullptr);
    ui->tableView->setItemDelegateForColumn(7,nullptr);
}

/**********************************************************************************
 *    Fonction setFocusOnButton() : Sélectionne le 1er bouton de la ligne créer   *
 *********************************************************************************/
void MainWindow::setFocusOnButton()
{
    QModelIndex index = ui->tableView->currentIndex();
    int row = index.row() + 1;
    int column = 0;
    QModelIndex newIndex  = ui->tableView->model()->index(row,column);
    ui->tableView->selectionModel()->select(newIndex, QItemSelectionModel::Select);
    ui->tableView->setCurrentIndex(newIndex);
    ui->tableView->setFocus();
    ui->tableView->edit(newIndex);
}

void MainWindow::on_CreateRowButton_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();  // Recupere l'index
    QString data;                                       // creer un string
    int row = index.row();                              // recupere la ligne
    int nbre_row=ui->tableView->model()->rowCount();
    if(nbre_row==row+1)
    {
        ui->tableView->model()->insertRow(nbre_row);
        data = ui->tableView->model()->data(index).toString();
        ui->tableView->model()->setData(ui->tableView->model()->index(row+1,0),ui->tableView->model()->data(ui->tableView->model()->index(row,0)).toInt()+1);
        //ui->tableView->model()->setData(ui->tableView->model()->index(row+1,1),dataCol1.at((dataCol1.indexOf(data))%6+1));
        ui->tableView->model()->setData(ui->tableView->model()->index(row+1,10),ui->tableView->model()->data(ui->tableView->model()->index(row,10)).toInt()+1);
        ui->tableView->model()->setData(ui->tableView->model()->index(row+1,11),ui->tableView->model()->data(ui->tableView->model()->index(row,11)).toInt()+1);
        setFocusOnButton();
    }
}

void MainWindow::on_InsertRowButton_clicked()
{
    int index;
    QModelIndexList l = ui->tableView->selectionModel()->selection().indexes();
    if (l.isEmpty())
    {
        index = 0;
    }
    else
    {
        index = l[0].row();
    }

    ui->tableView->model()->insertRow(index);
    setFocusOnButton();
    ui->tableView->model()->setData(ui->tableView->model()->index(index,0),ui->tableView->model()->data(ui->tableView->model()->index(index-1,0)).toInt()+1);
//    ui->tableView->model()->setData(ui->tableView->model()->index(index,1),dataCol1.at(1));
//    ui->tableView->model()->setData(ui->tableView->model()->index(index,2),0);
//    ui->tableView->model()->setData(ui->tableView->model()->index(index,6),"Non");
//    ui->tableView->model()->setData(ui->tableView->model()->index(index,7),"Attendre");
    ui->tableView->model()->setData(ui->tableView->model()->index(index,10),ui->tableView->model()->data(ui->tableView->model()->index(index-1,10)).toInt()+1);
    ui->tableView->model()->setData(ui->tableView->model()->index(index,11),ui->tableView->model()->data(ui->tableView->model()->index(index-1,11)).toInt()+1);
    for(int i = index+1; i < ui->tableView->model()->rowCount(); i++)
    {
        ui->tableView->model()->setData(ui->tableView->model()->index(i,0),ui->tableView->model()->data(ui->tableView->model()->index(i,0)).toInt()+1);
        ui->tableView->model()->setData(ui->tableView->model()->index(i,10),ui->tableView->model()->data(ui->tableView->model()->index(i,10)).toInt()+1);
        ui->tableView->model()->setData(ui->tableView->model()->index(i,11),ui->tableView->model()->data(ui->tableView->model()->index(i,11)).toInt()+1);

    }

}

void MainWindow::on_DeleteRowButton_clicked()
{
    int index;
    QModelIndexList l = ui->tableView->selectionModel()->selection().indexes();
    if (l.isEmpty()) {
        index = 0;
    } else {
        index = l[0].row();
    }
    ui->tableView->model()->removeRow(index);
    for(int i = index; i < ui->tableView->model()->rowCount(); i++)
    {
        ui->tableView->model()->setData(ui->tableView->model()->index(i,0),ui->tableView->model()->data(ui->tableView->model()->index(i,0)).toInt()-1);
        ui->tableView->model()->setData(ui->tableView->model()->index(i,10),ui->tableView->model()->data(ui->tableView->model()->index(i,10)).toInt()-1);
        ui->tableView->model()->setData(ui->tableView->model()->index(i,11),ui->tableView->model()->data(ui->tableView->model()->index(i,11)).toInt()-1);

    }
}

void MainWindow::on_ExportFileButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as",QString(),"texte (*.txt)");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    QTextStream textStream(&file);
    QModelIndex j;
    QModelIndex testindex;
    QString data,data2;
    int indexComboBox,indexComboBoxAction,ligne;
    double angle = 0;
    double newValue;

    for(int i = 0; i < ui->tableView->model()->rowCount(); i++)
    {
        j = ui->tableView->model()->index(i,1);
        data = ui->tableView->model()->data(j).toString();
        indexComboBox = dataCol1.indexOf(data);

        testindex = ui->tableView->model()->index(i,0);
        ligne = ui->tableView->model()->data(testindex).toInt();
        textStream << ligne << ",";

        switch(indexComboBox)
        {
        case 0: //Début match
            textStream << "P"
                       << ",";
            testindex = ui->tableView->model()->index(i,2);
            if(ui->tableView->model()->data(testindex).toString() == "Bleu")
                textStream << "B" << "," ; //Si c'est l'équipe bleu, on affiche B,sinon on affiche V pour équipe verte
            else
                textStream << "V" << "," ;
            testindex = ui->tableView->model()->index(i,4); // x
            textStream << ui->tableView->model()->data(testindex).toString()
                       << ",";
            testindex = ui->tableView->model()->index(i,5); // y
            textStream << ui->tableView->model()->data(testindex).toString()
                       << ",";
            testindex = ui->tableView->model()->index(i,3); // angle
            angle = ui->tableView->model()->data(testindex).toDouble()*10;
            textStream << angle
                       << ","
                       << "0"
                       << ","
                       << "0";
            break;

        case 1: //Ligne Droite
            textStream << "L"
                       << ",";
            testindex = ui->tableView->model()->index(i,2);
            if (ui->tableView->model()->data(testindex).toInt() > 0) textStream << "F";
            else textStream << "B";
            textStream << ","
                       << abs(ui->tableView->model()->data(testindex).toInt())
                       << ","
                       << "0"
                       << ","
                       << "0";
            break;

        case 2: //Rotation
            testindex = ui->tableView->model()->index(i,2);
            angle = (ui->tableView->model()->data(testindex).toDouble()) * 10;
            textStream << "T"
                       << ",";
            testindex = ui->tableView->model()->index(i,3);
            if(ui->tableView->model()->data(testindex).toString() == "Absolu")
                textStream << "A" << "," << "0" << "," << "0" << "," << angle;
            else
                textStream << "R" << "," << "0" << "," << "0" << "," << angle;
            break;

        case 3: //Courbe
            textStream << "C"
                       << ",";
            testindex = ui->tableView->model()->index(i,4);//test s'il est ç la column 4
            if (ui->tableView->model()->data(testindex).toString() == "Gauche") textStream << "L"
                                                                                           << ",";
            else textStream << "R"
                            << ",";

            testindex = ui->tableView->model()->index(i,2);
            textStream << ui->tableView->model()->data(testindex).toString()
                       << ","
                       << "0"
                       << ",";
            testindex = ui->tableView->model()->index(i,3);
            textStream << ui->tableView->model()->data(testindex).toDouble()*10;
            break;

        case 4: //Action
                {
                    textStream << "A"
                               << ","
                               << "0"
                               << ",";
                    testindex = ui->tableView->model()->index(i,2);
                    data2 = ui->tableView->model()->data(testindex).toString();
                    indexComboBoxAction = dataAction.indexOf(data2);

                    qDebug() << " Index Action : " << indexComboBoxAction;

                    QString indexToAction[29] = {/*"150","171","172","173", "174", "175","176","177","178","154","155","156","180","181","182",*/
                                                 "0","1","2","3","4","5","9","10","11","20","30","40","41","42","50","60"}; //case en tableau

                    textStream << indexToAction[indexComboBoxAction];

                    textStream << ",";
                                  testindex = ui->tableView->model()->index(i,3);
                                  if (ui->tableView->model()->data(testindex).toString() == "Oui") textStream << "Y";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Non") textStream << "N";
                                  //else if (ui->tableView->model()->data(testindex).toString() == "Avant") textStream << "F";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Avant") textStream << "70";
                                  //else if (ui->tableView->model()->data(testindex).toString() == "Arrière") textStream << "B";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Arrière") textStream << "66";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Etage 0") textStream << "0";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Etage 1") textStream << "1";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Etage 2") textStream << "2";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Etage 3") textStream << "3";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Etage 4") textStream << "4";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Etage 5") textStream << "5";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Etage 6") textStream << "6";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Etage 7") textStream << "7";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Etage 8") textStream << "8";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Gauche") textStream << "0";//L
                                  else if (ui->tableView->model()->data(testindex).toString() == "Droite") textStream << "1";//R
                                  else if (ui->tableView->model()->data(testindex).toString() == "ON") textStream << "1";
                                  else if (ui->tableView->model()->data(testindex).toString() == "OFF") textStream << "0";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Barillet_1") textStream << "1";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Barillet_2") textStream << "2";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Barillet_3") textStream << "3";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Barillet_4") textStream << "4";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Barillet_5") textStream << "5";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Cerise_1") textStream << "1";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Cerise_2") textStream << "2";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Cerise_3") textStream << "3";
                                  else if (ui->tableView->model()->data(testindex).toString() == "Cerise_4") textStream << "4";
                                  else textStream << "0";
                                  //textStream << ui->tableView->model()->data(testindex).toString() //ajouter du texte
                                  textStream << ","
                                             << "0";


                    break;
                }
        case 5: //Recalage
        {
            textStream << "R"
                       << ",";
            testindex = ui->tableView->model()->index(i,2);
            if (ui->tableView->model()->data(testindex).toString() == "Avant" ) textStream << "F"
                                                                                           << ",";
            else textStream << "B"
                            << ",";
            testindex=ui->tableView->model()->index(i,4);
            QString coordonne = ui->tableView->model()->data(testindex).toString();
            testindex = ui->tableView->model()->index(i,5);
            if(ui->tableView->model()->data(testindex).toString() == "Rob-") newValue = 115;
            if(ui->tableView->model()->data(testindex).toString() == "Rob+") newValue = 1885;
            if(ui->tableView->model()->data(testindex).toString() == "Gob-") newValue = 168;
            if(ui->tableView->model()->data(testindex).toString() == "Gob+") newValue = 1832;

            testindex = ui->tableView->model()->index(i,4);
            if (ui->tableView->model()->data(testindex).toString() == "Abscisse")
            {
                textStream << newValue
                           << ","
                           << "0"
                           << ","
                           << "0"
                           << ","
                           << "X"
                           << ",";
            }
            else { textStream << newValue
                              << ","
                              << "0"
                              << ","
                              << "0"
                              << ","
                              << "Y"
                              << ",";
            }
            testindex = ui->tableView->model()->index(i,3);
            if (ui->tableView->model()->data(testindex).toString() == "Mécanique") textStream << "M"
                                                                                              << ","
                                                                                              << "0"
                                                                                              << ",";
            else textStream << "C"
                            << ","
                            << "0"
                            << ",";
            break;
        }

        case 6: //XYT
            textStream << "X"
                       << ",";
            testindex = ui->tableView->model()->index(i,2);
            if (ui->tableView->model()->data(testindex).toString() == "Avant" ) textStream << "F"
                                                                                           << ",";
            else textStream << "B"
                            << ",";
            testindex = ui->tableView->model()->index(i,3);
            textStream << ui->tableView->model()->data(testindex).toString()
                       << ",";
            testindex = ui->tableView->model()->index(i,4);
            textStream << ui->tableView->model()->data(testindex).toString()
                       << ",";
            testindex = ui->tableView->model()->index(i,5);
            textStream << ui->tableView->model()->data(testindex).toString()
                       << "0";
            break;

        }

        if(indexComboBox != 5)
        {
            textStream << ",";
            testindex = ui->tableView->model()->index(i,6);
            if (ui->tableView->model()->data(testindex).toString() == "Précis")
            {
                textStream << "P"
                           << ",";

            }
            else textStream << "0"
                            << ",";

            testindex = ui->tableView->model()->index(i,7);
            if (ui->tableView->model()->data(testindex).toString() == "Attendre")
            {
                textStream << "W"
                           << ","
                           << "T"
                           << ",";
            }
            else if (ui->tableView->model()->data(testindex).toString() == "Saut(Position)")
            {
                textStream << "J"
                           << ","
                           << "0"
                           << ",";
            }
            else if (ui->tableView->model()->data(testindex).toString() == "Saut(Délais)")
            {
                textStream << "E"
                           << ","
                           << "P"
                           << ",";
            }
            else if (ui->tableView->model()->data(testindex).toString() == "Enchainement")
            {
                textStream << "E"
                           << ","
                           << "T"
                           << ",";
            }
            else if (ui->tableView->model()->data(testindex).toString() == "Clothoïde")
            {
                textStream << "C"
                           << ","
                           << "0"
                           << ",";
            }
        }

        textStream << "0" //espace après le P,W,T
                   << ","
                   << "0"
                   << ",";
        testindex = ui->tableView->model()->index(i,10);
        textStream << ui->tableView->model()->data(testindex).toString()
                   << ",";
        testindex = ui->tableView->model()->index(i,11);
        textStream << ui->tableView->model()->data(testindex).toString()
                   << ","
                   << "0"
                   << ","
                   << "0"
                   << ","
                   << "0";

        textStream << "\n";
    }
    file.close();
}

void MainWindow::on_ImportFileButton_clicked()
{
    QModelIndexList l;
    int index;
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file",QString(),"texte (*.txt)");
    QFile file(fileName);
    QString text;
    QStringList liste;
    int i = 0, confirm = 0;

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    do{
        l = ui->tableView->selectionModel()->selection().indexes();
        if (l.isEmpty()) {
            index = 0;
        }else if(confirm==0){
            index = l[0].row();
            i = index;
            confirm  = 1;
        }else{
            index = l[0].row();
        }
        ui->tableView->model()->insertRow(index);
        text = in.readLine();
        if(!text.isNull())
        {
            liste = text.split(",");
            //i = liste[0].toInt();
            qDebug() << liste;
            ui->tableView->model()->setData(ui->tableView->model()->index(index,0),liste[0].toInt()+i);
            ui->tableView->model()->setData(ui->tableView->model()->index(index,10),liste[11].toInt()+i);
            ui->tableView->model()->setData(ui->tableView->model()->index(index,11),liste[12].toInt()+i);

            if(liste[1] == "P") //Debut match
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(index,1),dataCol1[0]);
                if(liste[2] == "B")
                    ui->tableView->model()->setData(ui->tableView->model()->index(index,2),"Bleu");
                else
                    ui->tableView->model()->setData(ui->tableView->model()->index(index,2),"Vert");
                ui->tableView->model()->setData(ui->tableView->model()->index(index,3),liste[5].toDouble()/10);
                ui->tableView->model()->setData(ui->tableView->model()->index(index,4),liste[3]);
                ui->tableView->model()->setData(ui->tableView->model()->index(index,5),liste[4]);
            }
            else if(liste[1] == "L") //Ligne Droite
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(index,1),dataCol1[1]);
                if(liste[2] =="F")
                    ui->tableView->model()->setData(ui->tableView->model()->index(index,2),liste[3]);
                else
                    ui->tableView->model()->setData(ui->tableView->model()->index(index,2),(-(liste[3].toInt())));


            }
            else if(liste[1] == "T") //Rotation
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(index,1),dataCol1[2]);
                ui->tableView->model()->setData(ui->tableView->model()->index(index,2),liste[5].toDouble() / 10);
                if(liste[2] == "A") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Absolu");
                else ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Relatif");
            }
            else if(liste[1] == "C") //Courbe
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(index,1),dataCol1[3]);
                if(liste[2] == "R") ui->tableView->model()->setData(ui->tableView->model()->index(index,4),"Droite");
                else ui->tableView->model()->setData(ui->tableView->model()->index(index,4),"Gauche");
                ui->tableView->model()->setData(ui->tableView->model()->index(index,2),liste[3]);
                ui->tableView->model()->setData(ui->tableView->model()->index(index,3),liste[5].toDouble() / 10);
            }
            else if(liste[1] == "A") //Action
                        {
                            ui->tableView->model()->setData(ui->tableView->model()->index(index,1),dataCol1[4]);
                            ui->tableView->model()->setData(ui->tableView->model()->index(index,2),dataAction[getAction(liste[3].toInt())]);
                            ui->tableView->model()->setData(ui->tableView->model()->index(index,3),liste[4]);
                            if(liste[4] == "0") ui->tableView->model()->setData(ui->tableView->model()->index(index,3)," ");
                            else if(liste[3] == "0")
                            {
                            if(liste[4] == "Y") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Oui");
                            else if (liste[4] == "N") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Non");
                            }
                            //if(liste[4] == "F") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Avant");
                            if(liste[4] == "70") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Avant");
                            //else if (liste[4] == "B") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Arrière");
                            else if (liste[4] == "66") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Arrière");
                            else if(liste[3] == "20")// si liste colonne 3 = etage gateau
                            {
                                if(liste[4] == "0") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 0");
                                else if (liste[4] == "1") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 1");
                                else if (liste[4] == "2") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 2");
                                else if (liste[4] == "3") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 3");
                                else if (liste[4] == "4") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 4");
                                else if (liste[4] == "5") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 5");
                                else if (liste[4] == "6") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 6");
                                else if (liste[4] == "7") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 7");
                                else if (liste[4] == "8") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 8");
                            }
                            else if((liste[3] == "40")||(liste[3] == "41"))// si colonne 3 = attrape cérise ou range attrape cérise
                            {
                                if(liste[4] == "0") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Gauche");
                                else if (liste[4] == "1") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Droite");

                            }
                            else if(liste[3] == "50") // si colonne 3 = deguisement
                            {
                                if(liste[4] == "0") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"OFF");
                                else if (liste[4] == "1") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"ON");
                            }
                            else if(liste[3] == "60")// si liste colonne 3 = Barillet
                            {
                                if (liste[4] == "1") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Barillet_1");
                                else if (liste[4] == "2") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Barillet_2");
                                else if (liste[4] == "3") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Barillet_3");
                                else if (liste[4] == "4") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Barillet_4");
                                else if (liste[4] == "5") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Barillet_5");
                            }
                            else if(liste[3] == "30")// si liste colonne 3 = Barillet
                            {
                                if (liste[4] == "1") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Cerise_1");
                                else if (liste[4] == "2") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Cerise_2");
                                else if (liste[4] == "3") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Cerise_3");
                                else if (liste[4] == "4") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Cerise_4");
                                else if (liste[4] == "5") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Cerise_5");
                            }

                        }
            else if(liste[1] == "R") //Recalage
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(index,1),dataCol1[5]);
                if(liste[2] == "F") ui->tableView->model()->setData(ui->tableView->model()->index(index,2),"Avant");
                else ui->tableView->model()->setData(ui->tableView->model()->index(index,2),"Arrière");
                if(liste[6] == "Y") ui->tableView->model()->setData(ui->tableView->model()->index(index,4),"Ordonnée");
                else ui->tableView->model()->setData(ui->tableView->model()->index(index,4),"Abscisse");
                if(liste[7] == "M") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Mécanique");
                else ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Arrière");
                if(liste[3] == "115") ui->tableView->model()->setData(ui->tableView->model()->index(index,5),"Rob-");
                else if(liste[3] == "1885") ui->tableView->model()->setData(ui->tableView->model()->index(index,5),"Rob+");
                else if(liste[3] == "168") ui->tableView->model()->setData(ui->tableView->model()->index(index,5),"Gob-");
                else if(liste[3] == "1832") ui->tableView->model()->setData(ui->tableView->model()->index(index,5),"Gob+");
            }
            else if(liste[1] == "X") //XYT
            {
                ui->tableView->model()->setData(ui->tableView->model()->index(index,1),dataCol1[6]);
                if(liste[2] == "F") ui->tableView->model()->setData(ui->tableView->model()->index(index,2),"Avant");
                else ui->tableView->model()->setData(ui->tableView->model()->index(index,2),"Arrière");
                ui->tableView->model()->setData(ui->tableView->model()->index(index,3),liste[3]);
                ui->tableView->model()->setData(ui->tableView->model()->index(index,4),liste[4]);
                ui->tableView->model()->setData(ui->tableView->model()->index(index,5),liste[5].toDouble() / 10);
            }

            if(liste[6] == "P")
                ui->tableView->model()->setData(ui->tableView->model()->index(index,6),"Précis");
            else
                ui->tableView->model()->setData(ui->tableView->model()->index(index,6),"Non");

            if(liste[7] == "W" && liste[8] == "T") ui->tableView->model()->setData(ui->tableView->model()->index(index,7),"Attendre");
            else if (liste[7] == "J" && liste[8] == "0") ui->tableView->model()->setData(ui->tableView->model()->index(index,7),"Saut(Position)");
            else if (liste[7] == "E" && liste[8] == "P") ui->tableView->model()->setData(ui->tableView->model()->index(index,7),"Saut(Délais)");
            else if (liste[7] == "E" && liste[8] == "T") ui->tableView->model()->setData(ui->tableView->model()->index(index,7),"Enchainement");
            else if (liste[7] == "M" && liste[8] == "0") ui->tableView->model()->setData(ui->tableView->model()->index(index,7),"Enchainement");
            else if (liste[7] == "C" && liste[8] == "0") ui->tableView->model()->setData(ui->tableView->model()->index(index,7),"Clothoïde");
        }

    }
    while(!text.isNull());
    //Suprimmer la derniere ligne
    ui->tableView->model()->removeRow(index);
   if(dataCol1.indexOf(ui->tableView->model()->data(ui->tableView->model()->index(index,1)).toString())==0)
        ui->tableView->model()->removeRow(index);

    // recalculer les sauts des index d'aprés
    file.close();

    //actualisation des indices après l'import
    QModelIndex testindex;
    int pos, delai;
    int indexdebut = (liste[0].toInt()+i)+1;
    int y = (liste[11].toInt()+i)+1;

    for(int ligne = index; ligne < ui->tableView->model()->rowCount(); ligne++)
    {
        testindex = ui->tableView->model()->index(ligne,10);
        pos = ui->tableView->model()->data(testindex).toInt();
        testindex = ui->tableView->model()->index(ligne,11);
        delai = (ui->tableView->model()->data(testindex).toInt())-pos;
        ui->tableView->model()->setData(ui->tableView->model()->index(ligne,0),indexdebut);
        ui->tableView->model()->setData(ui->tableView->model()->index(ligne,10),y);
        ui->tableView->model()->setData(ui->tableView->model()->index(ligne,11),y+delai);
        y++;
        indexdebut++;
    }
    updateVisu(testindex);
    //connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tableViewIsClicked(const QModelIndex &)));
}

void MainWindow::relacherGateau(unsigned int avant_arr)
{
    // Iterate through the ptrGateau array in reverse order

        for (int i = 0; i<NOMBRECOUCHE; i++)
        {
            if (((coordGateau[i][3]/10)==1)&&(avant_arr==0))
            {
                // on relache a l'avant
                coordGateau[i][3] = coordGateau[i][3]-10;
                qDebug("relache avant %d",i);
            }
            if (((coordGateau[i][3]/10)==2)&&(avant_arr==1))
            {
                // on relache a l'arriere
                coordGateau[i][3] = coordGateau[i][3]-20;
                qDebug("relache avant %d",i);
            }
        }



}

void MainWindow::detecterCollisionGateau(unsigned int avant_arr) {


    // Create new detection zone rectangles at the robot's current position
    unsigned int size_rectangle=50;
    QRectF frontZoneRect = robot1->mapToScene(robot1->boundingRect().center().x() +100-size_rectangle/2 , robot1->boundingRect().center().y() -size_rectangle/2, size_rectangle, size_rectangle).boundingRect();
    QRectF backZoneRect = robot1->mapToScene(robot1->boundingRect().center().x()  -100-size_rectangle/2 , robot1->boundingRect().center().y() -size_rectangle/2, size_rectangle, size_rectangle).boundingRect();


    // Create QGraphicsRectItems for the front and back zones
    frontZone = new QGraphicsRectItem(frontZoneRect);
    backZone = new QGraphicsRectItem(backZoneRect);

    // Set the pen and brush for the front and back zones
    QPen zonePen(Qt::green, 2);
    QBrush zoneBrush(Qt::transparent);
    frontZone->setPen(zonePen);
    frontZone->setBrush(zoneBrush);
    backZone->setPen(zonePen);
    backZone->setBrush(zoneBrush);

    // Add the front and back zones to the scene

    // Create a flag variable to keep track of whether the robot has picked up a cake
    bool pickedUpCake = false;
    if(avant_arr==0)
    {
        qDebug("test si colision avant niveau ascenseur %d",Etage_ascenseur);
        ui->graphicsView->scene()->addItem(frontZone);
        for (int i = 0; i < NOMBRECOUCHE; i++)
        {
            if (((coordGateau[i][3]/10)==0)&&((coordGateau[i][3])>=Etage_ascenseur))
            {
                if (ptrGateau[i]->collidesWithItem(frontZone))
                {
                    // Update the value of coordonneesBase[i][3] to 1
                    coordGateau[i][3] = 10+coordGateau[i][3];
                    qDebug("Collision avec échantillon en face %d",i);
                    // If this is the first cake that the robot is picking up, set coordonneesBase[i][5] to 1
                    /*if (!pickedUpCake) {
                        coordGateau[i][5] = 1;
                        pickedUpCake = true;

                    }*/
                    //break;
                }

            }
        }
        ui->graphicsView->scene()->removeItem(frontZone);
    }
    else
    {
        qDebug("test si colision avant");
        ui->graphicsView->scene()->addItem(backZone);
        for (int i = 0; i < NOMBRECOUCHE; i++)
        {
            if ((coordGateau[i][3]/10)==0)
            {
                if (ptrGateau[i]->collidesWithItem(backZone))
                {
                    // Update the value of coordonneesBase[i][3] to 1
                    coordGateau[i][3] = 20+coordGateau[i][3];
                    qDebug("Collision avec échantillon en face %d",i);
                    // If this is the first cake that the robot is picking up, set coordonneesBase[i][5] to 1
                    /*if (!pickedUpCake) {
                        coordGateau[i][5] = 1;
                        pickedUpCake = true;

                    }*/
                    //break;
                }

            }
        }
        ui->graphicsView->scene()->removeItem(backZone);
    }

    // Remove previously added detection zone rectangles from the scene
    /*QList<QGraphicsItem*> itemsToRemove = ui->graphicsView->scene()->items();
    for(QGraphicsItem* item : itemsToRemove) {
        if (item == frontZone || item == backZone) {
            ui->graphicsView->scene()->removeItem(item);
            delete item;
        }
    }*/
    /*ui->graphicsView->scene()->removeItem(frontZone);
    ui->graphicsView->scene()->removeItem(backZone);*/
}

void MainWindow::Mise_a_jour_coord_gateau()
{
    qreal xOffset_cake,yOffset_cake,dist_avant=100, dist_arr=-100,nombre_gateau;


    for(int i=0;i<NOMBRECOUCHE;i++)
    {
        if((coordGateau[i][3]/10)==1) // entre 10 et 19 pris a l'avant
        {
            xOffset_cake = dist_avant * cos((PosRotrob * M_PI)/180)-0*sin(((PosRotrob) * M_PI)/180);
            yOffset_cake = dist_avant * sin((PosRotrob * M_PI)/180)+0*cos(((PosRotrob) * M_PI)/180);
            coordGateau[i][0]=PosXrob + xOffset_cake;
            coordGateau[i][1]=PosYrob + yOffset_cake;
            qDebug("mise a jour %i %i %i",i,coordGateau[i][0],coordGateau[i][1]);
        }
        if((coordGateau[i][3]/10)==2) // entre 20 et 29 pris a l'arriere
        {
            xOffset_cake = dist_arr * cos((PosRotrob * M_PI)/180)-0*sin(((PosRotrob) * M_PI)/180);
            yOffset_cake = dist_arr * sin((PosRotrob * M_PI)/180)+0*cos(((PosRotrob) * M_PI)/180);
            coordGateau[i][0]=PosXrob + xOffset_cake;
            coordGateau[i][1]=PosYrob + yOffset_cake;
            qDebug("mise a jour %i",i);
        }
    }
}


void MainWindow::afficher_Gateau()
{
    for(int i=0;i<NOMBRECOUCHE;i++)
    {
        ptrGateau[i]->setPos(coordGateau[i][1], coordGateau[i][0]);
    }
}






int MainWindow::getAction(int chiffre)
{
    int toReturn = 0;
    switch(chiffre)
    {
        case 0: //Evitement
        toReturn =  0;
        break;
        case 1: //Set odo
        toReturn =  1;
        break;
        case 2: //wait
        toReturn =  2;
        break;
        case 3: //wait other bot
        toReturn =  3;
        break;
        case 4: //check pos adversaire
        toReturn =  4;
        break;
        case 5: //Fin de match
        toReturn =  5;
        break;
        case 9: //Ajouter point
        toReturn =  6;
        break;
        case 10: //Prise gateau
        toReturn =  7;
        break;
        case 11: //Relacher gateau
        toReturn =  8;
        break;
        case 20: //Monter gateau
        toReturn =  9;
        break;
        case 30: //pose cérise gateau
        toReturn =  10;
        break;
        case 40: //Attrape cérise
        toReturn =  11;
        break;
        case 41: //Range Attrape cérise
        toReturn =  12;
        break;
        case 42: //Tir cérise
        toReturn =  13;
        break;
        case 50: //déguisement
        toReturn =  14;
        break;
        case 60: //Barillet
        toReturn =  15;
        break;

    }
return toReturn;
}

void MainWindow::on_pushButton_clicked()
{
/*

        QPixmap robot(":/Images/Cake2023/ROB2023.png");

        switch(robot2Flag){
        case 0 :
            robot2Flag++;
            break;

        case 1:
            qDebug("TESSSSSSSSST0000000000000000");
            robot2 =  scene->addPixmap(robot);

            robot2->setOffset(GLOBALOFFSETX - robot2->boundingRect().center().x(),GLOBALOFFSETY - robot2->boundingRect().center().y());
            robot2->setTransformOriginPoint(robot2->boundingRect().center());

            robot2->setPos(ui->secondY->value(),ui->secondX->value());
            robot2->setRotation(90-ui->secondTheta->value());

            robot2Flag++;

            break;

        default://case 2:

            qDebug("TESSSSSSSSST");
            scene->removeItem(robot2);
            robot2Flag = 1;
            break;

        }
        */
}
void MainWindow::Affichage_Debug_Coord_Gateau()
{
    for (int i=0;i<NOMBRECOUCHE;i++)
    {
        qDebug() << "Cake position: "
                 << ptrGateau[i]->pos()
                 << "coordGateau[" << i << "]: "
                 << coordGateau[i][0] << ", "
                 << coordGateau[i][1] << ", "
                 << coordGateau[i][2] << ", "
                 << coordGateau[i][3] << ", "
                 << coordGateau[i][4];

    }
}
