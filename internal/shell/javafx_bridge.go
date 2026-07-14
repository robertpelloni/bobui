package shell

import (
	"log"
)

// JFXNode represents a remote JavaFX Node mapping.
type JFXNode struct {
	ID    string
	Class string
}

// JavaFXBridge extends ShellBridge to support JFX integration.
type JavaFXBridge struct {
	*ShellBridge
	nodes map[string]*JFXNode
}

// NewJavaFXBridge initializes the specific JavaFX integration layer.
func NewJavaFXBridge() *JavaFXBridge {
	return &JavaFXBridge{
		ShellBridge: NewShellBridge(),
		nodes:       make(map[string]*JFXNode),
	}
}

// MapJFXNode maps a JavaFX Scene/Node into the BQt tracking system.
func (jb *JavaFXBridge) MapJFXNode(nodeID, nodeClass string) {
	jb.nodes[nodeID] = &JFXNode{ID: nodeID, Class: nodeClass}
	log.Printf("BQt/JFX Bridge: Mapped JavaFX Node '%s' (Class: %s)", nodeID, nodeClass)
}

// DispatchJFXEvent simulates receiving an event from Java via JNI and pushing it non-blockingly into the BQt EventLoop.
func (jb *JavaFXBridge) DispatchJFXEvent(nodeID, eventName, payload string) {
	if _, exists := jb.nodes[nodeID]; !exists {
		log.Printf("BQt/JFX Bridge Warning: Attempted to dispatch event for unmapped node '%s'", nodeID)
		return
	}

	// The critical link: Using the unified EventLoop to process external shell events asynchronously
	jb.EventLoop.Post(func() {
		log.Printf("BQt/JFX Bridge: Handled Event '%s' on Node '%s' asynchronously (Payload: %s)", eventName, nodeID, payload)
	})
}
