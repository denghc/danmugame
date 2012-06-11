#pragma once
// class for .obj format model
#include "modelclass.h"
#include <vector>
class ObjModelClass :
	public ModelClass
{
public:
	ObjModelClass();
	ObjModelClass(char* objFileName);
	~ObjModelClass(void);
	virtual bool Initialize(ID3D11Device*);
	//virtual bool Initialize(ID3D11Device* device, WCHAR* textureFilename);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	virtual int GetIndexCount();
	virtual bool Update();
	void SetModelAttribute(bool isRH, bool computeNorm);

protected:
	virtual bool InitializeBuffers(ID3D11Device*);
	// every model has to implement this function
	virtual bool InitializeShaders(ID3D11Device*) = 0;
	virtual void ShutdownBuffers();
	virtual void RenderBuffers(ID3D11DeviceContext*);
	virtual bool LoadTexture(ID3D11Device*);
	virtual void ReleaseTexture();//call texture manager to release

protected:
	// vs and ps by subset order
	std::vector<std::string> m_psList;
	std::vector<std::string> m_vsList;
	unsigned int m_vertextCount;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	char* m_objFileName;
	bool m_isRHCoorSys;
	bool m_computeNormals;

	int m_triangleCount;	//Total Triangles
	int m_totalVerts;
	int m_meshTriangles;
	int m_subsetCount;


	std::vector<int> subsetIndexStart;
	std::vector<std::string> subsetTextureFileName;
};

