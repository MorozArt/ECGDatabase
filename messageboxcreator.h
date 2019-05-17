#ifndef MESSAGEBOXCREATOR_H
#define MESSAGEBOXCREATOR_H

#include <QMessageBox>


class MessageBoxCreator
{
public:
    MessageBoxCreator();
    static void showMessageBox(QWidget *parent, QString title, QString message);
    static void showMessageBoxNoSelectedFiles(QWidget *parent);
    static int execQuestionMessageBox(QWidget *parent, QString question);
};

#endif // MESSAGEBOXCREATOR_H
