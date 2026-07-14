#ifndef BOBQUPPCOMPONENTREGISTRY_H
#define BOBQUPPCOMPONENTREGISTRY_H

#include <QObject>
#include <QString>
#include <QQmlEngine>
#include "BobQUltimatePPHost.h"
#include <memory>

/**
 * @class BobQUppComponentRegistry
 * @brief Singleton registry for dynamically bridging U++ `Ctrl` objects into BobQ.
 *
 * Exposes a QML-friendly factory that allows developers to request standard
 * BobQ/Qt layout or widget components, and returns a `BobQUltimatePPHost` pre-loaded
 * with the equivalent Ultimate++ component (e.g., `Upp::Button`, `Upp::ArrayCtrl`).
 * This achieves seamless interoperability and "mix-and-match" toolkit behavior.
 */
class BobQUppComponentRegistry : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    static BobQUppComponentRegistry* create(QQmlEngine *engine, QJSEngine *scriptEngine);
    static BobQUppComponentRegistry* instance();

    /**
     * @brief Creates a QQuickPaintedItem pre-loaded with a mapped U++ Control.
     * @param componentName The generic name of the component (e.g., "Button", "Slider", "Tree")
     * @param parent Optional parent item
     * @return A valid BobQUltimatePPHost acting as a drop-in replacement for the requested widget.
     */
    Q_INVOKABLE QQuickItem* createComponent(const QString& componentName, QQuickItem* parent = nullptr);

private:
    explicit BobQUppComponentRegistry(QObject *parent = nullptr);
    virtual ~BobQUppComponentRegistry() = default;

    static BobQUppComponentRegistry* s_instance;
};

#endif // BOBQUPPCOMPONENTREGISTRY_H
