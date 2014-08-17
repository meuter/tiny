#include <tiny/math/matrix.h>

using tiny::math::float4;


int main(int argc, char *argv[])
{
    float x = static_cast<float>(argc);

    float4 v{x,x*2.0f,x*3.0f,x*4.0f};
    float4 u{x,x*2.0f,x*3.0f,x*4.0f};

    return float4(u+v).length();

}
