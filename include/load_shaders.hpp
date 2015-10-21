#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include "ShaderFile.hpp"
#include "ShaderProgram.hpp"

ShaderProgram loadShaders(std::vector<ShaderFile> const&);
