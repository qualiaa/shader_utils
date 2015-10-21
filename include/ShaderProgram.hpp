#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <memory>
#include "ShaderObject.hpp"
#include "ShaderFile.hpp"

class ShaderProgram
{
public:
    using ShaderPtr = std::shared_ptr<ShaderObject>;
    using ShaderPtrList = std::vector<ShaderPtr>;

private:
    GLuint id_;
    ShaderPtrList attached_shaders_;

public:
    ShaderProgram();
    template <typename ForwardIt>
    ShaderProgram(std::move_iterator<ForwardIt> start,
                  std::move_iterator<ForwardIt> end);
    ShaderProgram(ShaderProgram&&);
    ShaderProgram& operator=(ShaderProgram&&);
    ~ShaderProgram();

    ShaderPtrList const& attachedShaders() const { return attached_shaders_;  }
    void attachShader(ShaderObject&&);
    void attachShader(ShaderPtr const&);
    void detachShader(ShaderPtr const&);
    void link();

    std::pair<GLboolean, std::string> getLinkErrors() const;

    operator GLuint() const { return id_; }
    friend void swap(ShaderProgram&, ShaderProgram&) noexcept;

    ShaderProgram(ShaderProgram const&) = delete;
    ShaderProgram& operator=(ShaderProgram const&) = delete;
};
