#include <QIcon>
#include "messageboxcreator.h"
#include "globalvariable.h"

MessageBoxCreator::MessageBoxCreator()
{

}

void MessageBoxCreator::showMessageBox(QWidget *parent, QString title, QString message) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    if(title == ERROR_TITLE) {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowIcon(QPixmap(":/images/false.png"));
    } else {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowIcon(QPixmap(":/images/true.png"));
    }
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MessageBoxCreator::showMessageBoxNoSelectedFiles(QWidget *parent) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(ERROR_TITLE);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowIcon(QPixmap(":/images/false.png"));
    msgBox.setText("Не выбран ни один файл!");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
