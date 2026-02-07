// Type definitions for OmniUI

declare namespace Omni {

    export interface Widget {
        show(): void;
        hide(): void;
        resize(w: number, h: number): void;
    }

    export class Application {
        static instance(): Application;
        quit(): void;
    }

    export class Button implements Widget {
        constructor(label: string);
        show(): void;
        hide(): void;
        resize(w: number, h: number): void;

        onClicked: () => void;
    }

    export class JuceComponent implements Widget {
        constructor();
        show(): void;
        hide(): void;
        resize(w: number, h: number): void;

        loadPreset(path: string): void;
    }
}

declare function print(msg: string): void;
