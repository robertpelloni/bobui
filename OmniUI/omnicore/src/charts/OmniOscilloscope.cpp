#include "OmniOscilloscope.h"

OmniOscilloscope::OmniOscilloscope(QQuickItem *parent) : QQuickItem(parent) {
    setObjectName("OmniOscilloscope");
    // qDebug() << "Initialized OmniOscilloscope";
}

OmniOscilloscope::~OmniOscilloscope() = default;
