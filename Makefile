BREWPATH = $(shell brew --prefix)

CC = g++
CFLAGS = -g -std=c++11 -Wno-deprecated-register -Wno-deprecated-declarations -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
INCFLAGS = -I./include -I$(BREWPATH)/include
LDFLAGS = -framework GLUT -framework OpenGL -L$(BREWPATH)/lib -lfreeimage

RM = /bin/rm -f
all: SceneViewer
SceneViewer: main.o Shader.o Camera.o Obj.o Scene.o shaders/lighting.frag shaders/projective.vert
	$(CC) -o SceneViewer main.o Shader.o Camera.o Obj.o Scene.o $(LDFLAGS)
main.o: main.cpp include/hw3AutoScreenshots.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp 
Shader.o: src/Shader.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Shader.cpp
Camera.o: src/Camera.cpp include/Camera.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Camera.cpp
Obj.o: src/Obj.cpp include/Obj.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Obj.cpp
Scene.o: src/Scene.cpp src/Scene.inl include/Scene.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Scene.cpp
clean: 
	$(RM) *.o SceneViewer

 
