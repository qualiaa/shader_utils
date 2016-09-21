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
    using ObjectPtr = std::shared_ptr<ShaderObject>;
    using ObjectPtrList = std::vector<ObjectPtr>;

private:
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

    ObjectPtrList const& attachedObjects() const { return attached_objects_;  }
    void attachObject(ShaderObject&&);
    void attachObject(ObjectPtr const&);
    void detachObject(ObjectPtr const&);
    void detachAllObjects();
    void link();

    std::pair<GLboolean, std::string> getLinkErrors() const;

    operator GLuint() const { return id_; }
    friend void swap(ShaderProgram&, ShaderProgram&) noexcept;
};
