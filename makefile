all: trabalhocg
clean:
	rm trabalhocg
trabalhocg: *.cpp
	g++ -o trabalhocg *.cpp -lGL -lGLU -lglut