package audio

import (
	"log"
	"github.com/robertpelloni/bqt/internal/net"
)

type DSPTask struct {
	NodeID string
	Buffer []float32
}

// DistributeDSPTask shards a heavy audio node to the Go mesh for remote processing.
func DistributeDSPTask(nodeID string, buffer []float32) {
	mesh := net.GetMeshNode()
	payload := map[string]interface{}{
		"type":    "dsp_offload",
		"node_id": nodeID,
		"data":    buffer,
	}
	mesh.Broadcast(payload)
	log.Printf("OMNIAUDIO Go: DSP Task [%s] offloaded to Mesh Cluster.", nodeID)
}

// HandleRemoteDSP handles an incoming task from a struggling peer.
func HandleRemoteDSP(peerID, nodeID string, buffer []float32) {
	log.Printf("OMNIAUDIO Go: Processing remote DSP task for %s", peerID)
	// Process natively using local Go AudioGraph...
	GetAudioGraph().ProcessBlock(buffer)
	
	// Send back to the original peer
	net.GetMeshNode().Broadcast(map[string]interface{}{
		"type":    "dsp_result",
		"node_id": nodeID,
		"data":    buffer,
	})
}
