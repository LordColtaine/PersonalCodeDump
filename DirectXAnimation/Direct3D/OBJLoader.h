#pragma once
#include <vector>
#include <DirectXMath.h>
#include <string>
#include<fstream>

using namespace DirectX;
using namespace std;

struct Object
{
	//int numvertices;
	vector<XMFLOAT3> vertices;
	vector<XMFLOAT2> textcoords;
	vector<XMFLOAT3> normals;
	vector<unsigned short> indices;
	struct VerticesWithTex
	{
		XMFLOAT3 vertex;
		XMFLOAT3 tex;
	};
	vector<VerticesWithTex> verticesWTex;

	const vector<string> explode(const string& s, const char& c)
	{
		string buff{ "" };
		vector<string> v;

		for (auto n : s)
		{
			if (n != c) buff += n; else
				if (n == c && buff != "") { v.push_back(buff); buff = ""; }
		}
		if (buff != "") v.push_back(buff);

		return v;

	}
	void LoadFromFile(std::string Path)
	{
		ifstream f(Path);
		if (f)
		{
			while (!f.eof())
			{
				char buf[256];
				char first = 0, second = 0;
				f >> first;
				if (!first)break;
				if ('v' == first)
				{
					second = f.peek();
					if ('n' == second)
					{
						f >> second;
						float x, y, z;
						f >> x >> y >> z;
						normals.push_back(XMFLOAT3(x, y, z));
					}
					else if ('t' == second)
					{
						f >> second;
						float u, v;
						f >> u >> v;
						textcoords.push_back(XMFLOAT2(u, v));
					}
					else
					{
						float x, y, z;
						f >> x >> y >> z;
						vertices.push_back(XMFLOAT3(x, y, z));
					}
				}
				else if ('f' == first)
				{
					f.getline(buf, 256);
					string buffer(buf, strlen(buf));
					vector<string> words = explode(buffer, ' ');
					vector<int> faces;
					
					for (auto word : words)
					{
						vector<string> elements = explode(word, '/');
						int i = 0, val = 0;
						while (word[i] >= '0' && word[i] <= '9')
						{
							int num = word[i] - '0';
							val = val * 10 + num;
							i++;
							if (i >= word.size())break;
						}
						faces.push_back(val-1);
					}
					
					for (size_t i = 0; i < faces.size() - 2; i++)
					{
						indices.push_back(faces[0]);
						indices.push_back(faces[i + 1]);
						indices.push_back(faces[i + 2]);
					}
					
				}
				else
				{
					f.getline(buf, 256);
				}
			}
			f.close();
			
		}
	}
	void PrintVertices()
	{
		for (auto vertex : vertices)
		{
			//cout << vertex.x << " " << vertex.y << " " << vertex.z << endl;
			printf("%f %f %f\n", vertex.x, vertex.y, vertex.z);
		}
	}

	void PrintNormals()
	{
		for (auto normal : normals)
		{
			//cout << vertex.x << " " << vertex.y << " " << vertex.z << endl;
			printf("%f %f %f\n", normal.x, normal.y, normal.z);
		}


	}

	void PrintTextCoords()
	{
		for (auto coord : textcoords)
		{
			//cout << vertex.x << " " << vertex.y << " " << vertex.z << endl;
			printf("%f %f\n", coord.x, coord.y);
		}
	}

	void PrintIndices()
	{
		for (int i = 0; i < indices.size(); i++)
		{
			printf("%d ", indices[i]);
			if (0 == i % 3)
				printf("\n");
		}
	}
};