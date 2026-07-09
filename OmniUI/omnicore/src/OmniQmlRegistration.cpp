#include "OmniQmlRegistration.h"
#include "OmniInputManager.h"
#include "OmniButton.h"
#include "OmniSlider.h"
#include "OmniTextField.h"
#include "OmniDial.h"
#include "OmniProgressBar.h"
#include "OmniCheckBox.h"
#include "OmniCodeEditor.h"
#include "OmniCalendar.h"
#include "OmniTerminal.h"
#include "OmniWindow.h"
#include "OmniListView.h"
#include "OmniTreeView.h"
#include "OmniStackPane.h"
#include "OmniGridPane.h"
#include "OmniFlowPane.h"
#include "OmniTilePane.h"
#include "OmniImGuiCanvas.h"
#include "OmniPlot.h"
#include "OmniJuceView.h"
#include "OmniDockArea.h"
#include "OmniSplitView.h"
#include "OmniMidiHandler.h"
#include "OmniSequencer.h"
#include "OmniAudioPlayer.h"
#include "OmniDatabase.h"
#include "OmniHttpClient.h"
#include "OmniWebSocket.h"
#include "OmniPluginManager.h"
#include "OmniIPC.h"
#include "OmniFileSystem.h"
#include "OmniThemeManager.h"
#include "OmniNotificationCenter.h"
#include "OmniNeuralEngine.h"
#include "OmniTimeMachine.h"
#include "OmniAssetManager.h"
#include "OmniValueTree.h"
#include "OmniVM.h"
#include "OmniMacroRecorder.h"
#include "OmniMeshNode.h"
#include "OmniUserManager.h"
#include "OmniVoiceMesh.h"
#include "OmniClipboard.h"
#include "OmniFileMesh.h"
#include "OmniUndoStack.h"
#include "OmniParticleSystem.h"
#include "OmniShaderEffect.h"
#include "OmniRaymarcher.h"
#include "OmniRhiNode.h"
#include "OmniDropShadow.h"
#include "OmniAnimator.h"

#include <QQmlEngine>
#include <QDebug>

void OmniUI::registerQmlTypes() {
    qDebug() << "OmniUI: Registering QML Types...";
    
    // Singletons
    qmlRegisterSingletonType<OmniInputManager>("OmniUI", 1, 0, "InputManager", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniInputManager::instance(); });
    qmlRegisterSingletonType<OmniPluginManager>("OmniUI", 1, 0, "PluginManager", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniPluginManager::instance(); });
    qmlRegisterSingletonType<OmniFileSystem>("OmniData", 1, 0, "FileSystem", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniFileSystem::instance(); });
    qmlRegisterSingletonType<OmniTimeMachine>("OmniData", 1, 0, "TimeMachine", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniTimeMachine::instance(); });
    qmlRegisterSingletonType<OmniAssetManager>("OmniData", 1, 0, "AssetManager", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniAssetManager::instance(); });
    qmlRegisterSingletonType<OmniValueTree>("OmniData", 1, 0, "ValueTree", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniValueTree::instance(); });
    qmlRegisterSingletonType<OmniThemeManager>("OmniUI", 1, 0, "ThemeManager", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniThemeManager::instance(); });
    qmlRegisterSingletonType<OmniNotificationCenter>("OmniUI", 1, 0, "NotificationCenter", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniNotificationCenter::instance(); });
    qmlRegisterSingletonType<OmniNeuralEngine>("OmniUI", 1, 0, "NeuralEngine", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniNeuralEngine::instance(); });
    qmlRegisterSingletonType<OmniVM>("OmniUI", 1, 0, "VM", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniVM::instance(); });
    qmlRegisterSingletonType<OmniMacroRecorder>("OmniUI", 1, 0, "MacroRecorder", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniMacroRecorder::instance(); });
    qmlRegisterSingletonType<OmniUserManager>("OmniUI", 1, 0, "UserManager", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniUserManager::instance(); });
    qmlRegisterSingletonType<OmniVoiceMesh>("OmniNet", 1, 0, "VoiceMesh", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniVoiceMesh::instance(); });
    qmlRegisterSingletonType<OmniClipboard>("OmniUI", 1, 0, "Clipboard", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniClipboard::instance(); });
    qmlRegisterSingletonType<OmniAnimator>("OmniUI", 1, 0, "Animator", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniAnimator::instance(); });
    qmlRegisterSingletonType<OmniMeshNode>("OmniNet", 1, 0, "MeshNode", [](QQmlEngine *e, QJSEngine *s) -> QObject* { return OmniMeshNode::instance(); });

    // Widgets
    qmlRegisterType<OmniButton>("OmniUI", 1, 0, "Button");
    qmlRegisterType<OmniSlider>("OmniUI", 1, 0, "Slider");
    qmlRegisterType<OmniTextField>("OmniUI", 1, 0, "TextField");
    qmlRegisterType<OmniDial>("OmniUI", 1, 0, "Dial");
    qmlRegisterType<OmniProgressBar>("OmniUI", 1, 0, "ProgressBar");
    qmlRegisterType<OmniCheckBox>("OmniUI", 1, 0, "CheckBox");
    qmlRegisterType<OmniCodeEditor>("OmniUI", 1, 0, "CodeEditor");
    qmlRegisterType<OmniCalendar>("OmniUI", 1, 0, "Calendar");
    qmlRegisterType<OmniTerminal>("OmniUI", 1, 0, "Terminal");
    qmlRegisterType<OmniJuceView>("OmniUI", 1, 0, "JuceView");
    qmlRegisterType<OmniListView>("OmniUI", 1, 0, "ListView");
    qmlRegisterType<OmniTreeView>("OmniUI", 1, 0, "TreeView");
    qmlRegisterType<OmniImGuiCanvas>("OmniUI", 1, 0, "ImGuiCanvas");
    qmlRegisterType<OmniPlot>("OmniUI", 1, 0, "Plot");
    
    // Effects
    qmlRegisterType<OmniParticleSystem>("OmniGfx", 1, 0, "ParticleSystem");
    qmlRegisterType<OmniShaderEffect>("OmniGfx", 1, 0, "ShaderEffect");
    qmlRegisterType<OmniRaymarcher>("OmniGfx", 1, 0, "Raymarcher");
    qmlRegisterType<OmniRhiNode>("OmniGfx", 1, 0, "RhiNode");
    qmlRegisterType<OmniDropShadow>("OmniGfx", 1, 0, "DropShadow");
    
    // Layouts
    qmlRegisterType<OmniWindow>("OmniLayout", 1, 0, "Window");
    qmlRegisterType<OmniDockArea>("OmniLayout", 1, 0, "DockArea");
    qmlRegisterType<OmniSplitView>("OmniLayout", 1, 0, "SplitView");
    qmlRegisterType<OmniStackPane>("OmniLayout", 1, 0, "StackPane");
    qmlRegisterType<OmniGridPane>("OmniLayout", 1, 0, "GridPane");
    qmlRegisterType<OmniFlowPane>("OmniLayout", 1, 0, "FlowPane");
    qmlRegisterType<OmniTilePane>("OmniLayout", 1, 0, "TilePane");

    // Audio
    qmlRegisterType<OmniMidiHandler>("OmniAudio", 1, 0, "MidiHandler");
    qmlRegisterType<OmniSequencer>("OmniAudio", 1, 0, "Sequencer");
    qmlRegisterType<OmniAudioPlayer>("OmniAudio", 1, 0, "AudioPlayer");

    // Data & Networking
    qmlRegisterType<OmniDatabase>("OmniData", 1, 0, "Database");
    qmlRegisterType<OmniHttpClient>("OmniNet", 1, 0, "HttpClient");
    qmlRegisterType<OmniWebSocket>("OmniNet", 1, 0, "WebSocket");
    qmlRegisterType<OmniIPC>("OmniNet", 1, 0, "IPC");
    
    qDebug() << "OmniUI: QML Types Registered successfully. Full Multi-Framework Parity Reached.";
}

#include "OmniQmlRegistration.moc"
