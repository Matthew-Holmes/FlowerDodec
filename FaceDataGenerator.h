#ifndef FACEDATAGENERATOR_CLASS_H
#define FACEDATAGENERATOR_CLASS_H

#include<glad/glad.h>
#include <vector>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "SurfacePentagon.h"
#include "SurfaceSquare.h"
#include "FaceData.h"

class FaceDataGenerator {
public:
	std::vector<FaceData> genCubeData();
	std::vector<FaceData> genDodecData();
	std::vector<FaceData> genSimpleData();
};


#endif
