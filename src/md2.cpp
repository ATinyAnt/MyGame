#include "md2.h"

// vector subtraction
vector_t operator-(vector_t a, vector_t b)
{
	vector_t c;

	c.point[0] = a.point[0] - b.point[0];
	c.point[1] = a.point[1] - b.point[1];
	c.point[2] = a.point[2] - b.point[2];

	return c;
}

// scalar-vector multiplication
vector_t operator*(float f, vector_t b)
{
	vector_t c;

	c.point[0] = f * b.point[0];
	c.point[1] = f * b.point[1];
	c.point[2] = f * b.point[2];

	return c;
}

// vector division
vector_t operator/(vector_t a, vector_t b)
{
	vector_t c;

	c.point[0] = a.point[0] / b.point[0];
	c.point[1] = a.point[1] / b.point[1];
	c.point[2] = a.point[2] / b.point[2];

	return c;
}

// vector addition
vector_t operator+(vector_t a, vector_t b)
{
	vector_t c;

	c.point[0] = a.point[0] + b.point[0];
	c.point[1] = a.point[1] + b.point[1];
	c.point[2] = a.point[2] + b.point[2];

	return c;
}

texture_t *LoadPCXTexture(char *filename);
texture_t *LoadBMPTexture(char *filename);

// CMD2Model constructor
CMD2Model::CMD2Model()
{
     numVertices = 0;    // vertices
     numTriangles = 0;   // triangles
     numFrames = 0;      // frames
     numST = 0;          // texture coordinates
     frameSize = 0;      // needed?
     currentFrame = 0;   // current keyframe 
     nextFrame = 1;      // next keyframe
     interpol = 0.0;     // interpolation percent
     triIndex = NULL;    // triangle indices
     st = NULL;          // texture coordinate indices
     vertexList = NULL;  // vertices

     modelTex = NULL;    // skin/texture
	modelState = MODEL_IDLE;
}    

// CMD2Model destructor
CMD2Model::~CMD2Model()
{
}

// CMD2Model::SetupSkin()
// access: private
// desc: sets up the model skin/texture for OpenGL
void CMD2Model::SetupSkin(texture_t *thisTexture)
{
     // set the proper parameters for an MD2 texture
     glGenTextures(1, &thisTexture->texID);
     glBindTexture(GL_TEXTURE_2D, thisTexture->texID);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     
     switch (thisTexture->textureType)
     {
     case BMP:
          gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, thisTexture->width, thisTexture->height,GL_RGB, GL_UNSIGNED_BYTE, thisTexture->data);
          break;
     case PCX:
          gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, thisTexture->width, thisTexture->height,GL_RGBA, GL_UNSIGNED_BYTE, thisTexture->data);
		  break;
     }
}

// CMD2Model::Load()
// access: public
// desc: loads model and skin
bool CMD2Model::Load(char *modelFile, char *skinFile)
{
     FILE *filePtr;                          // file pointer
     int fileLen;                            // length of model file
     char *buffer;                           // file buffer
     
     modelHeader_t *modelHeader;             // model header
     
     stIndex_t *stPtr;                       // texture data
     frame_t *frame;                              // frame data
     vector_t *vertexListPtr;                // index variable
     mesh_t *bufIndexPtr;          // index variables
     int i, j;                               // index variables
     
     // open the model file
     filePtr = fopen(modelFile, "rb");
     if (filePtr == NULL)
          return FALSE;
     
     // find length of file
     fseek(filePtr, 0, SEEK_END);
     fileLen = ftell(filePtr);
     fseek(filePtr, 0, SEEK_SET);
     
     // read entire file into buffer
     buffer = new char [fileLen+1];
     fread(buffer, sizeof(char), fileLen, filePtr);
     
     // extract model file header from buffer
     modelHeader = (modelHeader_t*)buffer;
     
     vertexList = new vector_t [modelHeader->numXYZ * modelHeader->numFrames];
	 vertexList = new vector_t [modelHeader->numXYZ * modelHeader->numFrames];
     
     numVertices = modelHeader->numXYZ;
     numFrames = modelHeader->numFrames;
     frameSize = modelHeader->framesize;
     
     for (j = 0; j < numFrames; j++)
     {
          frame = (frame_t*)&buffer[modelHeader->offsetFrames + frameSize * j];
          
          vertexListPtr = (vector_t*)&vertexList[numVertices * j];
          for (i = 0; i < numVertices; i++)
          {
               vertexListPtr[i].point[0] = frame->scale[0] * frame->fp[i].v[0] + frame->translate[0];
               vertexListPtr[i].point[1] = frame->scale[1] * frame->fp[i].v[1] + frame->translate[1];
               vertexListPtr[i].point[2] = frame->scale[2] * frame->fp[i].v[2] + frame->translate[2];
          }
     }
     
     modelTex = LoadTexture(skinFile, FT_PCX);
     if (modelTex != NULL)
          SetupSkin(modelTex);
     else
          return false;
     
     numST = modelHeader->numST;
     st = new texCoord_t [numST];
     
     stPtr = (stIndex_t*)&buffer[modelHeader->offsetST];
     for (i = 0; i < numST; i++)
     {
          st[i].s = (float)stPtr[i].s / (float)modelTex->width;
          st[i].t = (float)stPtr[i].t / (float)modelTex->height;
     }
     
     numTriangles = modelHeader->numTris;
     triIndex = new mesh_t [numTriangles];
     
     // point to triangle indexes in buffer
     bufIndexPtr = (mesh_t*)&buffer[modelHeader->offsetTris];
     
     // create a mesh (triangle) list
     for (j = 0; j < numFrames; j++)         
     {
          // for all triangles in each frame
          for(i = 0; i < numTriangles; i++)
          {
               triIndex[i].meshIndex[0] = bufIndexPtr[i].meshIndex[0];
               triIndex[i].meshIndex[1] = bufIndexPtr[i].meshIndex[1];
               triIndex[i].meshIndex[2] = bufIndexPtr[i].meshIndex[2];
               triIndex[i].stIndex[0] = bufIndexPtr[i].stIndex[0];
               triIndex[i].stIndex[1] = bufIndexPtr[i].stIndex[1];
               triIndex[i].stIndex[2] = bufIndexPtr[i].stIndex[2];
          }
     }
     
     // close file and free memory
     fclose(filePtr);
     free(buffer);
     
     currentFrame = 0;
     nextFrame = 1;
     interpol = 0.0;
     
     return true;
}

// CMD2Model::LoadModel()
// access: public
// desc: loads model from file
int CMD2Model::LoadModel(char *modelFile)
{
     FILE *filePtr;                          // file pointer
     int fileLen;                            // length of model file
     char *buffer;                           // file buffer
     
     modelHeader_t *modelHeader;             // model header
     
     stIndex_t *stPtr;                       // texture data
     frame_t *frame;                              // frame data
     vector_t *vertexListPtr;                // index variable
     mesh_t *triIndex, *bufIndexPtr;         // index variables
     int i, j;                               // index variables
     
     // open the model file
     filePtr = fopen(modelFile, "rb");
     if (filePtr == NULL)
          return FALSE;
     
     // find length of file
     fseek(filePtr, 0, SEEK_END);
     fileLen = ftell(filePtr);
     fseek(filePtr, 0, SEEK_SET);
     
     // read entire file into buffer
     buffer = new char [fileLen+1];
     fread(buffer, sizeof(char), fileLen, filePtr);
     
     // extract model file header from buffer
     modelHeader = (modelHeader_t*)buffer;
     
     // allocate vertex list
     vertexList = new vector_t [modelHeader->numXYZ * modelHeader->numFrames];
	 vertexList = new vector_t [modelHeader->numXYZ * modelHeader->numFrames];
     
     numVertices = modelHeader->numXYZ;
     numFrames = modelHeader->numFrames;
     frameSize = modelHeader->framesize;
     
     for (j = 0; j < numFrames; j++)
     {
          frame = (frame_t*)&buffer[modelHeader->offsetFrames + frameSize * j];
          
          vertexListPtr = (vector_t*)&vertexList[numVertices * j];
          for (i = 0; i < numVertices; i++)
          {
               vertexListPtr[i].point[0] = frame->scale[0] * frame->fp[i].v[0] + frame->translate[0];
               vertexListPtr[i].point[1] = frame->scale[1] * frame->fp[i].v[1] + frame->translate[1];
               vertexListPtr[i].point[2] = frame->scale[2] * frame->fp[i].v[2] + frame->translate[2];
          }
     }
     
     numST = modelHeader->numST;
     
     st = new texCoord_t [numST];
     
     stPtr = (stIndex_t*)&buffer[modelHeader->offsetST];
     for (i = 0; i < numST; i++)
     {
          st[i].s = 0.0;
          st[i].t = 0.0;
     }
     
     numTriangles = modelHeader->numTris;
     triIndex = new mesh_t [numTriangles];
     
     // point to triangle indexes in buffer
     bufIndexPtr = (mesh_t*)&buffer[modelHeader->offsetTris];
     
     // create a mesh (triangle) list
     for (j = 0; j < numFrames; j++)         
     {
          // for all triangles in each frame
          for(i = 0; i < numTriangles; i++)
          {
               triIndex[i].meshIndex[0] = bufIndexPtr[i].meshIndex[0];
               triIndex[i].meshIndex[1] = bufIndexPtr[i].meshIndex[1];
               triIndex[i].meshIndex[2] = bufIndexPtr[i].meshIndex[2];
               triIndex[i].stIndex[0] = bufIndexPtr[i].stIndex[0];
               triIndex[i].stIndex[1] = bufIndexPtr[i].stIndex[1];
               triIndex[i].stIndex[2] = bufIndexPtr[i].stIndex[2];
          }
     }
     
     // close file and free memory
     fclose(filePtr);
     
     modelTex = NULL;
     currentFrame = 0;
     nextFrame = 1;
     interpol = 0.0;
     
     return 0;
}

// CMD2Model::SetTexture()
// access: public
// desc: sets a new texture object 
int CMD2Model::SetTexture(texture_t *texture)
{
     int i;
     
     if (texture != NULL)
	{
		free(modelTex);
          modelTex = texture;
	}
     else
          return -1;
     
     SetupSkin(modelTex);
     
     for (i = 0; i < numST; i++)
     {
          st[i].s /= (float)modelTex->width;
          st[i].t /= (float)modelTex->height;
     }
     
     return 0;
}

// CMD2Model::Animate()
// access: public
// desc: animates the model between the keyframes startFrame and endFrame
int CMD2Model::Animate(int startFrame, int endFrame, float percent)
{
     vector_t *vList;              // current frame vertices
     vector_t *nextVList;          // next frame vertices
     int i;                                  // index counter
     float x1, y1, z1;                  // current frame point values
     float x2, y2, z2;                  // next frame point values
     
     vector_t vertex[3]; 
     
     if ((startFrame > currentFrame))
          currentFrame = startFrame;
     
     if ((startFrame < 0) || (endFrame < 0))
          return -1;
     
     if ((startFrame >= numFrames) || (endFrame >= numFrames))
          return -1;
     
     if (interpol >= 1.0)
     {
          interpol = 0.0f;
          currentFrame++;
          if (currentFrame >= endFrame)
               currentFrame = startFrame;
          
          nextFrame = currentFrame + 1;
          
          if (nextFrame >= endFrame)
               nextFrame = startFrame;
          
     }
     
     vList = &vertexList[numVertices*currentFrame];
	 nextVList = &vertexList[numVertices*nextFrame];
     
     glBindTexture(GL_TEXTURE_2D, modelTex->texID);
     glBegin(GL_TRIANGLES);
     for (i = 0; i < numTriangles; i++)
     {
          // get first points of each frame
          x1 = vList[triIndex[i].meshIndex[0]].point[0];
          y1 = vList[triIndex[i].meshIndex[0]].point[1];
          z1 = vList[triIndex[i].meshIndex[0]].point[2];
          x2 = nextVList[triIndex[i].meshIndex[0]].point[0];
          y2 = nextVList[triIndex[i].meshIndex[0]].point[1];
          z2 = nextVList[triIndex[i].meshIndex[0]].point[2];
          
          // store first interpolated vertex of triangle
          vertex[0].point[0] = x1 + interpol * (x2 - x1);
          vertex[0].point[1] = y1 + interpol * (y2 - y1);
          vertex[0].point[2] = z1 + interpol * (z2 - z1);
          
          // get second points of each frame
          x1 = vList[triIndex[i].meshIndex[2]].point[0];
          y1 = vList[triIndex[i].meshIndex[2]].point[1];
          z1 = vList[triIndex[i].meshIndex[2]].point[2];
          x2 = nextVList[triIndex[i].meshIndex[2]].point[0];
          y2 = nextVList[triIndex[i].meshIndex[2]].point[1];
          z2 = nextVList[triIndex[i].meshIndex[2]].point[2];
          
          // store second interpolated vertex of triangle
          vertex[2].point[0] = x1 + interpol * (x2 - x1);
          vertex[2].point[1] = y1 + interpol * (y2 - y1);
          vertex[2].point[2] = z1 + interpol * (z2 - z1);   
          
          // get third points of each frame
          x1 = vList[triIndex[i].meshIndex[1]].point[0];
          y1 = vList[triIndex[i].meshIndex[1]].point[1];
          z1 = vList[triIndex[i].meshIndex[1]].point[2];
          x2 = nextVList[triIndex[i].meshIndex[1]].point[0];
          y2 = nextVList[triIndex[i].meshIndex[1]].point[1];
          z2 = nextVList[triIndex[i].meshIndex[1]].point[2];
          
          // store third interpolated vertex of triangle
          vertex[1].point[0] = x1 + interpol * (x2 - x1);
          vertex[1].point[1] = y1 + interpol * (y2 - y1);
          vertex[1].point[2] = z1 + interpol * (z2 - z1);
          
          // calculate the normal of the triangle
          CalculateNormal(vertex[0].point, vertex[2].point, vertex[1].point);
          
          // render properly textured triangle
          glTexCoord2f(st[triIndex[i].stIndex[0]].s,
               st[triIndex[i].stIndex[0]].t);
          glVertex3fv(vertex[0].point);
          
          glTexCoord2f(st[triIndex[i].stIndex[2]].s ,
               st[triIndex[i].stIndex[2]].t);
          glVertex3fv(vertex[2].point);
          
          glTexCoord2f(st[triIndex[i].stIndex[1]].s,
               st[triIndex[i].stIndex[1]].t);
          glVertex3fv(vertex[1].point);
     }
     glEnd();
     
     interpol += percent;  // increase percentage of interpolation between frames
     
     return 0;
}

// RenderFrame()
// desc: renders a single key frame
int CMD2Model::RenderFrame(int keyFrame)
{
     vector_t *vList;
     int i;
     
     // create a pointer to the frame we want to show
     vList = &vertexList[numVertices * keyFrame];
     
	if (modelTex != NULL)
		// set the texture
		glBindTexture(GL_TEXTURE_2D, modelTex->texID);
     
     // display the textured model with proper lighting normals
     glBegin(GL_TRIANGLES);
     for(i = 0; i < numTriangles; i++)
     {
          CalculateNormal(vList[triIndex[i].meshIndex[0]].point,
               vList[triIndex[i].meshIndex[2]].point,
               vList[triIndex[i].meshIndex[1]].point);

		if (modelTex != NULL)
			glTexCoord2f(st[triIndex[i].stIndex[0]].s,
				st[triIndex[i].stIndex[0]].t);

          glVertex3fv(vList[triIndex[i].meshIndex[0]].point);
          
		if (modelTex != NULL)
			glTexCoord2f(st[triIndex[i].stIndex[2]].s ,
				st[triIndex[i].stIndex[2]].t);

          glVertex3fv(vList[triIndex[i].meshIndex[2]].point);
          
		if (modelTex != NULL)
			glTexCoord2f(st[triIndex[i].stIndex[1]].s,
				st[triIndex[i].stIndex[1]].t);

          glVertex3fv(vList[triIndex[i].meshIndex[1]].point);
     }
     glEnd();
     
     return 0;
}

// Unload()
// desc: unloads model data from memory
int CMD2Model::Unload()
{
     if (triIndex != NULL)
          free(triIndex);
     if (vertexList != NULL)
          free(vertexList);
     if (st != NULL)
          free(st);
     
     return 0;
}

// SetState()
// desc: set the model state
int CMD2Model::SetState(modelState_t state)
{
	modelState = state;
	return 0;
}

// GetState()
// desc: retrieve the model state
modelState_t CMD2Model::GetState()
{
	return modelState;
}


bool CMD2Model::init(CMD2Model &md2)
{
	//triIndex=new char[sizeof(md2.triIndex)];
	triIndex=new mesh_t[sizeof(md2.triIndex)];
	//st=new char[sizeof(md2.st)];
	st=new texCoord_t[sizeof(md2.st)];
	vertexList=new vector_t[sizeof(md2.vertexList)];
	modelTex=new texture_t[sizeof(md2).modelTex];
	memcpy(triIndex,md2.triIndex,sizeof(md2.triIndex));
	memcpy(st,md2.st,sizeof(md2.st));
	memcpy(vertexList,md2.vertexList,sizeof(md2.vertexList));
	memcpy(modelTex,md2.modelTex,sizeof(md2.modelTex));
	return true;
}

int *CMD2Model::GetFrame(modelState_t state)
{
	int *returnstate=new int[2];
	switch (state)
	{
	case MODEL_IDLE:
		returnstate[0]=0;
		returnstate[1]=39;
		break;
	case MODEL_RUN:
		returnstate[0]=40;
		returnstate[1]=46;
		break;
	case MODEL_ATTACK:
		returnstate[0]=46;
		returnstate[1]=53;
		break;
	case MODEL_PAIN1:
		returnstate[0]=54;
		returnstate[1]=57;
		break;
	case MODEL_PAIN2:
		returnstate[0]=58;
		returnstate[1]=61;
		break;
	case MODEL_PAIN3:
		returnstate[0]=62;
		returnstate[1]=65;
		break;
	case MODEL_JUMP:
		returnstate[0]=66;
		returnstate[1]=72;
		break;
	case MODEL_FLIPOFF:
		returnstate[0]=72;
		returnstate[1]=83;
		break;
	case MODEL_SAULTE:
		returnstate[0]=84;
		returnstate[1]=94;
		break;
	case MODEL_TAUNT:
		returnstate[0]=95;
		returnstate[1]=111;
		break;
	case MODEL_WAVE:
		returnstate[0]=112;
		returnstate[1]=122;
		break;
	case MODEL_POINT:
		returnstate[0]=123;
		returnstate[1]=134;
		break;
	case MODEL_CROUCH_IDLE:
		returnstate[0]=135;
		returnstate[1]=153;
		break;
	case MODEL_CROUCH_WALK:
		returnstate[0]=154;
		returnstate[1]=159;
		break;
	case MODEL_CROUCH_ATTACK:
		returnstate[0]=160;
		returnstate[1]=168;
		break;
	case MODEL_CROUCH_PAIN:
		returnstate[0]=169;
		returnstate[1]=172;
		break;
	case MODEL_MODEL_CROUCH_DEATH:
		returnstate[0]=173;
		returnstate[1]=177;
		break;
	case MODEL_DEATH1:
		returnstate[0]=178;
		returnstate[1]=183;
		break;
	case MODEL_DEATH2:
		returnstate[0]=184;
		returnstate[1]=189;
		break;
	case MODEL_DEATH3:
		returnstate[0]=190;
		returnstate[1]=197;
		break;
// 	case MODEL_REPEAT:
// 		returnstate[0]=0;
// 		returnstate[1]=39;
// 		break;
// 	case MODEL_CUSTOM:
// 		returnstate[0]=0;
// 		returnstate[1]=39;
// 		break;
	default:
		break;
	}
	return returnstate;
}

texture_t * CMD2Model::LoadTexture(char *filename, int file_type)
{
	texture_t *thisTexture = NULL;

	switch (file_type)
	{
	case FT_BMP:
		thisTexture = LoadBMPTexture(filename);
		break;
	case FT_PCX:
		thisTexture = LoadPCXTexture(filename);
		break;
	default:
		return NULL;
	}

	return thisTexture;
}



unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
	unsigned char		*bitmapImage;			// bitmap image data
	unsigned int		imageIdx = 0;		// image index counter
	unsigned char		tempRGB;				// swap variable
	
	// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;
	
	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}
	
	// read the bitmap information header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	
	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	
	// allocate enough memory for the bitmap image data
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);
	
	// verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}
	
	// read in the bitmap image data
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);
	
	// make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}
	
	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	
	// close the file and return the bitmap image data
	fclose(filePtr);
	return bitmapImage;
}


texture_t *LoadBMPTexture(char *filename)
{
	BITMAPINFOHEADER texInfo;		// BMP header
	texture_t *thisTexture;			// the texture
	
	// allocate memory for the texture
	thisTexture = (texture_t*)malloc(sizeof(texture_t));
	if (thisTexture == NULL)
		return NULL;
	
	// store BMP data in texture
	thisTexture->data = LoadBitmapFile(filename, &texInfo);
	if (thisTexture->data == NULL)
	{
		free(thisTexture);
		return NULL;
	}
	
	// store texture information
	thisTexture->width = texInfo.biWidth;
	thisTexture->height = texInfo.biHeight;
	thisTexture->palette = NULL;
	thisTexture->scaledHeight = 0;
	thisTexture->scaledWidth = 0;
	thisTexture->textureType = BMP;
	
	return thisTexture;
}



unsigned char *LoadPCXFile(char *filename, PCXHEADER *pcxHeader)
{
     int idx = 0;                  // counter index
     int c;                             // used to retrieve a char from the file
     int i;                             // counter index
     int numRepeat;      
     FILE *filePtr;                // file handle
     int width;                         // pcx width
     int height;                        // pcx height
     unsigned char *pixelData;     // pcx image data
     unsigned char *paletteData;   // pcx palette data

     // open PCX file
     filePtr = fopen(filename, "rb");
     if (filePtr == NULL)
          return NULL;

     // retrieve first character; should be equal to 10
     c = getc(filePtr);
     if (c != 10)
     {
          fclose(filePtr);
          return NULL;
     }

     // retrieve next character; should be equal to 5
     c = getc(filePtr);
     if (c != 5)
     {
          fclose(filePtr);
          return NULL;
     }

     // reposition file pointer to beginning of file
     rewind(filePtr);

     // read 4 characters of data to skip
     fgetc(filePtr);
     fgetc(filePtr);
     fgetc(filePtr);
     fgetc(filePtr);

     // retrieve leftmost x value of PCX
     pcxHeader->xMin = fgetc(filePtr);       // loword
     pcxHeader->xMin |= fgetc(filePtr) << 8; // hiword

     // retrieve bottom-most y value of PCX
     pcxHeader->yMin = fgetc(filePtr);       // loword
     pcxHeader->yMin |= fgetc(filePtr) << 8; // hiword

     // retrieve rightmost x value of PCX
     pcxHeader->xMax = fgetc(filePtr);       // loword
     pcxHeader->xMax |= fgetc(filePtr) << 8; // hiword

     // retrieve topmost y value of PCX
     pcxHeader->yMax = fgetc(filePtr);       // loword
     pcxHeader->yMax |= fgetc(filePtr) << 8; // hiword

     // calculate the width and height of the PCX
     width = pcxHeader->xMax - pcxHeader->xMin + 1;
     height = pcxHeader->yMax - pcxHeader->yMin + 1;

     // allocate memory for PCX image data
     pixelData = (unsigned char*)malloc(width*height);

     // set file pointer to 128th byte of file, where the PCX image data starts
     fseek(filePtr, 128, SEEK_SET);
     
     // decode the pixel data and store
     while (idx < (width*height))
     {
          c = getc(filePtr);
          if (c > 0xbf)
          {
               numRepeat = 0x3f & c;
               c = getc(filePtr);

               for (i = 0; i < numRepeat; i++)
               {
                    pixelData[idx++] = c;
               }
          }
          else
               pixelData[idx++] = c;

          fflush(stdout);
     }

     // allocate memory for the PCX image palette
     paletteData = (unsigned char*)malloc(768);

     // palette is the last 769 bytes of the PCX file
     fseek(filePtr, -769, SEEK_END);

     // verify palette; first character should be 12
     c = getc(filePtr);
     if (c != 12)
     {
          fclose(filePtr);
          return NULL;
     }

     // read and store all of palette
     for (i = 0; i < 768; i++)
     {
          c = getc(filePtr);
          paletteData[i] = c;
     }

     // close file and store palette in header
     fclose(filePtr);
     pcxHeader->palette = paletteData;

     // return the pixel image data
     return pixelData;
}



texture_t *LoadPCXTexture(char *filename)
{
	PCXHEADER texInfo;            // header of texture
	texture_t *thisTexture;       // the texture
	unsigned char *unscaledData;// used to calculate pcx
	int i;                             // index counter
	int j;                             // index counter
	int width;                         // width of texture
	int height;                        // height of texture
	
	// allocate memory for texture struct
	thisTexture = (texture_t*)malloc(sizeof(texture_t));
	if (thisTexture == NULL)
		return NULL;
	
	// load the PCX file into the texture struct
	thisTexture->data = LoadPCXFile(filename, &texInfo);
	if (thisTexture->data == NULL)
	{
		free(thisTexture->data);
		return NULL;
	}
	
	// store the texture information
	thisTexture->palette = texInfo.palette;
	thisTexture->width = texInfo.xMax - texInfo.xMin + 1;
	thisTexture->height = texInfo.yMax - texInfo.yMin + 1;
	thisTexture->textureType = PCX;
	
	// allocate memory for the unscaled data
	unscaledData = (unsigned char*)malloc(thisTexture->width*thisTexture->height*4);
	
	// store the unscaled data via the palette
	for (j = 0; j < thisTexture->height; j++) 
	{
		for (i = 0; i < thisTexture->width; i++) 
		{
			unscaledData[4*(j*thisTexture->width+i)+0] = (unsigned char)thisTexture->palette[3*thisTexture->data[j*thisTexture->width+i]+0];
			unscaledData[4*(j*thisTexture->width+i)+1] = (unsigned char)thisTexture->palette[3*thisTexture->data[j*thisTexture->width+i]+1];
			unscaledData[4*(j*thisTexture->width+i)+2] = (unsigned char)thisTexture->palette[3*thisTexture->data[j*thisTexture->width+i]+2];
			unscaledData[4*(j*thisTexture->width+i)+3] = (unsigned char)255;
		}
	}
	
	// find width and height's nearest greater power of 2
	width = thisTexture->width;
	height = thisTexture->height;
	
	// find width's
	i = 0;
	while (width)
	{
		width /= 2;
		i++;
	}
	thisTexture->scaledHeight = (long)pow(2.0, i-1);
	
	// find height's
	i = 0;
	while (height)
	{
		height /= 2;
		i++;
	}
	thisTexture->scaledWidth = (long)pow(2.0, i-1);
	
	// clear the texture data
	if (thisTexture->data != NULL)
	{
		free(thisTexture->data);
		thisTexture->data = NULL;
	}
	
	// reallocate memory for the texture data
	thisTexture->data = (unsigned char*)malloc(thisTexture->scaledWidth*thisTexture->scaledHeight*4);
	
	// use the GL utility library to scale the texture to the unscaled dimensions
	gluScaleImage (GL_RGBA, thisTexture->width, thisTexture->height, GL_UNSIGNED_BYTE, unscaledData, thisTexture->scaledWidth, thisTexture->scaledHeight, GL_UNSIGNED_BYTE, thisTexture->data);
	
	return thisTexture;
}


void CalculateNormal( float *p1, float *p2, float *p3 )
{
	float a[3], b[3], result[3];
	float length;

	a[0] = p1[0] - p2[0];
	a[1] = p1[1] - p2[1];
	a[2] = p1[2] - p2[2];

	b[0] = p1[0] - p3[0];
	b[1] = p1[1] - p3[1];
	b[2] = p1[2] - p3[2];

	result[0] = a[1] * b[2] - b[1] * a[2];
	result[1] = b[0] * a[2] - a[0] * b[2];
	result[2] = a[0] * b[1] - b[0] * a[1];

	// calculate the length of the normal
	length = (float)sqrt(result[0]*result[0] + result[1]*result[1] + result[2]*result[2]);

	// normalize and specify the normal
	glNormal3f(result[0]/length, result[1]/length, result[2]/length);
}