u32 test;

#include "string.c"
#include "math.c"

typedef struct
{
    int p_indices[3];
    int uv_indices[3];
}
OBJFace;

typedef struct
{
    int p_count;
    Vec3 ps[10000];
    
    int uv_count;
    Vec2 uvs[10000];
    
    int face_count;
    OBJFace faces[10000];
}
OBJData;

static void
add_obj(AssetFileHeader *buffer, u32 *bytes_filled, OBJData *obj, char *filename, MeshID slot_index)
{
    FileContents obj_file_contents = platform_read_entire_file(filename);
    
    // TODO: Can we determine how much memory to allocate by the size of the file?
    obj->p_count = 0;
    obj->uv_count = 0;
    obj->face_count = 0;
    
    char *string = (char *)obj_file_contents.memory;
    while (*string)
    {
        if (*string == '#')
        {
            while (*string != '\n')
            {
                ++string;
            }
            ++string;
            continue;
        }
        
        if (*string == 'v')
        {
            ++string;
            if (*string == ' ')
            {
                // NOTE: Now we know we're on a vertex position line.
                for (int component = 0; component < 3; ++component)
                {
                    while (*string == ' ')
                    {
                        ++string;
                    }
                    
                    obj->ps[obj->p_count].e[component] = ascii_to_float(string);
                    
                    while (*string != ' ' && *string != '\n')
                    {
                        ++string;
                    }
                }
                
#if 0
                // NOTE: Ensure we didn't just add a duplicate vertex position.
                float *v1 = game_state->vertices[vertex_count];
                for (int i = 0; i < vertex_count; ++i)
                {
                    float *v2 = game_state->vertices[i];
                    if (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2])
                    {
                        vertex_count += -1;
                        
                        for ();
                        
                        break;
                    }
                }
#endif
                
                ++obj->p_count;
                // TODO: Clean this process up.
                continue;
            }
            else if (*string == 't')
            {
                // NOTE: Now we know we're on a UV line.
                for (int component = 0; component < 2; ++component)
                {
                    while (!is_digit(*string))
                    {
                        ++string;
                    }
                    
                    obj->uvs[obj->uv_count].e[component] = ascii_to_float(string);
                    
                    while (*string != ' ' && *string != '\n')
                    {
                        ++string;
                    }
                }
                
                ++obj->uv_count;
                // TODO: Clean this process up.
                continue;
            }
        }
        else if (*string == 'f')
        {
            ++string;
            if (*string == ' ')
            {
                // NOTE: Now we know we're on a face indices line.
                for (int i = 0; i < 3; ++i)
                {
                    while (!is_digit(*string))
                    {
                        ++string;
                    }
                    
                    obj->faces[obj->face_count].p_indices[i] = ascii_to_int(string) - 1;
                    while (is_digit(*string))
                    {
                        ++string;
                    }
                    
                    if (*string == '/')
                    {
                        ++string;
                        if (is_digit(*string))
                        {
                            // NOTE: Texture coordinate index.
                            obj->faces[obj->face_count].uv_indices[i] = ascii_to_int(string) - 1;
                            while (is_digit(*string))
                            {
                                ++string;
                            }
                        }
                    }
                    
                    while (*string != ' ' && *string != '\n')
                    {
                        ++string;
                    }
                }
                // TODO: Clean this process up.
                ++obj->face_count;
                continue;
            }
        }
        
        ++string;
    }
    
    // NOTE: Done parsing the file!
    platform_free_file_memory(obj_file_contents.memory);
    
#if 0
    for (int i = 0; i < vertex_count; ++i)
    {
        float *v1 = game_state->vertices[i];
        for (int j = 0; j < vertex_count; ++j)
        {
            float *v2 = game_state->vertices[j];
            if (i != j)
            {
                if (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2])
                {
                    ASSERT(0);
                }
            }
        }
    }
#endif 
    
    u8 *write_position = (u8 *)buffer + *bytes_filled;
    PackedMeshHeader *mesh_header = (PackedMeshHeader *)write_position;
    mesh_header->index_count = obj->face_count * 3;
    
    u16 *indices = (u16 *)(write_position + sizeof(PackedMeshHeader));
    Vertex *verts = (Vertex *)(indices + mesh_header->index_count);
    
    u16 vert_count = 0;
    
    for (int f = 0; f < obj->face_count; ++f)
    {
        OBJFace a = obj->faces[f];
        Vec3 a_face_n = get_tri_normal(obj->ps[a.p_indices[0]],
                                       obj->ps[a.p_indices[1]],
                                       obj->ps[a.p_indices[2]]);
        
        for (int i = 0; i < 3; ++i)
        {
            // TODO: Loop over all other vert positions to determine if this is a duplicate vert position. If so, change the index to refer to the earlier one.
            
            Vec3 vert_p = obj->ps[a.p_indices[i]];
            Vec2 vert_uv = obj->uvs[a.uv_indices[i]];
            Vec3 vert_n = a_face_n;
            
            for (int j = 0; j < obj->face_count; ++j)
            {
                if (f != j)
                {
                    OBJFace b = obj->faces[j];
                    
                    if (b.p_indices[0] == a.p_indices[i] || b.p_indices[1] == a.p_indices[i] || b.p_indices[2] == a.p_indices[i])
                    {
                        Vec3 b_face_n = get_tri_normal(obj->ps[b.p_indices[0]],
                                                       obj->ps[b.p_indices[1]],
                                                       obj->ps[b.p_indices[2]]);
                        
                        float cos_theta = vec3_dot(a_face_n, b_face_n);
                        if (cos_theta > 1.0f)
                        {
                            vert_n = vec3_add(vert_n, b_face_n);
                        }
                    }
                }
            }
            
            vert_n = vec3_norm(vert_n);
            
            // NOTE: We now know what position, uv, and normal this triangle wants from this vertex. We can now check if we already have a vertex like that.
            b32 duplicate_found = false;
            for (u16 l = 0; l < vert_count; ++l)
            {
                // TODO: Check within a certain threshold instead?
                if (verts[l].px == vert_p.x &&
                    verts[l].py == vert_p.y &&
                    verts[l].pz == vert_p.z &&
                    verts[l].nx == vert_n.x &&
                    verts[l].ny == vert_n.y &&
                    verts[l].nz == vert_n.z &&
                    verts[l].uvx == vert_uv.x &&
                    verts[l].uvy == vert_uv.y)
                {
                    *indices = l;
                    ++indices;
                    duplicate_found = true;
                    break;
                }
            }
            
            if (!duplicate_found)
            {
                verts[vert_count].px = vert_p.x;
                verts[vert_count].py = vert_p.y;
                verts[vert_count].pz = vert_p.z;
                verts[vert_count].nx = vert_n.x;
                verts[vert_count].ny = vert_n.y;
                verts[vert_count].nz = vert_n.z;
                verts[vert_count].uvx = vert_uv.x;
                verts[vert_count].uvy = vert_uv.y;
                
                *indices = vert_count;
                ++indices;
                
                ++vert_count;
            }
        }
    }
    
    mesh_header->vert_count = vert_count;
    u32 size = sizeof(PackedMeshHeader) + (mesh_header->index_count * sizeof(u16)) + (mesh_header->vert_count * sizeof(Vertex));
    
    TableOfContentsEntry *entry = buffer->table_of_contents + slot_index;
    entry->offset = *bytes_filled;
    entry->size = size;
    
    *bytes_filled += size;
}
