#include <vector>
#include <glm/glm.hpp>

#ifndef __IMAGE_H__
#define __IMAGE_H__

class Image {
public:
	int width;
	int height;
	std::vector<glm::vec3> pixels; // RGB colors

	Image(const int w, const int h) {
		pixels = std::vector<glm::vec3>(w * h, glm::vec3(0.0f, 0.0f, 0.0f));
		width = w;
		height = h;
	}

	void init();
	void draw();

private:
	unsigned int fbo; // frame buffer object
	unsigned int tbo; // texture buffer object
};

#endif