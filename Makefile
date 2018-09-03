#Name: Praveen Balireddy
#Roll: 2018201052

PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

OBJS =  termzilla.o Terminal.o pageManager.o page.o file.o path.o CommandParser.o CommandHandler.o error.o
CFLAGS = -I.
LDIR = -L.
CFLAGS += -g
#ifeq ($(BUILD_MODE),debug)
#	CFLAGS += -g
##	CFLAGS += -O2
#else
#	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
#endif

all: termzilla

termzilla:	$(OBJS)
	$(CXX) -o $@ $^

%.o:	$(PROJECT_ROOT)%.cpp $(PROJECT_ROOT)%.h
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS)  $(LDIR) -o $@ $< 

%.o:	$(PROJECT_ROOT)%.c $(PROJECT_ROOT)%.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm termzilla $(OBJS)
