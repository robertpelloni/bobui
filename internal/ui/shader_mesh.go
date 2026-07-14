package ui

import (
	"log"
	"github.com/robertpelloni/bqt/internal/net"
)

type ShaderSync struct {
	ActiveShaders map[string]string // TargetID -> FragmentShaderSource
}

var (
	shaderInstance *ShaderSync
)

func GetShaderSync() *ShaderSync {
	if shaderInstance == nil {
		shaderInstance = &ShaderSync{ActiveShaders: make(map[string]string)}
	}
	return shaderInstance
}

// BroadcastShaderUpdate blasts a new visual skin to all Go peers.
func (ss *ShaderSync) BroadcastShaderUpdate(targetID, source string) {
	ss.ActiveShaders[targetID] = source
	
	payload := map[string]interface{}{
		"type":    "shader_sync",
		"target":  targetID,
		"content": source,
	}
	net.GetMeshNode().Broadcast(payload)
	log.Printf("OMNIGFX Go: New Shader Skin broadcasted for %s", targetID)
}

// HandleRemoteShader handles incoming visual skins from P2P peers.
func (ss *ShaderSync) HandleRemoteShader(peerID, targetID, source string) {
	log.Printf("OMNIGFX Go: Syncing remote shader from %s for %s", peerID, targetID)
	ss.ActiveShaders[targetID] = source
	// Trigger local GPU recompile of the target fragment shader...
}
