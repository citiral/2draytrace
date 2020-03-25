
CFLAGS = -Iinclude/ -Wall -MMD
SOURCES = $(notdir $(wildcard src/*.cpp))
OUTPUT = build
OBJECTS = $(addprefix $(OUTPUT)/, $(patsubst %.cpp,%.o,$(SOURCES)))


.phony: all clean

all: $(OUTPUT) game.exe

clean:
	rmdir build -r

$(OUTPUT):
	mkdir $(OUTPUT)

game.exe: $(OBJECTS)
	g++ -o $(OUTPUT)/$@ $(OBJECTS) -Wall -Wextra -Llib -lmingw32 -lSDL2main  -lSDL2 -lopengl32 -lglew32

$(OUTPUT)/%.o: src/%.cpp
	g++ -o $@ -c $< $(CFLAGS)
	
-include $(OBJECTS:%.o=%.d)