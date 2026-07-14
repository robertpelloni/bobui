#ifndef OMNIOSCILLOSCOPE_H
#define OMNIOSCILLOSCOPE_H

#include <QQuickItem>
#include <QDebug>

class OmniOscilloscope : public QQuickItem {
    Q_OBJECT
public:
    explicit OmniOscilloscope(QQuickItem *parent = nullptr);
    virtual ~OmniOscilloscope();
};

#endif // OMNIOSCILLOSCOPE_H
