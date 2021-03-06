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



#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/data/fonts/consola.ttf");
    QFontDatabase::addApplicationFont(":/data/fonts/consolab.ttf");
    QFontDatabase::addApplicationFont(":/data/fonts/consolai.ttf");
    QFontDatabase::addApplicationFont(":/data/fonts/consolaz.ttf");

    qDebug()<<QString("Start ")+APPLICATION_NAME+QString(" version ")+CURRENT_VERSION;
    MainWindow w;

    if(argc > 1)
    {
        QString filename = argv[1];
        w.open(filename);
    }

    w.show();
    
    return a.exec();
}
