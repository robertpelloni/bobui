package audio

import (
	"sync"
	"testing"
	"time"
	"github.com/robertpelloni/bqt/internal/ui"
)

func TestSpatialAudioPanCalculation(t *testing.T) {
	sa := GetSpatialAudio()
	sa.SetScreenDimensions(1000.0, 1000.0)

	// Left edge should be -1.0
	if pan := sa.CalculatePan(0.0, 500.0); pan != -1.0 {
		t.Errorf("Expected pan -1.0, got %f", pan)
	}

	// Right edge should be 1.0
	if pan := sa.CalculatePan(1000.0, 500.0); pan != 1.0 {
		t.Errorf("Expected pan 1.0, got %f", pan)
	}

	// Center should be 0.0
	if pan := sa.CalculatePan(500.0, 500.0); pan != 0.0 {
		t.Errorf("Expected pan 0.0, got %f", pan)
	}
}

func TestSpatialAudioEventLoopIntegration(t *testing.T) {
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	sa := GetSpatialAudio()
	sa.SetScreenDimensions(1000.0, 1000.0)

	// Reset signals to prevent singleton pollution
	sa.PlayClick = ui.NewSignal("PlaySpatialClick")
	sa.PlayNotification = ui.NewSignal("PlaySpatialNotification")

	var wg sync.WaitGroup
	wg.Add(2)

	clickReceived := false
	notificationReceived := false

	sa.PlayClick.Connect(func(args ...interface{}) {
		defer wg.Done()
		if len(args) >= 2 {
			if pan, ok := args[1].(float32); ok && pan == -1.0 {
				clickReceived = true
			}
		}
	})

	sa.PlayNotification.Connect(func(args ...interface{}) {
		defer wg.Done()
		if len(args) >= 3 {
			if pan, ok := args[2].(float32); ok && pan == 1.0 {
				notificationReceived = true
			}
		}
	})

	sa.PlaySpatialClick("mouse1", 0.0, 500.0, 1.0)
	sa.PlaySpatialNotification("mouse1", 1000.0, 500.0, "beep.wav")

	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		if !clickReceived {
			t.Error("Expected PlayClick signal to be processed by EventLoop with -1.0 pan")
		}
		if !notificationReceived {
			t.Error("Expected PlayNotification signal to be processed by EventLoop with 1.0 pan")
		}
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for SpatialAudio signals via EventLoop")
	}
}
