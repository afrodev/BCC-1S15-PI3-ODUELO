TARGETS = exemplo

CC = gcc

FLAGS = -std=c99

LIBS = -lm -lopencv_core -lopencv_highgui -lallegro  -lallegro_color -lallegro_audio -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_dialog -lallegro_acodec

all: $(TARGETS)

duelolib.o: duelolib.c duelolib.h
	$(CC) $(FLAGS) $< -c

camera.o: camera.c camera.h
	$(CC) $(FLAGS) $< -c

exemplo: fluxo.c camera.o duelolib.o
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f *~ *.o

destroy: clean
	rm -f $(TARGETS)