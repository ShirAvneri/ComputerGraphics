#include "Utils.h"

#include <iomanip>
glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texture_coords;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			texture_coords.push_back(Utils::Vec2fFromStream(issLine));
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	/* Printing mesh to console:
	
	std::cout << std::fixed << std::setprecision(5);
	std::cout << "\n----------" << " Vertices " << "(" << vertices.size() << ") ----------" << std::endl;
	for each (glm::vec3 v in vertices)
	{
		std::cout << std::setw(8) << v.x << std::setw(15) << v.y << std::setw(15) << v.z << std::endl;
	}
	std::cout << "\n----------" << " Faces " << "(" << faces.size() << ") ----------" << std::endl;
	for each (Face f in faces)
	{
		std::cout << std::setw(6) << "[v1: " << f.GetVertexIndex(0) << ", vt1: " << f.GetTextureIndex(0) << ", vn1: " << f.GetNormalIndex(0) << "]\t\t";
		std::cout << std::setw(6) << "[v2: " << f.GetVertexIndex(1) << ", vt2: " << f.GetTextureIndex(1) << ", vn2: " << f.GetNormalIndex(1) << "]\t\t";
		std::cout << std::setw(6) << "[v3: " << f.GetVertexIndex(2) << ", vt3: " << f.GetTextureIndex(2) << ", vn3: " << f.GetNormalIndex(2) << "]" << std::endl;
	}
	*/

	return std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath), texture_coords);
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}