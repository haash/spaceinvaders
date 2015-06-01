all: build

# ==========================
#  Directory specification.
# ==========================

COMPILER_FLAGS := -Wall -Wfatal-errors

CLASS_HFILES := $(wildcard c-*.h)
CLASS_OFILES = $(CLASS_HFILES:.h=.o)

SRC_CPPFILES := $(wildcard src-*.cpp)
SRC_OFILES = $(SRC_CPPFILES:.cpp=.o)

# ==========================
#  Compile rule for class
#  and source files.
# ==========================

%.o: %.cpp
	g++ $(COMPILER_FLAGS) -c $*.cpp -o $@

all_classes: $(CLASS_OFILES)

all_sources: $(SRC_OFILES)

build: all_classes all_sources
	g++ $(COMPILER_FLAGS) *.o -o bin-spaceinvaders -lncurses -pthread

# ==========================
#  Rules for laziness (can
#	 be removed).
# ==========================
ac: all_classes
as: all_sources
c: clean
cc: clean build

# ==========================
#  Cleaner.
# ==========================

clean:
	rm -f *.o
	rm -f bin-*
