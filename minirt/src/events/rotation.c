#include "minirt.h"

t_vec3 rotate_y(t_vec3 v, float angle_deg)
{
    float rad = angle_deg * M_PI / 180.0f;
    float c = cosf(rad);
    float s = sinf(rad);

    t_vec3 result;
    result.x = v.x * c + v.z * s;
    result.y = v.y;
    result.z = -v.x * s + v.z * c;
    return result;
}

t_vec3 rotate_x(t_vec3 v, float angle_deg)
{
    float rad = angle_deg * M_PI / 180.0f;
    float c = cosf(rad);
    float s = sinf(rad);

    t_vec3 result;
    result.x = v.x;
    result.y = v.y * c - v.z * s;
    result.z = v.y * s + v.z * c;
    return result;
}
