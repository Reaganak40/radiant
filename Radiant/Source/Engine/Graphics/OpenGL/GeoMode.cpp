#include "pch.h"
#include "GeoMode.h"
#include "ErrorHandling.h"

namespace rdt::core {

    void ActivateGeoMode(GeoMode mode)
    {
        switch (mode) {
        case FillMode:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case OutlineMode:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        default:
            ASSERT(false);
        }
    }
}