#pragma once

#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include "ShaderObject.hpp"
#include "ShaderFile.hpp"

class ShaderProgram
{
public:
    using ObjectPtr = std::shared_ptr<ShaderObject>;
    using ObjectPtrList = std::vector<ObjectPtr>;

private:
    static GLuint boundId_;

    GLuint id_;
    ObjectPtrList attached_objects_;
    bool linked_ = false;

public:
    static void bind(ShaderProgram const&);
    static void unbind();

    ShaderProgram();
    template <typename ForwardIt>
    ShaderProgram(std::move_iterator<ForwardIt> start,
                  std::move_iterator<ForwardIt> end);
    ShaderProgram(ShaderProgram const&) = delete;
    ShaderProgram& operator=(ShaderProgram const&) = delete;
    ShaderProgram(ShaderProgram&&);
    ShaderProgram& operator=(ShaderProgram&&);
    ~ShaderProgram();

    void setUniform(std::string, GLint)              const;
    void setUniform(std::string, GLuint)             const;
    void setUniform(std::string, GLfloat)            const;
    void setUniform(std::string, GLdouble)           const;
    void setUniform(std::string, glm::vec2 const&)   const;
    void setUniform(std::string, glm::vec3 const&)   const;
    void setUniform(std::string, glm::vec4 const&)   const;
    void setUniform(std::string, glm::dvec2 const&)  const;
    void setUniform(std::string, glm::dvec3 const&)  const;
    void setUniform(std::string, glm::dvec4 const&)  const;
    void setUniform(std::string, glm::ivec2 const&)  const;
    void setUniform(std::string, glm::ivec3 const&)  const;
    void setUniform(std::string, glm::ivec4 const&)  const;
    void setUniform(std::string, glm::uvec2 const&)  const;
    void setUniform(std::string, glm::uvec3 const&)  const;
    void setUniform(std::string, glm::uvec4 const&)  const;
    void setUniform(std::string, glm::mat2 const&,
                    bool transpose = false)  const;
    void setUniform(std::string, glm::mat3 const&,
                    bool transpose = false)  const;
    void setUniform(std::string, glm::mat4 const&,
                    bool transpose = false)  const;
    void setUniform(std::string, glm::dmat2 const&,
                    bool transpose = false)  const;
    void setUniform(std::string, glm::dmat3 const&,
                    bool transpose = false)  const;
    void setUniform(std::string, glm::dmat4 const&,
                    bool transpose = false)  const;

    ObjectPtrList const& attachedObjects() const { return attached_objects_;  }
    void attachObject(ShaderObject&&);
    void attachObject(ObjectPtr const&);
    void detachObject(ObjectPtr const&);
    void detachAllObjects();
    void link();

    GLuint getId() const { return id_; }
    std::pair<GLboolean, std::string> getLinkErrors() const;
    bool bound() const;

    operator GLuint() const { return id_; }
    friend void swap(ShaderProgram&, ShaderProgram&) noexcept;

private:
    void ensureProgramIsBound() const;
    GLint getUniformLocation(std::string const& name) const;
};
