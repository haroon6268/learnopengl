#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

char* read_file(char* file_name){

  FILE *f;
  f = fopen(file_name, "r");

  if(!f){
    printf("Error opening file...\n");
    return NULL;
  }

  fseek(f, 0, SEEK_END);
  long length = ftell(f);
  fseek(f, 0, SEEK_SET);

  char* buff = malloc(sizeof(char) * length + 1);

  fread(buff, sizeof(char), length, f);

  buff[length] = '\0';
  
  fclose(f);
  
  return buff;
}

int compile_shader(unsigned int* shader_id, GLenum shader_type, const char* shader_source){
  *shader_id = glCreateShader(shader_type);
  glShaderSource(*shader_id, 1, &shader_source, NULL);
  glCompileShader(*shader_id);

  int success;
  char infoLog[512];
  glGetShaderiv(*shader_id, GL_COMPILE_STATUS, &success);

  char* shaderType;

  if(shader_type == GL_FRAGMENT_SHADER){
    shaderType = "FRAGMENT"; 
  }else{
    shaderType = "VERTEX";
  }

  if(!success){
    glGetShaderInfoLog(*shader_id, 512, NULL, infoLog);
    printf("ERROR::SHADER::%s::COMPILATION_FAILED: %s\n", shaderType, infoLog);
    return 1;
  }
  return 0;
}





