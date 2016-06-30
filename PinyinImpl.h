/**********************************************************************
** Copyright (C) 2004 MurphyTalk
**
** This file is part of the MurphyTalk PinYin Chinese input method,
** which is for the Qtopia Environment.
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
#ifndef PINYIMPL_H
#define PINYIMPL_H

#include <qpe/inputmethodinterface.h>

class QPinyinFrame;
class QPixmap;

class QPinyinImpl : public InputMethodInterface
{
public:
    QPinyinImpl();
    virtual ~QPinyinImpl();

#ifndef QT_NO_COMPONENT
    QRESULT queryInterface( const QUuid&, QUnknownInterface** );
    Q_REFCOUNT
#endif

    virtual QWidget *inputMethod( QWidget *parent, Qt::WFlags f );
    virtual void resetState();
    virtual QPixmap *icon();
    virtual QString name();
    virtual void onKeyPress( QObject *receiver, const char *slot );

private:
    QPinyinFrame *m_pinyin_frame;
    QPixmap *icn;
    ulong ref;
};

#endif

/*
 * Revision history
 * 
 * $Log: PinyinImpl.h,v $
 * Revision 1.2  2004/07/10 15:02:23  Lu Mu
 * v0.0.1 released
 * TODO: phase support
 *
 * Revision 1.1.1.1  2004/07/07 16:24:13  Lu Mu
 * Created
 *
 *
 */
