package ui

import (
	"log"
	"math"
	"github.com/robertpelloni/bqt/internal/kernel"
)

type HapticsEngine struct {
	Enabled bool
}

// ProcessProximity calculates spatial drones based on peer cursor distances.
func (he *HapticsEngine) ProcessProximity() {
	if !he.Enabled { return }
	
	im := kernel.GetInputManager()
	devices := im.GetDevices()
	
	if len(devices) < 2 { return }

	// Calculate distance between Local User (0) and Peers
	local := devices[0]
	for i := 1; i < len(devices); i++ {
		peer := devices[i]
		dist := math.Sqrt(math.Pow(local.X-peer.X, 2) + math.Pow(local.Y-peer.Y, 2))
		
		if dist < 100 {
			// Trigger Spatial Audio Drone
			log.Printf("OMNIHAPTIC Go: Peer Proximity Detected (%f px)", dist)
		}
	}
}
