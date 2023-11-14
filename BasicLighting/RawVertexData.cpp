#include "RawVertexData.h"

float* RawVertexData::PyramidData()
{
	float* vertices = new float[9] {
			1, 1,1,
			1, -1,1,
			-1,-1,1,
	};

	return vertices;
}

float* RawVertexData::CubeData()
{
	float* vertices = new float[Cube_Data_Size] {
		   -1, -1, -1,
			1, -1, -1,
			1, 1, -1,
			1, 1, -1,
			-1, 1, -1,
			-1, -1, -1,

			-1, -1, 1,
			1, -1, 1,
			1, 1, 1,
			1, 1, 1,
			-1, 1, 1,
			-1, -1, 1,

			-1, 1, 1,
			-1, 1, -1,
			-1, -1, -1,
			-1, -1, -1,
			-1, -1, 1,
			-1, 1, 1,

			1, 1, 1,
			1, 1, -1,
			1, -1, -1,
			1, -1, -1,
			1, -1, 1,
			1, 1, 1,

			-1, -1, -1,
			1, -1, -1,
			1, -1, 1,
			1, -1, 1,
			-1, -1, 1,
			-1, -1, -1,

			-1, 1, -1,
			1, 1, -1,
			1, 1, 1,
			1, 1, 1,
			-1, 1, 1,
			-1, 1, -1,
	};

	return vertices;
}

float* RawVertexData::CubeWithNormalsData()
{
	float* vertices = new float[Cube_With_Normals_Data]{
	//COORDINATES VEC3    //NORAMALS VEC3    //TEXUTRE VEC2
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	return vertices;
}
