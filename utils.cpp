#define ArrayCount(arr) (sizeof(arr)/sizeof(arr[0]))

inline float 
Lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

inline float
Clamp01(float value)
{
    float result = value;
    if (result > 1.0) result = 1.0;
    if (result < 0.0) result = 0.0;
    return result;
}

inline int
ClampBetweenNegativeOneAndOne(int x)
{
    if(x < -1) return -1;
    if(x > 1) return 1;
    return x;
}

