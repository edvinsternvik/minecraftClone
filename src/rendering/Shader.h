#pragma once
#include <string>
#include <map>

class Shader {
public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void useShader();
	void setUniformMatrix4f(const char* uniform, float* value);
	void setUniformMatrix4fv(const char* uniform, int count, float* value);
	void setUniform3fv(const char* uniform, int count, float* value);
	void setUniform2i(const char* uniform, int value1, int value2);
	void setUniform2f(const char* uniform, float value1, float value2);

	static Shader* const getCurrentShader() { return Shader::currentShader; }

private:
	unsigned int m_programId;
	std::map<const char*, int> uniformLocations;

	static Shader* currentShader;

private:
	std::string getShaderSource(const char* shaderPath);
	unsigned int createShader(const char* shaderPath, int shaderType);
	int getUniformLocation(const char* uniform);
};