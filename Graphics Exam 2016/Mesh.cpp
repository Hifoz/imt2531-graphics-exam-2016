#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "./libraries/tiny_obj_loader.h"

Mesh::Mesh()
{
}

Mesh::Mesh(std::string path, bool wantUVs)
{
	ModelData model =  loadModelData(path, "./resources/models/", wantUVs);
	createBufferObjects(model);
}


Mesh::~Mesh()
{
}


void Mesh::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


GLuint Mesh::getVAO()
{
	return VAO;
}

std::vector<GLuint> Mesh::getVBOs()
{
	std::vector<GLuint> VBOs = {positionBuffer, colourBuffer, normalBuffer};
	return VBOs;
}

int Mesh::getDrawCount()
{
	return drawCount;
}



/**
 * Creates and stores the needed VAO, VBO(s) and IBO for rendering the model
 * @param model A struct containing the needed data loaded from file.
 */
void Mesh::createBufferObjects(ModelData& model)
{
	drawCount = model.numberOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	// Position
	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, model.vertexPosition.size() * sizeof(GLfloat), &model.vertexPosition[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Colour
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, model.vertexColour.size() * sizeof(GLfloat), &model.vertexColour[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Normal
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, model.vertexNormal.size() * sizeof(GLfloat), &model.vertexNormal[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	// TexCoords
	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, model.vertexTexCoord.size() * sizeof(GLfloat), &model.vertexTexCoord[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/**
 * Loads model data from obj and mtl files
 * @param  modelPath   path and name for the .obj file. F.eks: "./resources/models/car.obj"
 * @param  materialDir path to the directory containing the mtl file. F.eks: "./resources/models/"
 * @return             A ModelData struct. It is used to pass the information to the createBufferObject function.
 */
ModelData Mesh::loadModelData(std::string modelPath, std::string materialDir, bool wantUVs)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	ModelData model;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, modelPath.c_str(), materialDir.c_str())) {
		throw std::runtime_error(err);
	}

	if(!err.empty()) {
		printf("%s\n", err.c_str());
	}

	int index = 0;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygons)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				/*
				 * You can access the model data in the following ways.
				 * Position
				 * 	attrib.vertices[model.vertexOrder*idx.vertex_index+0] gives the x coordinate. +1 and +2 gives y and z.
				 * Normal
				 * 	attrib.normals[model.normalOrder*idx.normal_index+0] same as position
				 * Color
				 * 	materials[shapes[s].mesh.material_ids[f]].diffuse[0] gets the R component of the diffuse color data. +1 and +2 gets the GB components
				 * Index
				 * 	You can just increment the value. You are free to optimize by cheking if the current vertex has already been loaded.
				 *
				 * For more information see tiny_obj_loader.h and https://github.com/syoyo/tinyobjloader
				 */
				for (auto i = 0; i < 3; i++)
				{
					model.vertexPosition.push_back({attrib.vertices[model.vertexOrder*idx.vertex_index + i]});
					model.vertexColour.push_back({materials[shapes[s].mesh.material_ids[f]].diffuse[i]});
					model.vertexNormal.push_back({attrib.normals[model.vertexOrder*idx.normal_index + i]});
				}

				if (wantUVs)
				{
					model.vertexTexCoord.push_back({ attrib.texcoords[model.uvOrder*idx.texcoord_index] });
					model.vertexTexCoord.push_back({ attrib.texcoords[model.uvOrder*idx.texcoord_index + 1] });
				}
				else
				{
					model.vertexTexCoord.push_back({ 0 });
					model.vertexTexCoord.push_back({ 0 });
				}

				model.indices.push_back(index++);
			}

			index_offset += fv;
		}
		model.numberOfIndices = index;
	}

	return model;
}
