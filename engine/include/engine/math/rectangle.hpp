#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "engine/math/vector2.hpp"

namespace mana {
    /**
     * A rectangle defining position and dimensions.
     * */
    template<typename T>
    struct Rectangle {
        Vector2 <T> position; //Top left point of the rectangle
        Vector2 <T> dimensions;

        Rectangle(Vector2 <T> position, Vector2 <T> dimensions)
                : position(position), dimensions(dimensions) {
        }

        Rectangle()
                : position(0), dimensions(0) {
        }

        Vector2 <T> center() {
            return Vector2<T>(position.x + dimensions.x / 2, position.y - dimensions.y / 2);
        }

        Vector2 <T> top() {
            return Vector2<T>(position.x + dimensions.x / 2, position.y);
        }

        Vector2 <T> right() {
            return Vector2<T>(position.x + dimensions.x, position.y - dimensions.y / 2);
        }

        Vector2 <T> bottom() {
            return Vector2<T>(position.x + dimensions.x / 2, position.y - dimensions.y);
        }

        Vector2 <T> left() {
            return Vector2<T>(position.x, position.y - dimensions.y / 2);
        }

        bool testPoint(Vector2 <T> point) {
            return point.x >= position.x && point.y <= position.y && point.x <= position.x + dimensions.x &&
                   point.y >= position.y - dimensions.y;
        }

        bool testOverlap(Rectangle<T> other) {
            if (position.x > other.position.x + other.dimensions.x)
                return false;
            else if (position.y < other.position.y - other.dimensions.y)
                return false;
            else if (position.x + dimensions.x < other.position.x)
                return false;
            else if (position.y - dimensions.y > other.position.y)
                return false;
            else
                return true;
        }
    };

    typedef Rectangle<float> Rectf;
    typedef Rectangle<int> Recti;
}

#endif // RECTANGLE_HPP