#include "Uniform.h"
template Uniform<int>;
template Uniform<ivec4>;
template Uniform<ivec2>;
template Uniform<float>;
template Uniform<vec2>;
template Uniform<vec3>;
template Uniform<vec4>;
template Uniform<quat>;
template Uniform<mat4>;

/*You only need to implement one of the
Set methods for each type—the one that
takes an arrayand a length.The other Set
method overloads are for convenience.
Implement the two convenience overloads
—one of them is for setting a single
uniform, the other is for setting a vector.
Both overloads should just call the Set
function :*/
template <typename T>
void Uniform<T>::Set(unsigned int slot, const T & value) 
{
	Set(slot, (T*)&value, 1);
}
template <typename T>
void Uniform<T>::Set(unsigned int s, std::vector<T>&v) 
{
	Set(s, &v[0], (unsigned int)v.size());
}
/*Create a UNIFORM_IMPL macro.The first
argument will be which OpenGL function
to call, the second is the type of structure
that is being used, and the last argument
is the data type of the same structure.The
UNIFORM_IMPL macro assembles these
pieces of information into a function
declaration :*/
#define UNIFORM_IMPL(gl_func, tType, dType) \
template<> void Uniform<tType>::Set(unsigned int slot, \
	tType * data, unsigned int length) {\
		gl_func(slot, (GLsizei)length, (dType*)&data[0]); \
}
/*Call the UNIFORM_IMPL macro for each
uniform data type to generate the
appropriate Set functions.The only data
type this method won't work for is mat4:*/

UNIFORM_IMPL(glUniform1iv, int, int)
UNIFORM_IMPL(glUniform4iv, ivec4, int)
UNIFORM_IMPL(glUniform2iv, ivec2, int)
UNIFORM_IMPL(glUniform1fv, float, float)
UNIFORM_IMPL(glUniform2fv, vec2, float)
UNIFORM_IMPL(glUniform3fv, vec3, float)
UNIFORM_IMPL(glUniform4fv, vec4, float)
UNIFORM_IMPL(glUniform4fv, quat, float)
/*The Set function for matrices needs to be
specified manually; otherwise, the
UNIFORM_IMPL macro won't work. This is
because the glUniformMatrix4fv
function takes an additional Boolean
argument asking whether the matrix
should be transposed or not.Set the
transposed Boolean to false:*/
template<> 
void Uniform<mat4>::Set(unsigned int slot, mat4 * inputArray, unsigned int arrayLength) 
{
	glUniformMatrix4fv(slot, (GLsizei)arrayLength, false, (float*)&inputArray[0]);
}