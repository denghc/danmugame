/*

    ObjModel - Used to represent an OBJ model.
*/

#include "Structures.h"
#include<fstream>
#include<vector>
#include<string>
#include"objLoader.h"
#include"TokenStream.h"
#include"D3DX10Math.h"
#include<sstream>

ObjLoader::ObjLoader( )
{
    vertices_ = 0;
    normals_ = 0;
    texCoords_ = 0;
    totalVerts_ = 0;
}

      
ObjLoader::~ObjLoader( )
{
    Release( );
}


void ObjLoader::Release( )
{
    totalVerts_ = 0;

    if( vertices_ != 0 ) delete[] vertices_;
    if( normals_ != 0 ) delete[] normals_;
    if( texCoords_ != 0 ) delete[] texCoords_;

    vertices_ = 0;
    normals_ = 0;
    texCoords_ = 0;
}


bool ObjLoader::LoadOBJ( char *fileName )
{
    std::ifstream fileStream;
    int fileSize = 0;

    fileStream.open( fileName, std::ifstream::in );
   
    if( fileStream.is_open( ) == false )
        return false;

    fileStream.seekg( 0, std::ios::end );
    fileSize = ( int )fileStream.tellg( );
    fileStream.seekg( 0, std::ios::beg );

    if( fileSize <= 0 )
        return false;

    char *buffer = new char[fileSize];

    if( buffer == 0 )
        return false;

    memset( buffer, '\0', fileSize );

    TokenStream tokenStream, lineStream, faceStream;
    std::string tempLine, token;

    fileStream.read( buffer, fileSize );
    tokenStream.SetTokenStream( buffer );

    delete[] buffer;

    tokenStream.ResetStream( );

    std::vector<float> verts, norms, texC;
    std::vector<int> faces;

    char lineDelimiters[2] = { '\n', ' ' };

    while( tokenStream.MoveToNextLine( &tempLine ) )
    {
        lineStream.SetTokenStream( ( char* )tempLine.c_str( ) );
        tokenStream.GetNextToken( 0, 0, 0 );

        if( !lineStream.GetNextToken( &token, lineDelimiters, 2 ) )
            continue;

        if( strcmp( token.c_str( ), "v" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            verts.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            verts.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            verts.push_back( ( float )atof( token.c_str( ) ) );
        }
        else if( strcmp( token.c_str( ), "vn" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            norms.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            norms.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            norms.push_back( ( float )atof( token.c_str( ) ) );
        }
        else if( strcmp( token.c_str( ), "vt" ) == 0 )
        {
            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            texC.push_back( ( float )atof( token.c_str( ) ) );

            lineStream.GetNextToken( &token, lineDelimiters, 2 );
            texC.push_back( ( float )atof( token.c_str( ) ) );
        }
        else if( strcmp( token.c_str( ), "f" ) == 0 )
        {
            char faceTokens[3] = { '\n', ' ', '/' };
            std::string faceIndex;

            faceStream.SetTokenStream( ( char* )tempLine.c_str( ) );
            faceStream.GetNextToken( 0, 0, 0 );

            for( int i = 0; i < 3; i++ )
            {
                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );

                faceStream.GetNextToken( &faceIndex, faceTokens, 3 );
                faces.push_back( ( int )atoi( faceIndex.c_str( ) ) );
            }
        }
        else if( strcmp( token.c_str( ), "#" ) == 0 )
        {
            int a = 0;
            int b = a;
        }

        token[0] = '\0';
    }

    // "Unroll" the loaded obj information into a list of triangles.

    int vIndex = 0, nIndex = 0, tIndex = 0;
    int numFaces = ( int )faces.size( ) / 9;

    totalVerts_ = numFaces * 3;

    vertices_ = new float[totalVerts_ * 3];

    if( ( int )norms.size( ) != 0 )
    {
        normals_ = new float[totalVerts_ * 3];
    }

    if( ( int )texC.size( ) != 0 )
    {
        texCoords_ = new float[totalVerts_ * 2];
    }

    for( int f = 0; f < ( int )faces.size( ); f+=3 )
    {
        vertices_[vIndex + 0] = verts[( faces[f + 0] - 1 ) * 3 + 0];
        vertices_[vIndex + 1] = verts[( faces[f + 0] - 1 ) * 3 + 1];
        vertices_[vIndex + 2] = verts[( faces[f + 0] - 1 ) * 3 + 2];
        vIndex += 3;

        if(texCoords_)
        {
            texCoords_[tIndex + 0] = texC[( faces[f + 1] - 1 ) * 2 + 0];
            texCoords_[tIndex + 1] = texC[( faces[f + 1] - 1 ) * 2 + 1];
            tIndex += 2;
        }

        if(normals_)
        {
            normals_[nIndex + 0] = norms[( faces[f + 2] - 1 ) * 3 + 0];
            normals_[nIndex + 1] = norms[( faces[f + 2] - 1 ) * 3 + 1];
            normals_[nIndex + 2] = norms[( faces[f + 2] - 1 ) * 3 + 2];
            nIndex += 3;
        }
    }

    verts.clear( );
    norms.clear( );
    texC.clear( );
    faces.clear( );

    return true;
}

bool ObjLoader::LoadRSObjModel(std::string fileName, bool isRHCoordSys, bool computeNormals, 
	std::vector<int>& subsetIndexStart, std::vector<std::string>& subsetTextureFileName)
{
	HRESULT hr = 0;

	std::ifstream fileIn (fileName.c_str());	//Open file
	std::string textureName;					//String to hold our obj material library filename

	//Arrays to store our model's information


	//Make sure we have a default if no tex coords or normals are defined
	bool hasTexCoord = false;
	bool hasNorm = false;

	//Temp variables to store into vectors
	std::string meshMaterialsTemp;
	int vertPosIndexTemp;
	int vertNormIndexTemp;
	int vertTCIndexTemp;

	wchar_t checkChar;		//The variable we will use to store one char from file at a time
	std::string face;		//Holds the string containing our face vertices
	int vIndex = 0;			//Keep track of our vertex index count

	subsetCount = 0;
	triangleCount = 0;	//Total Triangles
	totalVerts = 0;
	meshTriangles = 0;

	//Check to see if the file was opened
	if (fileIn)
	{
		while(fileIn)
		{			
			checkChar = fileIn.get();	//Get next char

			switch (checkChar)
			{		
			case '#':
				checkChar = fileIn.get();
				while(checkChar != '\n')
					checkChar = fileIn.get();
				break;
			case 'v':	//Get Vertex Descriptions
				checkChar = fileIn.get();
				if(checkChar == ' ')	//v - vert position
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;	//Store the next three types

					if(isRHCoordSys)	//If model is from an RH Coord System
						vertPos.push_back(D3DXVECTOR3( vx, vy, vz * -1.0f));	//Invert the Z axis
					else
						vertPos.push_back(D3DXVECTOR3( vx, vy, vz));
				}
				if(checkChar == 't')	//vt - vert tex coords
				{			
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;		//Store next two types

					if(isRHCoordSys)	//If model is from an RH Coord System
						vertTexCoord.push_back(D3DXVECTOR2(vtcu, 1.0f - vtcv));	//Reverse the "v" axis
					else
						vertTexCoord.push_back(D3DXVECTOR2(vtcu, vtcv));	

					hasTexCoord = true;	//We know the model uses texture coords
				}
				//Since we compute the normals later, we don't need to check for normals
				//In the file, but i'll do it here anyway
				if(checkChar == 'n')	//vn - vert normal
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;	//Store next three types

					if(isRHCoordSys)	//If model is from an RH Coord System
						vertNorm.push_back(D3DXVECTOR3( vnx, vny, vnz * -1.0f ));	//Invert the Z axis
					else
						vertNorm.push_back(D3DXVECTOR3( vnx, vny, vnz ));	

					hasNorm = true;	//We know the model defines normals
				}
				break;

				//New group (Subset)
			case 'g':	//g - defines a group
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					subsetIndexStart.push_back(vIndex);		//Start index for this subset
					subsetCount++;
				}
				break;

				//Get Face Index
			case 'f':	//f - defines the faces
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					face = "";
					std::string VertDef;	//Holds one vertex definition at a time // 3 points
					triangleCount = 0;

					checkChar = fileIn.get();
					while(checkChar != '\n')
					{
						face += checkChar;			//Add the char to our face string
						checkChar = fileIn.get();	//Get the next Character
						if(checkChar == ' ')		//If its a space...
							triangleCount++;		//Increase our triangle count
					}

					//Check for space at the end of our face string
					if(face[face.length()-1] == ' ')
						triangleCount--;	//Each space adds to our triangle count

					triangleCount -= 1;		//Ever vertex in the face AFTER the first two are new faces

					std::stringstream ss(face);

					if(face.length() > 0)
					{
						int firstVIndex, lastVIndex;	//Holds the first and last vertice's index

						for(int i = 0; i < 3; ++i)		//First three vertices (first triangle)
						{
							ss >> VertDef;	//Get vertex definition (vPos/vTexCoord/vNorm) // 1 point

							std::string vertPart;
							int whichPart = 0;		//(vPos, vTexCoord, or vNorm)

							//Parse this string
							for(int j = 0; j < VertDef.length(); ++j)
							{
								if(VertDef[j] != '/')	//If there is no divider "/", add a char to our vertPart
									vertPart += VertDef[j];

								//If the current char is a divider "/", or its the last character in the string
								if(VertDef[j] == '/' || j ==  VertDef.length()-1)
								{
									std::stringstream stringToInt(vertPart);	//Used to convert wstring to int

									if(whichPart == 0)	//If vPos
									{
										stringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1

										//Check to see if the vert pos was the only thing specified
										if(j == VertDef.length()-1)
										{
											vertNormIndexTemp = 0;
											vertTCIndexTemp = 0;
										}
									}

									else if(whichPart == 1)	//If vTexCoord
									{
										if(vertPart != "")	//Check to see if there even is a tex coord
										{
											stringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;	//subtract one since c++ arrays start with 0, and obj start with 1
										}
										else	//If there is no tex coord, make a default
											vertTCIndexTemp = 0;

										//If the cur. char is the second to last in the string, then
										//there must be no normal, so set a default normal
										if(j == VertDef.length()-1)
											vertNormIndexTemp = 0;

									}								
									else if(whichPart == 2)	//If vNorm
									{
										std::stringstream stringToInt(vertPart);

										stringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1
									}

									vertPart = "";	//Get ready for next vertex part
									whichPart++;	//Move on to next vertex part					
								}
							}

							//Check to make sure there is at least one subset
							if(subsetCount == 0)
							{
								subsetIndexStart.push_back(vIndex);		//Start index for this subset
								subsetCount++;
							}

							//Avoid duplicate vertices
							bool vertAlreadyExists = false;
							if(totalVerts >= 3)	//Make sure we at least have one triangle to check
							{
								//Loop through all the vertices
								for(int iCheck = 0; iCheck < totalVerts; ++iCheck)
								{
									//If the vertex position and texture coordinate in memory are the same
									//As the vertex position and texture coordinate we just now got out
									//of the obj file, we will set this faces vertex index to the vertex's
									//index value in memory. This makes sure we don't create duplicate vertices
									if(vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if(vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);		//Set index for this vertex
											vertAlreadyExists = true;		//If we've made it here, the vertex already exists
										}
									}
								}
							}

							//If this vertex is not already in our vertex arrays, put it there
							if(!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;	//We created a new vertex
								indices.push_back(totalVerts-1);	//Set index for this vertex
							}							

							//If this is the very first vertex in the face, we need to
							//make sure the rest of the triangles use this vertex
							if(i == 0)
							{
								firstVIndex = indices[vIndex];	//The first vertex index of this FACE

							}

							//If this was the last vertex in the first triangle, we will make sure
							//the next triangle uses this one (eg. tri1(1,2,3) tri2(1,3,4) tri3(1,4,5))
							if(i == 2)
							{								
								lastVIndex = indices[vIndex];	//The last vertex index of this TRIANGLE
							}
							vIndex++;	//Increment index count
						}

						meshTriangles++;	//One triangle down

						//If there are more than three vertices in the face definition, we need to make sure
						//we convert the face to triangles. We created our first triangle above, now we will
						//create a new triangle for every new vertex in the face, using the very first vertex
						//of the face, and the last vertex from the triangle before the current triangle
						for(int l = 0; l < triangleCount-1; ++l)	//Loop through the next vertices to create new triangles
						{
							//First vertex of this triangle (the very first vertex of the face too)
							indices.push_back(firstVIndex);			//Set index for this vertex
							vIndex++;

							//Second Vertex of this triangle (the last vertex used in the tri before this one)
							indices.push_back(lastVIndex);			//Set index for this vertex
							vIndex++;

							//Get the third vertex for this triangle
							ss >> VertDef;

							std::string vertPart;
							int whichPart = 0;

							//Parse this string (same as above)
							for(int j = 0; j < VertDef.length(); ++j)
							{
								if(VertDef[j] != '/')
									vertPart += VertDef[j];
								if(VertDef[j] == '/' || j ==  VertDef.length()-1)
								{
									std::stringstream stringToInt(vertPart);

									if(whichPart == 0)
									{
										stringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;

										//Check to see if the vert pos was the only thing specified
										if(j == VertDef.length()-1)
										{
											vertTCIndexTemp = 0;
											vertNormIndexTemp = 0;
										}
									}
									else if(whichPart == 1)
									{
										if(vertPart != "")
										{
											stringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if(j == VertDef.length()-1)
											vertNormIndexTemp = 0;

									}								
									else if(whichPart == 2)
									{
										std::stringstream stringToInt(vertPart);

										stringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}

									vertPart = "";
									whichPart++;							
								}
							}					

							//Check for duplicate vertices
							bool vertAlreadyExists = false;
							if(totalVerts >= 3)	//Make sure we at least have one triangle to check
							{
								for(int iCheck = 0; iCheck < totalVerts; ++iCheck)
								{
									if(vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if(vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);			//Set index for this vertex
											vertAlreadyExists = true;		//If we've made it here, the vertex already exists
										}
									}
								}
							}

							if(!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;					//New vertex created, add to total verts
								indices.push_back(totalVerts-1);		//Set index for this vertex
							}

							//Set the second vertex for the next triangle to the last vertex we got		
							lastVIndex = indices[vIndex];	//The last vertex index of this TRIANGLE

							meshTriangles++;	//New triangle defined
							vIndex++;		
						}
					}
				}
				break;

			case 't':	//mtllib - material library filename
				checkChar = fileIn.get();
				if(checkChar == 'e')
				{
					checkChar = fileIn.get();
					if(checkChar == 'x')
					{
						checkChar = fileIn.get();
						if(checkChar == 't')
						{
							checkChar = fileIn.get();
							if(checkChar == 'u')
							{
								checkChar = fileIn.get();
								if(checkChar == 'r')
								{
									checkChar = fileIn.get();
									if(checkChar == 'e')
									{
										checkChar = fileIn.get();
										if(checkChar == ' ')
										{
										//Store the material libraries file name
											fileIn >> textureName;
											subsetTextureFileName.push_back(textureName);
										}
										else// no texture, but still insert an empty string
										{
											subsetTextureFileName.push_back("");
										}
									}
								}
							}
						}
					}
				}

				break;

			default:				
				break;
			}
		}
	}
	else	//If we could not open the file
	{
		//create message
// 		std::string message = "Could not open: ";
// 		message += fileName;
// 
// 		MessageBox(0, message.c_str(),	//display message
// 			L"Error", MB_OK);

		return false;
	}

	subsetIndexStart.push_back(vIndex); //There won't be another index start after our last subset, so set it here

	//sometimes "g" is defined at the very top of the file, then again before the first group of faces.
	//This makes sure the first subset does not conatain "0" indices.
	if(subsetIndexStart[1] == 0)
	{
		subsetIndexStart.erase(subsetIndexStart.begin()+1);
		subsetCount--;
	}

	//Make sure we have a default for the tex coord and normal
	//if one or both are not specified
	if(!hasNorm)
		vertNorm.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	if(!hasTexCoord)
		vertTexCoord.push_back(D3DXVECTOR2(0.0f, 0.0f));

	
	ObjVertex tempVert;

	//Create our vertices using the information we got 
	//from the file and store them in a vector
	for(int j = 0 ; j < totalVerts; ++j)
	{
		tempVert.pos = vertPos[vertPosIndex[j]];
		tempVert.normal = vertNorm[vertNormIndex[j]];
		tempVert.texCoord = vertTexCoord[vertTCIndex[j]];

		vertices.push_back(tempVert);
	}

	//////////////////////Compute Normals///////////////////////////
	//If computeNormals was set to true then we will create our own
	//normals, if it was set to false we will use the obj files normals
	if(computeNormals)
	{
		std::vector<D3DXVECTOR3> tempNormal, tempTangent, tempBinormal;

		//normalized and unnormalized normals
		D3DXVECTOR3 unnormalized = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//Used to get vectors (sides) from the position of the verts
		float vecX, vecY, vecZ;

		//Two edges of our triangle
		D3DXVECTOR3 edge1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 edge2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//Compute face normals	
		//	face binormal
		//	face tangent
		for(int i = 0; i < meshTriangles; ++i)
		{
			//Get the vector describing one edge of our triangle (edge 0,2)
			vecX = vertices[indices[(i*3)]].pos.x - vertices[indices[(i*3)+2]].pos.x;
			vecY = vertices[indices[(i*3)]].pos.y - vertices[indices[(i*3)+2]].pos.y;
			vecZ = vertices[indices[(i*3)]].pos.z - vertices[indices[(i*3)+2]].pos.z;		
			edge1 = D3DXVECTOR3(vecX, vecY, vecZ);	//Create our first edge

			//Get the vector describing another edge of our triangle (edge 2,1)
			vecX = vertices[indices[(i*3)+2]].pos.x - vertices[indices[(i*3)+1]].pos.x;
			vecY = vertices[indices[(i*3)+2]].pos.y - vertices[indices[(i*3)+1]].pos.y;
			vecZ = vertices[indices[(i*3)+2]].pos.z - vertices[indices[(i*3)+1]].pos.z;		
			edge2 = D3DXVECTOR3(vecX, vecY, vecZ);	//Create our second edge

			//Cross multiply the two edge vectors to get the un-normalized face normal
			D3DXVECTOR3 N;
			D3DXVec3Cross(&N, &edge1, &edge2);
			tempNormal.push_back(N);			//Save unormalized normal (for normal averaging)

			D3DXVECTOR2 F, G;
			D3DXVECTOR3 D, E;
			F = vertices[indices[i * 3 + 1]].texCoord - vertices[indices[i * 3]].texCoord;
			G = vertices[indices[i * 3 + 2]].texCoord - vertices[indices[i * 3]].texCoord;
			D = vertices[indices[i * 3 + 1]].pos - vertices[indices[i * 3]].pos;
			E = vertices[indices[i * 3 + 2]].pos - vertices[indices[i * 3]].pos;

			D3DXVECTOR3 T, U, TP, UP;
			T = D / (F.x * G.y - F.y * G.x);
			U = D / (F.x * G.y - F.y * G.x);

			TP = T - D3DXVec3Dot(&N, &T) * N;
			UP = U - D3DXVec3Dot(&N, &U) * N - D3DXVec3Dot(&TP, &U) * TP;
			D3DXVec3Normalize(&T, &TP);
			D3DXVec3Normalize(&U, &UP);
			tempBinormal.push_back(U);
			tempTangent.push_back(T);
		}

		//Compute vertex normals (normal Averaging)
		D3DXVECTOR3 normalSum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 tangentSum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 binormalSum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		int facesUsing = 0;
		float tX;
		float tY;
		float tZ;

		//Go through each vertex
		for(int i = 0; i < totalVerts; ++i)
		{
			//Check which triangles use this vertex
			for(int j = 0; j < meshTriangles; ++j)
			{
				if(indices[j*3] == i ||
					indices[(j*3)+1] == i ||
					indices[(j*3)+2] == i)
				{
					tX = normalSum.x + tempNormal[j].x;
					tY = normalSum.y + tempNormal[j].y;
					tZ = normalSum.z + tempNormal[j].z;
					normalSum = D3DXVECTOR3(tX, tY, tZ);	//If a face is using the vertex, add the unormalized face normal to the normalSum
					
					tX = tangentSum.x + tempTangent[j].x;
					tY = tangentSum.y + tempTangent[j].y;
					tZ = tangentSum.z + tempTangent[j].z;
					tangentSum = D3DXVECTOR3(tX, tY, tZ);
					
					tX = binormalSum.x + tempBinormal[j].x;
					tY = binormalSum.y + tempBinormal[j].y;
					tZ = binormalSum.z + tempBinormal[j].z;
					binormalSum = D3DXVECTOR3(tX, tY, tZ);

					facesUsing++;
				}
			}

			//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
			normalSum = normalSum / facesUsing;
			binormalSum = binormalSum / facesUsing;
			tangentSum = tangentSum / facesUsing;

			//Normalize the normalSum vector
			D3DXVECTOR3 nout;
			D3DXVec3Normalize(&nout, &normalSum);

			//Store the normal in our current vertex
			vertices[i].normal.x = nout.x;
			vertices[i].normal.y = nout.y;
			vertices[i].normal.z = nout.z;

			//Clear normalSum and facesUsing for next vertex
			normalSum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			D3DXVec3Normalize(&nout, &binormalSum);

			//Store the normal in our current vertex
			vertices[i].binormal.x = nout.x;
			vertices[i].binormal.y = nout.y;
			vertices[i].binormal.z = nout.z;

			//Clear normalSum and facesUsing for next vertex
			binormalSum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			D3DXVec3Normalize(&nout, &tangentSum);

			//Store the normal in our current vertex
			vertices[i].tangent.x = nout.x;
			vertices[i].tangent.y = nout.y;
			vertices[i].tangent.z = nout.z;

			//Clear normalSum and facesUsing for next vertex
			tangentSum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			facesUsing = 0;

		}
	}

	return true;
}

ObjVertex* ObjLoader::GetVetexes()
{
	return &vertices[0];
}

unsigned int* ObjLoader::GetIndices()
{
	return &indices[0];
}

int ObjLoader::GetMeshTriangles()
{
	return meshTriangles;
}

int ObjLoader::GetTotVertexes()
{
	return totalVerts;
}

void ObjLoader::GetObjInfo(int* tc, int* tv, int* mt, int*sc)
{
	*tc = triangleCount;
	*tv = totalVerts;
	*mt = meshTriangles;
	*sc = subsetCount;
}