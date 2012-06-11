/*

    ObjModel - Used to represent an RSOBJ model.

	merge from external .obj loader example
	I make the following change
	removing "mtlib" and "usemtl"
	but adding 1 grammar rule:
		texture texturefileName
	
	and i change the file extension as .rsobj 
*/


#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_
#include <vector>

struct ObjVertex;

class ObjLoader
{
public:
	ObjLoader( );      
    ~ObjLoader( );

    void Release( );
    bool LoadOBJ(char *fileName);
	bool LoadRSObjModel(std::string fileName, bool isRHCoordSys, bool computeNormals, std::vector<int>& subsetIndexStart,
		std::vector<std::string>& subsetTextureFileName);
	void GetObjInfo(int* tc, int* tv, int* mt, int*sc);
	int GetMeshTriangles();
	int GetTotVertexes();
	ObjVertex* GetVetexes();
    unsigned int* GetIndices();

private:
    float *vertices_;
    float *normals_;
    float *texCoords_;
    int	totalVerts_;

private:
	
	int subsetCount;

	std::vector<unsigned int> indices;
	std::vector<D3DXVECTOR3> vertPos;
	std::vector<D3DXVECTOR3> vertNorm;
	std::vector<D3DXVECTOR2> vertTexCoord;

	//Vertex definition indices
	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;

	int triangleCount;	//Total Triangles
	int totalVerts;
	int meshTriangles;
	std::vector<ObjVertex> vertices;
};

#endif