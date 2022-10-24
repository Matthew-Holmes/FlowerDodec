#include "FaceDataGenerator.h"

std::vector<FaceData> FaceDataGenerator::genDodecData() {

	GLfloat angle = glm::radians(180.f) - 2.0f * std::atan( (1.0f + std::sqrt(5.0f)) / 2.0f);
	// angle we rotate at the origin to ensure that the dihedral angle is correct
	// use total interior angles of a quadrilateral to compute
	std::vector<FaceData> dodecFaceData;
	// top face
	dodecFaceData.emplace_back(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
	// five faces attached to each edge
	dodecFaceData.emplace_back(
		  glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::vec3(1.0f, 0.0f, 0.0f)); 
	dodecFaceData.emplace_back(
		glm::rotate(glm::mat4(1.0f), glm::radians(72.0f), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::vec3(1.0f, 0.0f, 0.0f)); 
	dodecFaceData.emplace_back(
		glm::rotate(glm::mat4(1.0f), glm::radians(72.0f * 2.0f), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::vec3(1.0f, 0.0f, 0.0f)); 
	dodecFaceData.emplace_back(
		glm::rotate(glm::mat4(1.0f), glm::radians(72.0f * 3.0f), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::vec3(1.0f, 0.0f, 0.0f)); 
	dodecFaceData.emplace_back(
		glm::rotate(glm::mat4(1.0f), glm::radians(72.0f * 4.0f), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::vec3(1.0f, 0.0f, 0.0f));

	// now reflect and rotate to get the other half
	std::vector<FaceData> tmp = dodecFaceData;
	for (FaceData dat : tmp) {
		dat.orientation = glm::rotate(glm::mat4(1.0f), glm::radians(72.0f * 2.0f), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(1.0f, 0.0f, 0.0f))
			* dat.orientation;
		dodecFaceData.push_back(dat);
	}

	std::vector<glm::vec3> cols{
		glm::vec3(1.0f, 0.0f, 0.f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(0.5f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 0.5f)
	};

	std::vector<glm::vec3> cols2, cols3;
	for (glm::vec3 col : cols) {
		cols2.push_back(glm::vec3(col.z, col.x, col.y));
		cols3.push_back(glm::vec3(col.y, col.z, col.x));
	}

	
	cols.insert(cols.end(), cols2.begin(), cols2.end());
	cols.insert(cols.end(), cols3.begin(), cols3.end());
	

	auto it = cols.begin();

	for (auto &dat : dodecFaceData) {
		dat.color = *it;
		it++;
	}

	// alternative color method
	// too pastel and monochromatice
	// consider (r, g, b) values to lie on the surface of a sphere so asspociate faces
	// with the color in color space
	//GLfloat reg = 100.0f; // how much to cubify our color sphere
	//// make sure is even

	//for (FaceData& dat : dodecFaceData) {
	//	dat.color = glm::vec3( dat.orientation * glm::vec4(dat.color, 1.0f) );
	//	dat.color *= 1.0f / glm::pow(
	//		glm::pow(dat.color.x, reg)
	//		+ glm::pow(dat.color.y, reg)
	//		+ glm::pow(dat.color.z, reg)
	//		, 1.0f / reg); // now project onto cube
	//	dat.color *= 0.5f; // radius 0.5
	//	dat.color += glm::vec3(0.5f, 0.5f, 0.5f); // centre in middle of color space

	//}

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
