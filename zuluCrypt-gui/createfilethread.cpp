/*
 * 
 *  Copyright ( c ) 2011
 *  name : mhogo mchungu
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  ( at your option ) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "createfilethread.h"

createFileThread::createFileThread( QString file,double size )
{
	m_cancelled = 0 ;
	m_file = file ;
	m_size = size ;
}

void createFileThread::cancelOperation()
{
	m_cancelled = 1 ;
}

void createFileThread::start()
{
	QThreadPool::globalInstance()->start( this );
}

void createFileThread::run()
{
	/*
	 * This is the entry point for this class.
	 * The class basically cr*eates a file of a size specified by a user by writing '\0' to it.
	 *
	 * Then it creates a cryptsetup mapper and writes '\0' to the file through the mapper.
	 *
	 * This medhod of writing random data to the file seem to be faster than creating the file using random
	 * data from /dev/urandom
	 */

	this->createFile();

	if( m_cancelled == 1 )
		return ;

	emit doneCreatingFile();

	this->fillCreatedFileWithRandomData();
}

void createFileThread::createFile()
{
	double i ;

	QFile file( m_file ) ;
	file.open( QIODevice::WriteOnly ) ;

	const int SIZE = 1024 ;

	char data[SIZE];

	memset( data,0,SIZE );

	int x ;
	int y = -1 ;

	double data_written = 0 ;

	double k = m_size / SIZE ;

	emit progress( 0 );

	for( i = 0 ; i < k ; i++ ){
		if( m_cancelled == 1 )
			break ;

		file.write( data,SIZE ) ;

		data_written += SIZE ;

		x = ( int )( data_written * 100 / m_size ) ;

		if( x > y ){
			emit progress( x );
			y = x ;
		}
	}

	emit progress( 100 );

	file.setPermissions( QFile::ReadOwner|QFile::WriteOwner ) ;
	file.close();
}

void createFileThread::fillCreatedFileWithRandomData()
{
	this->openVolume()  ;
	
	this->writeVolume() ;

	this->closeVolume() ;
}

void createFileThread::closeVolume()
{
	QString path = m_file ;
	path.replace( "\"","\"\"\"" ) ;
	QString exe = QString( "%1 -q -d \"%2\"" ).arg( ZULUCRYPTzuluCrypt ).arg( path );

	QProcess p ;
	p.start( exe );
	p.waitForFinished();
	p.close();
}

void createFileThread::openVolume()
{
	QString path = m_file ;
	path.replace( "\"","\"\"\"" ) ;
	/*
	 * We do not let the cli write random data to the file by using -X( we use -J )because we want to write the random data
	 * ourselves giving us the ability to knoe exactly much data is already written
	 */
	QString exe = QString( "%1 -J -d \"%2\"" ).arg( ZULUCRYPTzuluCrypt ).arg( path );

	QProcess p ;
	p.start( exe );
	p.waitForFinished();
	p.close();
}

void createFileThread::writeVolume()
{
	QFile path( miscfunctions::mapperPath( m_file ) ) ;

	path.open( QIODevice::WriteOnly ) ;

	int j ;
	int k = -1 ;

	double data_written = 0 ;

	const int SIZE = 1024 ;
	char data[SIZE];

	memset( data,0,SIZE );

	emit progress( 0 );

	while( path.write( data,SIZE ) > 0 ){

		data_written += SIZE ;

		j = ( int )( data_written * 100 / m_size ) ;

		if( j > k ){
			emit progress( j );
			k = j ;
		}

		if( m_cancelled == 1 )
			break ;
	}

	emit progress( 100 );
	path.close();
}

createFileThread::~createFileThread()
{
	emit exitStatus( m_cancelled );
}
