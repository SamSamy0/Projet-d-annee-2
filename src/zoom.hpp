#pragma once

class Zoom {
    float facteurZoom_;
    public :
        Zoom();
        float getZoom() const;
        void zoomIn();
        void zoomOut();
};
