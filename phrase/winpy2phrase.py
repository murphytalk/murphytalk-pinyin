#################################################################
# tool to extract 2-hanzi phrases from WINPY.txt
# also create MurphyTalk phrase file and pinyin phrase index file
# 
# you have to install cjkcodec 
#
# MurphyTalk 2004/7
# $Date: 2004/07/31 17:21:05 $

import codecs
import cjkcodecs


#fileter a line in winpy.txt
#only filter 2-hanzi phrases
#result -> pair (pinyin,phrase)
def filter(line):
	def is_alpha(a):
		return (a>='a' and a<='z') or a=="*"

	pair=()	
	if len(line)>4:
		if(not is_alpha(line[0]) and not is_alpha(line[1]) and is_alpha(line[2])):
			pinyin=line[2:]
			pinyin=pinyin[:len(pinyin)-2] #get rid of the last return char
			pair=(pinyin,line[:2])
			
	return pair

#map -> {pinyin:[phrases,...]}
def build_phrase_map(map):
	fSrc=codecs.open("WINPY.TXT","r","cjkcodecs.gb2312")

	lines=fSrc.readlines()
	fSrc.close()
	
	a=""
	phrases=[]
	i=1
	count=0
	for l in lines:
		pair=filter(l)
		if len(pair)==2:
			print "Line %d filtered"%i
			if(map.has_key(pair[0])):
				phrases=map[pair[0]]
				phrases.append(pair[1])
			else:
				map[pair[0]]=[pair[1]]
			count+=1
		else:
			print "Line %d discarded"%i
		i+=1
			
	print "%d phrases extracted"%count		

def main():
	phrases_map={}
	build_phrase_map(phrases_map)
	fPhrase=codecs.open("murphytalk_phrase.dat","wt","utf-8")
	fPhrase.write("MurphyTalk Phrase Table\nVer 0.03\n");

	fIndex=open("murphytalk_phrase_idx.txt","wt")
	fIndex.write("MurphyTalk Pinyin Phrase Index Table\nVer 0.03\n");

	
	keys=phrases_map.keys()
	
	freq=chr(0)+chr(0)+chr(0)+chr(0)
	keys.sort()
	for k in keys:
		fIndex.write(k);
		fIndex.write("\t%d\t"%len(phrases_map[k]));
		for phrase in phrases_map[k]:
			#record offset in phrase file into index file
			fIndex.write("%d "%fPhrase.tell())
			#write frequency into phrase file (uint32,4 bytes)
			#we don't use the codec but its underlying file stream here
			fPhrase.stream.write(freq)
			#write phrase text into phrase file
			fPhrase.write("%s "%phrase)
			
		fIndex.write("\n");

	fIndex.close()
	fPhrase.close()
	

main()

#
# $Log: winpy2phrase.py,v $
# Revision 1.3  2004/07/31 17:21:05  Lu Mu
# create phrase file in format V0.03
#
#
