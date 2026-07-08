import os

base_dir = r"C:\Users\hyper\workspace\bobui\OmniUI\core"
include_dir = os.path.join(base_dir, "include")
src_dir = os.path.join(base_dir, "src")

components = {
    "widgets/OmniButton": {"base": "QPushButton", "header": "<QPushButton>"},
    "widgets/OmniSlider": {"base": "QSlider", "header": "<QSlider>"},
    "widgets/OmniTextField": {"base": "QLineEdit", "header": "<QLineEdit>"},
    "widgets/OmniDial": {"base": "QDial", "header": "<QDial>"},
    "widgets/OmniCheckBox": {"base": "QCheckBox", "header": "<QCheckBox>"},
    "widgets/OmniComboBox": {"base": "QComboBox", "header": "<QComboBox>"},
    "widgets/OmniProgressBar": {"base": "QProgressBar", "header": "<QProgressBar>"},
    "widgets/OmniCodeEditor": {"base": "QPlainTextEdit", "header": "<QPlainTextEdit>"},
    "widgets/OmniCalendar": {"base": "QCalendarWidget", "header": "<QCalendarWidget>"},
    "charts/OmniLineChart": {"base": "QWidget", "header": "<QWidget>"},
    "charts/OmniOscilloscope": {"base": "QWidget", "header": "<QWidget>"},
    "layout/OmniDockArea": {"base": "QWidget", "header": "<QWidget>"},
    "layout/OmniSplitView": {"base": "QSplitter", "header": "<QSplitter>"},
    "net/OmniHttpClient": {"base": "QObject", "header": "<QObject>"},
    "net/OmniWebSocket": {"base": "QObject", "header": "<QObject>"},
    "plugin/OmniPluginManager": {"base": "QObject", "header": "<QObject>"},
    "data/OmniDatabase": {"base": "QObject", "header": "<QObject>"},
    "gfx/OmniShaderEffect": {"base": "QWidget", "header": "<QWidget>"},
    "gfx/OmniParticles": {"base": "QWidget", "header": "<QWidget>"}
}

os.makedirs(include_dir, exist_ok=True)

for comp, data in components.items():
    name = comp.split('/')[-1]
    folder = comp.rsplit('/', 1)[0] if '/' in comp else ''
    
    os.makedirs(os.path.join(src_dir, folder), exist_ok=True)
    
    h_path = os.path.join(include_dir, f"{name}.h")
    cpp_path = os.path.join(src_dir, folder, f"{name}.cpp")
    
    with open(h_path, "w") as f:
        f.write(f'''#ifndef {name.upper()}_H
#define {name.upper()}_H

#include {data['header']}
#include <QDebug>

class {name} : public {data['base']} {{
    Q_OBJECT
public:
    explicit {name}(QWidget *parent = nullptr);
    virtual ~{name}();
}};

#endif // {name.upper()}_H
''')
        
    with open(cpp_path, "w") as f:
        parent_arg = "QWidget *parent"
        base_call = "QWidget(parent)"
        if data['base'] == 'QObject':
            parent_arg = "QObject *parent"
            base_call = "QObject(parent)"
        elif data['base'] != 'QWidget':
            base_call = f"{data['base']}(parent)"
            
        f.write(f'''#include "{name}.h"

{name}::{name}({parent_arg}) : {base_call} {{
    setObjectName("{name}");
    // qDebug() << "Initialized {name}";
}}

{name}::~{name}() = default;
''')

print("Generated core missing files successfully.")
