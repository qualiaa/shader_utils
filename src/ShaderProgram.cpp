#include "ShaderProgram.hpp"

#include <algorithm>
#include <functional>
#include <sstream>
#include <tuple>
#include <vector>
#include "GLException.hpp"

GLuint ShaderProgram::boundId_ {0};

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

void ShaderProgram::setUniform(std::string name, GLint v)              const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform1i(uniformId,v);
}
void ShaderProgram::setUniform(std::string name, GLuint v)             const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform1ui(uniformId,v);
}
void ShaderProgram::setUniform(std::string name, GLfloat v)            const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform1f(uniformId,v);
}
void ShaderProgram::setUniform(std::string name, GLdouble v)           const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform1d(uniformId,v);
}
void ShaderProgram::setUniform(std::string name, glm::vec2 const& v)   const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform2fv(uniformId,2,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::vec3 const& v)   const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform3fv(uniformId,3,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::vec4 const& v)   const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform4fv(uniformId,4,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::dvec2 const& v)  const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform2dv(uniformId,2,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::dvec3 const& v)  const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform3dv(uniformId,3,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::dvec4 const& v)  const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform4dv(uniformId,4,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::ivec2 const& v)  const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform2iv(uniformId,2,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::ivec3 const& v)  const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform3iv(uniformId,3,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::ivec4 const& v)  const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform4iv(uniformId,4,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::uvec2 const& v) const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform2uiv(uniformId,2,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::uvec3 const& v) const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform3uiv(uniformId,3,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::uvec4 const& v) const
{
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniform4uiv(uniformId,4,&v[0]);
}
void ShaderProgram::setUniform(std::string name, glm::mat2 const& v, bool t)   const
{
    GLboolean b = t ? GL_TRUE : GL_FALSE;
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniformMatrix2fv(uniformId,2,b,&v[0][0]);
}
void ShaderProgram::setUniform(std::string name, glm::mat3 const& v, bool t)   const
{
    GLboolean b = t ? GL_TRUE : GL_FALSE;
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniformMatrix3fv(uniformId,3,b,&v[0][0]);
}
void ShaderProgram::setUniform(std::string name, glm::mat4 const& v, bool t)   const
{
    GLboolean b = t ? GL_TRUE : GL_FALSE;
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniformMatrix4fv(uniformId,4,b,&v[0][0]);
}
void ShaderProgram::setUniform(std::string name, glm::dmat2 const& v, bool t)  const
{
    GLboolean b = t ? GL_TRUE : GL_FALSE;
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniformMatrix2dv(uniformId,2,b,&v[0][0]);
}
void ShaderProgram::setUniform(std::string name, glm::dmat3 const& v, bool t)  const
{
    GLboolean b = t ? GL_TRUE : GL_FALSE;
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniformMatrix3dv(uniformId,3,b,&v[0][0]);
}
void ShaderProgram::setUniform(std::string name, glm::dmat4 const& v, bool t)  const
{
    GLboolean b = t ? GL_TRUE : GL_FALSE;
    ensureProgramIsBound();
    GLuint uniformId = getUniformLocation(name);
    glUniformMatrix4dv(uniformId,4,b,&v[0][0]);
}

void ShaderProgram::bind(ShaderProgram const& s)
{
    if (not s.linked_) {
        throw GLException("Attempted to bind shader program which is not linked");
    }
    glUseProgram(s.id_);
    boundId_ = s.id_;
}

void ShaderProgram::unbind()
{
    glUseProgram(0);
    boundId_ = 0;
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

bool ShaderProgram::bound() const
{
    return id_ == boundId_;
}

void ShaderProgram::ensureProgramIsBound() const
{
    if (not bound()) {
        throw GLException("Attempted to set uniform on unbound shader");
    }
}

void swap(ShaderProgram& a, ShaderProgram& b) noexcept
{
    using std::swap;

    swap(a.id_, b.id_);
    swap(a.attached_objects_, b.attached_objects_);
    swap(a.linked_, b.linked_);
}

GLuint ShaderProgram::getUniformLocation(std::string const& name) const
{
    GLuint uniformId = glGetUniformLocation(id_, name.c_str());
    if (uniformId == -1) {
        std::stringstream sstream;
        sstream << "Uniform " << name << " not found in shader";
        throw GLException(sstream.str());
    }
    return uniformId;
}
