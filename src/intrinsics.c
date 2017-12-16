typedef union __declspec(intrin_type) __declspec(align(16)) __m128 {
    float               m128_f32[4];
    unsigned __int64    m128_u64[2];
    __int8              m128_i8[16];
    __int16             m128_i16[8];
    __int32             m128_i32[4];
    __int64             m128_i64[2];
    unsigned __int8     m128_u8[16];
    unsigned __int16    m128_u16[8];
    unsigned __int32    m128_u32[4];
} __m128;

float _mm_cvtss_f32(__m128 _A);
__m128 _mm_set_ss(float _A);
__m128 _mm_sqrt_ss(__m128);

float square_root(float n)
{
    float result = _mm_cvtss_f32(_mm_sqrt_ss(_mm_set_ss(n)));
    return result;
}
