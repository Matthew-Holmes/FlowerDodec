#include "SurfacePentagon.h"
#include <iostream>

// length of one pentagon side if there is an inscribed unit sphere
// on a dodahedron
const GLfloat SurfacePentagon::A = 2.0f / (
	std::sqrt((5 / 2) + (11 / 10) * std::sqrt(5.0f))
	);
// same situation - distance from centre to a vertex
const GLfloat SurfacePentagon::R = std::sqrt(
	(5.0f + std::sqrt(5.0f)) / 10.0f) * SurfacePentagon::A;



SurfacePentagon::SurfacePentagon(GLuint meshd) : mesh_depth(meshd) {}

void SurfacePentagon::generate() {
	gen_seg();
	int szv = vertices.size();
	std::vector<GLuint> oldind = indices;
	GLfloat rot = glm::radians(72.0f);
	for (int i = 0; i < 4; i++) {
		// copy rotated version of the vertices
		for (int j = 0; j < szv / 3; j++) {
			GLfloat x = vertices[3 * j];
			GLfloat y = vertices[3 * j + 1];
			GLfloat z = vertices[3 * j + 2];
			GLfloat xrot = std::cos(rot) * x - std::sin(rot) * y;
			GLfloat yrot = std::sin(rot) * x + std::cos(rot) * y;
			vertices.push_back(xrot);
			vertices.push_back(yrot);
			vertices.push_back(z); // invariant to rotation
		}
		for (auto& k : oldind) {
			k += szv / 6;
		}
		indices.insert(indices.end(), oldind.begin(), oldind.end());
		rot += glm::radians(72.0f);
	}
}

void SurfacePentagon::gen_seg() {
	// generate the vertices and indices for the first segment
	// the first according to a clockwise traversal of a pentagon
	// split into 5 triangles radiating from its centre
	// where the pentagon will touch the inscribed sphere

	// fresh start
	vertices.clear();
	indices.clear();

	gen_seg_outline(); // creates one triangle

	for (int k = 0; k < mesh_depth; k++) {
		std::vector<GLuint>  tmpind = indices; // so can loop over the old
		// will then append our new indices

		auto it = indices.begin();
		while (it != indices.end()) {
			// loop over the old indices three at a time and build new mesh
			// three more vertices at the midpoints of the triangles edges
			// then four new triangles
			GLfloat topx = vertices[*it * 6];
			GLfloat topy = vertices[*it * 6 + 1];
			GLfloat rhsx = vertices[*(it + 1) * 6];
			GLfloat rhsy = vertices[*(it + 1) * 6 + 1];
			GLfloat lhsx = vertices[*(it + 2) * 6];
			GLfloat lhsy = vertices[*(it + 2) * 6 + 1];

			std::vector<GLfloat> xcoords { topx, rhsx, lhsx }; 
			std::vector<GLfloat> ycoords { topy, rhsy, lhsy };

			for (int i = 0; i < 3; i++) {
				GLfloat midx = xcoords[i % 3] / 2.0f + xcoords[(i + 1) % 3] / 2.0f;
				GLfloat midy = ycoords[i % 3] / 2.0f + ycoords[(i + 1) % 3] / 2.0f;
				GLfloat norm = std::sqrt(midx * midx + midy * midy + 1.0f);
				vertices.push_back(midx);
				vertices.push_back(midy);
				vertices.push_back(1.0f);
				vertices.push_back(midx / norm);
				vertices.push_back(midy / norm);
				vertices.push_back(1.0f / norm);
			}

			// vertices now stored in positions
			//  	     it
			//     end-1	end-3
			// it+2     end-2     it+1
			

			GLuint szv = vertices.size() / 6;
			// new indices for the four resultant triangles now

			std::vector<GLuint> four_triangles{
				*it,        szv - 3,   szv - 1,
				szv - 3,  *(it + 1),   szv - 2,
				szv - 1,    szv - 2,   *(it + 2),
				szv - 3,    szv - 2,   szv - 1     
			};

			tmpind.insert(tmpind.begin(), four_triangles.begin(), four_triangles.end());
			it += 3; // onto the next triangle
			}
		indices = tmpind; // done another mesh level so update indices
	}
}

void SurfacePentagon::gen_seg_outline() {
	// make sure we have a fresh start
	vertices.clear(); 
	indices.clear();
	// outline of segment
	// centre (of pentagon)
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(1.0f);
	// normalised
	vertices.push_back(0.0f);
	vertices.push_back(0.0f);
	vertices.push_back(1.0f);
	indices.push_back(0);
	// top
	vertices.push_back(0.0f);
	vertices.push_back(R);
	vertices.push_back(1.0f);
	// normalised
	GLfloat topnorm = std::sqrt(R * R + 1.0f);
	vertices.push_back(0.0f);
	vertices.push_back(R / topnorm);
	vertices.push_back(1.0f / topnorm);
	indices.push_back(1);
	// right
	vertices.push_back(R * std::sin(glm::radians(72.0f)));
	vertices.push_back(R * std::cos(glm::radians(72.0f)));
	vertices.push_back(1.0f);
	// normalised 
	GLfloat rightnorm = std::sqrt(R * std::sin(glm::radians(72.0f)) * R * std::sin(glm::radians(72.0f))
								+ R * std::cos(glm::radians(72.0f)) * R * std::cos(glm::radians(72.0f))
								+ 1.0f);
	vertices.push_back(R * std::sin(glm::radians(72.0f)) / rightnorm);
	vertices.push_back(R * std::cos(glm::radians(72.0f)) / rightnorm);
	vertices.push_back(1.0f / rightnorm);

	indices.push_back(2);
	return;
}
