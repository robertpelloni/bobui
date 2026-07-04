package audio

import (
	"fmt"
	"sync"
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
}

var (
	graphInstance *AudioGraph
	graphOnce     sync.Once
)

func GetAudioGraph() *AudioGraph {
	graphOnce.Do(func() {
		graphInstance = &AudioGraph{
			nodes: make(map[string]AudioNode),
			links: make(map[string][]string),
		}
	})
	return graphInstance
}

// AddNode injects a new DSP component into the Go audio engine.
func (ag *AudioGraph) AddNode(node AudioNode) {
	ag.mu.Lock()
	defer ag.mu.Unlock()
	ag.nodes[node.Name()] = node
}

// Connect links two nodes for real-time PCM streaming.
func (ag *AudioGraph) Connect(srcName, destName string) error {
	ag.mu.Lock()
	defer ag.mu.Unlock()

	if _, ok := ag.nodes[srcName]; !ok {
		return fmt.Errorf("node %s not found", srcName)
	}
	if _, ok := ag.nodes[destName]; !ok {
		return fmt.Errorf("node %s not found", destName)
	}

	ag.links[srcName] = append(ag.links[srcName], destName)
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
	defer ag.mu.Unlock()
	if ag.isRunning {
		return
	}
	ag.isRunning = true
	fmt.Println("AudioGraph: Audio processing thread STARTED. (Simulating JUCE DeviceManager)")
}

func (ag *AudioGraph) Stop() {
	ag.mu.Lock()
	defer ag.mu.Unlock()
	if !ag.isRunning {
		return
	}
	ag.isRunning = false
	fmt.Println("AudioGraph: Audio processing thread STOPPED.")
}
