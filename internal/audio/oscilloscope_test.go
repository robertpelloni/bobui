package audio

import (
	"reflect"
	"sync"
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestOscilloscope_ProcessAudio(t *testing.T) {
	// Re-initialize the global event loop to prevent pollution from other tests.
	ui.GetEventLoop().Stop()

	go ui.GetEventLoop().Run()
	defer ui.GetEventLoop().Stop()

	scope := NewOscilloscope(4)

	inputData := []float32{0.1, 0.2, 0.3, 0.4, 0.5, 0.6} // Intentionally larger than buffer

	var wg sync.WaitGroup
	wg.Add(1)

	// Wait for the asynchronous signal emission
	var once sync.Once
	scope.DataReady.Connect(func(args ...interface{}) {
		once.Do(func() {
			wg.Done()
		})
	})

	scope.ProcessAudio(inputData)

	// Wait for the event loop to process the signal
	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
	case <-time.After(1 * time.Second):
		t.Fatal("Timeout waiting for DataReady signal emission")
	}

	snapshot := scope.GetBufferSnapshot()

	// Verify it only captured up to the buffer size and ignores the rest
	expected := []float32{0.1, 0.2, 0.3, 0.4}
	if !reflect.DeepEqual(snapshot, expected) {
		t.Fatalf("Expected buffer snapshot %v, got %v", expected, snapshot)
	}
}

func TestOscilloscope_ZeroPadding(t *testing.T) {
	// Re-initialize the global event loop to prevent pollution.
	ui.GetEventLoop().Stop()

	go ui.GetEventLoop().Run()
	defer ui.GetEventLoop().Stop()

	scope := NewOscilloscope(4)

	inputData := []float32{0.5, 0.5} // Intentionally smaller than buffer

	scope.ProcessAudio(inputData)

	// We don't strictly need to wait for the signal here to test the synchronous memory copy,
	// but we must let the event loop process the emission so it doesn't queue up.
	time.Sleep(50 * time.Millisecond)

	snapshot := scope.GetBufferSnapshot()

	// Verify the remaining buffer is zero-padded
	expected := []float32{0.5, 0.5, 0.0, 0.0}
	if !reflect.DeepEqual(snapshot, expected) {
		t.Fatalf("Expected buffer snapshot %v, got %v", expected, snapshot)
	}
}
