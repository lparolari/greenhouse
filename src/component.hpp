#pragma once

namespace greenhouse::component
{
    class Component
    {
        // @brief Initializes the componet. To be called in Arduino's `setup()` function.
        virtual void begin() = 0;
    };
} // namespace greenhouse::component
