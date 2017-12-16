#define MAX_ASSETS_IN_A_FILE 256 // TODO: This is dumb.

// TODO: We need some kind of "key" for the table of contents. An enum works, but we need to think of something better.
typedef enum
{
    // TODO: Reserve a null asset spot?
    MESH_ID_player,
    MESH_ID_tree,
    MESH_ID_rock,
    
    MESH_ID_count
}
MeshID;

typedef enum
{
    // TODO: Reserve a null asset spot?
    TEXTURE_ID_player,
    TEXTURE_ID_tree,
    TEXTURE_ID_rock,
    
    TEXTURE_ID_count
}
TextureID;

#pragma pack(push, 1)
typedef struct
{
    // NOTE: For now, we'll assume that our asset file will not go above 4GB.
    u32 offset;
    u32 size;
}
TableOfContentsEntry;
#pragma pack(pop)

// TODO: Can we really not envelope these in one pragma thing?

#pragma pack(push, 1)
typedef struct
{
    TableOfContentsEntry table_of_contents[MAX_ASSETS_IN_A_FILE]; // TODO: This count will be different for different files...
    // NOTE: Actual data starts here!
}
AssetFileHeader;
#pragma pack(pop)

// NOTE: Mesh format stuff!
#pragma pack(push, 1)
typedef struct
{
    u32 index_count;
    u32 vert_count;
    // NOTE: Index and vertex data starts here!
    // uint16 indices[];
    // Vertex verts[];
}
PackedMeshHeader;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    float px, py, pz;
    float nx, ny, nz;
    float uvx, uvy;
}
Vertex;
#pragma pack(pop)

// NOTE: Bitmap format stuff!
#pragma pack(push, 1)
typedef struct
{
    u32 width;
    u32 height;
}
PackedTextureHeader;
#pragma pack(pop)
