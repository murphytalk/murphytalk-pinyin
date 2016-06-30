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
**
**********************************************************************/
#ifndef PINYINFRM_H
#define PINYINFRM_H

#include <qframe.h>
#include <qwindowsystem_qws.h>
#include "PinyinEngine.h"

class QPinyinFrame : public QFrame, public QWSServer::KeyboardFilter
{
	Q_OBJECT
public:
	QPinyinFrame(QWidget* parent=0, const char* name=0, WFlags f=0);
	~QPinyinFrame();

	void resetState(){
		if(m_bMakingPhrase){
			m_making_phrase_pinyin+=m_ime_info.pinyin;
		}
		m_ime_info.pinyin="";
		m_ime_info.candidates_count=0;
		m_engine.clear_key();
	}
	QSize sizeHint() const ;
public:
	bool GetKey(int,int);
	virtual void show();
	virtual void hide();

	void paintEvent(QPaintEvent * e); 
	void SendKey ( int  , int c = 0);

	void mouseReleaseEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);

signals:
	void key( ushort, ushort, ushort, bool, bool );

private:
	PinyinEngine m_engine;
	bool m_bEnglishMode;
	bool m_bMakingPhrase;
	QRect m_indicator_rect/*for English or Chinese mode*/;
	QRect m_about_rect;/*the about text rect*/
	QRect m_leftbtn_rect,m_rightbtn_rect;

	typedef struct ime_info_struc{
		String       pinyin;
		QString      phrase;
		unsigned int candidates_count;
		unsigned int first_visible; //index of first visible candidate hanzi,start from 0
		unsigned int candidates_on_page; //hanzi candidates on current page
		unsigned int candidates_on_prev_page; //hanzi candidates on previous page

		ime_info_struc(){
			candidates_count = 0;
		}

		bool prev_page_available(){
			return candidates_count>0&&first_visible>0;
		}

		bool next_page_available(){
			return candidates_count>0&&(first_visible+candidates_on_page)<candidates_count;
		}
	}ime_info_t;

	ime_info_t m_ime_info;

	String     m_making_phrase_pinyin;
private:
	virtual bool filter ( int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat );
	int get_charunicode(unsigned int nIndexOnPage);
	inline QChar QPinyinFrame::get_char(unsigned int nIndex/*this is global index*/){
	       	return m_engine.get_char(nIndex);
	}
	
	inline QString QPinyinFrame::get_phrase(unsigned int nIndex/*this is global index*/){
       		return m_engine.get_phrase(nIndex);
     	}
	bool prev_page();
	bool next_page();
	bool commit_selection(int k);
	inline void search(){		
		m_ime_info.candidates_count=m_engine.search(m_ime_info.pinyin.c_str());
		m_ime_info.first_visible=0;
#ifdef X86
		printf("%s,%d matched\n",m_ime_info.pinyin.c_str(),m_ime_info.candidates_count);
#endif
	}
	int init_gui_dimention();
	bool send_hanzi_mark(int ascii_mark);
};
#endif
/*
 * Revision history
 * 
 * $Log: PinyinFrame.h,v $
 * Revision 1.7  2004/07/31 17:20:19  Lu Mu
 * support chinese punctuations
 *
 * Revision 1.6  2004/07/20 11:26:05  Lu Mu
 * (1)phrase frequency
 * (2)self define phrase
 *
 * Revision 1.5  2004/07/17 07:11:54  Lu Mu
 * 1)phrase support
 * 2)candidates sequence number,changed to 1,2,..,9,0
 * 3)space key to select first candidate
 *
 * Revision 1.4  2004/07/15 13:56:18  Lu Mu
 * fixed : hanzi candidates overlapped by system task bar while default font is not efont
 *
 * Revision 1.3  2004/07/10 16:58:11  Lu Mu
 * bug fix of displaying hanzi problem after the first page
 *
 * Revision 1.2  2004/07/10 15:02:23  Lu Mu
 * v0.0.1 released
 * TODO: phase support
 *
 * Revision 1.1.1.1  2004/07/07 16:24:13  Lu Mu
 * Created
 *
 *
 */
