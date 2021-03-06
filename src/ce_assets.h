#pragma once

#include <ce_math.h>

#include "rendering/vertex.h"
#include <string>
#include <vector>

namespace ce {
	struct File {
		std::string name = "";
	};
	struct ShaderFile : public File {
		std::string
			vertName = "",
			geomName = "",
			fragName = "",
			vertex = "",
			geometry = "",
			fragment = "";
	};
	struct TextureFile : public File {
		unsigned char* data = NULL;
		int
			width = 0,
			height = 0,
			channelCount = 0;
	};
	struct MaterialFile : public File {
		glm::vec4
			ambient = glm::vec4(0.0f),
			diffuse = glm::vec4(0.0f),
			speclular = glm::vec4(0.0f);
		std::string
			diffuseTex = "missing.png",
			specularTex = "missing.png",
			shader = NULL;
	};
	struct FacePart{
			unsigned index=0,uv=0,normal=0;
	};
	struct MeshFile : public File {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> uv;
		std::vector<glm::vec3> normals;
		std::vector<std::vector<FacePart>> faces;
	};
}
