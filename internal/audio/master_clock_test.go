package audio

import (
	"sync"
	"testing"
	"time"
	"github.com/robertpelloni/bqt/internal/ui"
)

func TestMasterClockEventLoopIntegration(t *testing.T) {
	// Initialize UI EventLoop
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	mc := GetMasterClock()

	// Reset signals to prevent singleton pollution across tests
	mc.BpmChanged = ui.NewSignal("BpmChanged")
	mc.IsRunningChanged = ui.NewSignal("IsRunningChanged")
	mc.Beat = ui.NewSignal("Beat")
	mc.SixteenthStep = ui.NewSignal("SixteenthStep")

	var wg sync.WaitGroup
	wg.Add(3)

	bpmChangedReceived := false
	isRunningStartedReceived := false
	sixteenthStepReceived := false

	mc.BpmChanged.Connect(func(args ...interface{}) {
		defer wg.Done()
		bpmChangedReceived = true
	})

	var isRunningOnce sync.Once
	mc.IsRunningChanged.Connect(func(args ...interface{}) {
		if len(args) > 0 {
			if isRunning, ok := args[0].(bool); ok && isRunning {
				isRunningOnce.Do(func() {
					defer wg.Done()
					isRunningStartedReceived = true
				})
			}
		}
	})

	var sixteenthStepOnce sync.Once
	mc.SixteenthStep.Connect(func(args ...interface{}) {
		sixteenthStepOnce.Do(func() {
			defer wg.Done()
			sixteenthStepReceived = true
		})
	})

	mc.SetBpm(140.0)
	mc.Start()

	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		mc.Stop()
		if !bpmChangedReceived {
			t.Error("Expected BpmChanged signal to be processed by EventLoop")
		}
		if !isRunningStartedReceived {
			t.Error("Expected IsRunningChanged signal to be processed by EventLoop")
		}
		if !sixteenthStepReceived {
			t.Error("Expected SixteenthStep signal to be processed by EventLoop")
		}
	case <-time.After(3 * time.Second):
		mc.Stop()
		t.Fatal("Timeout waiting for MasterClock signals via EventLoop")
	}
}
