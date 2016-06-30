using namespace std;

#ifndef QT
#include "PinyinPhrase.h"

const char index_file[]="murphytalk_phrase_idx.txt";

extern std::ostream &
utf8_write_wchar (std::ostream &os, ucs4_t wc);

void save_matched_strings(const char *fname,PhraseStringVector& strs)
{
	std::ofstream out(fname,std::ios::app|std::ios::out);
	
	for(PhraseStringVector::iterator pos=strs.begin();pos!=strs.end();pos++){
		utf8_write_phase_string(out,*pos);
		out<<std::endl;
	}
	out<<std::endl<<std::endl;
}

void enum_offset_freq_vector(PhraseOffsetFrequencyPairVector& v)
{
	for(PhraseOffsetFrequencyPairVector::iterator pos=v.begin();
	    pos!=v.end();pos++){
		printf("offset %d freq %d\n",pos->first,pos->second);
	}
}

void search_and_hit(PinyinPhraseTable& phase)
{
	PhraseOffsetFrequencyPairVector v;
	PinyinPhraseKey key("py");

	cout<<phase.find_phrases(v,key)<<" phrases matched"<<endl;

	PhraseStringVector strs;
	phase.get_phrases_by_offsets(v,strs);
	save_matched_strings("out.txt",strs);

	printf("listing offset and freq..\n");
	enum_offset_freq_vector(v);

	//hit the second matched phrase	
	phase.set_frequency(v[1].first,v[1].second+1);
}

#include <stdio.h>
int main()
{
	remove("out.txt");

	PinyinPhraseTable phase(index_file);
	

	for(int i=0;i<2;i++){
		printf("#%d ===============> \n",i+1);
		search_and_hit(phase);
	}
	phase.save_index(index_file);
}
#else

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include "../PinyinEngine.h"

int main()
{
	PinyinEngine ime(table_file);
	unsigned int n = ime.search((const char*)"pin");
	cout<<n<<" matched"<<endl;
	
	QFile file("out.txt");
	if ( file.open( IO_WriteOnly ) ) {
		QTextStream stream(&file);
		stream.setEncoding(QTextStream::UnicodeUTF8);

		QChar c;
		for(unsigned int i=0;i<n;i++){
			c=ime.get_char(i);
			stream<<c;
			
		}

		file.close();
	}

}

#endif
