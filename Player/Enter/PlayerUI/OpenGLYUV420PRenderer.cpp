#include "OpenGLYUV420PRenderer.h"
#include "GL/glew.h"
#include <iostream>
#include "Base/StringPiece.h"
#include <fstream>
#include <sstream>

using namespace balsampear;
using namespace std;
namespace balsampear
{

	OpenGLYUV420PRender::OpenGLYUV420PRender()
		:OpenGLRenderer()
	{

	}

	OpenGLYUV420PRender::~OpenGLYUV420PRender()
	{

	}

	void OpenGLYUV420PRender::createTexture()
	{
		glGenTextures(3, textureYUV);
		for (int i = 0; i < 3; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, textureYUV[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
	}

	StringPiece vert = "..\\shader\\yuv420p.vert";
	StringPiece frag = "..\\shader\\yuv420p.frag";
	void OpenGLYUV420PRender::createSahder()
	{
		StringPiece vertexCode;
		StringPiece fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile;
		// ��֤ifstream��������׳��쳣��
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// ���ļ�
			vShaderFile.open(vert);
			fShaderFile.open(frag);
			std::stringstream vertShaderStream, fragShaderStream;
			// ��ȡ�ļ��Ļ������ݵ���������
			vertShaderStream << vShaderFile.rdbuf();
			fragShaderStream << fShaderFile.rdbuf();
			// �ر��ļ�������
			vShaderFile.close();
			fShaderFile.close();
			// ת����������string
			vertexCode = vertShaderStream.str();
			fragmentCode = fragShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//���붥����ɫ��
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, NULL);
		glCompileShader(vertexShader);
		int success = 0;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//����Ƭ����ɫ��
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shader_ = glCreateProgram();
		glAttachShader(shader_, vertexShader);
		glAttachShader(shader_, fragmentShader);
		glLinkProgram(shader_);
		// ��ӡ���Ӵ�������еĻ���
		glGetProgramiv(shader_, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader_, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void OpenGLYUV420PRender::useShader()
	{
		glUseProgram(shader_);
	}

	void OpenGLYUV420PRender::fillData(const Byte* data, int width, int height)
	{
		size_t sizeY = (size_t)width * height;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureYUV[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureYUV[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, data + sizeY);
		glGenerateMipmap(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureYUV[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, data + sizeY * 5 / 4);
		glGenerateMipmap(GL_TEXTURE_2D);

		glUniform1i(glGetUniformLocation(shader_, "textureY"), 0);
		glUniform1i(glGetUniformLocation(shader_, "textureU"), 1);
		glUniform1i(glGetUniformLocation(shader_, "textureV"), 2);
	}

}