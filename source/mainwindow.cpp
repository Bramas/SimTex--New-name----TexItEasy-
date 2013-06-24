#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "widgetlinenumber.h"
#include "widgettextedit.h"
#include "widgetscroller.h"
#include "syntaxhighlighter.h"
#include "file.h"
#include "builder.h"
#include "dialogwelcome.h"
#include "dialogconfig.h"

#include <QList>
#include <QScrollBar>
#include <QFileDialog>
#include <QSettings>
#include <QPushButton>
#include <QDebug>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    widgetTextEdit(new WidgetTextEdit(this)),
    widgetLineNumber(new WidgetLineNumber(this)),
    widgetScroller(new WidgetScroller),
    dialogWelcome(new DialogWelcome(this)),
    dialogConfig(new DialogConfig(this))
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint);
    widgetLineNumber->setWidgetTextEdit(widgetTextEdit);
    widgetScroller->setWidgetTextEdit(widgetTextEdit);
    SyntaxHighlighter * syntaxHighlighter = new SyntaxHighlighter(widgetTextEdit);

    // Load settings
    {
        QSettings settings;
        settings.beginGroup("mainwindow");
        if(settings.contains("geometry")) this->setGeometry(settings.value("geometry").toRect());
        settings.endGroup();
    }


    // Connect things that can update the widgetTextEdit

    connect(widgetTextEdit,SIGNAL(textChanged()),widgetLineNumber,SLOT(update()));
    //connect(widgetTextEdit->verticalScrollBar(),SIGNAL(valueChanged(int)),widgetLineNumber,SLOT(update()));

    //connect(widgetTextEdit,SIGNAL(textChanged()),widgetScroller,SLOT(updateText()));
    //connect(widgetTextEdit->verticalScrollBar(),SIGNAL(valueChanged(int)),widgetScroller,SLOT(update()));

    connect(widgetScroller,SIGNAL(changed(int)),widgetTextEdit,SLOT(scrollTo(int)));

    // Connect menubar Actions

    connect(this->ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(this->ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(this->ui->actionSaveAs,SIGNAL(triggered()),this,SLOT(saveAs()));


    connect(this->ui->actionPdfLatex,SIGNAL(triggered()),this->widgetTextEdit->getCurrentFile()->getBuilder(),SLOT(pdflatex()));

    ui->gridLayout->addWidget(widgetLineNumber,0,0);
    ui->gridLayout->addWidget(widgetTextEdit,0,1);
    //ui->gridLayout->addWidget(widgetScroller,0,2);

    ui->gridLayout->addWidget(widgetScroller,0,2);
    ui->gridLayout->setColumnMinimumWidth(0,40);
    //ui->gridLayout->setColumnMinimumWidth(2,100);


    connect(this->dialogWelcome->getActionNew(),SIGNAL(clicked()), this->dialogWelcome, SLOT(close()));
    connect(this->dialogWelcome->getActionOpen(),SIGNAL(clicked()), this->dialogWelcome, SLOT(close()));
    connect(this->dialogWelcome->getActionOpenLast(),SIGNAL(clicked()), this->dialogWelcome, SLOT(close()));
    connect(this->dialogWelcome->getActionOpen(),SIGNAL(clicked()), this, SLOT(open()));
    connect(this->dialogWelcome->getActionOpenLast(),SIGNAL(clicked()), this, SLOT(openLast()));
    this->dialogWelcome->show();

    connect(this->ui->actionSettings,SIGNAL(triggered()),this->dialogConfig,SLOT(show()));

}

MainWindow::~MainWindow()
{
    // Save settings
    {
        QSettings settings;
        settings.beginGroup("mainwindow");
        settings.setValue("geometry", this->geometry());
        settings.endGroup();
    }
    delete ui;
}

void MainWindow::openLast()
{
    //load last file if it exists
    {
        QSettings settings;
        if(settings.contains("files/filename"))
        {
            this->open(settings.value("files/filename").toString());
        }
    }
}
void MainWindow::open(QString filename)
{
    //get the filname
    if(filename.isEmpty())
    {
        filename = QFileDialog::getOpenFileName(this,tr("Ouvrir un fichier"));

        if(filename.isEmpty())
        {
            return;
        }
    }
    QString basename(filename);
    qDebug()<<"Open : "<<filename;
    //window title
    this->setWindowTitle(basename.replace(QRegExp("^.*[\\\\\\/]([^\\\\\\/]*)$"),"\\1")+" - SimTex");
    //udpate the settings
    {
        QSettings settings;
        settings.setValue("files/filename", filename);
    }
    //open
    this->widgetTextEdit->getCurrentFile()->open(filename);

    //udpate the widget
    this->widgetTextEdit->setText(this->widgetTextEdit->getCurrentFile()->getData().toLocal8Bit());

}
void MainWindow::save()
{
    if(this->widgetTextEdit->getCurrentFile()->getFilename().isEmpty())
    {
        return this->saveAs();
    }
    this->widgetTextEdit->getCurrentFile()->setData(this->widgetTextEdit->toPlainText());
    this->widgetTextEdit->getCurrentFile()->save();
}

void MainWindow::saveAs()
{
    //this->widgetTextEdit->getCurrentFile()->setData("sdfsdfg");
    //return;
    QString filename = QFileDialog::getSaveFileName(this,tr("Enregistrer un fichier"));
    this->widgetTextEdit->getCurrentFile()->setData(this->widgetTextEdit->toPlainText());
    if(filename.isEmpty())
    {
        return;
    }
    this->widgetTextEdit->getCurrentFile()->save(filename);
    this->setWindowTitle(filename.replace(QRegExp("^.*[\\\\\\/]([^\\\\\\/]*)$"),"\\1")+" - SimTex");
}
