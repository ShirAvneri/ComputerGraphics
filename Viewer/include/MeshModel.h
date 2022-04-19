#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "TransformableObject.h"
#include "Face.h"

class MeshModel : public TransformableObject
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, std::vector<glm::vec2> texture_coords, CANONIAL_PROJECTION projection_type = CANONIAL_PROJECTION::NONE);
	virtual ~MeshModel();

	const std::string& GetModelName() const { return model_name; }

	const Face& GetFace(const size_t index) const { return faces[index]; }
	std::vector<Face>& GetFaces() { return faces; }
	size_t GetFacesCount() const { return faces.size(); }

	glm::vec3& GetVertex(const size_t index) { return vertices[index]; }
	std::vector<glm::vec3>& GetVertices() { return vertices; }
	size_t GetVerticesCount() const { return vertices.size(); }
	size_t GetModelVerticesCount() const { return model_vertices.size(); }

	bool NormalExists(int index) { return index > 0 && index < normals.size(); }
	glm::vec3& GetNormal(const size_t index) { return normals[index]; }
	std::vector<glm::vec3>& GetNormals() { return normals; }
	size_t GetNormalsCount() const { return normals.size(); }

	std::vector<std::pair<glm::vec3, glm::vec3>> GetBoundingBox();
	std::vector<glm::vec4> GetAxes();
	std::vector<glm::vec3>& GetTrianglesColors() { return triangles_colors; }

	void SetMaterial(const std::string name, const Material& material);
	std::string GetMaterialName() { return material_name; }
	void ClearMaterialName() { material_name.clear(); }

	GLuint GetVAO() const { return vao; }

	Material material;
	CanonialProjection canonial_projection;


private:
	std::string model_name;
	std::string material_name;
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> triangles_colors;

	GLuint vbo;
	GLuint vao;
	std::vector<Vertex> model_vertices;
	std::vector<glm::vec3> texture_coords;

	std::pair<glm::vec3, glm::vec3> GetMinAndMaxPoint();
	float GetExtremeCoordinate();
};
