
SPRITEWORLD= ../..
INCDIR= $(SPRITEWORLD)/Headers
UTILDIR= $(SPRITEWORLD)/Utils
BLITKERNEL= $(SPRITEWORLD)/BlitKernel
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

SWX_CFLAGS = -I$(INCDIR)
SWX_CFLAGS += -I$(UTILDIR) -I$(BLITKERNEL)
SWX_LIBS = -L$(LIBDIR) -lSpriteWorldX

CC= gcc
CXX= g++
CFLAGS= $(GL_CFLAGS) $(SDL_CFLAGS) $(SDLIMAGE_CFLAGS) $(SWX_CFLAGS)
CXXFLAGS= $(CFLAGS)
LDFLAGS= $(GL_LIBS) $(SDL_LIBS) $(SDLIMAGE_LIBS) $(SWX_LIBS) 


VPATH= $(INCDIR):./

all: simple

simple : Simple.o
	$(CC) -o $@ $^ $(LDFLAGS)

classsimple : ClassSimple.o
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: dist clean

dist:
	tar czf Simple.tar.gz -C .. Simple \
	        --exclude Simple.tar.gz --exclude CVS --exclude .DS_Store 

clean:
	rm -f simple
	rm -f *.o

