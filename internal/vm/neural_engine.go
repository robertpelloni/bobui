package vm

import (
	"encoding/json"
	"sync"
	"github.com/robertpelloni/bqt/internal/ui"
)

type NeuralEngine struct {
	mu sync.Mutex
}

var (
	neuralInstance *NeuralEngine
	neuralOnce     sync.Once
)

func GetNeuralEngine() *NeuralEngine {
	neuralOnce.Do(func() {
		neuralInstance = &NeuralEngine{}
	})
	return neuralInstance
}

// CaptureDOM serializes the entire Go Window Manager state into JSON for LLMs.
func (ne *NeuralEngine) CaptureDOM() (string, error) {
	wm := ui.GetWindowManager()
	
	// Create a structured representation of the Go UI
	dom := map[string]interface{}{
		"os": "BQt-Go",
		"windows": wm.Windows,
	}

	data, err := json.Marshal(dom)
	if err != nil {
		return "", err
	}
	return string(data), nil
}
