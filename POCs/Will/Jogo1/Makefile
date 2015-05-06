TARGETS = exemplo

CC = gcc

FLAGS = -std=c99

LIBS = -lm -lopencv_core -lopencv_highgui -lallegro  -lallegro_color -lallegro_audio -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_dialog -lallegro_acodec

all: $(TARGETS)

camera.o: camera.c camera.h
	$(CC) $(FLAGS) $< -c

exemplo: testeBiblio.c camera.o
	$(CC) $(FLAGS) $^ -o $@ $(LIBS)
	
clean:
	rm -f *~ *.o

destroy: clean
	rm -f $(TARGETS)