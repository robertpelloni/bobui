#include "BobQUppComponentRegistry.h"
#include <QDebug>
#include "../../../OmniUI/omnicore/include/BobQJuceHost.h"

namespace juce {
    class Slider : public Component { public: Slider() {} };
    class TextButton : public Component { public: TextButton() {} };
    class TreeView : public Component { public: TreeView() {} };
}


// Mock definitions for U++ components to simulate the registry mapping
namespace Upp {
    // Inherits from Ctrl (mocked in BobQUltimatePPHost.cpp)
    class Button : public Ctrl {
    public:
        Button() { /* Initialize U++ Button */ }
    };

    class SliderCtrl : public Ctrl {
    public:
        SliderCtrl() { /* Initialize U++ Slider */ }
    };

    class ArrayCtrl : public Ctrl {
    public:
        ArrayCtrl() { /* Initialize U++ ArrayCtrl (List/Tree) */ }
    };
}

BobQUppComponentRegistry* BobQUppComponentRegistry::s_instance = nullptr;

BobQUppComponentRegistry::BobQUppComponentRegistry(QObject *parent) : QObject(parent) {
}

BobQUppComponentRegistry* BobQUppComponentRegistry::create(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return instance();
}

BobQUppComponentRegistry* BobQUppComponentRegistry::instance() {
    if (!s_instance) {
        s_instance = new BobQUppComponentRegistry();
    }
    return s_instance;
}

QQuickItem* BobQUppComponentRegistry::createComponent(const QString& componentName, QQuickItem* parent) {
    BobQUltimatePPHost* host = new BobQUltimatePPHost(parent);

    // Map generic component names to concrete U++ and JUCE equivalents
    if (componentName.startsWith("Juce", Qt::CaseInsensitive)) {
        BobQJuceHost* juceHost = new BobQJuceHost(parent);
        if (componentName.compare("JuceButton", Qt::CaseInsensitive) == 0) {
            juceHost->setJuceComponent(std::make_unique<juce::TextButton>());
        } else if (componentName.compare("JuceSlider", Qt::CaseInsensitive) == 0) {
            juceHost->setJuceComponent(std::make_unique<juce::Slider>());
        } else {
            qWarning() << "BobQUppComponentRegistry: Unrecognized JUCE component mapping for" << componentName;
        }
        return juceHost;
    }

    if (componentName.compare("Button", Qt::CaseInsensitive) == 0) {
        host->setUppCtrl(std::make_unique<Upp::Button>());
    }
    else if (componentName.compare("Slider", Qt::CaseInsensitive) == 0) {
        host->setUppCtrl(std::make_unique<Upp::SliderCtrl>());
    }
    else if (componentName.compare("List", Qt::CaseInsensitive) == 0 ||
             componentName.compare("Tree", Qt::CaseInsensitive) == 0 ||
             componentName.compare("ArrayCtrl", Qt::CaseInsensitive) == 0) {
        host->setUppCtrl(std::make_unique<Upp::ArrayCtrl>());
    }
    else {
        qWarning() << "BobQUppComponentRegistry: Unrecognized Ultimate++ component mapping for" << componentName;
        // Host remains valid but empty (transparent)
    }

    return host;
}
