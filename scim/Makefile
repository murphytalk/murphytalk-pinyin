###################################### 
# 
# Generic makefile 

TARGET:= test
LIBS := 

ifdef DEBUG
	CC_DEBUG	= -g -DMC_DEBUG -DOSAL_DEBUG -DHEAP_DEBUG -DDEBUG
else
	CC_DEBUG	= -O2
endif

ifdef PROF
	CC_PROF		= -pg
	LDOPTS 		=
else
	CC_PROF		=
	LDOPTS		=
endif
CFLAGS	= -Wall $(CC_DEBUG) $(CC_PROF) $(CC_DEFS) $(CC_INCDIR)
CXX = g++


# LD variables
#
LD		= ld
INCLD		= $(QTDIR)/include
LIBDIRS		= $(QTDIR)/lib
LIBS		= m qte qpe qtopia
LDFLAGS 	= $(LDOPTS) $(LIBDIRS) $(LIBS)


SOURCE := main.cpp scim_pinyin.cpp ../PinyinEngine.cpp

OBJS	= $(patsubst %.cpp,%.o,$(SOURCE))

DEPS := $(patsubst %.o,%.d,$(OBJS)) 


CPPFLAGS = $(CC_DEBUG) -MD -Wall -fno-exceptions -fno-rtti -I$(INCLD) -L$(LIBDIRS)

all : $(TARGET) 
	$(CXX)  $(CPPFLAGS) -o $(TARGET) $(OBJS) $(addprefix -l,$(LIBS)) $(LIBFLAGS)	

$(TARGET):$(OBJS)

deps : $(DEPS) 
clean : cleanall 
	rm *.o *.d ~* -fr 
cleanall: 
	rm  $(TARGET) *~ -fr

rebuild: cleanall all 

%.o:%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@ 

