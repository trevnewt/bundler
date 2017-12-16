#define false 0
#define true 1

typedef u32 b32;

#define ASSERT(expression) if (!(expression)) {*(int *)8 = 0;}
#define ARRAY_COUNT(array) sizeof(array) / sizeof((array)[0])

typedef struct FileContents
{
    void *memory;
    u32 size;
}
FileContents;

FileContents platform_read_entire_file(char *filename);
void platform_free_file_memory(void *memory);
b32 platform_write_entire_file(char *filename, u32 memsize, void *memory);
