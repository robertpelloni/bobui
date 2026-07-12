package audio

import (
	"sync"
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestMidiHandlerEventLoopIntegration(t *testing.T) {
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	mh := GetMidiHandler()

	// Reset signals to prevent singleton pollution
	mh.CurrentDeviceChanged = ui.NewSignal("CurrentDeviceChanged")
	mh.NoteReceived = ui.NewSignal("NoteReceived")

	var wg sync.WaitGroup
	wg.Add(1)

	var emittedDevice string
	deviceChangedReceived := false

	mh.CurrentDeviceChanged.Connect(func(args ...interface{}) {
		defer wg.Done()
		deviceChangedReceived = true
		if len(args) > 0 {
			if v, ok := args[0].(string); ok {
				emittedDevice = v
			}
		}
	})

	mh.SetCurrentDevice("Virtual MIDI 1")

	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		if !deviceChangedReceived {
			t.Fatal("Expected CurrentDeviceChanged signal to be dispatched via EventLoop")
		}
		if emittedDevice != "Virtual MIDI 1" {
			t.Errorf("Expected emitted device Virtual MIDI 1, got %s", emittedDevice)
		}
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for MidiHandler signals via EventLoop")
	}
}
