/*
TODO:
- Consolidate meshes and textures into the same single file?
- Automatically grab blender / gimp files???
*/

#include "windows.h"
#include "platform.h"
#include "win32_file_io.c"
#include "win32_crt_float.c"

#include "asset_file_format.h"

#include "image.c"
#include "mesh.c"

#define MAX_FILE_SIZE (u64)2*1024*1024*1024

void WinMainCRTStartup(void)
{
    // NOTE: Allocate maximum size up front, fill that buffer, then write out to disk whatever portion of it we actually filled.
    void *buffer = VirtualAlloc(0, MAX_FILE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    
    // NOTE: Allocate transient memory (re-used each time we load a mesh) to store intermediate data representations.
    //OBJData *obj_data = VirtualAlloc(0, sizeof(OBJData), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    
    AssetFileHeader *file_header = (AssetFileHeader *)buffer;
    u32 bytes_filled = sizeof(AssetFileHeader);
    
    add_bmp(file_header, &bytes_filled, "w:/bundler/dat/fir_test.bmp", TEXTURE_ID_tree);
    add_bmp(file_header, &bytes_filled, "w:/bundler/dat/softcube_texture.bmp", TEXTURE_ID_player);
    //add_obj(file_header, &bytes_filled, obj_data, "w:/bundler/dat/fir.obj", MESH_ID_tree);
    //add_obj(file_header, &bytes_filled, obj_data, "w:/bundler/dat/softcube.obj", MESH_ID_player);
    
    platform_write_entire_file("textures.packed", bytes_filled, buffer);
}
