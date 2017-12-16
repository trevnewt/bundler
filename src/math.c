#define min(a, b) (a < b) ? a : b
#define max(a, b) (a > b) ? a : b

#define pi32 3.14159265359f

#include "intrinsics.c"

typedef union {
    struct
    {
        float x, y;
    };
    float e[2];
} Vec2;

static inline Vec2
vec2(float x, float y)
{
    Vec2 result;
    result.x = x;
    result.y = y;
    return result;
}

typedef union
{
    struct 
    {
        float x, y, z;
    };
    struct
    {
        float r, g, b;
    };
    float e[3];
}
Vec3;

static inline
Vec3 vec3(float x, float y, float z)
{
    Vec3 result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

static inline
Vec3 vec3_add(Vec3 a, Vec3 b)
{
    Vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

static inline
Vec3 vec3_sub(Vec3 a, Vec3 b)
{
    Vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

static inline
Vec3 vec3_mul(Vec3 v, float s)
{
    Vec3 result;
    result.x = v.x*s;
    result.y = v.y*s;
    result.z = v.z*s;
    return result;
}

static inline
Vec3 vec3_div(Vec3 v, float s)
{
    Vec3 result;
    result.x = v.x/s;
    result.y = v.y/s;
    result.z = v.z/s;
    return result;
}

static inline
float vec3_dot(Vec3 a, Vec3 b)
{
    return (a.x*b.x + a.y*b.y + a.z*b.z);
}

static inline Vec3
vec3_cross(Vec3 a, Vec3 b)
{
    Vec3 result;
    result.x = a.y*b.z - a.z*b.y;
    result.y = a.z*b.x - a.x*b.z;
    result.z = a.x*b.y - a.y*b.x;
    return result;
}

static inline float
vec3_len(Vec3 v)
{
    return square_root(v.x*v.x + v.y*v.y + v.z*v.z);
}

static Vec3
vec3_norm(Vec3 v)
{
    Vec3 result = {0};
    
    if (v.x || v.y || v.z)
    {
        float mag = vec3_len(v);
        result = vec3_div(v, mag);
    }
    
    return result;
}

static Vec3
get_tri_normal(Vec3 a, Vec3 b, Vec3 c)
{
    Vec3 result;
    
    Vec3 ab = vec3_sub(b, a);
    Vec3 bc = vec3_sub(c, b);
    result = vec3_norm(vec3_cross(ab, bc));
    
    return result;
}
