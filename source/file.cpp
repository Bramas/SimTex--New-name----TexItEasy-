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

#include "file.h"
#include "builder.h"
#include "viewer.h"
#include "widgettextedit.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QTimer>
#define AUTO_SAVE 120000

File::File(WidgetTextEdit* widgetTextEdit,QString filename) :
    _autoSaveTimer(new QTimer),
    builder(new Builder(this)),
    _codec(QTextCodec::codecForLocale()->name()),
    filename(filename),
    _modified(false),
    viewer(new Viewer(this)),
    _widgetTextEdit(widgetTextEdit)
{
    connect(_autoSaveTimer, SIGNAL(timeout()), this, SLOT(autoSave()));
}
void File::save(QString filename)
{
    //Check the filename
    if(!filename.isEmpty())
    {
        this->filename = filename;
    }
    if(this->filename.isEmpty())
    {
        return;
    }

    if(_modified)
    {
        this->data = this->_widgetTextEdit->toPlainText();
    }

    // Save
    QFile file(this->filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out.setCodec(_codec.toLatin1());
    //out.setGenerateByteOrderMark(true);
    out << this->data;

    _modified = false;
    _autoSaveTimer->stop();
    _autoSaveTimer->start(AUTO_SAVE);
}

const QString File::open(QString filename, QString codec)
{
    // Get the filename

    if(filename.isEmpty())
    {
        if(this->filename.isEmpty())
        {

            //this->filename = QFileDialog::getOpenFileName(this->_parent,tr("Ouvrir un fichier"));
            if(this->filename.isEmpty())
            {
                return QString("");
            }
        }
    }
    else
    {
        this->filename = filename;
    }
    // Open the file

    QFile file(this->filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString("");

    this->data = QString("");

    QTextStream in(&file);
    if(codec.isEmpty())
    {
        in.setCodec("UTF-8");
    }
    else
    {
        in.setCodec(codec.toLatin1());
    }

    while (!in.atEnd()) {
        data.append(in.readLine()+"\n");
    }
    if(codec.isEmpty() && data.contains(QString::fromUtf8("�")))
    {
        this->open(this->filename,"ISO 8859-1");
        return QString("");
    }
    this->_codec = in.codec()->name();
    this->_widgetTextEdit->setText(this->data);
    this->refreshLineNumber();
    _modified = false;
    _autoSaveTimer->stop();
    _autoSaveTimer->start(AUTO_SAVE);
    return this->data;

}

void File::refreshLineNumber()
{
    int lineNumber = this->_widgetTextEdit->document()->blockCount();
    //qDebug()<<"refreshLineNumber "<<lineNumber;
    _lineNumberSinceLastBuild.clear();
    for(int idx = 0; idx < lineNumber; ++idx)
    {
        _lineNumberSinceLastBuild.insert(idx,idx);
    }
}
void File::insertLine(int lineNumber, int lineCount)
{
    //qDebug()<<"insertLine "<<lineNumber<<" : "<<lineCount;
    for(int idx = lineNumber; idx < _lineNumberSinceLastBuild.size(); ++idx)
    {
        _lineNumberSinceLastBuild.insert(idx,_lineNumberSinceLastBuild.value(idx) - lineCount);
    }

    for(int idx = 0; idx < lineCount; ++idx)
    {
        _lineNumberSinceLastBuild.insert(_lineNumberSinceLastBuild.size(),_lineNumberSinceLastBuild.value(_lineNumberSinceLastBuild.size()-1)+1);
    }
    for(int idx = 0; idx > lineCount; --idx)
    {
        _lineNumberSinceLastBuild.remove(_lineNumberSinceLastBuild.size()-1);
    }
}

void File::autoSave()
{
    if(this->filename.isEmpty())
    {
        return;
    }
    QFile file(this->getAutoSaveFilename());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out.setCodec(_codec.toLatin1());
    //out.setGenerateByteOrderMark(true);
    out << this->data;
}
