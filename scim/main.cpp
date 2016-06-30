//#include "scim_pinyin.h"
using namespace std;

#if 0
extern std::ostream &
utf8_write_wchar (std::ostream &os, ucs4_t wc);
int main()
{
	PinyinTable ime(NULL,"pinyin_table.txt");
	
	PinyinKey key;
	
	key.set_key(scim_default_pinyin_validator,"pin");

	std::vector <ucs4_t> chars;
	int n=ime.find_chars(chars,key);
	cout<<n<<" matched"<<endl;
	
	ofstream o("out.txt");
	for(int i=0;i<chars.size();i++){
		utf8_write_wchar(o,chars[i]);
	}
}
#else

#include <qstring.h>
#include <qfile.h>
#include <qtextstream.h>
#include "../PinyinEngine.h"

const char table_file[]="pinyin_table.txt";

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
