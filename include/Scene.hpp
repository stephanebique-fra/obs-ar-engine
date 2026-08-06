#pragma once

#include <vector>
#include "Advertisement.hpp"

class Scene
{
public:
    std::vector<Advertisement>& advertisements()
    {
        return m_advertisements;
    }

    const std::vector<Advertisement>& advertisements() const
    {
        return m_advertisements;
    }

private:
    std::vector<Advertisement> m_advertisements;
};