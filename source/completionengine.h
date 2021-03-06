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

#ifndef COMPLETIONENGINE_H
#define COMPLETIONENGINE_H
#include <QListWidget>
#include <QStringList>
#include <QString>

class WidgetTextEdit;

class CompletionEngine : public QListWidget
{
    Q_OBJECT

public:
    CompletionEngine(WidgetTextEdit * parent);

    void proposeCommand(int left, int top, int lineHeight, QString commandBegin);
    QString acceptedWord();

    void addCustomWordFromSource();
public slots:
 //   void setFocus(void);

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void loadFile(QString filename);
    QStringList _words;
    QStringList _customWords;
    QString _commandBegin;
};

#endif // COMPLETIONENGINE_H
