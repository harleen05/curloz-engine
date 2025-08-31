#define GLM_ENABLE_EXPERIMENTAL
#include "loader/bone.h"
#include "glm/gtx/quaternion.hpp"

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
    if (1 == m_NumRotations)
    {
        auto rotation = glm::normalize(m_Rotations[0].orientation);
        return glm::toMat4(rotation);
    }

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,
        m_Rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation,
        m_Rotations[p1Index].orientation, scaleFactor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

