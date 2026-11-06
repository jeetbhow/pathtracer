#include "pathtracer/geometry/ray.hpp"

#include <sstream>

namespace pathtracer
{

    std::string Ray::to_string() const
    {
        std::ostringstream out;
        out << "Ray{origin: (" << origin_.x << ", " << origin_.y << ", " << origin_.z
            << "), direction: (" << direction_.x << ", " << direction_.y << ", " << direction_.z << ")}";
        return out.str();
    }

} // namespace pathtracer
