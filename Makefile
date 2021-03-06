#############################################################################
# Makefile for building libmurphypinyin.so.0.0.1
# Generated by tmake at 18:41, 2004/07/10
#     Project: MurphyPinyin
#    Template: lib
#############################################################################

####### Compiler, tools and options

CC	=	arm-linux-gcc
CXX	=	arm-linux-g++
CFLAGS	=	-pipe -Wall -W -O2 -fPIC -DNO_DEBUG
CXXFLAGS=	-pipe -DQT_QWS_SL5XXX -DQT_QWS_CUSTOM -DQWS -fno-exceptions -fno-rtti -Wall -W -O2 -fPIC -DNO_DEBUG
INCPATH	=	-I$(QPEDIR)/include -I$(QTDIR)/include
LINK	=	arm-linux-g++
LFLAGS	=	-shared -Wl,-soname,libmurphypinyin.so.0
LIBS	=	$(SUBLIBS) -L$(QTDIR)/lib -lqpe -lqte
AR	=	arm-linux-ar cqs
RANLIB	=	
MOC	=	$(QTDIR)/bin/moc
UIC	=	$(QTDIR)/bin/uic

TAR	=	tar -cf
GZIP	=	gzip -9f

####### Files

HEADERS =	PinyinFrame.h \
		PinyinImpl.h \
		PinyinEngine.h \
		scim/scim_pinyin.h \
		phrase/PinyinPhrase.h \
		public.h \
		Config.h
SOURCES =	PinyinFrame.cpp \
		PinyinImpl.cpp \
		PinyinEngine.cpp \
		scim/scim_pinyin.cpp \
		phrase/PinyinPhrase.cpp \
		public.cpp \
		Config.cpp
OBJECTS =	PinyinFrame.o \
		PinyinImpl.o \
		PinyinEngine.o \
		scim/scim_pinyin.o \
		phrase/PinyinPhrase.o \
		public.o \
		Config.o
INTERFACES =	
UICDECLS =	
UICIMPLS =	
SRCMOC	=	moc_PinyinFrame.cpp
OBJMOC	=	moc_PinyinFrame.o
DIST	=	
TARGET	=	libmurphypinyin.so.0.0.2
TARGETA	=	DIST/libmurphypinyin.a
TARGETD	=	libmurphypinyin.so.0.0.2
TARGET0	=	libmurphypinyin.so
TARGET1	=	libmurphypinyin.so.0
TARGET2	=	libmurphypinyin.so.0.0
INTERFACE_DECL_PATH = .

####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

####### Build rules


all: DIST/libmurphypinyin.so.0.0.2

DIST/libmurphypinyin.so.0.0.2: $(OBJECTS) $(OBJMOC) $(SUBLIBS) 
	rm -fr DIST
	mkdir DIST
	-rm -f $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGETD) $(OBJECTS) $(OBJMOC) $(LIBS)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)
	-rm -f DIST/$(TARGET)
	-rm -f DIST/$(TARGET0)
	-rm -f DIST/$(TARGET1)
	-rm -f DIST/$(TARGET2)
	-mv $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2) DIST/
	(cd DIST;rm -fr *.tar;$(TAR) murphypinyin.tar *;rm -f *so*)

staticlib: $(TARGETA)

$(TARGETA): $(UICDECLS) $(OBJECTS) $(OBJMOC) 
	-rm -f $(TARGETA)
	$(AR) $(TARGETA) $(OBJECTS) $(OBJMOC)

moc: $(SRCMOC)

tmake: Makefile

Makefile: MurphyPinyin.pro
	tmake MurphyPinyin.pro -o Makefile

dist:
	$(TAR) MurphyPinyin.tar MurphyPinyin.pro $(SOURCES) $(HEADERS) $(INTERFACES) $(DIST)
	$(GZIP) MurphyPinyin.tar

clean:
	-rm -f $(OBJECTS) $(OBJMOC) $(SRCMOC) $(UICIMPLS) $(UICDECLS) $(TARGET)
	-rm -f $(TARGET0) $(TARGET1) $(TARGET2) $(TARGETA)
	-rm -f *~ core

####### Sub-libraries


###### Combined headers


####### Compile

PinyinFrame.o: PinyinFrame.cpp \
		PinyinFrame.h \
		PinyinEngine.h \
		scim/scim_pinyin.h

PinyinImpl.o: PinyinImpl.cpp \
		PinyinFrame.h \
		PinyinEngine.h \
		scim/scim_pinyin.h \
		PinyinImpl.h

PinyinEngine.o: PinyinEngine.cpp \
		PinyinEngine.h \
		scim/scim_pinyin.h

public.o: public.cpp \
	  public.h

Config.o: Config.cpp \
	  Config.h

scim/scim_pinyin.o: scim/scim_pinyin.cpp \
		scim/scim_pinyin.h

phrase/PinyinPhrase.o: phrase/PinyinPhrase.cpp \
		phrase/PinyinPhrase.h \
		scim/scim_pinyin.h

moc_PinyinFrame.o: moc_PinyinFrame.cpp \
		PinyinFrame.h \
		PinyinEngine.h \
		scim/scim_pinyin.h

moc_PinyinFrame.cpp: PinyinFrame.h
	$(MOC) PinyinFrame.h -o moc_PinyinFrame.cpp

