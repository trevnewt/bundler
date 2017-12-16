#define GENERIC_READ         0x80000000L
#define GENERIC_WRITE        0x40000000L
#define FILE_SHARE_READ      0x00000001
#define OPEN_EXISTING        3
#define CREATE_ALWAYS        2
#define INVALID_HANDLE_VALUE (void *)0xffffffffffffffff

#define MEM_RELEASE          0x8000
#define MEM_COMMIT           0x00001000
#define MEM_RESERVE          0x00002000
#define PAGE_READWRITE       0x04

// NOTE: Typedefs!
typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef __int8  s8;
typedef __int16 s16;
typedef __int32 s32;
typedef __int64 s64;

typedef float f32;
typedef double f64;

typedef struct {
    u32 nLength;
    void *lpSecurityDescriptor;
    int bInheritHandle;
} SECURITY_ATTRIBUTES;

typedef union {
    struct {
        u32 LowPart;
        s32 HighPart;
    };
    struct {
        u32 LowPart;
        s32 HighPart;
    } u;
    s64 QuadPart;
} LARGE_INTEGER;

typedef struct {
    // TODO: These two are 32-bit on 32-bit Windows. Clean up!
    u64 Internal;
    u64 InternalHigh;
    union {
        struct {
            u32 offset;
            u32 offset_high;
        };
        void *pointer;
    };
    void *hEvent;
} OVERLAPPED;

// Kernel32
void *VirtualAlloc      (void *, u64, u32, u32);
s32 VirtualFree         (void *, u64, u32);
void *LoadLibraryA      (const char *);
void  OutputDebugStringA(const char *);
void  ExitProcess       (u32);
void *GetModuleHandleA  (const char *);
void *CreateFileA       (const char *, u32, u32, SECURITY_ATTRIBUTES *, u32, u32, void *);
s32 GetFileSizeEx     (void *, LARGE_INTEGER *);
s32 ReadFile          (void *, void *, u32, u32 *, OVERLAPPED *);
s32 WriteFile         (void *, const void *, u32, u32 *, OVERLAPPED *);
s32 CloseHandle       (void *);
s32 GetLastError      (void);
