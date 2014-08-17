#include <utility>

template <unsigned int N>
class Pixel {
public:
    template<typename... Floats> //can't use float... anyway
    Pixel(Floats&&... floats) : color{std::forward<Floats>(floats)...} {
        static_assert(sizeof...(Floats) == N, "You must provide N arguments.");
    }

private:
    float color[N];
};

int main() {
    Pixel<3> p(3.4f, 5.6f, 8.f);
    Pixel<3> p2(1.2f); //static_assert fired
}
