package audio

import (
	"sync"
	"testing"
	"time"
	"github.com/robertpelloni/bqt/internal/ui"
)

func TestAudioGraphEventLoopIntegration(t *testing.T) {
	// Initialize UI EventLoop
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	// Initialize Audio Graph
	graph := GetAudioGraph()

	// Create a UI signal to simulate cross-framework dispatch from audio layer
	audioStartedSignal := ui.NewSignal("AudioGraphStarted")

	var wg sync.WaitGroup
	wg.Add(1)
	signalReceived := false

	// Connect a slot to verify non-blocking event loop dispatch
	audioStartedSignal.Connect(func(args ...interface{}) {
		defer wg.Done()
		signalReceived = true
	})

	// Simulate starting the audio graph and emitting a signal
	// We'll wrap this in a goroutine to ensure it doesn't block
	go func() {
		graph.Start()
		audioStartedSignal.Emit("started")
		graph.Stop()
	}()

	// Wait for the slot to be executed by the EventLoop
	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		if !signalReceived {
			t.Fatal("Expected signal to be received and processed by EventLoop")
		}
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for audio graph signal dispatch via EventLoop (possible blocking)")
	}
}
