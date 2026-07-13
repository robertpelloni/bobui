package audio

import (
	"sync"
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

// TestConcurrentAudioGraph verifies the stability of the native Go audio graph under load.
// It instantiates OmniSynthesizer and OmniGain nodes concurrently, verifying that the asynchronous
// signal/slot dispatch in event_loop.go maintains real-time performance without blocking.
func TestConcurrentAudioGraph(t *testing.T) {
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	graph := GetAudioGraph()
	// Reset signals to prevent singleton pollution
	graph.GraphChanged = ui.NewSignal("GraphChanged")

	const numWorkers = 50
	const iterations = 20

	var wg sync.WaitGroup
	// We expect graphChanged to be emitted three times per worker per iteration (AddNode x2, Connect)
	// But it could be emitted more times if multiple tests share the singleton, or less if batched.
	// We'll use a simpler wait condition to avoid waitgroup panics from exact counts.

	var signalCount int
	var mu sync.Mutex

	graph.GraphChanged.Connect(func(args ...interface{}) {
		mu.Lock()
		defer mu.Unlock()
		signalCount++
	})

	startTime := time.Now()

	// Launch concurrent workers
	wg.Add(numWorkers)
	for i := 0; i < numWorkers; i++ {
		go func(workerID int) {
			defer wg.Done()
			for j := 0; j < iterations; j++ {
				// Instantiate nodes
				synth := NewSynthesizer()
				gain := NewOmniGain()

				// Concurrently manipulate the graph and nodes
				graph.AddNode(synth)
				graph.AddNode(gain)

				_ = graph.Connect(synth.Name(), gain.Name())

				// Rapidly change states to stress the event loop dispatch
				synth.SetPolyphony((j % 8) + 1)
				gain.SetVolume(float32(j) / float32(iterations))
			}
		}(i)
	}

	done := make(chan struct{})
	go func() {
		// Wait for all workers to finish their loops
		wg.Wait()

		// Give the event loop a small buffer to drain remaining signals
		time.Sleep(100 * time.Millisecond)
		close(done)
	}()

	select {
	case <-done:
		elapsed := time.Since(startTime)
		mu.Lock()
		count := signalCount
		mu.Unlock()
		t.Logf("Concurrent graph load test completed %d signal dispatches in %v", count, elapsed)

		// Expected minimum signals: numWorkers * iterations * 2 (AddNode twice per iteration)
		// Plus 1 for each Connect
		expectedMin := numWorkers * iterations * 3
		if count < expectedMin {
			t.Errorf("Expected at least %d signal dispatches, but got %d", expectedMin, count)
		}
		if elapsed > 2*time.Second {
			t.Errorf("Performance degraded under load, elapsed time: %v (expected < 2s)", elapsed)
		}
	case <-time.After(3 * time.Second):
		t.Fatal("Timeout under concurrent load (event starvation or deadlock in signal dispatch)")
	}
}
