all: trabalhocg
clean:
	rm trabalhocg
trabalhocg: *.cpp
	g++ -o trabalhocg *.cpp *.h -lGL -lGLU -lglut