CC  := gcc
CPP := g++
RC := windres
RM = rm -f

CXXFLAGS := -D_UNICODE -DUNICODE
CFLAGS := 
LIBS := -lole32

OBJS := IMEEngine.o IFELanguage.o
BIN := ife.exe

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean:
	${RM} *.o *.exe

$(BIN): $(OBJS)
	$(CPP) -municode $(OBJS) $(BIN_LIB) -o $(BIN) $(CXXFLAGS) $(LIBS)

%.o: %.cpp
	$(CPP) -c $< -o $@ $(CXXFLAGS)
