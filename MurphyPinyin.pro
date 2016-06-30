TEMPLATE	=	lib
CONFIG		+= 	qt warn_on release
HEADERS		=   	PinyinFrame.h PinyinImpl.h  PinyinEngine.h scim/scim_pinyin.h
SOURCES		=   	PinyinFrame.cpp PinyinImpl.cpp  PinyinEngine.cpp scim/scim_pinyin.cpp
TARGET		= 	murphypinyin
DESTDIR		= 	DIST
INCLUDEPATH 	+= 	$(QPEDIR)/include
DEPENDPATH      += 	../$(QPEDIR)/include ../../taskbar
LIBS            += 	-lqpe
VERSION		= 	0.0.1

#TRANSLATIONS += ../../i18n/de/libqpickboard.ts
