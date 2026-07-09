#include "FibaCourt.hpp"

Court::Point FibaCourt::markerPosition(FibaMarker marker)
{
    switch (marker)
    {
    case FibaMarker::CourtTopLeft:
        return { 0.0f, 0.0f };

    case FibaMarker::CourtTopRight:
        return { 28.0f, 0.0f };

    case FibaMarker::CourtBottomRight:
        return { 28.0f, 15.0f };

    case FibaMarker::CourtBottomLeft:
        return { 0.0f, 15.0f };

    case FibaMarker::CentreCircle:
        return { 14.0f, 7.5f };

    case FibaMarker::FreeThrowLeftTop:
        return { 5.8f, 5.95f };

    case FibaMarker::FreeThrowLeftBottom:
        return { 5.8f, 9.05f };

    case FibaMarker::FreeThrowRightTop:
        return { 22.2f, 5.95f };

    case FibaMarker::FreeThrowRightBottom:
        return { 22.2f, 9.05f };

    default:
        return { 0.0f, 0.0f };
    }
}