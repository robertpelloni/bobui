#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include "OmniApp.h"
#include "JuceWidget.h"

// Simple main to demonstrate the hybrid
int main(int argc, char *argv[])
{
    OmniApplication app(argc, argv);
    app.initializeJuce();

    QWidget window;
    window.setWindowTitle("OmniUI: Qt + JUCE + WASM");
    window.resize(800, 600);

    QVBoxLayout* layout = new QVBoxLayout(&window);

    // native Qt widget
    QPushButton* btn = new QPushButton("This is a native Qt Button", &window);
    layout->addWidget(btn);

    // Omni/JUCE widget
    JuceWidget* juceContent = new JuceWidget(&window);
    layout->addWidget(juceContent);

    QObject::connect(btn, &QPushButton::clicked, [juceContent]() {
        qDebug() << "Qt Button Clicked! Sending signal to JUCE...";
        // In a real app, we would call a method on juceContent
    });

    window.show();

    return app.exec();
}
