void
platform_free_file_memory(void *memory)
{
    if (memory)
    {
        VirtualFree(memory, 0, MEM_RELEASE);
    }
}

FileContents
platform_read_entire_file(char *filename)
{
    s32 error_code = 0;
    // TODO: I think we want to eventually change the dynamic allocation in here...
    FileContents result = {0};
    void *file_handle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER file_size;
        if (GetFileSizeEx(file_handle, &file_size))
        {
            ASSERT(file_size.QuadPart <= 0xFFFFFFFF);
            
            u32 file_size_32 = (u32)file_size.QuadPart;
            result.memory = VirtualAlloc(0, file_size.QuadPart, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            if (result.memory)
            {
                u32 bytes_read;
                if (ReadFile(file_handle, result.memory, file_size_32, &bytes_read, 0) && file_size_32 == bytes_read)
                {
                    result.size = file_size_32;
                }
                else
                {
                    platform_free_file_memory(result.memory);
                    result.memory = 0;
                }
            }
        }
        else
        {
            error_code = GetLastError();
        }
        CloseHandle(file_handle);
    }
    else
    {
        error_code = GetLastError();
        OutputDebugStringA("Couldn't get a handle to the file\n");
    }
    
    return result;
}

b32
platform_write_entire_file(char *filename, u32 memsize, void *memory)
{
    b32 result = false;
    void *file_handle = CreateFileA(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        u32 bytes_written;
        if (WriteFile(file_handle, memory, memsize, &bytes_written, 0))
        {
            result = (bytes_written == memsize);
        }
        else 
        {
        }
        CloseHandle(file_handle);
    }
    return result;
}
