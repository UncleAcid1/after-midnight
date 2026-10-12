
OBJECTS = ScrollingDemo.o

SPRITEWORLD= ../..
INCDIR= $(SPRITEWORLD)/Headers
UTILDIR= $(SPRITEWORLD)/Utils
LIBDIR= $(SPRITEWORLD)/Libraries

OS = $(shell uname)
ifeq ("$(OS)","Darwin")

GL_CFLAGS = -DHAVE_OPENGL
GL_LIBS = -framework OpenGL
SDL_CFLAGS = -I/Library/Frameworks/SDL.framework/Headers
SDL_LIBS = -framework SDL -framework Cocoa -lSDLmain
SDLIMAGE_CFLAGS = -DHAVE_SDL_IMAGE
SDLIMAGE_LIBS = -framework SDL_image

else

SDL_PREFIX = /usr/local
SDL_CONFIG = sdl-config

GL_CFLAGS = -DHAVE_OPENGL
GL_LIBS = -L/usr/X11R6/lib -lGL
SDL_CFLAGS = -I$(SDL_PREFIX)/include/SDL $(shell $(SDL_CONFIG) --cflags)
SDL_LIBS = -L$(SDL_PREFIX)/lib -lSDL -lpthread $(shell $(SDL_CONFIG) --libs)
SDLIMAGE_CFLAGS = -DHAVE_SDL_IMAGE
SDLIMAGE_LIBS = -L/usr/local/lib -lSDL_image

endif

SWX_CFLAGS = -I$(INCDIR) -I$(UTILDIR)
SWX_LIBS = -L$(LIBDIR) -lSpriteWorldX

CC = gcc
CFLAGS= $(GL_CFLAGS) $(SDL_CFLAGS) $(SDLIMAGE_CFLAGS) $(SWX_CFLAGS)
LDFLAGS= $(GL_LIBS) $(SDL_LIBS) $(SDLIMAGE_LIBS) $(SWX_LIBS) 


VPATH= $(INCDIR):./

all: scrollingdemo

scrollingdemo : $(OBJECTS)
	$(CC) -o scrollingdemo $(OBJECTS) $(LDFLAGS)

.PHONY: dist clean

dist:
	tar czf ScrollingDemo.tar.gz -C .. ScrollingDemo \
	        --exclude ScrollingDemo.tar.gz --exclude CVS --exclude .DS_Store 

clean:
	rm -f scrollingdemo
	rm -f *.o

