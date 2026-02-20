#pragma once

#include <glad/glad.h>

char* read_file(char* file_name);
int compile_shader(unsigned int* shader_id, GLenum shader_type, const char* shader_source);
