/**********************************************************************
** Copyright (C) 2004 MurphyTalk
**
** This file is part of the MurphyTalk PinYin Chinese input method,
** which is for the Qtopia Environment.
**
** This file is partially based on Smart Chinese Input Method 
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
#include "PinyinEngine.h"
#include "public.h"

PinyinEngine::PinyinEngine(const char *table_file,const char *phrase_index_file)
	:m_table(NULL,table_file),m_table_filename(table_file),
	 m_phrases_table(phrase_index_file),m_phrase_idx_filename(phrase_index_file)
{
}

PinyinEngine::~PinyinEngine()
{
	save_table();
}

unsigned int PinyinEngine::search(const char* pinyin)
{
	m_key.set_key(pinyin);

	if(isPhrase()){		
		unsigned int count=m_phrases_table.find_phrases(m_offset_freq_pairs,m_key);
		m_phrases_table.get_phrases_by_offsets(m_offset_freq_pairs,m_phrases);
		return count;
	}
	else{
		return m_table.find_chars(m_chars,m_key.get_key_by_index(0));
	}
}

QChar PinyinEngine::get_char(unsigned int index)
{	
	//if(index>=m_chars.size()) return QChar();
	QChar c(m_chars[index]);
	return c;
}

QString PinyinEngine::get_phrase(unsigned int index)
{
	QString str;
	for(unsigned int i = 0;i<m_phrases[index].size();i++){
		str+=QChar(m_phrases[index][i]);
	}
	return str;
}

void PinyinEngine::hit(unsigned int index)
{
	if(isPhrase()){
		m_phrases_table.set_frequency(m_offset_freq_pairs[index].first,
					      m_offset_freq_pairs[index].second+1);
	}
	else{
		PinyinKey& key=m_key.get_key_by_index(0);
		uint32 freq=m_table.get_char_frequency(m_chars[index],key)+1;
		m_table.set_char_frequency(m_chars[index],freq,key);
	}
}

void PinyinEngine::save_table()
{
	m_table.save_table(m_table_filename.c_str());
	m_phrases_table.save_index(m_phrase_idx_filename.c_str());
	printX86("table saved\n");
}

void PinyinEngine::append_phrase(QString& phrase,const char* pinyin)
{
	if(phrase.length()<2)  return;

	String sPinyin(pinyin);
	trim(sPinyin);

	if(sPinyin.size()==0)  return;

	PhraseString str;
	for(unsigned int i=0;i<phrase.length();i++){
		str.push_back(phrase[i].unicode());
	}
	m_phrases_table.append_phrase(str,sPinyin.c_str());
}
/*
 * Revision history
 * 
 * $Log: PinyinEngine.cpp,v $
 * Revision 1.3  2004/07/20 11:26:05  Lu Mu
 * (1)phrase frequency
 * (2)self define phrase
 *
 * Revision 1.2  2004/07/17 07:10:45  Lu Mu
 * phrase support
 *
 * Revision 1.1  2004/07/10 15:02:22  Lu Mu
 * v0.0.1 released
 * TODO: phase support
 *
 *
 */
