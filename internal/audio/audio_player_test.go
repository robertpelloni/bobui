package audio

import (
	"sync"
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestAudioPlayerEventLoopIntegration(t *testing.T) {
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	ap := GetAudioPlayer()

	// Reset signals to prevent singleton pollution
	ap.SourceChanged = ui.NewSignal("SourceChanged")
	ap.IsPlayingChanged = ui.NewSignal("IsPlayingChanged")

	var wg sync.WaitGroup
	wg.Add(3)

	var emittedSource string
	var emittedIsPlayingStart bool
	var emittedIsPlayingStop bool

	ap.SourceChanged.Connect(func(args ...interface{}) {
		defer wg.Done()
		if len(args) > 0 {
			if v, ok := args[0].(string); ok {
				emittedSource = v
			}
		}
	})

	var isPlayingOnce sync.Once
	var isStoppingOnce sync.Once

	ap.IsPlayingChanged.Connect(func(args ...interface{}) {
		if len(args) > 0 {
			if isPlaying, ok := args[0].(bool); ok {
				if isPlaying {
					isPlayingOnce.Do(func() {
						defer wg.Done()
						emittedIsPlayingStart = true
					})
				} else {
					isStoppingOnce.Do(func() {
						defer wg.Done()
						emittedIsPlayingStop = true
					})
				}
			}
		}
	})

	ap.SetSource("test.wav")
	ap.Play()
	ap.Stop()

	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		if emittedSource != "test.wav" {
			t.Errorf("Expected SourceChanged test.wav, got %s", emittedSource)
		}
		if !emittedIsPlayingStart {
			t.Error("Expected IsPlayingChanged true signal to be processed by EventLoop")
		}
		if !emittedIsPlayingStop {
			t.Error("Expected IsPlayingChanged false signal to be processed by EventLoop")
		}
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for AudioPlayer signals via EventLoop")
	}
}
