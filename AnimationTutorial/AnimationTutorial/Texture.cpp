#include "Texture.h"

Texture::Texture() 
{
	mWidth = 0;
	mHeight = 0;
	mChannels = 0;
	glGenTextures(1, &mHandle);
}
Texture::Texture(const char* path) 
{
	glGenTextures(1, &mHandle);
	Load(path);
}
Texture::~Texture() 
{
	glDeleteTextures(1, &mHandle);
}

void Texture::Load(const char* path) 
{
	glBindTexture(GL_TEXTURE_2D, mHandle);
	int width, height, channels;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	mWidth = width;
	mHeight = height;
	mChannels = channels;
}

/*The Set function needs to activate a
texture unit, bind the handle that the
Texture class contains to that texture
unit, then set the specified uniform index
to contain the texture unit that is
currently bound.The Unset function
unbinds the current texture from the
specified texture unit :*/
void Texture::Set(unsigned int uniformIndex, unsigned int textureIndex) 
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, mHandle);
	glUniform1i(uniformIndex, textureIndex);
}
void Texture::UnSet(unsigned int textureIndex) 
{
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

/*The GetHandle getter function is simple :*/
unsigned int Texture::GetHandle() 
{
	return mHandle;
}

