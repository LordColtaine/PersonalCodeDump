#include "Draw.h"

/*You need to be able to convert the
DrawMode enum into GLenum.We will do
this with a static helper function.The only
thing this function needs to do is figure
out what the input draw mode is and
return the appropriate GLenum value :*/
static GLenum DrawModeToGLEnum(DrawMode input) 
{
	switch (input) 
	{
	case DrawMode::Points:
		return GL_POINTS;
	case DrawMode::LineStrip: 
		return GL_LINE_STRIP;
	case DrawMode::LineLoop:
		return GL_LINE_LOOP;
	case DrawMode::Lines:
		return GL_LINES;
	case DrawMode::Triangles:
		return GL_TRIANGLES;
	case DrawMode::TriangleStrip:
		return GL_TRIANGLE_STRIP;
	case DrawMode::TriangleFan:
		return GL_TRIANGLE_FAN;
	}
	std::cout << "DrawModeToGLEnum unreachable code hit\n";
		return 0;
}

void Draw(unsigned int vertexCount, DrawMode mode) 
{
	glDrawArrays(DrawModeToGLEnum(mode), 0, vertexCount);
}

void DrawInstanced(unsigned int vertexCount, DrawMode mode, unsigned int numInstances) 
{
	glDrawArraysInstanced(DrawModeToGLEnum(mode), 0, vertexCount, numInstances);
}

void Draw(IndexBuffer& inIndexBuffer, DrawMode mode) 
{
	unsigned int handle = inIndexBuffer.GetHandle();
	unsigned int numIndices = inIndexBuffer.Count();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glDrawElements(DrawModeToGLEnum(mode), numIndices, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void DrawInstanced(IndexBuffer& inIndexBuffer, DrawMode mode, unsigned int instanceCount) 
{
	unsigned int handle = inIndexBuffer.GetHandle();
	unsigned int numIndices = inIndexBuffer.Count();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glDrawElementsInstanced(DrawModeToGLEnum(mode), numIndices, GL_UNSIGNED_INT, 0, instanceCount);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
