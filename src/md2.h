#ifndef __MD2_H
#define __MD2_H

#include "stdafx.h"
// #include <windows.h>			// standard Windows app include
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include <gl/gl.h>				// standard OpenGL include
// #include <gl/glu.h>				// OpenGL utilties

#define MAX_FRAMES 512

// types of texture file formats
enum texTypes_t
{
	PCX,
	BMP,
	TGA
};

// model animation states
enum modelState_t
{
	MODEL_IDLE,		// idle animation
	MODEL_RUN,		// running animation	
	MODEL_ATTACK,//46-53	
	MODEL_PAIN1,
	MODEL_PAIN2,
	MODEL_PAIN3,
	MODEL_JUMP,		// jumping animation
	MODEL_FLIPOFF,	//有点像打的动作
	MODEL_SAULTE,	//手指天
	MODEL_TAUNT,	//胜利庆祝
	MODEL_WAVE,		
	MODEL_POINT,	//打，动作有点不协调
	MODEL_CROUCH_IDLE,	//蹲下
	MODEL_CROUCH_WALK,
	MODEL_CROUCH_ATTACK,
	MODEL_CROUCH_PAIN,
	MODEL_MODEL_CROUCH_DEATH,
	MODEL_DEATH1,
	MODEL_DEATH2,
	MODEL_DEATH3,
	MODEL_REPEAT,
	MODEL_CUSTOM,
};

typedef struct 
{
	// C Compatible version: enum texTypes textureType;
	// We use C++ only version
	texTypes_t textureType;

	int width;					// width of texture
	int height;					// height of texture
	long int scaledWidth;
	long int scaledHeight;

	unsigned int texID;			// the texture object id of this texture
	unsigned char *data;		// the texture data
	unsigned char *palette;
} texture_t;

// only partial pcx file header
typedef struct
{
	unsigned char manufacturer;
	unsigned char version;
	unsigned char encoding;
	unsigned char bits;
	unsigned char xMin;
	unsigned char yMin;
	unsigned char xMax;
	unsigned char yMax;
	unsigned char *palette;
} PCXHEADER;


/*
	Vector Functionality
*/
// a single vertex
typedef struct
{
   float point[3];
} vector_t;

vector_t operator-(vector_t a, vector_t b);
vector_t operator*(float f, vector_t b);
vector_t operator/(vector_t a, vector_t b);
vector_t operator+(vector_t a, vector_t b);

void CalculateNormal( float *p1, float *p2, float *p3 );


/* 
	MD2 Model Functionality
*/


// texture coordinate
typedef struct
{
   float s;
   float t;
} texCoord_t;

// texture coordinate index
typedef struct
{
   short s;
   short t;
} stIndex_t;

// info for a single frame point
typedef struct
{
   unsigned char v[3];
   unsigned char normalIndex;	// not used
} framePoint_t;

// information for a single frame
typedef struct
{
   float scale[3];
   float translate[3];
   char name[16];
   framePoint_t fp[1];
} frame_t;

// data for a single triangle
typedef struct
{
   unsigned short meshIndex[3];		// vertex indices
   unsigned short stIndex[3];		// texture coordinate indices
} mesh_t;

typedef struct
{
   int ident;		 // identifies as MD2 file "IDP2"
   int version;	 // mine is 8
   int skinwidth;    // width of texture
   int skinheight;   // height of texture
   int framesize;    // number of bytes per frame
   int numSkins;     // number of textures
   int numXYZ;       // number of points
   int numST;        // number of texture
   int numTris;      // number of triangles
   int numGLcmds;
   int numFrames;    // total number of frames
   int offsetSkins;  // offset to skin names (64 bytes each)
   int offsetST;     // offset of texture s-t values
   int offsetTris;   // offset of triangle mesh
   int offsetFrames; // offset of frame data (points)
   int offsetGLcmds; // type of OpenGL commands to use
   int offsetEnd;    // end of file
} modelHeader_t;


class CMD2Model
{
private:

     int numFrames;			// number of model frames
     int numVertices;         // number of vertices
     int numTriangles;        // number of triangles
     int numST;               // number of skins
     int frameSize;           // size of each frame in bytes
     int currentFrame;        // current frame # in animation
     int nextFrame;           // next frame # in animation
     float interpol;          // percent through current frame
     mesh_t *triIndex;        // triangle list
     texCoord_t *st;          // texture coordinate list
     vector_t *vertexList;    // vertex list
     texture_t *modelTex;     // texture data

	modelState_t modelState;	// current model animation state

     void SetupSkin(texture_t *thisTexture);

public:

     CMD2Model();        // constructor
     ~CMD2Model();       // destructor

     // load model and skin/texture at the same time
     bool Load(char *modelFile, char *skinFile);

     // load model only
     int LoadModel(char *modelFile);

     // set model's texture/skin
     int SetTexture(texture_t *texture);

     // render model with interpolation to get animation
     int Animate(int startFrame, int endFrame, float percent);

     // render a single frame
     int RenderFrame(int keyFrame);

     // free memory of model
     int Unload();

	// set animation state of model
	int SetState(modelState_t state);
	
	//bool SetSpeed(int speed);
	//用于初始化其它
	bool init(CMD2Model &md2);

	int *GetFrame(modelState_t state);
	// retrieve animation state of model
	modelState_t GetState();

	enum
	{
		FT_BMP,
		FT_PCX
	};
private:
	texture_t *LoadTexture(char *filename, int file_type);
};

#define BITMAP_ID 0x4D42		// the universal bitmap ID
#endif