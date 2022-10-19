#ifndef FACEDATA_CLASS_H
#define FACEDATA_CLASS_H

#include<glad/glad.h>
#include <glm/glm.hpp>

class FaceData {
public:
	FaceData(glm::mat4 ort, glm::vec3 col) : orientation(ort), color(col) {};
	glm::mat4 orientation;
	glm::vec3 color;
};

#endif // !FACEDATA_CLASS_H
