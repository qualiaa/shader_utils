#include "ShaderProgram.hpp"

#include <algorithm>
#include <functional>
#include <tuple>
#include <vector>
#include "GLException.hpp"

ShaderProgram::ShaderProgram()
    : id_{glCreateProgram()}
{
    if (not id_) {
        throw GLException("Could not allocate shader program");
    }
}

template <typename ForwardIt>
ShaderProgram::ShaderProgram(std::move_iterator<ForwardIt> start,
                             std::move_iterator<ForwardIt> end)
    : ShaderProgram{}
{
    static_assert(
        std::is_same<typename ForwardIt::value_type, ShaderObject>::value,
        "Must contain objects of type ShaderObject"
    );

    std::for_each(start, end, [this](auto&& x) { attachObject(std::move(x)); });
}

ShaderProgram::ShaderProgram(ShaderProgram&& other)
    : id_{other.id_}
    , attached_objects_{other.attached_objects_}
    , linked_{other.linked_}
{
    other.id_ = 0;
    other.attached_objects_.clear();
    other.linked_ = false;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other)
{
    ShaderProgram temp(std::move(other));
    swap(*this,temp);
    return *this;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id_);
}

void ShaderProgram::bind(ShaderProgram const& s)
{
    if (not s.linked_) {
        throw GLException("Attempted to bind shader program which is not linked");
    }
    glUseProgram(s.id_);
}

void ShaderProgram::unbind()
{
    glUseProgram(0);
}

void ShaderProgram::attachObject(ShaderObject&& so)
{
    attached_objects_.push_back(std::make_shared<ShaderObject>(std::move(so)));
    glAttachShader(id_, *attached_objects_.back());
}

void ShaderProgram::attachObject(ObjectPtr const& p)
{
    auto beg = attached_objects_.begin();
    auto end = attached_objects_.end();

    auto res = std::find_if(beg, end,
            [&p] (ObjectPtr const& x) {
                return p.get() == x.get();
            });

    if (res == end) {
        attached_objects_.push_back(p);
        glAttachShader(id_, *attached_objects_.back());
    }
}

void ShaderProgram::detachObject(ObjectPtr const& p)
{
    auto beg = attached_objects_.begin();
    auto end = attached_objects_.end();

    auto res =
        std::remove_if(beg, end,
            [&p] (ObjectPtr const& x) {
                return p.get() == x.get();
            });

    if (res != end) {
        glDetachShader(id_, *p);
        attached_objects_.erase(res,end);
    }
}

void ShaderProgram::detachAllObjects()
{
    for (auto object : attached_objects_) {
        glDetachShader(id_, *object);
    }
    attached_objects_.clear();
}


void ShaderProgram::link()
{
    glLinkProgram(id_);

    // Throw exception on error
    GLboolean status; std::string log;
    std::tie(status, log) = getLinkErrors();
    if (status == GL_FALSE) {
        throw GLShaderLinkError{log};
    }

    linked_ = true;
}

std::pair<GLboolean, std::string> ShaderProgram::getLinkErrors() const
{
    std::string error_string;

    GLint link_status;
    glGetProgramiv(id_, GL_LINK_STATUS, &link_status);

    if (link_status == GL_FALSE) {
        GLint link_log_length;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &link_log_length);
        error_string.resize(link_log_length);
        glGetProgramInfoLog(id_, link_log_length, NULL, &(error_string[0]));
    }

    return {link_status, error_string};
}

void swap(ShaderProgram& a, ShaderProgram& b) noexcept
{
    using std::swap;

    swap(a.id_, b.id_);
    swap(a.attached_objects_, b.attached_objects_);
    swap(a.linked_, b.linked_);
}
