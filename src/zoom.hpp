#pragma once

class Zoom {
    float facteurZoom_{4};
    public :
        float getZoom() const;
        void zoomIn();
        void zoomOut();
};
