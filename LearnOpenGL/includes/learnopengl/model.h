#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <learnopengl/shader_s.h>
#include <learnopengl/mesh.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory);

class Model {
public:
	Model(char *path)
	{
		loadModel(path);
	}
	// draws the model, and thus all its meshes
	void Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); ++i)
		{
			meshes[i].Draw(shader);
		}
	}
private:
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;

	void loadModel(string path)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}
	// 递归的处理每个节点
	void processNode(aiNode *node, const aiScene *scene)
	{
		// 处理节点所有的网格（如果有的话）
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	Mesh processMesh(aiMesh *mesh, const aiScene *scene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		// 处理顶点位置、法线和纹理坐标
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex vertex;
			glm::vec3 temp;
			// positions
			temp.x = mesh->mVertices[i].x;
			temp.y = mesh->mVertices[i].y;
			temp.z = mesh->mVertices[i].z;
			vertex.Position = temp;
			// normals
			temp.x = mesh->mNormals[i].x;
			temp.y = mesh->mNormals[i].y;
			temp.z = mesh->mNormals[i].z;
			vertex.Normal = temp;
			// texture coordinates
			// Assimp允许一个模型在一个顶点上有最多8个不同的纹理坐标，我们只关心第一组纹理坐标
			if (mesh->mTextureCoords[0])	// 网格是否有纹理坐标
			{
				glm::vec2 temp;
				temp.x = mesh->mTextureCoords[0][i].x;
				temp.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = temp;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			// tangent
			temp.x = mesh->mTangents[i].x;
			temp.y = mesh->mTangents[i].y;
			temp.z = mesh->mTangents[i].z;
			vertex.Tangent = temp;
			// bitangent
			temp.x = mesh->mBitangents[i].x;
			temp.y = mesh->mBitangents[i].y;
			temp.z = mesh->mBitangents[i].z;
			vertex.Bitangent = temp;
			vertices.push_back(vertex);
		}
		// 处理索引
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
		// 处理材质
		if (mesh->mMaterialIndex >= 0)
		{
			// 一个网格只包含了一个指向材质对象的索引
			aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// diffuse: texture_diffuseN
			// specular: texture_specularN
			// normal: texture_normalN

			// 1. diffuse maps
			vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			// 3. normal maps
			std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			// 4. height maps
			std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}
		return Mesh(vertices, indices, textures);
	}
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
		{
			aiString str;
			mat->GetTexture(type, i, &str);	// 获取纹理的文件位置（假设了模型文件中纹理文件的路径是相对于模型文件的本地(Local)路径）
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); ++j)
			{
				if (textures_loaded[j].path == str.C_Str())
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			// 如果纹理没有被加载，则加载
			if (!skip)
			{
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
			}
		}
		return textures;
	}
};

unsigned int TextureFromFile(const char *path, const string &directory)
{
	string filename = path;
	filename = directory + '/' + filename;	// 拼接得到完整的纹理路径

	unsigned int textureID;
	glGenTextures(1, &textureID);
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//std::cout << "Image Info: " << std::endl;
		//std::cout << "width: " << width << ", " << "height: " << height << std::endl;
		//std::cout << "nrChannels: " << nrChannels << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filename.c_str() << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}
