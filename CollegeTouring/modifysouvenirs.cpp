#include "modifysouvenirs.h"
#include "ui_modifysouvenirs.h"

modifySouvenirs::modifySouvenirs(QWidget *parent, DbManager* database) :
    QDialog(parent),
    ui(new Ui::modifySouvenirs)
{
    ui->setupUi(this);

    databaseInternal = database;

    if(databaseInternal == nullptr) {
        const QString FILE_NAME = "collegetouring.db";
        QString dbPath = qApp->applicationDirPath();
        dbPath.append('/' + FILE_NAME);

        // open database with file path
        databaseInternal = new DbManager(dbPath);
        qDebug() << "Database should be located at: " << dbPath;
    }

     // populate campuses combo boxes
    for(auto &i : databaseInternal->getCampusNames()) {
        ui->campusSouvenirComboBox->addItem(i);
        ui->addCampusComboBox->addItem(i);
    }



    // populate souvenirs once on window init
    //qDebug() << ui->campusSouvenirComboBox->currentText();
    ui->souvenirNameComboBox->clear();
    for(auto &i: databaseInternal->getSouvenirNamesByCampus(ui->campusSouvenirComboBox->currentText())) {
        ui->souvenirNameComboBox->addItem(i);
    }
}

modifySouvenirs::~modifySouvenirs()
{
    delete ui;
    databaseInternal = nullptr; // this is database review form's dbmanager, don't delete
}

void modifySouvenirs::on_modifyButton_clicked()
{
    QString newSouvName;
    QString oldSouvName;
    QString campus;
    double newSouvValue;

    newSouvName = ui->nameLineEdit->text();
    oldSouvName = ui->souvenirNameComboBox->currentText();
    newSouvValue = ui->priceSpinBox->value();
    campus = ui->campusSouvenirComboBox->currentText();

    // qDebug() << "new: " << newSouvName << ' ' << newSouvValue << " old: " << oldSouvName << " campus: " << campus;

    databaseInternal->updateSouvenir(campus, oldSouvName, newSouvName, newSouvValue);

    // update souvenir names
    ui->souvenirNameComboBox->clear();
    for(auto &i: databaseInternal->getSouvenirNamesByCampus(ui->campusSouvenirComboBox->currentText())) {
        // qDebug() << "added " << i;
        ui->souvenirNameComboBox->addItem(i);
    }

    // update souvenir prices


}

void modifySouvenirs::on_campusSouvenirComboBox_currentIndexChanged(const QString &arg1)
{
    // qDebug() << "on_campusSouvenirComboBox_currentIndexChanged";
    ui->souvenirNameComboBox->clear();
    // populate souvenirs once on window init
    //qDebug() << ui->campusSouvenirComboBox->currentText();

    auto temp = databaseInternal->getSouvenirNamesByCampus(ui->campusSouvenirComboBox->currentText());

    for(auto &i: temp) {
        ui->souvenirNameComboBox->addItem(i);
    }
}

void modifySouvenirs::on_souvenirNameComboBox_currentIndexChanged(const QString &arg1)
{
    // qDebug() << "on_souvenirNameComboBox_currentIndexChanged";
    ui->nameLineEdit->setText(arg1);
    ui->priceSpinBox->setValue(databaseInternal->getSouvenirPrice(ui->souvenirNameComboBox->currentText(), ui->campusSouvenirComboBox->currentText()));
}

void modifySouvenirs::on_deleteButton_clicked()
{
    QString removeSouv;
    QString removeCampus;

    removeSouv = ui->souvenirNameComboBox->currentText();
    removeCampus = ui->campusSouvenirComboBox->currentText();

    // remove souvenir
    databaseInternal->removeSouvenir(removeCampus, removeSouv);

    // update souvenir names
    ui->souvenirNameComboBox->clear();
    for(auto &i: databaseInternal->getSouvenirNamesByCampus(ui->campusSouvenirComboBox->currentText())) {
        // qDebug() << "added " << i;
        ui->souvenirNameComboBox->addItem(i);
    }
}

void modifySouvenirs::on_addButton_clicked()
{
    QString campus = ui->addCampusComboBox->currentText();
    QString souvenirName = ui->addNameLineEdit->text();
    double price = ui->addPriceSpinBox->value();

    // check input for errors
    if(databaseInternal->souvExists(campus, souvenirName)) {
        QMessageBox::information(this, "Error", "Souvenir already exists.");
    }else if(souvenirName.isEmpty()) {
        QMessageBox::information(this, "Error", "Please input the new souvenir's information.");
    }else if(price <= 0.0){
        QMessageBox::information(this, "Error", "The new souvenir's price cannot be $0.00.");
    }else {
         databaseInternal->addSouvenir(campus, souvenirName, price);
         QMessageBox::information(nullptr, "Success", "Added souvenir to the database.");
    }
}
