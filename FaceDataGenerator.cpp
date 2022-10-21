#include "FaceDataGenerator.h"

std::vector<FaceData> FaceDataGenerator::genDodecData() {

	GLfloat angle = std::atan((1.0f + std::sqrt(5.0f) / 2.0f));
	std::vector<FaceData> dodecFaceData;
	dodecFaceData.emplace_back(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	dodecFaceData.emplace_back(
		  glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		  glm::vec3(0.0f, 1.0f, 0.0f));

	return dodecFaceData;
}

std::vector<FaceData> FaceDataGenerator::genCubeData() {
	std::vector<FaceData> cubeFaceData;
	cubeFaceData.emplace_back(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	cubeFaceData.emplace_back(
		glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::vec3(0.0f, 1.0f, 0.0f));
	cubeFaceData.emplace_back(
		glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::vec3(1.0f, 0.0f, 1.0f));
	cubeFaceData.emplace_back(
		glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
		glm::vec3(0.0f, 0.0f, 1.0f));
	cubeFaceData.emplace_back(
		glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		glm::vec3(0.0f, 1.0f, 1.0f));
	cubeFaceData.emplace_back(
		glm::rotate(glm::mat4(1.0f), glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		glm::vec3(1.0f, 1.0f, 0.0f));
	return cubeFaceData;
}

std::vector<FaceData> FaceDataGenerator::genSimpleData() { 
	std::vector<FaceData> simpleFaceData;
	simpleFaceData.emplace_back(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	return simpleFaceData;
}
