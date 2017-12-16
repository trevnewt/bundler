static b32
is_digit(char c)
{
    b32 result = false;
    if (c >= '0' && c <= '9')
    {
        result = true;
    }
    return result;
}

static float
ascii_to_float(char *string)
{
    float result;
    
    int int_result = 0;
    int factor = 1;
    int decimal_seen = 0;
    
    if (*string == '-')
    {
        ++string;
        factor = -1;
    }
    while(is_digit(*string) || *string == '.')
    {
        if (*string == '.')
        {
            decimal_seen = 1;
            ++string;
            continue;
        }
        int d = *string - '0';
        if (decimal_seen)
        {
            factor *= 10;
        }
        
        int_result = int_result*10 + d;
        
        ++string;
    }
    
    result = (float)int_result / (float)factor;
    
    return result;
}

static int
ascii_to_int(char *string)
{
    int result = 0;
    
    while(is_digit(*string))
    {
        int d = *string - '0';
        result = result*10 + d;
        
        ++string;
    }
    
    return result;
}
