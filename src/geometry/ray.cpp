#include "pathtracer/geometry/ray.hpp"

#include <sstream>

using std::ostringstream;
using std::string;

namespace pathtracer
{
    Vec3 Ray::at(float t) const
    {
        return origin + t * direction;
    }

    string Ray::to_string() const
    {
        ostringstream out;
        out << "Ray{origin: (" << origin.x << ", " << origin.y << ", " << origin.z
            << "), direction: (" << direction.x << ", " << direction.y << ", " << direction.z << ")}";
        return out.str();
    }

} // namespace pathtracer
