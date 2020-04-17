#include "iSpine.h"

#include "spine/extension.h"
#include "spine/spine.h"

#include "iStd.h"

void initSpine()
{
	_spSetMalloc(malloc);
	_spSetRealloc(realloc);
	_spSetFree(free);


}

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	Texture* tex = createImage(path);

	self->atlas;
	self->format = SP_ATLAS_RGBA8888;
	self->width = tex->width;
	self->height = tex->height;
	self->minFilter = SP_ATLAS_LINEAR;
	self->magFilter = SP_ATLAS_LINEAR;
	self->uWrap = SP_ATLAS_CLAMPTOEDGE;
	self->vWrap = SP_ATLAS_CLAMPTOEDGE;
	self->name;
	//
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{

}

char* _spUtil_readFile(const char* path, int* length)
{
	return _spReadFile(path, length);
	//return loadFile(path, *length);
}
