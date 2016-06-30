
/**********************************************************************
** Copyright (C) 2004 MurphyTalk
**
** This file is part of the MurphyTalk PinYin Chinese input method,
** which is for the Qtopia Environment.
**
** This is a based on Smart Chinese Input Method 
** by James Su
** Copyright (c) 2002 James Su <suzhe@turbolinux.com.cn>
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
** $Date: 2004/07/20 11:26:05 $
**
**********************************************************************/
#ifndef PINYINENGIN_H
#define PINYINENGIN_H

#include <qstring.h>
#include "scim/scim_pinyin.h"
#include "phrase/PinyinPhrase.h"

class PinyinEngine
{
public:
	PinyinEngine(const char *table_file,const char *phrase_index_file);
	~PinyinEngine();	

	unsigned int search(const char* pinyin);
	inline unsigned int get_char_count(){
		return m_chars.size();
	}
	QChar get_char(unsigned int index);
	QString get_phrase(unsigned int index);
	void hit(unsigned int index);
	void save_table();
	void append_phrase(QString&,const char* pinyin);
	inline bool isPhrase(){
		return m_key.get_key_count()>1;
	}
	inline String get_formatted_pinyin(){
		return m_key.get_key_string();
	}
	inline void clear_key(){
		m_key.clear_key();
	}
private:
	//PinyinKeyVector   m_keys;
	PinyinPhraseKey   m_key;

	//single hanzi
	PinyinTable m_table;
	CharVector  m_chars;
	String      m_table_filename;
	
	//phrase
       	PinyinPhraseTable               m_phrases_table;
	PhraseOffsetFrequencyPairVector m_offset_freq_pairs;
	PhraseStringVector              m_phrases;
	String                          m_phrase_idx_filename;
};

#endif
/*
 * Revision history
 * 
 * $Log: PinyinEngine.h,v $
 * Revision 1.3  2004/07/20 11:26:05  Lu Mu
 * (1)phrase frequency
 * (2)self define phrase
 *
 * Revision 1.2  2004/07/17 07:10:45  Lu Mu
 * phrase support
 *
 * Revision 1.1  2004/07/10 15:02:23  Lu Mu
 * v0.0.1 released
 * TODO: phase support
 *
 *
 */
