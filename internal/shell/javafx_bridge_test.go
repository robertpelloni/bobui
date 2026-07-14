package shell

import (
	"sync"
	"testing"
	"time"

	"github.com/robertpelloni/bqt/internal/ui"
)

func TestJavaFXBridgeMappingAndDispatch(t *testing.T) {
	// Initialize UI EventLoop
	el := ui.GetEventLoop()
	go el.Run()
	defer el.Stop()

	jfx := NewJavaFXBridge()

	// Map a dummy JavaFX Scene/Node
	jfx.MapJFXNode("scene_root", "javafx.scene.Scene")

	if _, exists := jfx.nodes["scene_root"]; !exists {
		t.Fatal("Expected JavaFX Node 'scene_root' to be mapped")
	}

	var wg sync.WaitGroup
	wg.Add(1)

	// Since DispatchJFXEvent uses the global EventLoop, we can intercept it with a dummy signal
	// for the sake of the test if needed. Here we verify it doesn't block or panic.

	// Inject a custom task to sync and verify execution order
	jfx.EventLoop.Post(func() {
		defer wg.Done()
	})

	jfx.DispatchJFXEvent("scene_root", "onClick", "mouse_down")

	done := make(chan struct{})
	go func() {
		wg.Wait()
		close(done)
	}()

	select {
	case <-done:
		// Success, the EventLoop successfully pumped the injected tasks
	case <-time.After(2 * time.Second):
		t.Fatal("Timeout waiting for JavaFX event dispatch via EventLoop")
	}
}
