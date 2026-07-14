import os

filepath = 'BobQ/omnicore/src/BobQUppComponentRegistry.cpp'
with open(filepath, 'r') as f:
    content = f.read()

# Implement the TODO: Complete the generic component registry (`BobQUppComponentRegistry`) so any QML string can summon a U++ or JUCE component.

# Let's add Juce mock definitions
content = content.replace('namespace Upp {', '''namespace juce {
    class Component {
    public:
        virtual ~Component() = default;
    };
    class Slider : public Component {
    public:
        Slider() {}
    };
    class TextButton : public Component {
    public:
        TextButton() {}
    };
    class TreeView : public Component {
    public:
        TreeView() {}
    };
}

namespace Upp {''')

# We need to add Juce support to BobQUltimatePPHost or a new BobQJuceHost?
# Let's check what's available.
