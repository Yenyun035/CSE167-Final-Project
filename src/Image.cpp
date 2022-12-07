#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Image.h"

#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>

void Image::init() {
    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &tbo);
}

void Image::draw() {
    glBindTexture(GL_TEXTURE_2D, tbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_FLOAT, &pixels[0][0]); // load texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

    // attach texture and the read frame
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tbo, 0); 
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // if not already so
    // copy framebuffer from read to write
    glBlitFramebuffer(0,0,width,height,0,0,width,height,GL_COLOR_BUFFER_BIT, GL_NEAREST); 
}