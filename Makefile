CC	:= clang++
CFLAGS	:= -std=c++11 -static
BIN	:= bin/
TARGET	:= $(BIN)/density
LIBPED	:= lib/libpedsim.a
LIB	:= -L./lib -lpedsim
INC	:= -I./include/ -I./src/pedsim
MAKE	:= make
SRCF	:= src/*.cpp


$(TARGET): $(SRCF) $(LIBPED)
	mkdir -p $(BIN)
	$(CC) $(SRCF) $(CFLAGS) $(LIB) $(INC) -o $(TARGET)

$(LIBPED): src/pedsim/*.cpp
	mkdir -p lib/
	$(MAKE) -C src/pedsim/

clean:
	$(RM) -r $(BIN) $(LIBPED)

.PHONY: clean
