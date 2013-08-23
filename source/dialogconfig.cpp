/***************************************************************************
 *   copyright       : (C) 2013 by Quentin BRAMAS                          *
 *   http://www.simtex.fr                                                  *
 *                                                                         *
 *   This file is part of SimTex.                                          *
 *                                                                         *
 *   SimTex is free software: you can redistribute it and/or modify        *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   SimTex is distributed in the hope that it will be useful,             *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with SimTex.  If not, see <http://www.gnu.org/licenses/>.       *                         *
 *                                                                         *
 ***************************************************************************/

#include "dialogconfig.h"
#include "configmanager.h"
#include "ui_dialogconfig.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

DialogConfig::DialogConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfig)
{
    ui->setupUi(this);
    connect(this->ui->pushButton_saveAndQuit,SIGNAL(clicked()),this,SLOT(saveAndClose()));
    connect(this->ui->pushButton_save,SIGNAL(clicked()),this,SLOT(save()));
    connect(this->ui->pushButton_quit,SIGNAL(clicked()),this,SLOT(close()));
    connect(this->ui->listWidget, SIGNAL(currentRowChanged( int )), this, SLOT(changePage( int )));
}

DialogConfig::~DialogConfig()
{
    delete ui;
}

void DialogConfig::changePage(int currentRow)
{
    //switch(currentRow)
    {
        //case 1:
            this->ui->stackedWidget->setCurrentIndex(currentRow);
    }
}

void DialogConfig::saveAndClose()
{
    this->save();
    QSettings settings;
    settings.beginGroup("theme");
    /*if(this->ui->radioButtonLightTheme->isChecked())
    {
        settings.setValue("theme",QString("light"));
    }
    else
    {
        settings.setValue("theme",QString("dark"));
    }*/

    settings.setValue("pointSize",this->ui->spinBoxPointSize->value());
    ConfigManager::Instance.setPointSize(this->ui->spinBoxPointSize->value());
    this->accept();
}

void DialogConfig::save()
{


    // Page Builder:

    ConfigManager::Instance.setBibtexCommand(this->ui->lineEdit_bibtex->text());
    ConfigManager::Instance.setPdflatexCommand(this->ui->lineEdit_pdflatex->text());
    ConfigManager::Instance.setLatexPath(this->ui->lineEdit_latexPath->text());
}
void DialogConfig::show()
{
    QSettings settings;
    settings.beginGroup("theme");

    /*if(!settings.value("theme",QString("dark")).toString().compare("dark"))
    {
        this->ui->radioButtonDarkTheme->setChecked(true);
        this->ui->radioButtonLightTheme->setChecked(false);
    }
    else
    {
        this->ui->radioButtonDarkTheme->setChecked(false);
        this->ui->radioButtonLightTheme->setChecked(true);
    }*/


    // Page Builder:

    this->ui->lineEdit_bibtex->setText(ConfigManager::Instance.bibtexCommand());
    this->ui->lineEdit_pdflatex->setText(ConfigManager::Instance.pdflatexCommand());
    this->ui->lineEdit_latexPath->setText(ConfigManager::Instance.latexPath());


    this->ui->spinBoxPointSize->setValue(ConfigManager::Instance.getTextCharFormats("normal").font().pointSize());
    QDialog::show();
}
