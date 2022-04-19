#include "MeshModel.h"

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, std::vector<glm::vec2> texture_coords, CANONIAL_PROJECTION projection_type) :
	faces(faces),
	vertices(vertices),
	normals(normals)
{
	const auto index = model_name.find_last_of('.');
	if (index == std::string::npos)
	{
		this->model_name = model_name;
	}
	else
	{
		this->model_name = model_name.substr(0, index);
	}

	material = { glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0 };
	canonial_projection = { projection_type, 1.0f, 0.0f };

	auto scale_by = 100.0f / GetExtremeCoordinate();
	glm::mat4 scale = glm::scale(glm::vec3(scale_by, scale_by, scale_by));
	model_vertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		const auto& face = faces[i];
		for (int j = 0; j < 3; j++)
		{
			int v_index = face.GetVertexIndex(j) - 1;
			int t_index = face.GetTextureIndex(j) - 1;
			int n_index = face.GetNormalIndex(j) - 1;

			Vertex vertex{};
			auto temp = scale * glm::vec4(vertices[v_index], 1.0f);
			vertex.point = temp / temp.w;
			vertex.normal = normals[n_index];

			if (texture_coords.size() > 0)
			{
				vertex.texture_coords = texture_coords[t_index];
			}

			model_vertices.push_back(vertex);
		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, model_vertices.size() * sizeof(Vertex), &model_vertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

std::pair<glm::vec3, glm::vec3> MeshModel::GetMinAndMaxPoint()
{
	constexpr float min_value = std::numeric_limits<float>::min(), max_value = std::numeric_limits<float>::max();
	glm::vec3 min_point(max_value, max_value, max_value), max_point(min_value, min_value, min_value);

	for each (glm::vec3 vertex in vertices)
	{
		min_point.x = std::min(min_point.x, vertex.x);
		min_point.y = std::min(min_point.y, vertex.y);
		min_point.z = std::min(min_point.z, vertex.z);
		max_point.x = std::max(max_point.x, vertex.x);
		max_point.y = std::max(max_point.y, vertex.y);
		max_point.z = std::max(max_point.z, vertex.z);
	}

	return std::make_pair(min_point, max_point);
}

float MeshModel::GetExtremeCoordinate()
{
	float extreme = 0;
	for (const auto& v : vertices) 
	{
		auto abs = glm::abs(v);
		extreme = std::max(extreme, std::max(abs.x, std::max(abs.y, abs.z)));
	}
	return extreme;
}

std::vector<std::pair<glm::vec3, glm::vec3>> MeshModel::GetBoundingBox()
{
	std::pair<glm::vec3, glm::vec3> extremums = GetMinAndMaxPoint();
	glm::vec3 min_point = extremums.first, max_point = extremums.second;
	std::vector<std::pair<glm::vec3, glm::vec3>> bounding_box;

	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, min_point.y, min_point.z), glm::vec3(max_point.x, min_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, min_point.y, min_point.z), glm::vec3(min_point.x, max_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, min_point.y, min_point.z), glm::vec3(min_point.x, min_point.y, max_point.z)));

	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, max_point.y, min_point.z), glm::vec3(max_point.x, min_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, max_point.y, min_point.z), glm::vec3(min_point.x, max_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, max_point.y, min_point.z), glm::vec3(max_point.x, max_point.y, max_point.z)));

	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, min_point.y, max_point.z), glm::vec3(max_point.x, min_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, min_point.y, max_point.z), glm::vec3(max_point.x, max_point.y, max_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(max_point.x, min_point.y, max_point.z), glm::vec3(min_point.x, min_point.y, max_point.z)));

	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, max_point.y, max_point.z), glm::vec3(min_point.x, max_point.y, min_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, max_point.y, max_point.z), glm::vec3(min_point.x, min_point.y, max_point.z)));
	bounding_box.push_back(std::make_pair(glm::vec3(min_point.x, max_point.y, max_point.z), glm::vec3(max_point.x, max_point.y, max_point.z)));

	return bounding_box;
}

/// <summary>
/// Returns a vector -> [0]: o, [1]: x, [2]: y, [3]: z
/// </summary>
std::vector<glm::vec4> MeshModel::GetAxes()
{
	std::pair<glm::vec3, glm::vec3> extremums = GetMinAndMaxPoint();
	glm::vec3 min_point = extremums.first, max_point = extremums.second;
	std::vector<glm::vec4> axes;

	float avg_x = (min_point.x + max_point.x) / 2.0f;
	float avg_y = (min_point.y + max_point.y) / 2.0f;
	float avg_z = (min_point.z + max_point.z) / 2.0f;

	axes.push_back(glm::vec4(avg_x, avg_y, avg_z, 1.0f));
	axes.push_back(glm::vec4(max_point.x, avg_y, avg_z, 1.0f)); // x axis
	axes.push_back(glm::vec4(avg_x, max_point.y, avg_z, 1.0f)); // y axis
	axes.push_back(glm::vec4(avg_x, avg_y, max_point.z, 1.0f)); // z axis

	return axes;
}

void MeshModel::SetMaterial(const std::string name, const Material& material)
{
	material_name = name;
	this->material.ambient = material.ambient;
	this->material.diffuse = material.diffuse;
	this->material.specular = material.specular;
	this->material.shininess = material.shininess;
}