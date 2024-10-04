#include "Shader.h"

/*Both Shader constructors must create a
new shader program handle by calling
glCreateProgram. The constructor
variant that takes two strings calls the
Load function with the strings. Since
mHandle is always a program handle, the
destructor needs to delete the handle:*/
Shader::Shader() 
{
	mHandle = glCreateProgram();
}
Shader::Shader(const std::string& vertex, const std::string& fragment) 
{
	mHandle = glCreateProgram();
	Load(vertex, fragment);
}
Shader::~Shader() {
	glDeleteProgram(mHandle);
}

/*The ReadFile helper function converts a
file into a string using std::ifstream to
read the contents of the file into
std::stringstream. The string stream
can be used to return the contents of the
file as a string:*/
std::string Shader::ReadFile(const std::string& path) 
{
	std::ifstream file;
	file.open(path);
	std::stringstream contents;
	contents << file.rdbuf();
	file.close();
	return contents.str();
}

/*The CompileVertexShader function is
boilerplate code for compiling an OpenGL
vertex shader. First, create the shader
object with glCreateShader, then set
the source for the shader with
glShaderSource. Finally, compile the
shader with glCompileShader. Check
for errors with glGetShaderiv:*/
unsigned int Shader::CompileVertexShader( const std::string& vertex)
{
	unsigned int v = glCreateShader(GL_VERTEX_SHADER);
	const char* v_source = vertex.c_str();
	glShaderSource(v, 1, &v_source, NULL);
	glCompileShader(v);
	int success = 0;
	glGetShaderiv(v, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		char infoLog[512];
		glGetShaderInfoLog(v, 512, NULL, infoLog);
		std::cout << "Vertex compilation failed.\n";
		std::cout << "\t" <<infoLog << "\n";
		glDeleteShader(v);
		return 0;
	};
	return v;
}

/*The CompileFragmentShader function
is almost identical to the
CompileVertexShader function. The
only real difference is the argument to
glCreateShader, indicating that you
are creating a fragment shader, not a
vertex shader:
unsigned int*/
unsigned int Shader::CompileFragmentShader(const std::string& fragment) 
{
	unsigned int f = glCreateShader(GL_FRAGMENT_SHADER);
	const char* f_source = fragment.c_str();
	glShaderSource(f, 1, &f_source, NULL);
	glCompileShader(f);
	int success = 0;
	glGetShaderiv(f, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		char infoLog[512];
		glGetShaderInfoLog(f, 512, NULL, infoLog);
		std::cout << "Fragment compilation failed.\n";
			std::cout << "\t" << infoLog << "\n";
		glDeleteShader(f);
		return 0;
	};
	return f;
}

/*The LinkShaders helper function is also
boilerplate.Attach the shaders to the
shader program handle that the
constructor created.Link the shaders by
calling glLinkProgramand check for
errors with glGetProgramiv.Once the
shaders are linked, you only need the
program; the individual shader objects
can be deleted with glDeleteShader :*/

bool Shader::LinkShaders(unsigned int vertex, unsigned int fragment) 
{
	glAttachShader(mHandle, vertex);
	glAttachShader(mHandle, fragment);
	glLinkProgram(mHandle);
	int success = 0;
	glGetProgramiv(mHandle, GL_LINK_STATUS, &success);
	if (!success) 
	{
		char infoLog[512];
		glGetProgramInfoLog(mHandle, 512, NULL, infoLog);
		std::cout << "ERROR: Shader linking failed.\n";
		std::cout << "\t" << infoLog << "\n";
		glDeleteShader(vertex);
		glDeleteShader(fragment
		);
		return false;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return true;
}

/*The PopulateAttributes function
enumerates all the attributes stored inside
the shader program, then stores them as a
key - value pair, where the key is the name
of the attributeand the value is its
location.You can count the number of
active attributes in a shader program with
the glGetProgramiv function, passing
GL_ACTIVE_ATTRIBUTES as the
parameter name.Then, loop through all
the attributes by indexand use
glGetActiveAttrib to get the name of
each attribute.Finally, call
glGetAttribLocation to get the
location of each attribute :*/
void Shader::PopulateAttributes() 
{
	int count = -1;
	int length;
	char name[128];
	int size;
	GLenum type;
	glUseProgram(mHandle);
	glGetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTES, &count);
	for (int i = 0; i < count;++i) 
	{
		memset(name, 0, sizeof(char) * 128);
		glGetActiveAttrib(mHandle, (GLuint)i, 128, &length, &size, &type, name);
		int attrib = glGetAttribLocation(mHandle, name);
		if (attrib >= 0) 
		{
			mAttributes[name] = attrib;
		}
	}
	glUseProgram(0);
}
/*The PopulateUniforms helper function
is very similar to the
PopulateAttributes helper function.
glGetProgramiv needs to take
GL_ACTIVE_UNIFORMS as the parameter
name and you need to call
glGetActiveUniform and
glGetUniformLocation:*/
void Shader::PopulateUniforms()
{
	int count = -1;
	int length;
	char name[128];
	int size;
	GLenum type;
	char testName[256];
	glUseProgram(mHandle);
	glGetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; ++i) 
	{
		memset(name, 0, sizeof(char) * 128);
		glGetActiveUniform(mHandle, (GLuint)i, 128, &length, &size, &type, name);
		int uniform = glGetUniformLocation(mHandle, name);
		if (uniform >= 0) 
		{ //Is uniform valid ?
				/*When a valid uniform is encountered, you
				need to determine whether the uniform is
				an array or not.To do this, search for the
				array bracket([) in the uniform name.If
				the bracket is found, the uniform is an
				array:*/
			std::string uniformName = name;
			// if name contains [, uniform is array
				std::size_t found = uniformName.find('[');
			if (found != std::string::npos)
			{
				/*If you encounter a uniform array, erase
					everything out of the string, starting with
					[.This will leave you with only the
					uniform name.Then, enter a loop where
					you try to retrieve every index from the
					array by appending[+index + ] to
					the uniform name.Once the first invalid
					index is found, break the loop :*/
				uniformName.erase(uniformName.begin()+found, uniformName.end());
				unsigned int uniformIndex = 0;
				while (true) 
				{
					memset(testName, 0, sizeof(char) * 256);
					sprintf_s(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);
					int uniformLocation = glGetUniformLocation(mHandle, testName);
					if(uniformLocation < 0) 
					{
						break;
					}
					mUniforms[testName] = uniformLocation;
				}
			}
			/* At this point, uniformName contains the
				name of the uniform.If that uniform was
				an array, the[0] part of the name has
				been removed.Store the uniform index by
				name in mUniforms :*/
			mUniforms[uniformName] = uniform;
		}
	}
	glUseProgram(0);
}
/*The final helper function is the Load
function, which is responsible for loading
the actual shaders.This function takes two
strings, which are either filenames or
inline shader definitions.Once the
shaders are read, call the Compile, Link,
and Populate helper functions to load
the shader :*/
void Shader::Load(const std::string & vertex, const std::string & fragment) 
{
	std::ifstream f(vertex.c_str());
	bool vertFile = f.good();
	f.close();
	f = std::ifstream(vertex.c_str());
	bool fragFile = f.good();
	f.close();
	std::string v_source = vertex;
	if (vertFile) 
	{
		v_source = ReadFile(vertex);
	}
	std::string f_source = fragment;
	if (fragFile) 
	{
		f_source = ReadFile(fragment);
	}
	unsigned int vert = CompileVertexShader(v_source);
	unsigned int frag = CompileFragmentShader(f_source);
	if (LinkShaders(vert, frag)) 
	{
		PopulateAttributes();
		PopulateUniforms();
	}
}
/*The Bind function needs to set the
current shader program to active, while
UnBind should make sure no Shader
objects are active.The GetHandle helper
function returns the OpenGL handle to
the Shader object :*/
void Shader::Bind() 
{
	glUseProgram(mHandle);
}
void Shader::UnBind() 
{
	glUseProgram(0);
}
unsigned int
Shader::GetHandle() 
{
	return mHandle;
}
/*Finally, you need a way of retrieving the
bind slots for attributes and uniforms.The
GetAttribute function will check
whether a given attribute name is present
in the attribute map.If it is, the integer
representing it is returned.If it is not, 0 is
returned. 0 is a valid attribute index, so in
case of an error, an error message is
logged as well :*/
unsigned int Shader::GetAttribute(const std::string & name) 
{
	std::map<std::string, unsigned int>::iterator it = mAttributes.find(name);
	if (it == mAttributes.end()) 
	{
		std::cout << "Bad attrib index: " << name << "\n";
			return 0;
	}
	return it->second;
}
/*The GetUniform function is
implemented almost identically to the
GetAttribute function, except instead
of the attribute map, it works on the
uniform map :*/
unsigned int
Shader::GetUniform(const std::string & name) 
{
	std::map<std::string, unsigned int>::iterator it = mUniforms.find(name);
	if (it == mUniforms.end())
	{
		std::cout << "Bad uniform index: " << name << "\n";
			return 0;
	}
	return it->second;
}