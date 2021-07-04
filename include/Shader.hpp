#ifndef SHADER_H
#define SHADER_H

#include <gl/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader{
    public:
        unsigned int ID; // Program ID

        // constructor reads and builds the sahder
        //Shader(const char* vertexPath, const char* fragmentPath);
        //use/activate the shader
        //void use();

        //void setBool(const std::string &name, bool value) const;
        //void setInt(const std::string &name, int value) const;
        //void setInt(const std::string &name, int value) const;
        //void setFloat(const std::string &name, float value) const;


        Shader(const char* vertexPath, const char* fragmentPath){
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            // ensure ifstream objects can throw exceptions:
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


            
            try{
                //open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                
                std::stringstream vShaderStream, fShaderStream;
                //read files buffer contents into streams
                
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                //close files
                vShaderFile.close();
                fShaderFile.close();
                //convert stream into string
                vertexCode      = vShaderStream.str();
                fragmentCode    = fShaderStream.str();

            } catch(std::ifstream::failure& e){
                std::cout << "Error::Shader::File_Not_Succesfully_Read\n";
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            //compile shaders
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            //vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            //check for compile errors
            glGetShaderiv(vertex,   GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "Error::Shader::Vertex::Compilation_Failed\n" << infoLog << std::endl;
            };
            //fragment shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            //check for compile errors
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success){
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout << "Error::Shader::Fragment::Compilation_Failed\n" << infoLog << std::endl;
            };
            //Shader Program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            //check for program errors
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success){
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                std::cout << "Error::Shader::Program::Linking_Failed\n" << infoLog << std::endl;
            };
            //Shaders are linked & no longer needed. BURN 'EM ALL!
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use(){
            glUseProgram(ID);
        }

        void setBool(const std::string &name, bool value) const{
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string &name, int value) const{
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value) const{
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }

        void setMat4(const std::string &name, const glm::mat4 &mat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

};

#endif