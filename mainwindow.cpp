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

const QStringList MainWindow::dataCol1 = {"Debut Match",
                                          "Ligne Droite",
                                          "Rotation",
                                          "Courbe",
                                          "Action",
                                          "Recalage",
                                          "XYT",
                                          /*"toto"*/};

const QStringList MainWindow::dataEquipe = {"Vert",
                                            "Bleu"};


const QStringList MainWindow::dataAction = { //"Ascenceur" // action number 150

                                             //,"Prise_bas"
                                             //,"Passe"
                                             //,"Pose_bas"
                                             //, "Pose_Haut"
                                             //,"Prise_bordure"
                                             //,"Pose_bordure"
                                             //,"Prise_distrib"
                                             //,"Ranger"

                                             //,"Res_prestest"
                                             //,"Res_mes"
                                             //,"Res_rang"

                                             //,"Pre_prise"
                                             //,"Prise_statuette"
                                             //,"Pose_statuette"

                                             /*,*/"Evitement"
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
                                             ,"Deguisement"};

int carreFlag = 1 , robot2Flag = 0;
double distanceLigneDroite;


QPointF bras[17]{};

int coordonneesBase[30][6]{ // coordonnées des échantillons {x,y,COULEUR,prise avant ,prise arrière, ...}

    {230,575,ROSE,0,0,0}, // en haut à gauche
    {230,775,YELLOW,0,0,0},

    {230,2425,ROSE,0,0,0}, // en haut à droite
    {230,2225,YELLOW,0,0,0},

    {1775,575,ROSE,0,0,0}, // en bas à gauche
    {1775,775,YELLOW,0,0,0},

    {1775,2425,ROSE,0,0,0}, // eh bas à droite
    {1775,2225,YELLOW,0,0,0},

    {730,1125,BROWN,0,0,0}, //milieu haut gauche
    {730,1875,BROWN,0,0,0}, //milieu haut droite
    {1280,1125,BROWN,0,0,0}, //milieu bas gauche
    {1280,1875,BROWN,0,0,0}, //milieu bas droite

};
//position actuelle des elements du jeu à faire modifer à chaque action
int coordonnees[30][6]{


};

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

/*************************************************************************************************/

/*************************************************************************************************/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,ptrEchantillon{new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,new QGraphicsPixmapItem,
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
    resetPosEchantillon();
    initVisu();
    initModel();




    MyComboBoxDelegate *cbd = new MyComboBoxDelegate(ui->tableView, this);

//    connect(cbd, SIGNAL(updateHeaders(const QModelIndex &)), this, SLOT(updateHeader(const QModelIndex &)));

    ui->tableView->setItemDelegateForColumn(1, cbd);
    //ui->tableView->setItemDelegateForColumn(10, cbd);
    //ui->tableView->setItemDelegateForColumn(11, cbd);

    initHeaderData();

    connect(ui->tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tableViewIsClicked(const QModelIndex &))); //affichage dès le lancement de l'éditeur

    ui->tableView->model()->insertRow(ui->tableView->model()->rowCount());
    ui->tableView->model()->setData(ui->tableView->model()->index(0,0),0);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,1),dataCol1.at(0));
    ui->tableView->model()->setData(ui->tableView->model()->index(0,2),dataEquipe.at(0));
    ui->tableView->model()->setData(ui->tableView->model()->index(0,3),0);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,4),50);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,5),50);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,6)," ");
    ui->tableView->model()->setData(ui->tableView->model()->index(0,7)," ");

    ui->tableView->model()->setData(ui->tableView->model()->index(0,10),1);
    ui->tableView->model()->setData(ui->tableView->model()->index(0,11),1);
    QModelIndex newIndex  = ui->tableView->model()->index(0,0);
    ui->tableView->setCurrentIndex(newIndex);
    robot1->setPos(700,625);
    robot1->setRotation(0);
    robotdep->setPos(700,625);
    robotdep->setRotation(0);
    ui->lcdPosX->display(675);
    ui->lcdPosY->display(250);
    ui->lcdPosT->display(90);

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

//visu des couches de gateaux**************************************************************************
    for (int i=0; i<36; i++){
    QPixmap pix(determinerCouleur(i));
    pix = pix.scaled(pix.width() * 0.27, pix.height() * 0.27, Qt::KeepAspectRatio);
    ptrEchantillon[i] = scene->addPixmap(pix);
    ptrEchantillon[i]->setPos(coordonneesBase[i][1], coordonneesBase[i][0]);
    ptrEchantillon[i]->setOffset(-ptrEchantillon[i]->boundingRect().center().x(), //met le point ref du gateau à son centre
                                 -ptrEchantillon[i]->boundingRect().center().y());
    }

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
}
//********************************************************************************************************************
QPixmap MainWindow::determinerCouleur(int i){
    QPixmap pixReturn;

    //on vérifie d'abord si l'échantillon doit être face caché
    //if (coordonnees[i][3] == 0){
        switch(coordonneesBase[i][2]){
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
void MainWindow::relacherDernierEchantillon()
{
    // Iterate through the ptrEchantillon array in reverse order
    for (int i = 11; i >= 0; i--)
    {
            if (coordonneesBase[i][3] == 1) { // If the robot has picked up the sample with the front end
                ptrEchantillon[i]->setParentItem(0); // Remove the cake from the robot
                ptrEchantillon[i]->setPos(ptrEchantillon[i]->pos()); // Set the position of the cake to its current position
                coordonneesBase[i][3] = 2;
                qDebug() << "Cake released from front end";
                QPointF currentPos = ptrEchantillon[i]->pos();
                qDebug() << "Current position of cake: x=" << currentPos.x() << ", y=" << currentPos.y();

            }
            else if (coordonneesBase[i][4] == 1) { // If the robot has picked up the sample with the front end
                ptrEchantillon[i]->setParentItem(0); // Remove the cake from the robot
                ptrEchantillon[i]->setPos(ptrEchantillon[i]->pos()); // Set the position of the cake to its current position
                 coordonneesBase[i][4] = 2;
                qDebug() << "Cake released from back end";
                QPointF currentPos = ptrEchantillon[i]->pos();
                qDebug() << "Current position of cake: x=" << currentPos.x() << ", y=" << currentPos.y();

            }
    }
}

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
    double PosXrob = 0, PosYrob = 0, PosRotrob = 0, PosXrobPres = 90, PosYrobPres =0 , PosRotrobPres = 0,
            PosCXrob= 0, PosCYrob= 0, distanceLigneDroite, rayonCourbe, rayonCourbeVentouse, angleCourbe,angleRotation;
    int newValue;


    bool resDeploye[2] {false,false};


    resetPosEchantillon();


    check = ui->checkBox->isChecked();
    setWindowTitle("éditeur de stratégie 2023 - The cherry on the cake ");


    qDebug() << nbUpdateVisu << "______________________________________________________________________________________________________";

    int cpt_boucle=0;


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
        for(int i=0;i<7;i++)
            if (item[i]) scene->removeItem(item[i]);
//        for(int i=0;i<4;i++)
//            scene->removeItem(collisionLine[i]);
//        for(int i=0;i<6;i++){// a enlever????
//            scene->removeItem(ventouse[i]);// a enlever????
//        }
//        for(int i=0;i<2;i++){// a enlever????
//            scene->removeItem(brasDistrib[i]);// a enlever????
//        }


        switch(indexComboBox)
        {
        case 0: //Début match

            old_equipe = equipe;
            testindex = ui->tableView->model()->index(table_ligne,2);
            if(ui->tableView->model()->data(testindex).toString() == "Bleu")
            {
                testindex = ui->tableView->model()->index(table_ligne,4);
                PosXrob = (ui->tableView->model()->data(testindex).toDouble());
                testindex = ui->tableView->model()->index(table_ligne,5);
                PosYrob = (ui->tableView->model()->data(testindex).toDouble());
                testindex = ui->tableView->model()->index(table_ligne,3);
                PosRotrob = ((PosRotrobPres)+ui->tableView->model()->data(testindex).toDouble());
                PosRotrobPres=PosRotrob;
                PosXrobPres=PosXrob;
                PosYrobPres=PosYrob;
                equipe = 0;
            }

            else if(ui->tableView->model()->data(testindex).toString() == "Vert")
            {
                testindex = ui->tableView->model()->index(table_ligne,4);
                PosXrob = (ui->tableView->model()->data(testindex).toDouble());
                testindex = ui->tableView->model()->index(table_ligne,5);
                PosYrob = 3000 - (ui->tableView->model()->data(testindex).toDouble());
                testindex = ui->tableView->model()->index(table_ligne,3);
                PosRotrob = -((PosRotrobPres)+ui->tableView->model()->data(testindex).toDouble());
                PosRotrobPres=PosRotrob;
                PosXrobPres=PosXrob;
                PosYrobPres=PosYrob;
                equipe = 1;
            }

            break;

        case 1: //Ligne Droite
            // effacer les trassé d'avant
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

           //if(table_ligne==index.row())
           //{
           //    path1.moveTo((PosYrob+(diag/2)*cos(((90-PosRotrobPres) * M_PI)/180)),PosXrob+(diag/2)*sin(((90-PosRotrobPres) * M_PI)/180));
           //    path1.arcTo(PosYrob-diag/2,PosXrob-diag/2,diag,diag,PosRotrobPres-90,360);
           //    item[1]=scene->addPath(path1,blacklinedot);
           //    path.moveTo((PosYrob+(diag/2)*cos(((90-PosRotrobPres) * M_PI)/180)),PosXrob+(diag/2)*sin(((90-PosRotrobPres) * M_PI)/180));
           //    path.arcTo(PosYrob-diag/2,PosXrob-diag/2,diag,diag,90-PosRotrobPres,PosRotrob-90);
           //    item[0]=scene->addPath(path,redline);
           //}
            break;

        case 3: //Courbe
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
            if((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="")
            {
                if (item[0]) scene->removeItem(item[0]);
                item[0] = 0;
                if (item[1]) scene->removeItem(item[1]);
                item[1] = 0;
                if (item[2]) scene->removeItem(item[2]);
                item[2] = 0;
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,0),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,0)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,2),"Prise_bas");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),"");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,10),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,10)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,11),ui->tableView->model()->data(ui->tableView->model()->index(table_ligne-1,11)).toInt()+1);
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,6),"Non");
                ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,7),"Enchainement");
            }

            numero_action=(ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toUInt());
            numero_manche=numero_action;
            //Inversion des pompes et bras selectionnés si il y a un changement d'équipe
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="VENT_AT") ||
               ((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="VENT_RE") ||
               ((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="BRAS_AT") ||
               ((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="BRAS_RE")||
               ((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="BRAS_PREPA")||
               ((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="BRAS_POSE"))
            {
                if(equipe!=old_equipe)
                {
                    switch(numero_action)
                    {
                    case 0: numero_action = 2;break;
                    case 2: numero_action = 0;break;
                    case 3: numero_action = 5;break;
                    case 5: numero_action = 3;break;
                    case 10:numero_action = 21;break;
                    case 21:numero_action = 10;break;
                    case 43:numero_action = 54;break;
                    case 54:numero_action = 43;break;
                    }
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),numero_action);
                }
            }

//Prise_gateau_______________________________________________________________________________________________________________
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Prise_gateau")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Avant")){
                qDebug("Test prise avant");
                // Pick up the cake at the front of the robot
                 detecterCollisionEchantillon();
            }
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Prise_gateau")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Arriere")){

                qDebug("Test prise arriere");
                  // Pick up the cake at the back of the robot
                 detecterCollisionEchantillon();
            }
//Relacher gateau _________________________________________________________________________________________________________________________________
        if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Relacher_gateau")
            &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Avant")){
            qDebug("Test relacher avant");
            //qDebug() << "Cake picked up with front end";
            // Release the last layer of cake that has been collected from the front
            relacherDernierEchantillon();
        }
        if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Relacher_gateau")
            &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())=="Arriere")){

            qDebug("Test relacher arriere");
            // Release the last layer of cake that has been collected from the front
            relacherDernierEchantillon();
        }


//Prise_bas____________________________________________________________________________________________________________

            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Prise_bas")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!="")){

                int brasChoisi = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt());
                QString brasChoisiStr = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString());
                int tabBrasChoisi[3];

                if (brasChoisiStr == "120" || brasChoisiStr == "102" || brasChoisiStr == "012" || brasChoisiStr == "021" || brasChoisiStr == "201" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),210);
                if (brasChoisiStr == "354" || brasChoisiStr == "453" || brasChoisiStr == "435" || brasChoisiStr == "543" || brasChoisiStr == "534" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),345);


                //on determine la centaine, la dizaine et l'unité

                tabBrasChoisi[0] = brasChoisi/100;
                tabBrasChoisi[1] = (brasChoisi - 100*tabBrasChoisi[0])/10; // on enleve la centaine pour trouver la dizaine
                tabBrasChoisi[2] = (brasChoisi - 100*tabBrasChoisi[0]) - 10*tabBrasChoisi[1]; // on enleve la centaine et la dizaine pour trouver l'unité

                // on creer le rectangle qui va contenir le rond qui correspond à la ventouse
                QRect ellipseVentouse(0,0,54,54);
                int echantillonAttrape;


                //on verifie les collisions avec la ventouse qui correspond au bras selectionner par la centaine , la dizaine et l'unité
                // ex : 210 va activer les bras 1 ,2 et 0
                for(int i = 0; i<3;i++){
                   if(tabBrasChoisi[i] != 0 || i == 2 ){
                       ventouse[tabBrasChoisi[i]] = scene->addEllipse(ellipseVentouse);
                       ventouse[tabBrasChoisi[i]]->setPen(redline);
                       echantillonAttrape = collisionVentouse(tabBrasChoisi[i],PosRotrob);
                       if(echantillonAttrape != -1){
                           coordonnees[echantillonAttrape][5] = tabBrasChoisi[i] + 1;
                           bras[tabBrasChoisi[i] + 1] = ventouse[tabBrasChoisi[i] + 1]->pos();
                       }
                   }

                }
            }
//Passe__________________________________________________________________________________________________________
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Passe")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!="")){

                int brasChoisi = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt());
                QString brasChoisiStr = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString());
                int tabBrasChoisi[3];

                if (brasChoisiStr == "120" || brasChoisiStr == "102" || brasChoisiStr == "012" || brasChoisiStr == "021" || brasChoisiStr == "201" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),210);
                if (brasChoisiStr == "354" || brasChoisiStr == "453" || brasChoisiStr == "435" || brasChoisiStr == "543" || brasChoisiStr == "534" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),345);


                //on determine la centaine, la dizaine et l'unité

                tabBrasChoisi[0] = brasChoisi/100;
                tabBrasChoisi[1] = (brasChoisi - 100*tabBrasChoisi[0])/10; // on enleve la centaine pour trouver la dizaine
                tabBrasChoisi[2] = (brasChoisi - 100*tabBrasChoisi[0]) - 10*tabBrasChoisi[1]; // on enleve la centaine et la dizaine pour trouver l'unité

                for(int i = 0; i<3;i++){
                    if(tabBrasChoisi[i] != 0 || i == 2 ){
                        for(int J = 0;J < 30; J++){
                            if (coordonnees[J][5] == tabBrasChoisi[i] + 1){
                                coordonnees[J][5] += 10;
                                coordonnees[J][3] = 2;

                            }

                    }
                }


            }
          }

//Pose_bas__________________________________________________________________________________________________________
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Pose_bas")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!="")){

                int brasChoisi = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt());
                QString brasChoisiStr = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString());
                int tabBrasChoisi[3];


                if (brasChoisiStr == "120" || brasChoisiStr == "102" || brasChoisiStr == "012" || brasChoisiStr == "021" || brasChoisiStr == "201" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),210);
                if (brasChoisiStr == "354" || brasChoisiStr == "453" || brasChoisiStr == "435" || brasChoisiStr == "543" || brasChoisiStr == "534" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),345);

                //on determine la centaine, la dizaine et l'unité

                tabBrasChoisi[0] = brasChoisi/100;
                tabBrasChoisi[1] = (brasChoisi - 100*tabBrasChoisi[0])/10; // on enleve la centaine pour trouver la dizaine
                tabBrasChoisi[2] = (brasChoisi - 100*tabBrasChoisi[0]) - 10*tabBrasChoisi[1]; // on enleve la centaine et la dizaine pour trouver l'unité

                //on remet le bras choisi à 0 dans le tableau de coordonnées
                for(int i = 0; i<3;i++){
                   if(tabBrasChoisi[i] != 0 || i == 2 ){
                        for(int J = 0;J < 30; J++){
                            if (coordonnees[J][5] == tabBrasChoisi[i] + 1){

                                coordonnees[J][5] = 0;
                                coordonnees[J][0] += ptrEchantillon[i]->boundingRect().center().x() - GLOBALOFFSETX;
                                coordonnees[J][1] += ptrEchantillon[i]->boundingRect().center().y() - GLOBALOFFSETY;
                            }
                        }
                   }
                }

            }
//Pose_Haut__________________________________________________________________________________________________________________
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Pose_Haut")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!="")){

                int brasChoisi = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt());
                QString brasChoisiStr = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString());
                int tabBrasChoisi[3];


                if (brasChoisiStr == "120" || brasChoisiStr == "102" || brasChoisiStr == "012" || brasChoisiStr == "021" || brasChoisiStr == "201" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),210);
                if (brasChoisiStr == "354" || brasChoisiStr == "453" || brasChoisiStr == "435" || brasChoisiStr == "543" || brasChoisiStr == "534" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),345);

                //on determine la centaine, la dizaine et l'unité

                tabBrasChoisi[0] = brasChoisi/100;
                tabBrasChoisi[1] = (brasChoisi - 100*tabBrasChoisi[0])/10; // on enleve la centaine pour trouver la dizaine
                tabBrasChoisi[2] = (brasChoisi - 100*tabBrasChoisi[0]) - 10*tabBrasChoisi[1]; // on enleve la centaine et la dizaine pour trouver l'unité

                //on remet le bras choisi à 0 dans le tableau de coordonnées
                for(int i = 0; i<3;i++){
                   if(tabBrasChoisi[i] != 0 || i == 2 ){
                        for(int J = 0;J < 30; J++){
                            if (coordonnees[J][5] == tabBrasChoisi[i] + 11){

                                coordonnees[J][5] = 0;
                                coordonnees[J][3] = 1;
                                coordonnees[J][0] += ptrEchantillon[i]->boundingRect().center().x() - GLOBALOFFSETX;
                                coordonnees[J][1] += ptrEchantillon[i]->boundingRect().center().y() - GLOBALOFFSETY;
                            }
                        }
                   }
                }
            }
//Prise_bordure_____________________________________________________________________________________________________________
            if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Prise_bordure")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!="")){

                int brasChoisi = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt());
                QString brasChoisiStr = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString());
                int tabBrasChoisi[3];

                if (brasChoisiStr == "120" || brasChoisiStr == "102" || brasChoisiStr == "012" || brasChoisiStr == "021" || brasChoisiStr == "201" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),210);
                if (brasChoisiStr == "354" || brasChoisiStr == "453" || brasChoisiStr == "435" || brasChoisiStr == "543" || brasChoisiStr == "534" )
                    ui->tableView->model()->setData(ui->tableView->model()->index(table_ligne,3),345);


                //on determine la centaine, la dizaine et l'unité

                tabBrasChoisi[0] = brasChoisi/100;
                tabBrasChoisi[1] = (brasChoisi - 100*tabBrasChoisi[0])/10; // on enleve la centaine pour trouver la dizaine
                tabBrasChoisi[2] = (brasChoisi - 100*tabBrasChoisi[0]) - 10*tabBrasChoisi[1]; // on enleve la centaine et la dizaine pour trouver l'unité

                // on creer le rectangle qui va contenir le rond qui correspond à la ventouse
                QRect ellipseVentouse(0,0,54,54);
                int echantillonAttrape;



                //on verifie les collisions avec la ventouse qui correspond au bras selectionner par la centaine , la dizaine et l'unité
                // ex : 210 va activer les bras 1 ,2 et 0
                for(int i = 0; i<3;i++){
                   if(tabBrasChoisi[i] != 0 || i == 2 ){
                       ventouse[tabBrasChoisi[i]] = scene->addEllipse(ellipseVentouse);
                       ventouse[tabBrasChoisi[i]]->setPen(greenline);
                       echantillonAttrape = collisionVentouse(tabBrasChoisi[i],PosRotrob);
                       if(echantillonAttrape != -1){
                           coordonnees[echantillonAttrape][5] = tabBrasChoisi[i] + 1 + 10;
                           bras[tabBrasChoisi[i] + 1] = ventouse[tabBrasChoisi[i] + 1]->pos();
                       }
                   }


                }
            }


//Prise_distrib_______________________________________________________________________________________________________________________
            else if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Prise_distrib")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!="")){
                QLine ligneDistrib(0,0,232,0);

                //on fait apparaitre le bon bras sur la scene
                int brasChoisi = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt());

                brasDistrib[brasChoisi] = scene->addLine(ligneDistrib);

                //on le place au bon endroit
                brasDistrib[brasChoisi]->setPen(redline);
                brasDistrib[brasChoisi]->setPos(robot1->pos() + robot1->boundingRect().center());
                brasDistrib[brasChoisi]->moveBy(GLOBALOFFSETX,-10);
                brasDistrib[brasChoisi]->setRotation(90 - PosRotrob + 180*brasChoisi); //on fait un tour complet si on choisi le bras 1

                //on vérifie la collision avec l'échantillon le plus plus éloigné de chaque distributeur (pour être sur d'être a la bonne distance)

                if(brasDistrib[brasChoisi]->collidesWithItem(ptrEchantillon[18])){ //jaune bas
                    coordonnees[20][5] = 12;
                    coordonnees[19][5] = 13;
                    coordonnees[18][5] = 11;
                }
                if(brasDistrib[brasChoisi]->collidesWithItem(ptrEchantillon[21])){ //jaune bas
                    coordonnees[23][5] = 12;
                    coordonnees[22][5] = 13;
                    coordonnees[21][5] = 11;
                }
                if(brasDistrib[brasChoisi]->collidesWithItem(ptrEchantillon[24])){ //jaune bas
                    coordonnees[26][5] = 12;
                    coordonnees[25][5] = 13;
                    coordonnees[24][5] = 11;
                }
                if(brasDistrib[brasChoisi]->collidesWithItem(ptrEchantillon[27])){ //jaune bas
                    coordonnees[29][5] = 12;
                    coordonnees[28][5] = 13;
                    coordonnees[27][5] = 11;
                }


            }
//Deploiement________________________________________________________________________________________________________________
            else if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Deploiement")//verifie si la 2ème colonne corespond au
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!="")){           //"deploiement" et que le 3ème n'est pas un string vide
                int ouvertFerme = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt()); // si la condition est vérifié, il prends la valeur
                                                                                                                        // de la 3ème colonne et le convertit en int et le
                chasseNeigeFlag[0] = ouvertFerme;                                                                       // stock dans la var ouvertFerme


            }


            else if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Res_prestest")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!=""))
            {
                resDeploye[(ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt())] = true;
            }
            else if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Res_mes")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!=""))
            {
                int carreMesure = (ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt());
                if (brasMesure[0]->collidesWithItem(ptrCarre[carreMesure])){

                    ptrCarre[carreMesure]->hide();
                }
                else if (brasMesure[1]->collidesWithItem(ptrCarre[carreMesure])){

                    ptrCarre[carreMesure]->hide();
                }
            }
            else if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Res_rang")
                &&((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toString())!=""))
            {
                resDeploye[(ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,3)).toInt())] = false;
            }

//**********************************| année 2023| *********************************************************************
//            qDebug() << ui->tableView->model()->data(ui->tableView->model()->index(table_ligne, 2)).toString();

//                if(((ui->tableView->model()->data(ui->tableView->model()->index(table_ligne,2)).toString())=="Relacher_gateau")){
//                    // Find the last cake that was picked up by the robot
//                    QGraphicsPixmapItem* lastCake = nullptr;
//                    for (int i = 11; i >= 0; i--) {
//                        if (coordonneesBase[i][3] == 1 || coordonneesBase[i][4] == 1) {
//                            lastCake = ptrEchantillon[i];
//                            break;
//                        }
//                    }
//                    if (lastCake != nullptr) {
//                        // Set the parent of the last cake to the scene
//                        lastCake->setParentItem(nullptr);
//                        // Set the position of the last cake to where it currently is
//                        QPointF scenePos = lastCake->mapToScene(QPointF(0, 0));
//                        lastCake->setPos(scenePos);
//                        // Clear the coordinates for the last cake
//                        coordonneesBase[lastCakeIndex][3] = 0;
//                        coordonneesBase[lastCakeIndex][4] = 0;
//                        qDebug() << "Last cake released";
//                    }
//                }
//            }

            break;
//**********************************************************************************************************************


        case 5: //Recalage
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
                for (int i=0; i<5; i++)
                    if (item[i]) scene->removeItem(item[i]);

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
      for(int i = 0; i < 30; ++i){
          if(coordonnees[i][5] > 10) coordonnees[i][4] = PosRotrob - 90 ;
          //afficherEchantillon();

      }
        //miseAJourEchantillons();
        //detecterEchantillons();
        //detecterCollisionEchantillon();
        afficherEchantillon();

    }

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
                                         "0","1","2","3","4","5","9","10","11","20","30","40","41","42","50"}; //case en tableau

            textStream << indexToAction[indexComboBoxAction];

            textStream << ",";
                          testindex = ui->tableView->model()->index(i,3);
                          if (ui->tableView->model()->data(testindex).toString() == "Oui") textStream << "Y";
                          else if (ui->tableView->model()->data(testindex).toString() == "Non") textStream << "N";
                          else if (ui->tableView->model()->data(testindex).toString() == "Avant") textStream << "F";
                          else if (ui->tableView->model()->data(testindex).toString() == "Arriere") textStream << "B";
                          else if (ui->tableView->model()->data(testindex).toString() == "Etage 0") textStream << "E0";
                          else if (ui->tableView->model()->data(testindex).toString() == "Etage 1") textStream << "E1";
                          else if (ui->tableView->model()->data(testindex).toString() == "Etage 2") textStream << "E2";
                          else if (ui->tableView->model()->data(testindex).toString() == "Etage 3") textStream << "E3";
                          else if (ui->tableView->model()->data(testindex).toString() == "Etage 4") textStream << "E4";
                          else if (ui->tableView->model()->data(testindex).toString() == "Etage 5") textStream << "E5";
                          else if (ui->tableView->model()->data(testindex).toString() == "Etage 6") textStream << "E6";
                          else if (ui->tableView->model()->data(testindex).toString() == "Etage 7") textStream << "E7";
                          else if (ui->tableView->model()->data(testindex).toString() == "Etage 8") textStream << "E8";
                          else if (ui->tableView->model()->data(testindex).toString() == "Gauche") textStream << "L";
                          else if (ui->tableView->model()->data(testindex).toString() == "Droite") textStream << "R";
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
                    ui->tableView->model()->setData(ui->tableView->model()->index(index,2),"bleu");
                else
                    ui->tableView->model()->setData(ui->tableView->model()->index(index,2),"vert");
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
                if(liste[4] == "Y") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Oui");
                else if (liste[4] == "N") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Non");
                if(liste[4] == "F") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Avant");
                else if (liste[4] == "B") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Arrière");
                if(liste[4] == "E0") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 0");
                else if (liste[4] == "E1") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 1");
                else if (liste[4] == "E2") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 2");
                else if (liste[4] == "E3") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 3");
                else if (liste[4] == "E4") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 4");
                else if (liste[4] == "E5") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 5");
                else if (liste[4] == "E6") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 6");
                else if (liste[4] == "E7") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 7");
                else if (liste[4] == "E8") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Etage 8");
                if(liste[4] == "L") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Gauche");
                else if (liste[4] == "R") ui->tableView->model()->setData(ui->tableView->model()->index(index,3),"Droite");
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
}


void MainWindow::detecterCollisionEchantillon() {
    // Remove previously added detection zone rectangles from the scene
    QList<QGraphicsItem*> itemsToRemove = ui->graphicsView->scene()->items();
    for(QGraphicsItem* item : itemsToRemove) {
        if (item == frontZone || item == backZone) {
            ui->graphicsView->scene()->removeItem(item);
            delete item;
        }
    }

    // Create new detection zone rectangles at the robot's current position
    QRectF frontZoneRect = robot1->mapToScene(robot1->boundingRect().center().x() + 40 , robot1->boundingRect().center().y() - 60, 125, 125).boundingRect();
    QRectF backZoneRect = robot1->mapToScene(robot1->boundingRect().center().x() - 160 , robot1->boundingRect().center().y() -60 , 125, 125).boundingRect();

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
    ui->graphicsView->scene()->addItem(frontZone);
    ui->graphicsView->scene()->addItem(backZone);

   // qDebug("test collision");
    //qDebug() << "Front " << frontZoneRect;
    // Check for collisions between the robot and samples
    for (int i = 0; i < 12; i++) {
        if ((coordonnees[i][3] != 1) && (coordonnees[i][4] != 1)) { //regarder si les gateau ne sont pas prise
            if (ptrEchantillon[i]->collidesWithItem(frontZone)) {
                // Update the value of coordonneesBase[i][3] to 1
                coordonneesBase[i][3] = 1;
                //ptrEchantillon[i]->setParentItem(robot1);
                //ptrEchantillon[i]->setPos(100, 0);
                //qDebug("Collision avec échantillon en face");
                //break;
            }
            // Check if the collision is inside the back detection zone
            else if (ptrEchantillon[i]->collidesWithItem(backZone)) {
                // Update the value of coordonneesBase[i][4] to 1
                coordonneesBase[i][4] = 1;
                //ptrEchantillon[i]->setParentItem(robot1);
                //ptrEchantillon[i]->setPos(-100, 0);
                //qDebug("Collision avec échantillon derrière");
                //break;
            }
        }
    }
}


void MainWindow::afficherEchantillon(){

    bool firstCakeCollected = false; // track if the first cake layer has been collected
    for (int i = 0; i < 12; i++) {
        if (coordonneesBase[i][3] == 1) { // If the robot has picked up the sample with the front end
            if (!firstCakeCollected) { // if this is the first cake layer collected
                ptrEchantillon[i]->setZValue(1); // set the z-value to a high value to keep it on top
                firstCakeCollected = true; // set flag to true
            }
            ptrEchantillon[i]->setParentItem(robot1); // Set the parent of the cake to the robot
            ptrEchantillon[i]->setPos(100,0); // Set the position of the cake to the front as the robot
            qDebug() << "Cake picked up with front end";
        }
        else if (coordonneesBase[i][4] == 1) { // If the robot has picked up the sample with the back end
            if (!firstCakeCollected) { // if this is the first cake layer collected
                ptrEchantillon[i]->setZValue(1); // set the z-value to a high value to keep it on top
                firstCakeCollected = true; // set flag to true
            }
            ptrEchantillon[i]->setParentItem(robot1); // Set the parent of the cake to the robot
            ptrEchantillon[i]->setPos(-100,0); // Set the position of the cake to the back as the robot
            qDebug() << "Cake picked up with back end";
        }
    }
}



int MainWindow::collisionVentouse(int i,int rotRob){
    QLine ligneVentouse(0,0,10,0);
    int toReturn = -1; //la fonction retournera -1 si aucune collision n'est détecté, autrement , elle renvoie le numéro de l'échantillon attrapé.
    int position[2] = {0,0};

    //on setup le pen qui va dessiner les lignes
    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(5);

    //on ajoute les lignes à la scène et on et leurs associe le pen
    collisionLine[0] = scene->addLine(ligneVentouse);
    collisionLine[0]->setPen(pen);
    collisionLine[1] = scene->addLine(ligneVentouse);
    collisionLine[1]->setPen(pen);
    collisionLine[2] = scene->addLine(ligneVentouse);
    collisionLine[2]->setPen(pen);
    collisionLine[3] = scene->addLine(ligneVentouse);
    collisionLine[3]->setPen(pen);

    //on détermine la position de la ventouse choisie par le paramètre i

    switch (i){
        case 0 : position[0] = (226.06)*sin(((rotRob) * M_PI)/180 - 28.6*M_PI/180) ;
                 position[1] =  (226.06)*cos(((rotRob) * M_PI)/180 - 28.6*M_PI/180);
                break;
        case 1 : position[0] = (137.5)*sin(((rotRob) * M_PI)/180) ;
                 position[1] =  (137.5)*cos(((rotRob) * M_PI)/180);
                break;
        case 2 : position[0] = (226.06)*sin(((rotRob) * M_PI)/180 + 28.6*M_PI/180) ;
                 position[1] =  (226.06)*cos(((rotRob) * M_PI)/180 + 28.6*M_PI/180);
                break;
        case 5 : position[0] = (226.06)*sin(((rotRob) * M_PI)/180 + 28.6*M_PI/180 + M_PI) ;
                 position[1] =  (226.06)*cos(((rotRob) * M_PI)/180 + 28.6*M_PI/180 + M_PI);
                break;
        case 4 : position[0] = (137.5)*sin(((rotRob) * M_PI)/180 + M_PI) ;
                 position[1] =  (137.5)*cos(((rotRob) * M_PI)/180 + M_PI);
                break;
        case 3 : position[0] = (226.06)*sin(((rotRob) * M_PI)/180 + 28.6*M_PI/180 - M_PI) ;
                 position[1] =  (226.06)*cos(((rotRob) * M_PI)/180 - 28.6*M_PI/180 + M_PI);
        break;
}

collisionLine[0]->setPos(ROBOTCENTRE);
collisionLine[0]->moveBy(position[0],position[1]);
collisionLine[0]->moveBy(27-5,0); // on soustrait la position par 5 pixels pour centrer la ligne qui est de 10 pixels
collisionLine[1]->setPos(ROBOTCENTRE);
collisionLine[1]->moveBy(position[0],position[1]);
collisionLine[1]->moveBy(27-5,54);
collisionLine[2]->setPos(ROBOTCENTRE);
collisionLine[2]->moveBy(position[0],position[1]);
collisionLine[2]->moveBy(0-5,27);
collisionLine[3]->setPos(ROBOTCENTRE);
collisionLine[3]->moveBy(position[0],position[1]);
collisionLine[3]->moveBy(54-5,27);

for(int j =0;j<30;j++){
if(collisionLine[0]->collidesWithItem(ptrEchantillon[j]) //ça fait peur , mais ça veut juste dire si les quatre lignes vertes touchent l'échantillon
   && collisionLine[1]->collidesWithItem(ptrEchantillon[j])
   && collisionLine[2]->collidesWithItem(ptrEchantillon[j])
   && collisionLine[3]->collidesWithItem(ptrEchantillon[j])){
    toReturn  = j;
}

}
//on cache les lignes de collision pour éviter les bug graphique (et parceque c'est moche) n'hesitez pas a commenter cette boucle pour mieux comprendre le système
for(int i=0;i<4;i++)
collisionLine[i]->hide();
return toReturn;
}

void MainWindow::resetPosEchantillon(void)
{

    for(int i = 0; i<30 ; i++)
    {

        for(int j = 0; j< 6 ; j++)
        {
            coordonnees[i][j] = coordonneesBase[i][j];
        }
    }
}

int MainWindow::getAction(int chiffre){
int toReturn = 0;
switch(chiffre){

//case 170 : //Deploiement --> changer en ascenseur
//toReturn = 0 ;
//break;
//
//case 171: //Prise_bas
//toReturn = 1  ;
//break;
//case 172: //Passe
//toReturn = 2  ;
//break;
//case 173: //Pose_bas
//toReturn = 3  ;
//break;
//case 174 : //Pose_Haut
//toReturn = 4  ;
//break;
//case 175 : //Prise_bordure
//toReturn = 5 ;
//break;
//case 176 : //Pose_bordure
//toReturn = 6 ;
//break;
//case 177 : //Prise_distrib
//toReturn = 7 ;
//break;
//case 178: //Ranger
//toReturn = 8 ;
//break;
//
//case 154: //Res_prestest
//toReturn = 9 ;
//break;
//case 155: //Res_mes
//toReturn = 10 ;
//break;
//case 156: //Res_rang
//toReturn = 11 ;
//break;
//
//case 180: //Pre_prise
//toReturn = 12 ;
//break;
//case 181: //Prise_statuette
//toReturn = 13 ;
//break;
//case 182: //Pose_statuette
//toReturn = 14 ;
//break;

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

