package audio

import (
	"fmt"
	"sync"
	"github.com/robertpelloni/bqt/internal/ui"
)

// AudioNode defines the contract for all Go-native DSP components.
type AudioNode interface {
	Name() string
	Process(buffer []float32)
}

type AudioGraph struct {
	mu        sync.RWMutex
	nodes     map[string]AudioNode
	links     map[string][]string // SrcID -> DestIDs
	isRunning bool

	GraphChanged      *ui.Signal
	ProcessingStarted *ui.Signal
	ProcessingStopped *ui.Signal
}

var (
	graphInstance *AudioGraph
	graphOnce     sync.Once
)

func GetAudioGraph() *AudioGraph {
	graphOnce.Do(func() {
		graphInstance = &AudioGraph{
			nodes:             make(map[string]AudioNode),
			links:             make(map[string][]string),
			GraphChanged:      ui.NewSignal("GraphChanged"),
			ProcessingStarted: ui.NewSignal("ProcessingStarted"),
			ProcessingStopped: ui.NewSignal("ProcessingStopped"),
		}
	})
	return graphInstance
}

// AddNode injects a new DSP component into the Go audio engine.
func (ag *AudioGraph) AddNode(node AudioNode) {
	ag.mu.Lock()
	ag.nodes[node.Name()] = node
	ag.mu.Unlock()

	ag.GraphChanged.Emit()
}

// Connect links two nodes for real-time PCM streaming.
func (ag *AudioGraph) Connect(srcName, destName string) error {
	ag.mu.Lock()

	if _, ok := ag.nodes[srcName]; !ok {
		ag.mu.Unlock()
		return fmt.Errorf("node %s not found", srcName)
	}
	if _, ok := ag.nodes[destName]; !ok {
		ag.mu.Unlock()
		return fmt.Errorf("node %s not found", destName)
	}

	ag.links[srcName] = append(ag.links[srcName], destName)
	ag.mu.Unlock()

	ag.GraphChanged.Emit()
	return nil
}

func (ag *AudioGraph) ProcessBlock(buffer []float32) {
	ag.mu.RLock()
	defer ag.mu.RUnlock()

	// Native Go Parallel DSP Processing
	for _, node := range ag.nodes {
		node.Process(buffer)
	}
}

func (ag *AudioGraph) Start() {
	ag.mu.Lock()
	if ag.isRunning {
		ag.mu.Unlock()
		return
	}
	ag.isRunning = true
	ag.mu.Unlock()

	fmt.Println("AudioGraph: Audio processing thread STARTED. (Simulating JUCE DeviceManager)")
	ag.ProcessingStarted.Emit()
}

func (ag *AudioGraph) Stop() {
	ag.mu.Lock()
	if !ag.isRunning {
		ag.mu.Unlock()
		return
	}
	ag.isRunning = false
	ag.mu.Unlock()

	fmt.Println("AudioGraph: Audio processing thread STOPPED.")
	ag.ProcessingStopped.Emit()
}
