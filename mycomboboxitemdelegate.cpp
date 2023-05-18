#include "mycomboboxitemdelegate.h"
#include "mainwindow.h"

//c'est ici qu'on vient si on veut changer le nom des contenus des cases

MyComboBoxDelegate::MyComboBoxDelegate(QObject *parent, MainWindow *mw)
    : QStyledItemDelegate(parent), mainWindow(mw) {

}

MyComboBoxDelegate::~MyComboBoxDelegate()
{
}


QWidget *MyComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &s, const QModelIndex &index) const
{
    const QAbstractItemModel *model = index.model();
    QComboBox *cb = new QComboBox(parent);

    if(index.column() == 1) // si on est sur la colonne une, on ajoute ou définit des variables qui seront stockés dans dataCol1
    {
        cb->addItems(mainWindow->dataCol1);

        connect(cb, SIGNAL(currentIndexChanged(int)), mainWindow, SLOT(setHeaderComboBox(int)));

        cb->setCurrentText(model->data(index).toString());
        return cb;
    }

    if(index.column() == 2 or index.column() == 3 or index.column() == 4 or index.column() == 5
    or index.column() == 6 or index.column() == 7 or index.column() == 10 or index.column() == 11 )
    {
        return cb;
    }
    return cb;
}

void MyComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *cb = qobject_cast<QComboBox *>(editor);
    Q_ASSERT(cb);
    model->setData(index, cb->currentText(), Qt::EditRole);
}

void MyComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const // permet d'afficher les différentes sous éléments des choix principaux
{
    const QAbstractItemModel *model = index.model();
    QComboBox *cb = static_cast<QComboBox*>(editor);

    if(index.column() == 2)//On va changer l'affichage de la 2eme colonne (la colonne équipe) pour l'option "Debut Match"
    {
        if(model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Debut Match"))
        {
            cb->addItem(QString("Vert"));
            cb->addItem(QString("Bleu"));
            cb->setCurrentText(model->data(index).toString());
        }

        else if((model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Recalage"))||(model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("XYT"))) //Si dans la 1er colonne on a comme option "Recalage" ou "XYT", on affiche le sens "avant" ou "arrière"
        {
            cb->addItem(QString("Arrière"));
            cb->addItem(QString("Avant"));
            cb->setCurrentText(model->data(index).toString());
        }

        else if(model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Action")) //si dans la 1er conlonne, on a l'option Action, modifer les contenus de la 2eme colonne
        {
  //***************| action relative à l'année dernière |***************************************
            //chasse neige
            //cb->addItem(QString("Ascenceur")); //action 150

            ////action liées aux bras
            //cb->addItem(QString("Prise_bas"));
            //cb->addItem(QString("Passe"));
            //cb->addItem(QString("Pose_bas"));
            //cb->addItem(QString("Pose_Haut"));
            //cb->addItem(QString("Prise_bordure"));
            //cb->addItem(QString("Pose_bordure"));
            //cb->addItem(QString("Prise_distrib"));
            //cb->addItem(QString("Ranger"));

            ////actions liées aux bras de mesures
            //cb->addItem(QString("Res_prestest"));
            //cb->addItem(QString("Res_mes"));
            //cb->addItem(QString("Res_rang"));

            ////actions liées à la statuette
            //cb->addItem(QString("Pre_prise"));
            //cb->addItem(QString("Prise_statuette"));
            //cb->addItem(QString("Pose_statuette"));



//***************| action relative à la CHAQUE ANNEE |***************************************

            cb->addItem(QString("Evitement")); //action 0
            cb->addItem(QString("Set_odo")); //action 1
            cb->addItem(QString("Wait")); //action 2
            cb->addItem(QString("Wait_other_bot")); //action 3
            cb->addItem(QString("Check_pos_adversaire")); //action 4
            cb->addItem(QString("Fin_de_match")); //action 5
            cb->addItem(QString("Ajouter_point")); //action 9

//***************| action relative à la coupe 2023 |***************************************

            //action liées au gateau
            cb->addItem(QString("Prise_gateau")); //action 10
            cb->addItem(QString("Relacher_gateau")); //action 11
            cb->addItem(QString("Etage_gateau")); //action 20

            //Action liés au cérise
            cb->addItem(QString("Pose_cerise"));//action 30
            cb->addItem(QString("Attrape_cerise"));//action 40
            cb->addItem(QString("Range_Attrape_cerise"));//action 41
            cb->addItem(QString("Tir_cerise"));//action 42
            cb->addItem(QString("Barillet"));//action 60

            //action liés au fin de jeu
            cb->addItem(QString("Deguisement")); //action 50

//********************************************************************************************

            cb->setCurrentText(model->data(index).toString());
        }
//        else if(model->data(model->index(index.row(), 0),Qt::DisplayRole).toString().contains("Ligne Droite"))
//            sb->setValue(500);
    }
    if(index.column() == 3)
    {
        if(model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Rotation"))
        {
            cb->addItem(QString("Relatif"));
            cb->addItem(QString("Absolu"));
            cb->setCurrentText(model->data(index).toString());
        }

        else if(model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Recalage"))
        {
            cb->addItem(QString("Mécanique"));
            cb->addItem(QString("Capteur"));
            cb->setCurrentText(model->data(index).toString());
        }

        else if(model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Evitement"))
        {     
            cb->addItem(QString("Oui"));
            cb->addItem(QString("Non"));
            cb->setCurrentText(model->data(index).toString());
        }

        else if((model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Prise_gateau"))
                ||(model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Relacher_gateau")))
        {
            cb->addItem(QString("Avant"));
            cb->addItem(QString("Arrière"));
            cb->setCurrentText(model->data(index).toString());
        }

        else if(model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Etage_gateau"))
        {
            cb->addItem(QString("Etage 0"));
            cb->addItem(QString("Etage 1"));
            cb->addItem(QString("Etage 2"));
            cb->addItem(QString("Etage 3"));
            cb->addItem(QString("Etage 4"));
            cb->addItem(QString("Etage 5"));
            cb->addItem(QString("Etage 6"));
            cb->addItem(QString("Etage 7"));
            cb->addItem(QString("Etage 8"));
            cb->setCurrentText(model->data(index).toString());
        }

        else if((model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Attrape_cerise"))
                ||(model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Range_Attrape_cerise"))
                ||(model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Tir_cerise")))
        {
            cb->addItem(QString("Gauche"));
            cb->addItem(QString("Droite"));
            cb->setCurrentText(model->data(index).toString());
        }
        else if((model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Wait")))
        {
            cb->addItem(QString("2"));
            cb->addItem(QString("5"));
            cb->addItem(QString("10"));
            cb->setCurrentText(model->data(index).toString());
        }
        else if((model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Deguisement")))
        {
            cb->addItem(QString("ON"));
            cb->addItem(QString("OFF"));
            cb->setCurrentText(model->data(index).toString());
        }
        else if((model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Barillet")))
        {
            cb->addItem(QString("Barillet_1"));
            cb->addItem(QString("Barillet_2"));
            cb->addItem(QString("Barillet_3"));
            cb->addItem(QString("Barillet_4"));
            cb->addItem(QString("Barillet_5"));
            cb->setCurrentText(model->data(index).toString());
        }
        else if((model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Pose_cerise")))
        {
            cb->addItem(QString("Cerise_1"));
            cb->addItem(QString("Cerise_2"));
            cb->addItem(QString("Cerise_3"));
            cb->addItem(QString("Cerise_4"));
            cb->setCurrentText(model->data(index).toString());
        }
    }

    if(index.column() == 4)
    {
        if(model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Recalage"))
        {
            cb->addItem(QString("Abscisse"));
            cb->addItem(QString("Ordonnée"));
            cb->setCurrentText(model->data(index).toString());
        }
        if(model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Courbe"))
        {
            cb->addItem(QString("Droite"));
            cb->addItem(QString("Gauche"));
            cb->setCurrentText(model->data(index).toString());
        }
    }

    if(index.column() == 5)
    {
        if(model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Recalage"))
        {
            cb->addItem(QString("Rob-")); //115
            cb->addItem(QString("Rob+")); //1885
            cb->addItem(QString("Gob-")); //163
            cb->addItem(QString("Gob+")); //1837
            cb->setCurrentText(model->data(index).toString());
        }
    }

    //if(index.column() == 6)
    //{
    //    if(model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Vert") || model->data(model->index(index.row(), 2),Qt::DisplayRole).toString().contains("Bleu"))
    //    {
    //        cb->addItem(QString("Case 1"));
    //        cb->addItem(QString("Case 2"));
    //        cb->addItem(QString("Case 3"));
    //        cb->addItem(QString("Case 4"));
    //        cb->addItem(QString("Case 5"));
    //        cb->setCurrentText(model->data(index).toString());
    //    }
    //}

    if(index.column() == 6)
    {
        if(!model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Debut Match"))//si différent de "debut match" ajouter
        {
            cb->addItem(QString("Précis"));
            cb->addItem(QString("Non"));
            cb->setCurrentText(model->data(index).toString());
        }
    }

    if(index.column() == 7)
    {
        if(!model->data(model->index(index.row(), 1),Qt::DisplayRole).toString().contains("Debut Match"))
        {
            cb->addItem(QString("Attendre"));
            cb->addItem(QString("Saut(Position)"));
            cb->addItem(QString("Saut(Délais)"));
            cb->addItem(QString("Enchainement"));
            cb->addItem(QString("Clothoïde"));
            cb->setCurrentText(model->data(index).toString());
        }
    }

    if(index.column() == 10 or index.column() == 11)
    {
        for(int i = 0; i <= model->rowCount(); i++)
        {
            cb->addItem(QString::number(i+1));
        }
        cb->setCurrentText(model->data(index).toString());
    }

}
