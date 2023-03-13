#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QtWidgets>
#include <QDebug>
#include <QTableView>
#include <QLayoutItem>
#include <QTextStream>
#include <QComboBox>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QtCore>
#include <QtGui>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QRandomGenerator>




#include "mymodel.h"
#include "mycomboboxitemdelegate.h"
#include "ui_mainwindow.h"


#define LONGUEUR_TABLE 3000
#define LARGEUR_TABLE 2000
#define LONGUEUR_ROBOT 240
#define LARGEUR_ROBOT 240
#define LARGEUR_VENTOUSE 150
#define DIAG_ROBOT1 339

#define OFF 0
#define ON 1

#define BROWN 1
#define ROSE 2
#define YELLOW 3
#define EMPTY 4

#define GLOBALOFFSETX -500
#define GLOBALOFFSETY -500

#define ORIGINE_X 0
#define ORIGINE_Y 0

#define OBSTACLE_1_X 895
#define OBSTACLE_1_Y 1850

#define OBSTACLE_2_X 1495
#define OBSTACLE_2_Y 1700

#define OBSTACLE_3_X 2095
#define OBSTACLE_3_Y 1850

QT_BEGIN_NAMESPACE
//class QTableView;

void drawRobotPath(int table_ligne, QModelIndex index, QGraphicsScene* scene, QGraphicsLineItem* item[3], QPen redline, double PosXrobPres, double PosYrobPres, double PosRotrobPres, double largeur_robot, double distanceLigneDroite);
//void calculcentrerotationgauche(double PosXrobPres, double PosYrobPres, double PosRotrobPres, double rayonCourbe, double angleCourbe, double &PosXrob, double &PosYrob, double &PosRotrob);


namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;

    //QTableView *tableView;

    //attributs commune à chaque année
    QGraphicsScene *scene;
    QGraphicsPixmapItem *image;
    QGraphicsPixmapItem *robot1, *robotdep, *robot2;
    QGraphicsPixmapItem *couche_marron1,*couche_marron2,*couche_marron3,*couche_marron4;
    QGraphicsPixmapItem *couche_rose1,*couche_rose2,*couche_rose3,*couche_rose4;
    QGraphicsPixmapItem *couche_jaune1,*couche_jaune2,*couche_jaune3,*couche_jaune4;
    QGraphicsLineItem *lineUp;
    QGraphicsLineItem *lineDown;
    QGraphicsLineItem *lineRight;
    QGraphicsLineItem *lineLeft;
    QGraphicsLineItem *lineMap1;
    QGraphicsLineItem *lineMap2;
    QGraphicsLineItem *lineMap3;
    QGraphicsLineItem *ligneDroite;
    QGraphicsLineItem *lineRandom;
    QGraphicsEllipseItem *ellipseRandom;

    //attributs spécifiques à l'année 2022
    int chasseNeigeFlag[2]{false,false};
    QGraphicsPixmapItem *ptrEchantillon[30],*ptrCarre[10];
    QGraphicsRectItem *brasMesure[2];
    QGraphicsEllipseItem *ventouse[17], *pos_gateau, ventouseFront, ventouseBack ;
    QGraphicsLineItem *collisionLine[6],*ventouseVerticale[2], *brasDistrib[2];
    QPointF switchPosVentouse(int i , int PosRotrob);



public:
    explicit MainWindow(QWidget *parent = nullptr);

    static const QStringList dataCol1;
    static const QStringList dataEquipe;
    static const QStringList dataAction;


    int nbUpdateVisu = 0;

public slots:

    //methodes commune à chaque année
    void setHeaderComboBox(int index);                  //Modifie les headers en fonction de l'index de la ComboBox
    void initHeaderData();                              //Initialise les headers
    void initModel();                                   //Initialise le modèle
    void tableViewIsClicked(const QModelIndex &index);  //A chaque fois que l'utilisateur clique sur le tableau, actualise les headers
    void resetComboBox();                               //Supprime les combobox
    void setFocusOnButton();                            //Sélectionne le 1er bouton de la ligne créer
    void updateHeader(const QModelIndex &index);        //Actualise les Headers
    void updateVisu(const QModelIndex &index);          //Actualise les éléments visuels de la table de jeu
    void SetView();                                     //Actualise la vue (taille des images dans le "graphics view")
    void initVisu();
    int getAction(int chiffre);                         //Permet de déterminer l'action qui correspond à chiffre. utilisé lors de !!l'importation!! de stratégie.

    //Initialise la table de jeu et la position initiale du robot

    //methode spécifiques à l'année 2022
    QPixmap determinerCouleur(int i);                   // détermine la pixmap à utiliser pour l'échantillon i du tableau coordonnées
    void afficherEchantillon(int i);                    //affiche l'échantillon i du tableau de coordonnées
    int collisionVentouse(int i, int rotRob);          // crée quatres lignes qui permettent de savoir si la ventouse est bien placée
    void resetPosEchantillon();                         // remet à zéro les positions des échantillons en reprenant les valeurs de base du tableau coordonnées




protected:
    virtual void resizeEvent(QResizeEvent *event);      //Détection d'évènements et redimensionne la vue

private slots:
    void on_DeleteRowButton_clicked();
    void on_CreateRowButton_clicked();
    void on_InsertRowButton_clicked();
    void on_ExportFileButton_clicked();
    void on_ImportFileButton_clicked();

    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
