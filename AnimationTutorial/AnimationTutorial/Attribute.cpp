#include "Attribute.h"

template Attribute<int>;
template Attribute<float>;
template Attribute<vec2>;
template Attribute<vec3>;
template Attribute<vec4>;
template Attribute<ivec4>;

/*The Attribute class has two simple
getters, one to retrieve the countand one
to retrieve the OpenGL handle.The count
represents how many total attributes
there are :*/
template<typename T>
unsigned int Attribute<T>::Count() 
{
	return mCount;
}
template<typename T>
unsigned int Attribute<T>::GetHandle() 
{
	return mHandle;
}
/*The Set function takes an array and a
length.It then binds the buffer that the
Attribute class is holding on to and
uses glBufferData to fill the buffer with
data.There is a convenience function for
Set that takes a vector reference instead
of an array.It calls the actual Set
function :*/
template<typename T>
void Attribute<T>::Set(T *inputArray, unsigned int arrayLength) 
{
	mCount = arrayLength;
	unsigned int size = sizeof(T);
	glBindBuffer(GL_ARRAY_BUFFER, mHandle);
	glBufferData(GL_ARRAY_BUFFER, size * mCount, inputArray, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
template<typename T>
void Attribute<T>::Set(std::vector<T>&input) 
{
	Set(&input[0], (unsigned int)input.size());
}
/*The SetAttribPointer function wraps
glVertesAttribPointer or
glVertesAttribIPointer.The
arguments and which function to call are
different based on what the type of the
Attribute class is.To remove any
ambiguity, provide explicit
implementations for all supported
template types.Start by implementing the
int, ivec4, and float types :*/
template<>
void Attribute<int>::SetAttribPointer(unsigned int s) 
{
	glVertexAttribIPointer(s, 1, GL_INT, 0, (void*)0);
}
template<>
void Attribute<ivec4>::SetAttribPointer(unsigned int s) 
{
	glVertexAttribIPointer(s, 4, GL_INT, 0, (void*)0);
}
template<>
void Attribute<float>::SetAttribPointer(unsigned int s) 
{
	glVertexAttribPointer(s, 1, GL_FLOAT, GL_FALSE, 0, 0);
}
/*Implement the vec2, vec3, and vec4
types next.These are all very similar to
the float type.The only difference is the
second argument to
glVertexAttribPointer :*/
template<>
void Attribute<vec2>::SetAttribPointer(unsigned int s) 
{
	glVertexAttribPointer(s, 2, GL_FLOAT, GL_FALSE, 0, 0);
}
template<>
void Attribute<vec3>::SetAttribPointer(unsigned int s) 
{
	glVertexAttribPointer(s, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
template<>
void Attribute<vec4>::SetAttribPointer(unsigned int s) 
{
	glVertexAttribPointer(s, 4, GL_FLOAT, GL_FALSE, 0, 0);
}
/*The last two functions of the Attribute
class need to bind and unbind the
attribute to a slot specified in the Shader
class.Since the
glVertexAttribPointer function is
different based on the templated type of
the Attribute class, Bind will call the
SetAttribPointer helper function :*/
template<typename T>
void Attribute<T>::BindTo(unsigned int slot) 
{
	glBindBuffer(GL_ARRAY_BUFFER, mHandle);
	glEnableVertexAttribArray(slot);
	SetAttribPointer(slot);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
template<typename T>
void Attribute<T>::UnBindFrom(unsigned int slot) 
{
	glBindBuffer(GL_ARRAY_BUFFER, mHandle);
	glDisableVertexAttribArray(slot);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}