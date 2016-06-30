/**********************************************************************
 * ** Copyright (C) 2004 MurphyTalk
 * **
 * ** This file is part of the MurphyTalk PinYin Chinese input method,
 * ** which is for the Qtopia Environment.
 * **
 * ** This is partially based on Smart Chinese Input Method 
 * ** by James Su
 * ** Copyright (c) 2002 James Su <suzhe@turbolinux.com.cn>
 * **
 * ** This file may be distributed and/or modified under the terms of the
 * ** GNU General Public License version 2 as published by the Free Software
 * ** Foundation and appearing in the file LICENSE.GPL included in the
 * ** packaging of this file.
 * **
 * ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 * **
 * **
 * ** murphytalk@gmail.com
 * ** $Date: 2004/07/31 17:23:37 $
 * **
 * **********************************************************************/
#ifndef PINYINPhrase_H
#define PINYINPhrase_H

#include "../scim/scim_pinyin.h"

class PinyinPhraseKey;
class PinyinPhraseEntry;
class PinyinPhraseKeyLessThan;

typedef std::vector<PinyinPhraseKey>	PinyinPhraseKeyVector;
typedef std::vector<PinyinPhraseEntry>	PinyinPhraseEntryVector;

typedef std::pair<uint32, uint32>	PhraseOffsetFrequencyPair;
typedef std::vector<PhraseOffsetFrequencyPair> PhraseOffsetFrequencyPairVector;

typedef uint32				PhraseOffset;
typedef std::vector<PhraseOffset>	PhraseOffsetVector;

typedef std::vector<ucs4_t>		PhraseString;
typedef std::vector<PhraseString>       PhraseStringVector;

typedef std::pair<PinyinPhraseEntryVector::const_iterator,PinyinPhraseEntryVector::const_iterator> 
	PinyinPhraseEntryVectorPosRangePair;

typedef std::map<uint32,uint32>         PhraseOffsetToFreqMap;

/*
class PhrasePinyinKeyLessThan
	: public std::binary_function <PinyinKey, PinyinKey, bool>
{
public:
	bool operator () (PinyinKey lhs,PinyinKey rhs) const {
		if (lhs.get_initial() < rhs.get_initial())
			return true;
		else if (lhs.get_initial() == rhs.get_initial()) {
			if(lhs.get_final() == SCIM_PINYIN_ZeroFinal || rhs.get_final() == SCIM_PINYIN_ZeroFinal)
				return false;
			else if (lhs.get_final() < rhs.get_final())
				return true;
			else
				return false;
		}
		return false;
	}
};
*/

#if 0
class PhrasePinyinKeyEqualTo
	: public std::binary_function <PinyinKey, PinyinKey, bool>
{
public:
	bool operator () (PinyinKey lhs,PinyinKey rhs) const {
		if(lhs.get_initial() == rhs.get_initial()){
			if(lhs.get_final() == SCIM_PINYIN_ZeroFinal || rhs.get_final() == SCIM_PINYIN_ZeroFinal)
				return true;
			else
				return lhs.get_final() == rhs.get_final();
		}
		else
			return false;
	}
};
#else
class PhrasePinyinKeyEqualTo
	: public std::binary_function <PinyinKey, PinyinKey, bool>
{
public:
	bool operator () (PinyinKey lhs,
					  PinyinKey rhs) const {
		if (lhs.get_initial() == rhs.get_initial() &&
			lhs.get_final() == rhs.get_final())
			return true;
		return false;
	}
};
#endif

class PinyinPhraseKey
{
public:
	PinyinPhraseKey();
	PinyinPhraseKey(const char *keystr);
	~PinyinPhraseKey();
	friend class PinyinPhraseKeyLessThan;
	friend class PinyinPhraseEntry;
public:
	void set_key(const char *keystr);
	inline bool isValid(){
		return m_keys.size()>0;
	}
	inline unsigned int get_key_count(){
		return m_keys.size();
	}
	inline void clear_key(){
		m_keys.clear();
	}
	inline PinyinKey& get_key_by_index(unsigned int index){
		return m_keys[index];
	}

	bool operator == (PinyinPhraseKey rhs) const {
		if(m_keys.size()!=rhs.get_key_count()){
			return false;
		}
		else{
			bool result=true;
			for(unsigned int i=0;i<m_keys.size();i++){
				if(!m_key_equal(m_keys[i],rhs.get_key_by_index(i))){
					result = false;
					break;
				}
			}
			return result;
		}                
	}
	bool operator != (PinyinPhraseKey rhs) const {
		return ! (*this == rhs);
	}
	String get_key_string();
protected:
	std::istream& input_text (std::istream &is);
protected:
	PhrasePinyinKeyEqualTo	m_key_equal;
#ifndef USE_ARRAY
        PinyinKeyVector		m_keys;
#else
	PinyinKey m_keys[2];	
#endif
};

class PinyinPhraseKeyLessThan
	: public std::binary_function <PinyinPhraseKey, PinyinPhraseKey, bool>
{
	bool final_equal_to(PinyinKey lhs,
			    PinyinKey rhs) const {
		return lhs.get_final()==rhs.get_final()||
		       lhs.get_final() == SCIM_PINYIN_ZeroFinal || rhs.get_final() == SCIM_PINYIN_ZeroFinal;		
	}
	
public:
	bool operator () (PinyinPhraseKey lhs,
			  PinyinPhraseKey rhs) const {

		unsigned int count = std::min(lhs.m_keys.size(),rhs.get_key_count());
		for(unsigned int i=0;i<count;i++){
			if(lhs.m_keys[i].get_initial()==rhs.m_keys[i].get_initial()){
				continue;
			}
			else{
				return lhs.m_keys[i].get_initial()<rhs.m_keys[i].get_initial();				
			}
		}

		for(unsigned int i=0;i<count;i++){
			if(final_equal_to(lhs.m_keys[i],rhs.m_keys[i])){
				continue;
			}
			else{
				return lhs.m_keys[i].get_final()<rhs.m_keys[i].get_final();				
			}
		}

		//equal?check count of pinyin
		return lhs.m_keys.size()<rhs.get_key_count();
	}
};

class PhraseOffsetFrequencyPairGreaterThanByFrequency
{
public:
	bool operator () (const PhraseOffsetFrequencyPair &lhs,
					  const PhraseOffsetFrequencyPair &rhs) const {
		return lhs.second > rhs.second;
	}
	bool operator () (const PhraseOffsetFrequencyPair &lhs,
					  uint32 rhs) const {
		return lhs.second > rhs;
	}
	bool operator () (uint32 lhs,
					  const PhraseOffsetFrequencyPair &rhs) const {
		return lhs > rhs.second;
	}
};

class PhraseOffsetFrequencyPairGreaterThanByPhraseOffset
{
public:
	bool operator () (const PhraseOffsetFrequencyPair &lhs,
					  const PhraseOffsetFrequencyPair &rhs) const {
		return lhs.first > rhs.first;
	}
	bool operator () (const PhraseOffsetFrequencyPair &lhs,
					  uint32 rhs) const {
		return lhs.first > rhs;
	}
	bool operator () (uint32 lhs,
					  const PhraseOffsetFrequencyPair &rhs) const {
		return lhs > rhs.first;
	}
};

class PinyinPhraseEntry
{
	PinyinPhraseKey			m_key;
	PhraseOffsetVector		m_phrases;
	friend class PinyinPhraseTable;
public:
	PinyinPhraseEntry(std::istream &is);
	PinyinPhraseEntry(const char*pinyin);
	PinyinPhraseEntry(PinyinPhraseKey& key);
	~PinyinPhraseEntry();
public:
	/**
	 * copy operator.
	 */
	const PinyinPhraseEntry& operator = (const PinyinPhraseEntry &entry) {
		if (this != &entry) {
			m_key = entry.m_key;
			m_phrases = entry.m_phrases;
		}
		return *this;
	}

	inline bool isValid(){
		return m_key.get_key_count()>0&&m_phrases.size()>0;
	}

	operator PinyinPhraseKey () const {
		return m_key;
	}

	bool operator == (PinyinPhraseEntry rhs) const {
		return m_key==rhs.m_key;
	}

	bool operator != (PinyinPhraseEntry rhs) const {
		return ! (*this == rhs);
	}

	void insert (const PhraseOffset &ch) {
#if 0		
		PhraseOffsetFrequencyPairVector::iterator i =
			std::lower_bound (m_phrases.begin (),m_phrases.end(),ch.first,
					  PhraseOffsetFrequencyPairGreaterThanByPhraseOffset());

		if (i != m_phrases.end () && i->first == ch.first) {
			if (ch.second > i->second)
				i->second = ch.second;
		}
		else {
			m_phrases.insert (i, ch);
	        }
#else
		m_phrases.push_back(ch);
#endif
	}

	inline void append(PinyinPhraseEntry& entry){
		std::copy(entry.m_phrases.begin(),entry.m_phrases.end(),
			  std::back_inserter<PhraseOffsetVector>(m_phrases));
	}

	inline PinyinPhraseKey& get_key(){
		return m_key;
	}

	unsigned int get_all_phrases(PhraseOffsetFrequencyPairVector& vect);
	std::ostream& output_text(std::ostream &os);
	//bool set_frequency(uint32 offset,uint32 freq);
protected:
	std::istream& input_text (std::istream &is);	
};

class PinyinPhraseTable
{
public:
	PinyinPhraseTable(const char* indexfile);
	~PinyinPhraseTable();
public:
	bool load_index(const char* indexfile);
	bool save_index(const char* indexfile);
	unsigned int find_phrases(PhraseOffsetFrequencyPairVector& phrases,PinyinPhraseKey& pinyin);
	unsigned int get_phrases_by_offsets(PhraseOffsetFrequencyPairVector& phrases_pair,PhraseStringVector& strs);
	void set_frequency(uint32 offset,uint32 freq);
	bool append_phrase(PhraseString&,const char* pinyin);
protected:
	PinyinPhraseEntryVector             m_phrases;
     	PinyinPhraseEntryVectorPosRangePair m_last_matched_phrases_range;
	PhraseOffsetToFreqMap               m_recent_hit_cache;
	PinyinPhraseEntryVector             m_new_phrases_cache;
protected:
	bool input (std::istream &is);
	//bool output(std::ostream &os);
	void insert(PinyinPhraseEntry& Phrase);
};

std::ostream& utf8_write_phase_string(std::ostream& os,PhraseString& str);

#endif

/*
 *  Revision history
 *  
 *  $Log: PinyinPhrase.h,v $
 *  Revision 1.4  2004/07/31 17:23:37  Lu Mu
 *  1)cache changes to index and phrase file(frequency and new phrase)
 *  2)support write to phrase index and phrase file format V0.03
 *
 *  Revision 1.3  2004/07/26 18:19:23  Lu Mu
 *  support the reading of phrase file format V0.03
 *
 *  Revision 1.2  2004/07/20 11:25:29  Lu Mu
 *  (1)phrase frequency
 *  (2)self define phrase
 *
 *  Revision 1.1  2004/07/17 07:10:31  Lu Mu
 *  phrase support
 *
 *
 */
