#include "databaseviewform.h"
#include "ui_databaseviewform.h"

databaseViewForm::databaseViewForm(QWidget *parent, bool adminUser, DbManager* databaseManager) :
    QDialog(parent),
    ui(new Ui::databaseViewForm)
{
    vector<QString> temp;

    database = databaseManager;

    ui->setupUi(this);

    temp = database->getCampusNames();
     // populate campuses combo box
    for(auto &i : database->getCampusNames()) {
        ui->collegeSelectBox->addItem(i);
        ui->collegeSelectBoxSouv->addItem(i);
    }

    // get admin status from main window
    isAdmin = adminUser;

    // populate the souvenir and campus distance models once on init
    // reusing code from the buttons for simplicity
    on_displayDistButton_clicked();
    on_displaySouvButton_clicked();

}

databaseViewForm::~databaseViewForm()
{
    delete ui;
    delete database;
}

void databaseViewForm::on_displayDistButton_clicked()
{
    if(ui->collegeSelectBox->currentText() == "") {
        QMessageBox::warning(this, "Error", "Please select a campus.");
    }else{
        qDebug() << "getting distances from " << ui->collegeSelectBox->currentText();

        auto model = database->getDistancesModel(ui->collegeSelectBox->currentText());

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Starting Campus"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Ending Campus"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Distance (mi)"));

        ui->campusesTableView->setModel(model);
        ui->campusesTableView->setColumnWidth(0,310);
        ui->campusesTableView->setColumnWidth(1,310);
        ui->campusesTableView->setColumnWidth(2,125);
    }

}

void databaseViewForm::on_displaySouvButton_clicked()
{
    if(ui->collegeSelectBox->currentText() == "") {
        QMessageBox::warning(this, "Error", "Please select a campus.");
    }else{
        qDebug() << "getting souvenirs from " << ui->collegeSelectBoxSouv->currentText();

        auto model = database->getSouvenirsModel(ui->collegeSelectBoxSouv->currentText());

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Campus"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Souvenir"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Price (USD)"));

        ui->souvenirTableView->setModel(model);
        ui->souvenirTableView->setColumnWidth(0,310);
        ui->souvenirTableView->setColumnWidth(1,310);
        ui->souvenirTableView->setColumnWidth(2,125);
    }
}
void databaseViewForm::on_pushButton_clicked()
{
    // only admins can modify the database
    if(isAdmin) {
        modDialog = new modifySouvenirs(nullptr, database);
        modDialog->exec();
        delete modDialog;

        // update database model
        auto model = database->getSouvenirsModel(ui->collegeSelectBoxSouv->currentText());

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Campus"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Souvenir"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Price (USD)"));

        ui->souvenirTableView->setModel(model);
        ui->souvenirTableView->setColumnWidth(0,310);
        ui->souvenirTableView->setColumnWidth(1,310);
        ui->souvenirTableView->setColumnWidth(2,125);
    } else {
        QMessageBox error;
        QPixmap icon;
        icon.load(":/images/bonk_emoji.png");
        icon = icon.scaled(100,100);

        error.setText("Error!");
        error.setInformativeText("Only administrators may modify the database.");
        error.setStandardButtons(QMessageBox::Ok);
        error.setIconPixmap(icon);
        error.setWindowTitle("BONK!");
        QApplication::beep();
        error.exec();
    }

}

void databaseViewForm::on_addCampus_clicked()
{
    // only admins can add campus
    if (isAdmin)
    {
        // opens new campus window
        addcampus = new addcampuses(nullptr, database);
        addcampus->exec();
        delete addcampus;

        on_displayDistButton_clicked();
        on_displaySouvButton_clicked();

        vector<QString> temp = database->getCampusNames();
         // populate campuses combo box
        ui->collegeSelectBox->clear();
        ui->collegeSelectBoxSouv->clear();
        for(auto &i : database->getCampusNames()) {
            ui->collegeSelectBox->addItem(i);
            ui->collegeSelectBoxSouv->addItem(i);
        }

    }
    else
    {
        QMessageBox::information(this, QObject::tr("System Message"), tr("Warning. Must be an Admin to add Campus"));
    }
}
