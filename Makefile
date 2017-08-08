CXX=g++
CPPFLAGS=-static -m32 -Wl,-subsystem,windows -Wall -Wextra -O2
LDLIBS=-lgdi32 -lversion -lpsapi
RM=rm

SRCS=src/main.cpp src/memory.cpp src/window.cpp src/wndproc.cpp src/drawing.cpp src/logic.cpp src/utils.cpp
OBJS=$(subst src/,obj/,$(subst .cpp,.o,$(SRCS)))

all: a

obj/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $(subst src/,obj/,$(subst .cpp,.o,$<))

a: $(OBJS)
	$(CXX) $(CPPFLAGS) -o bin/PokeMemViewer.exe $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) bin/PokeMemViewer.exe