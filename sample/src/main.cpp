#include "sample0.hpp"

int main() {
    GLFWDisplayAPI displayApi;
    Sample0 sample;
    return sample.loop(displayApi, OPENGL);
}