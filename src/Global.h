#ifndef GLOBAL_H
#define GLOBAL_H

#include <LNamespaces.h>

using namespace Louvre;

class Compositor;

class G
{
public:
    static Compositor *compositor() noexcept
    {
        return (Compositor*)Louvre::compositor();
    }

};

#endif

