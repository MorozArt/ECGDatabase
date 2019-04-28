#include <QMovie>
#include "copyprogress.h"
#include "ui_copyprogress.h"

CopyProgress::CopyProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyProgress)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::MSWindowsFixedSizeDialogHint);
    QMovie *movie = new QMovie(":/images/load.gif");
    ui->imageLabel->setMovie(movie);
    movie->start();
}

CopyProgress::~CopyProgress()
{
    delete ui;
}

void CopyProgress::copyFinish() {
    ui->textLabel->setText("Готово!");
    ui->imageLabel->setPixmap(QPixmap(":/images/true.png"));
    ui->okButton->setEnabled(true);
}

void CopyProgress::on_okButton_clicked()
{
    close();
}

void CopyProgress::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    delete this;
}
