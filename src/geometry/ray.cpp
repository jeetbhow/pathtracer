#include "pathtracer/geometry/ray.hpp"

#include <sstream>

namespace pathtracer
{
    Vec3 Ray::at(float t) const
    {
        return origin + t * direction;
    }

    std::string Ray::to_string() const
    {
        std::ostringstream out;
        out << "Ray{origin: (" << origin.x << ", " << origin.y << ", " << origin.z
            << "), direction: (" << direction.x << ", " << direction.y << ", " << direction.z << ")}";
        return out.str();
    }

} // namespace pathtracer
