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
** $Date: 2004/07/31 17:20:19 $
**
**********************************************************************/
#include "PinyinFrame.h"

#include <qpe/global.h>

#include <qpainter.h>
#include <qlist.h>
#include <qbitmap.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qhbuttongroup.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qcolor.h> 
#include <qpen.h>
#include <qwindowsystem_qws.h>
#include <qpe/qpeapplication.h>
#include <qnamespace.h>

#include "public.h"
#include "Config.h"

const char table_file[]=
#ifdef X86
"/root/pinyin_table.txt";
#else
"/home/zaurus/.murphytalk/pinyin_table.txt";
#endif

const char phrase_index_file[]=
#ifdef X86
"/root/murphytalk_phrase_idx.txt";
#else
"/home/zaurus/.murphytalk/murphytalk_phrase_idx.txt";
#endif

const char config_file[]=
#ifdef X86
"/root/murphytalk.conf";
#else
"/etc/murphytalk.conf";
#endif


/* XPM */
static const char * pix_prev[] = {
"16 13 3 1",
" 	c #C0C0C0C0C0C0",
"#	c #000000000000",
".	c #00000000FFFF",
"          #     ",
"         ##     ",
"        #.#     ",
"       #..#     ",
"      #...#     ",
"     #....#     ",
"    #.....#     ",
"     #....#     ",
"      #...#     ",
"       #..#     ",
"        #.#     ",
"         ##     ",
"          #     "};

static const char * pix_next[] = {
"16 13 3 1",
" 	c #C0C0C0C0C0C0",
"#	c #000000000000",
".	c #00000000FFFF",
"     #          ",
"     ##         ",
"     #.#        ",
"     #..#       ",
"     #...#      ",
"     #....#     ",
"     #.....#    ",
"     #....#     ",
"     #...#      ",
"     #..#       ",
"     #.#        ",
"     ##         ",
"     #          "};

const int PINYIN_X = 2;
const int PINYIN_Y = 2;

const unsigned short ZHONG = 0x4e2d;   //Hanzi : Chinese 
const unsigned short YING  = 0x82f1;   //Hanzi : English

const char ABOUT[]="About";

const char MAKEPHRASE_OPEN [] = "\xe3\x80\x90\xe9\x80\xa0\xe8\xaf\x8d\x20";
const char MAKEPHRASE_CLOSE[] = "\xe3\x80\x91";

//the following marks encoded in ucs-2(unicode 16 big endian)
const int COMMA_MARK	=0xff0c;
const int PERIOD_MARK	=0x3002;
const int BOOK_MARK1	=0x300a;
const int BOOK_MARK2	=0x300b;
const int QUESTION_MARK	=0xff1f;
const int COLON_MARK	=0xff1a;

QPinyinFrame::QPinyinFrame(QWidget* parent, const char* name, WFlags f) :
	QFrame(parent,name,f),m_engine(table_file,phrase_index_file),
	m_bEnglishMode(false),m_bMakingPhrase(false)
{
	(new QHBoxLayout(this))->setAutoAdd(TRUE);
	QPEApplication::grabKeyboard(); 
	qwsServer->setKeyboardFilter (this);

	Config config(config_file);

	String font_name=config.get_as_string("font");
	int    font_size=config.get_as_int("font-size");

	printX86("from config -> font=%s\n",font_name.c_str());
	printX86("from config -> font-size=%d\n",font_size);

	if(font_name.size()>0){
		if(font_size<=0){
			font_size=font().pointSize();
		}
		setFont(QFont(font_name.c_str(),font_size));
	}	
}

QPinyinFrame::~QPinyinFrame()
{
	printX86("ime killed\n");	
	QPEApplication::ungrabKeyboard(); 
}

QSize QPinyinFrame::sizeHint() const
{
	QPinyinFrame *This=const_cast<QPinyinFrame*>(this);
	int height=This->init_gui_dimention();
	return QSize(maximumWidth(), height);
}

/*
 * return frame height
 */
int QPinyinFrame::init_gui_dimention()
{
	static int sHeight = -1;

	if(sHeight>0)      return sHeight;

#if 0
	QPixmap next((const char **)pix_next);
	printX86("direction pix (%d,%d)\n",next.width(),next.height());
#endif
	const int pix_width=16,pix_height=13;

	QPainter painter(this);	
	QFontMetrics metric=painter.fontMetrics();
	QRect rect = metric.boundingRect(QChar(ZHONG));

	int width=640;
	printX86("text metric is %d,%d\n",rect.width(),rect.height());

	int x=PINYIN_X;
	int y=PINYIN_Y;
	painter.drawText(x,rect.height(),QChar(m_bEnglishMode?YING:ZHONG));
       	m_indicator_rect=QRect(x,y,rect.width(),rect.height());
	printX86("indicator rect(%d,%d,%d,%d)\n",m_indicator_rect.left(),m_indicator_rect.top(),m_indicator_rect.width(),m_indicator_rect.height());

	//horizonal line which seperates pinyin and hanzi candidates
	y+=rect.height()+2;
	QRect about = metric.boundingRect(QString(ABOUT));
	m_about_rect = QRect(width-2-about.width(),PINYIN_Y,about.width(),about.height());
	printX86("about rect(%d,%d,%d,%d)\n",m_about_rect.left(),m_about_rect.top(),m_about_rect.width(),m_about_rect.height());

	///////////////////////////////////////////////
	//the second row --  hanzi candicates
	y+=2;

	m_leftbtn_rect=QRect(x,y+2,pix_width,pix_height);		
	printX86("left btn rect(%d,%d,%d,%d)\n",m_leftbtn_rect.left(),m_leftbtn_rect.top(),m_leftbtn_rect.width(),
		       m_leftbtn_rect.height());

	m_rightbtn_rect=QRect(width-2-pix_width,y+2,pix_width,pix_height);		
	printX86("right btn rect(%d,%d,%d,%d)\n",m_rightbtn_rect.left(),m_rightbtn_rect.top(),m_rightbtn_rect.width(),
		       m_rightbtn_rect.height());

	sHeight=y+rect.height()+5;

	return sHeight;
}

bool QPinyinFrame::filter (int unicode, int keycode, int modifiers, bool isPress, bool autoRepeat)
{   
	if (!isHidden()&& isPress) {
		if(m_bEnglishMode){
			if(unicode == 9 && keycode == Qt::Key_Tab){
				m_bEnglishMode = false;
				update();			
			}
			else{
				SendKey(unicode,keycode); 
			}
			return true;		
		}

		if (GetKey (unicode, keycode))
			return true;
		else
			return false;
	} 
	else
		return false;
}   

void QPinyinFrame::paintEvent(QPaintEvent * e)
{
	QFrame::paintEvent(e);
	
	QPainter painter(this);
	painter.setClipRect (e->rect ());

	//FIXME: how to get screen size
	unsigned int width  = 640;

	///////////////////////////////////////////////
	//the first row - pinyin inputed 

	QFontMetrics metric=painter.fontMetrics();
	QRect rect = metric.boundingRect(QChar(ZHONG));

	int x=PINYIN_X;
	int y=PINYIN_Y;
	painter.drawText(x,rect.height(),QChar(m_bEnglishMode?YING:ZHONG));

	QPen penGray (QColor(128,128,128));
	QPen penWhite(QColor(255,255,255));
	QPen penText (QColor(0,0,0));
	QPen penPage (QColor(0,0,255));

	//vertical line which seperates chinese/english indicator with pinyin inputed
	painter.setPen(penGray);
	painter.drawLine(x+rect.width()+2,y,x+rect.width()+2,y+rect.height());
	painter.setPen(penWhite);
	painter.drawLine(x+rect.width()+3,y,x+rect.width()+3,y+rect.height());

	//making phrase?
	int X=x+rect.width()+7;
	int Y=rect.height();
	if(m_bMakingPhrase){
		QRect temp;

	        QString s=QString::fromUtf8(MAKEPHRASE_OPEN);
		painter.setPen(penText);
		painter.drawText(X,Y,s);
		temp = metric.boundingRect(s);
		X+=temp.width();

		painter.setPen(penPage);
		painter.drawText(X,Y,m_ime_info.phrase);
		temp = metric.boundingRect(m_ime_info.phrase);
		X+=temp.width();

		s=QString::fromUtf8(MAKEPHRASE_CLOSE);
		painter.setPen(penText);
		painter.drawText(X,Y,s);			
		temp = metric.boundingRect(s);
		X+=temp.width()+2;
	}
	//pinyin inputed by user
	painter.setPen(penText);
	painter.drawText(X,Y,QString(m_engine.get_formatted_pinyin().c_str()));

	//horizonal line which seperates pinyin and hanzi candidates
	y+=rect.height()+2;
	painter.setPen(penGray);
	painter.drawLine(x,y,x+width,y);
	painter.setPen(penWhite);
	painter.drawLine(x,y+1,x+width,y+1);


	//about
	painter.setPen(penPage);
	QRect about = metric.boundingRect(QString(ABOUT));
	painter.drawText(width-2-about.width(),rect.height(),QString(ABOUT));
	painter.drawLine(width-2-about.width(),rect.height()+1,width-2,rect.height()+1);

	///////////////////////////////////////////////
	//the second row --  hanzi candicates
	y+=2;

	//horizonal line at the bottom
	Y=y+rect.height();
	painter.setPen(penWhite);
	painter.drawLine(x,Y,x+width,Y);

	if(m_ime_info.prev_page_available()){
		QPixmap prev((const char **)pix_prev);
		painter.drawPixmap(x,y+2,prev);
		x+=prev.width()+2;
	}

	//hanzi candidates
	
	m_ime_info.candidates_on_page=0;	
	QString str;
	QRect hanzi;
	painter.setPen(penText);
	for(unsigned int i=m_ime_info.first_visible,xx=x,yy=Y-3;m_ime_info.candidates_on_page<10&&i<m_ime_info.candidates_count;i++){
		str.sprintf("%d.",(i-m_ime_info.first_visible+1)%10);//1. 2. ... 9. 0.
     		if(m_engine.isPhrase()){
			str+=get_phrase(i);
		}
		else{
			str+=get_char(i);
		}
		hanzi=metric.boundingRect(str);

		if(xx+hanzi.width()>=(unsigned int)m_rightbtn_rect.left()){//no space for this hanzi and right button
			if(i<m_ime_info.candidates_count-1){ //but we need another page
				break; //so give up to draw this hanzi
			}
			else{
				//this is the last page,do not need to show next button
				//so just check if the left space is enough to show this hanzi
				if(xx+hanzi.width()>width){
					break;//Ooops
				}			
			}			
		}
		
		painter.drawText(xx,yy,str);		
		xx+=hanzi.width()+10;
		m_ime_info.candidates_on_page++;
	}

	if(m_ime_info.next_page_available()){
		QPixmap next((const char **)pix_next);
		painter.drawPixmap(width-2-next.width(),y+2,next);
	}
}

bool QPinyinFrame::prev_page()
{
	if(m_ime_info.prev_page_available()){		
		printX86("prev page,firt visible index %d->",m_ime_info.first_visible);	
		m_ime_info.first_visible-=m_ime_info.candidates_on_prev_page;
		printX86("%d\n",m_ime_info.first_visible);	
		update();	
		return true;		
	}	
	else{
		return false;
	}
}

bool QPinyinFrame::next_page()
{
	if(m_ime_info.next_page_available()){		
		printX86("next page,firt visible index %d->",m_ime_info.first_visible);	
		m_ime_info.candidates_on_prev_page=m_ime_info.candidates_on_page;
		m_ime_info.first_visible+=m_ime_info.candidates_on_page;
		printX86("%d\n",m_ime_info.first_visible);	
		update();	
		return true;		
	}	
	else{
		return false;
	}	
}

/*
inline bool shift_pressed(int modifiers)
{
	return modifiers & Qt::ShiftButton;
}
*/

int QPinyinFrame::get_charunicode(unsigned int nIndexOnPage)
{
	unsigned int index = m_ime_info.first_visible + nIndexOnPage;
	printX86("get unicode:first visible index %d,current index %d,global index %d\n",
		m_ime_info.first_visible,nIndexOnPage,index);
	return (m_engine.get_char(index)).unicode();	
}

/*
  return true if need to update UI
 */
bool QPinyinFrame::commit_selection(int k)
{
	bool bUpdate = false;
	unsigned int index = ((k-'0')+9)%10;
	if(index<m_ime_info.candidates_on_page){
		m_engine.hit(m_ime_info.first_visible+index);
		if(m_engine.isPhrase()){
			QString phrase=get_phrase(m_ime_info.first_visible+index);
			if(m_bMakingPhrase){
				m_ime_info.phrase+=phrase;
			}
			else{
				printX86("phrase unicode:");
				for(unsigned int i=0;i<phrase.length();i++){
					printX86("%04X,",phrase[i].unicode());
					SendKey(phrase[i].unicode()); 
				}
				printX86("\n");
			}
		}
		else{
			if(m_bMakingPhrase){
				m_ime_info.phrase+=get_char(m_ime_info.first_visible+index);
			}
			else{
				SendKey(get_charunicode(index)); 
			}
		}
		resetState();
		bUpdate=true;
	}
	return bUpdate;
}

bool QPinyinFrame::send_hanzi_mark(int ascii_mark)
{
	int unicode = 0;	
	switch(ascii_mark){
		case ',':
			unicode = COMMA_MARK;
			break;
		case '.':
			unicode = PERIOD_MARK;
			break;
		case '<':
			unicode = BOOK_MARK1;
			break;
		case '>':
			unicode = BOOK_MARK2;
			break;
		case '?':
			unicode = QUESTION_MARK;
			break;
		case ':':
			unicode = COLON_MARK;
			break;
	}
	if(unicode!=0){
		SendKey(unicode);
		return true;
	}
	else{
		return false;
	}
}

bool QPinyinFrame::GetKey(int u, int k/*,int m*/)/*int unicode, int keycode, int modifiers*/
{
	printX86("key code is 0x%02X\n",k);
#if 0
	if(m_bEnglishMode){
		if(u == 9 && k == Qt::Key_Tab){
			m_bEnglishMode = false;
			update();			
		}
		else{
			SendKey(u,k); 
		}
		return true;		
	}
#endif
	bool bUpdate         = false;
	bool bKeyProcessed   = true;
	
        if( k >= '0' && k <= '9'){
commit:
		bUpdate=commit_selection(k);
		if(!bUpdate){
			bKeyProcessed=false;
		}
	}
	else if(k >= 'a' && k <= 'z'){
input:		
		m_ime_info.pinyin+=(char)k;
		search();
		bUpdate=true;	     
	}
	else if(k >= 'A' && k <= 'Z'){
		k = k - 'A'+ 'a';
		goto input;
	}
	else if(k == Qt::Key_Space){
		if(m_ime_info.candidates_on_page>0){
			k='1';
			goto commit;
		}
		else{
			bKeyProcessed=false;
		}
	}
	else if(k == Qt::Key_Right||k == Qt::Key_Down){
		if(!next_page()){
			bKeyProcessed=false;
		}					
	}
	else if(k == Qt::Key_Left||k == Qt::Key_Up){
		if(!prev_page()){
			bKeyProcessed=false;
		}					
	}
	else if( u == 8 && k == Qt::Key_Backspace){
		if(m_ime_info.pinyin.size()>0){			
			String::iterator pos=m_ime_info.pinyin.end()-1;
			m_ime_info.pinyin.erase(pos);
			search();
			bUpdate=true;
		}
		else{
			SendKey(u,k);
		}
		
	}
	else if(k == Qt::Key_Shift){
		if(m_bMakingPhrase){
			//commit the new phrase
			m_bMakingPhrase=false;
			m_engine.append_phrase(m_ime_info.phrase,m_making_phrase_pinyin.c_str());
			m_ime_info.phrase="";
			resetState();
			bUpdate=true;
		}
		else if(m_ime_info.pinyin.size()==0){
			printX86("entering making phrase mode...\n");
			m_making_phrase_pinyin="";
			m_bMakingPhrase=true;
			bUpdate=true;
		}
	}
	else if(u == 9 && k == Qt::Key_Tab){
		m_bEnglishMode=true;
		bUpdate=true;		
	}
	else if(  k == Qt::Key_Escape){
		if(m_bMakingPhrase){
			m_ime_info.phrase="";
			m_bMakingPhrase=false;
		}
		resetState();
		bUpdate=true;
	}
	else if(!send_hanzi_mark(k)){
		bKeyProcessed=false;
	}
	

	if(bUpdate){
		update();
	}
	
	if(!bKeyProcessed){
		SendKey(u,k);
	}


	return true;
}

void QPinyinFrame::show()
{
	QFrame::show ();
	QPEApplication::grabKeyboard();
	qwsServer->setKeyboardFilter (this);
}

void QPinyinFrame::hide()
{
	QFrame::hide ();
	QPEApplication::ungrabKeyboard(); 
	resetState();
	m_engine.save_table();	
	//qwsServer->setKeyboardFilter (NULL);
}
    
bool hit_test_helper(int x,int y,QRect& rect)
{
	if(x>=rect.left()&&x<=rect.right()&&
           y>=rect.top() &&y<=rect.bottom()){
		return true;
	}
	else{
		return false;
	}
}

void QPinyinFrame::mouseReleaseEvent(QMouseEvent* m)
{
	bool bUpdate = false;
	int x=m->x();
	int y=m->y();

	if(hit_test_helper(x,y,m_about_rect)){
		//show about infomation
		QMessageBox::information(this,"About",
                                         "MurphyTalk Pinyin "VERSION"<br><br>"
                                         "Created by <font color=\"#0000FF\">MurphyTalk</font><br>"
					 "aka:<font color=\"#0000FF\">DeepWater</font>@<b>Hi-PDA</b> community<br>"
					 "contact me at <font color=\"#0000FF\">murphytalk@gmail.com</font><br><br>"
					 "This software is partially based on scim chinese<br>"
					 "written by James Su(suzhe@tsinghua.org.cn)<br><br>"
					 "This small piece of cake is released under GPL;)");
	}	
	else if(hit_test_helper(x,y,m_leftbtn_rect)){
		prev_page();		
	}	
	else if(hit_test_helper(x,y,m_rightbtn_rect)){
		next_page();		
	}	
	else if(hit_test_helper(x,y,m_indicator_rect)){
		//switch between English/Chinese mode
		m_bEnglishMode=!m_bEnglishMode;
		bUpdate=true;
	}


	if(bUpdate){
		update();
	}
}

void QPinyinFrame::keyPressEvent(QKeyEvent*)
{
}

void QPinyinFrame::SendKey(int u , int c)
{
	qwsServer->sendKeyEvent ( u , c, 0, true, false);
	qwsServer->sendKeyEvent ( u , c, 0, false, false);
}
/*
 * Revision history
 * 
 * $Log: PinyinFrame.cpp,v $
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
 * Revision 1.4  2004/07/15 13:56:17  Lu Mu
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
