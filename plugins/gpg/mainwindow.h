/*
 *
 *  Copyright (c) 2012
 *  name : mhogo mchungu
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QCloseEvent>

#include "../../zuluCrypt-gui/socketsendkey.h"
#include "../../zuluCrypt-gui/dialogmsg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow( QWidget * parent = 0 ) ;
	void SetAddr( QString ) ;
	~MainWindow();
private slots:
	void pbOpen( void ) ;
	void pbCancel( void ) ;
	void pbKeyFile( void ) ;
	void gotConnected( void ) ;
	void doneWritingData( void ) ;
private:
	QByteArray getGPGKey( QString EXE,QString key,QString path ) ;
	void closeEvent( QCloseEvent * );
	void disableAll( void ) ;
	void enableAlll( void ) ;
	QString FindGPG( void ) ;
	void SetFocus( void );
	void Exit( int ) ;
	Ui::MainWindow * m_ui;
	QString m_addr ;
	QString m_keyFile ;
	void * m_handle ;
};

#endif // MAINWINDOW_H
