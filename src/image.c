#pragma pack(push, 1)
typedef struct
{
    u16 file_type;
    u32 file_size;
    u16 reserved0;
    u16 reserved1;
    u32 bitmap_offset;
    u32 size;
    s32 width;
    s32 height;
    u16 planes;
    u16 bits_per_pixel;
    u32 compression;
    u32 size_of_bitmap;
    s32 horz_resolution;
    s32 vert_resolution;
    u32 colors_used;
    u32 colors_important;
}
BMPHeader;
#pragma pack(pop)

static void
add_bmp(AssetFileHeader *buffer, u32 *bytes_filled, char *filename, TextureID slot_index)
{
    FileContents contents = platform_read_entire_file(filename);
    
    if (contents.memory)
    {
        BMPHeader *bmp_header = (BMPHeader *)contents.memory;
        u32 width = (u32)bmp_header->width;
        u32 height = (u32)bmp_header->height;
        
        u8 *write_position = (u8 *)buffer + *bytes_filled;
        PackedTextureHeader *texture_header = (PackedTextureHeader *)write_position;
        texture_header->width = width;
        texture_header->height = height;
        
        // NOTE: GIMP exports .bmp files in "ARGB", but due to Windows' little-endian bit order, they appear in the order ABGR.
        // NOTE: Swizzle to RGBA.
        u32 *in_pixel = (u32 *)((u8 *)contents.memory + bmp_header->bitmap_offset);
        u32 *out_pixel = (u32 *)(write_position + sizeof(PackedTextureHeader));
        for (u32 x = 0; x < width; ++x)
        {
            for (u32 y = 0; y < height; ++y)
            {
                u8 *pixel_color_in = (u8 *)in_pixel;
                u8 *pixel_color_out = (u8 *)out_pixel;
                
                u8 a = pixel_color_in[0];
                u8 r = pixel_color_in[3];
                u8 g = pixel_color_in[2];
                u8 b = pixel_color_in[1];
                
                pixel_color_out[0] = r;
                pixel_color_out[1] = g;
                pixel_color_out[2] = b;
                pixel_color_out[3] = a;
                
                ++in_pixel;
                ++out_pixel;
            }
        }
        
        // NOTE: Actual data has been written, now we just need to update the asset file header with an entry in the table of contents for this assset.
        u32 size = sizeof(PackedTextureHeader) + (width*height*sizeof(u32));
        TableOfContentsEntry *entry = buffer->table_of_contents + slot_index;
        entry->offset = *bytes_filled;
        entry->size = size;
        
        *bytes_filled += size;
    }
    else
    {
        OutputDebugStringA("[ERROR] Unable to read the BMP file.\n");
    }
    
    platform_free_file_memory(contents.memory);
}
