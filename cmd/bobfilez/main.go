package main

import (
	"log"
	"os"

	"gioui.org/app"
	"gioui.org/layout"
	"gioui.org/op"

	"github.com/robertpelloni/bqt/internal/audio"
	"github.com/robertpelloni/bqt/internal/shell"
	"github.com/robertpelloni/bqt/internal/ui"
	"github.com/robertpelloni/bqt/internal/ui/widgets"
)

func main() {
	log.Println("Bobfilez OS Shell: Bootstrapping via BQt Kernel...")

	// 1. Initialize BQt Shell Bridge
	bridge := shell.NewShellBridge()

	// Register shell lifecycle hooks
	bridge.RegisterConnectHook(func(platform string) {
		log.Printf("Bobfilez OS Shell successfully attached to BQt kernel via %s", platform)
	})

	bridge.AttachShell("desktop-gio")

	// 2. Initialize BQt Core Audio components via BQt Engine
	synthWidget := widgets.NewSynthWidget()

	// Create an audio node
	_ = audio.NewSynthesizer()

	// In a real integration, the audio graph feeds data to the scope, which emits DataReady.
	// For this minimal shim, we wire the SynthWidget directly to simulate the bridge rendering.

	// 3. Launch BQt Unified Event Loop
	go func() {
		// Run BQt engine loop
		bridge.Run()
	}()

	go func() {
		ui.GetEventLoop().Run()
	}()

	// 4. Start Desktop Window Loop (Bobfilez shell representation)
	go func() {
		w := new(app.Window)
		w.Option(app.Title("Bobfilez Desktop Shell (BQt Kernel)"), app.Size(800, 600))
		if err := loop(w, synthWidget); err != nil {
			log.Fatal(err)
		}
		os.Exit(0)
	}()

	// 5. Gio requires app.Main to be called on the main thread
	app.Main()
}

func loop(w *app.Window, synthWidget *widgets.SynthWidget) error {
	var ops op.Ops

	for {
		e := w.Event()
		switch e := e.(type) {
		case app.DestroyEvent:
			return e.Err
		case app.FrameEvent:
			gtx := app.NewContext(&ops, e)

			// Render the SynthWidget via BQt inside the bobfilez window
			layout.Stack{}.Layout(gtx, layout.Stacked(func(gtx layout.Context) layout.Dimensions {
				return synthWidget.Layout(gtx)
			}))

			e.Frame(gtx.Ops)
		}
	}
}
