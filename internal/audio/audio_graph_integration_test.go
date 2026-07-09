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
	// Disconnect previously connected signals from other tests if the singleton persisted
	graph.ProcessingStarted = ui.NewSignal("ProcessingStarted")
	graph.ProcessingStopped = ui.NewSignal("ProcessingStopped")
	graph.GraphChanged = ui.NewSignal("GraphChanged")

	var wg sync.WaitGroup
	wg.Add(3)

	startedReceived := false
	stoppedReceived := false
	graphChangedReceived := false

	// Connect slots to verify non-blocking event loop dispatch for native signals
	graph.ProcessingStarted.Connect(func(args ...interface{}) {
		defer wg.Done()
		startedReceived = true
	})

	graph.ProcessingStopped.Connect(func(args ...interface{}) {
		defer wg.Done()
		stoppedReceived = true
	})

	var graphChangedOnce sync.Once
	graph.GraphChanged.Connect(func(args ...interface{}) {
		graphChangedOnce.Do(func() {
			defer wg.Done()
			graphChangedReceived = true
		})
	})

	// Add a dummy node to trigger GraphChanged
	synth := NewSynthesizer()

	// Simulate starting the audio graph, adding a node, and stopping
	// We'll wrap this in a goroutine to ensure it doesn't block
	go func() {
		graph.Start()
		graph.AddNode(synth)
		graph.Stop()
	}()

	// Wait for the slots to be executed by the EventLoop
	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		if !startedReceived {
			t.Error("Expected ProcessingStarted signal to be received and processed by EventLoop")
		}
		if !stoppedReceived {
			t.Error("Expected ProcessingStopped signal to be received and processed by EventLoop")
		}
		if !graphChangedReceived {
			t.Error("Expected GraphChanged signal to be received and processed by EventLoop")
		}
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for audio graph signal dispatch via EventLoop (possible blocking)")
	}
}
