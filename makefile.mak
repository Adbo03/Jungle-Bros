CC = gcc
CFLAGS = -Wall 
PROGRAMS = main
SDL2 = -L"SDL2-2.26.4/x86_64-w64-mingw32/lib" -I"SDL2-2.26.4/x86_64-w64-mingw32/include/SDL2"
SDL2_mixer = -L"SDL2_mixer-2.6.3/x86_64-w64-mingw32/lib" -I"SDL2_mixer-2.6.3/x86_64-w64-mingw32/include/SDL2" 
SDL2_ttf = -L"SDL2_ttf-2.20.2/x86_64-w64-mingw32/lib" -I"SDL2_ttf-2.20.2/x86_64-w64-mingw32/include/SDL2" 
all : $(PROGRAMS)

main : main.c game_settings.o mario.o obstacle.o ennemis.o
	$(CC) $(CFLAGS) $(SDL2) $(SDL2_ttf) $(SDL2_mixer) -o $@ $^ -lmingw32 -lSDL2main -lm -lSDL2_mixer -lSDL2_ttf -lSDL2 
	

game_settings.o : game_settings.c game_settings.h  
	$(CC) $(CFLAGS) -lm $(SDL2) $(SDL2_ttf) $(SDL2_mixer) -c $< 

mario.o : mario.c mario.h
	$(CC) $(CFLAGS) $(SDL2) $(SDL2_ttf) $(SDL2_mixer) -c $<

obstacle.o : obstacle.c obstacle.h
	$(CC) $(CFLAGS) $(SDL2) $(SDL2_ttf) $(SDL2_mixer) -c $<

ennemis.o : ennemis.c ennemis.h
	$(CC) $(CFLAGS) $(SDL2) $(SDL2_ttf) $(SDL2_mixer) -c $<

clean : 
	del *.o *.exe
