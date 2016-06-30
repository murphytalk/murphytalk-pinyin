/**********************************************************************
** Copyright (C) 2004 MurphyTalk
**
** This file is part of the MurphyTalk PinYin Chinese input method,
** which is forthe Qtopia Environment.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
**
** murphytalk@gmail.com
** $Date: 2004/07/10 15:02:23 $
**
**********************************************************************/

#include <qapplication.h>
#include <qobject.h>
#include <qpixmap.h>
#include <qpainter.h>
#include "PinyinFrame.h"
#include "PinyinImpl.h"

/* XPM of inputmethod*/
static const char * pix_xpm[] = {
"16 13 3 1",
" 	c #FFFFFFFFFFFF",
"#	c #000000000000",
".	c #FFFFFFFFFFFF",
"                ",
"  # ####        ",
"  # ....#       ",
"  # ....#       ",
"  # ....#       ",
"  # # ##        ",
"  #      #...#  ",
"  #      #...#  ",
"  #       ####  ",
"  #       ...#  ",
"  #      #...#  ",
"  #       ###   ",
"                "};


QPinyinImpl::QPinyinImpl()
:m_pinyin_frame(0), icn(0), ref(0)
{
}

QPinyinImpl::~QPinyinImpl()
{
	delete m_pinyin_frame;
	delete icn;
}

QWidget *QPinyinImpl::inputMethod( QWidget *parent, Qt::WFlags f )
{
	if ( !m_pinyin_frame ){
		m_pinyin_frame = new QPinyinFrame( parent, "MurphyTalk", f );
	}
	
	return m_pinyin_frame;
}

void QPinyinImpl::resetState()
{
}

QPixmap *QPinyinImpl::icon()
{
	if ( !icn ){
		icn = new QPixmap( (const char **)pix_xpm );
	}
	return icn;
}

QString QPinyinImpl::name()
{
	return qApp->translate( "InputMethods", "MurphyTalk" );
}

void QPinyinImpl::onKeyPress( QObject *receiver, const char *slot )
{
	if ( m_pinyin_frame ){
		QObject::connect( m_pinyin_frame, SIGNAL(key(ushort,ushort,ushort,bool,bool)), receiver, slot );
	}
}

#ifndef QT_NO_COMPONENT
QRESULT QPinyinImpl::queryInterface( const QUuid &uuid, QUnknownInterface **iface )
{
	*iface = 0;
	
	if ( uuid == IID_QUnknown )
		*iface = this;
	else if ( uuid == IID_InputMethod )
		*iface = this;

	if ( *iface )
		(*iface)->addRef();
	
	return QS_OK;
}

Q_EXPORT_INTERFACE()
{
	Q_CREATE_INSTANCE( QPinyinImpl )
}
#endif

/*
 * Revision history
 * 
 * $Log: PinyinImpl.cpp,v $
 * Revision 1.2  2004/07/10 15:02:23  Lu Mu
 * v0.0.1 released
 * TODO: phase support
 *
 * Revision 1.1.1.1  2004/07/07 16:24:13  Lu Mu
 * Created
 *
 *
 */
