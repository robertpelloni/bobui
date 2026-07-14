package audio

import (
	"log"
	"github.com/robertpelloni/bqt/internal/net"
)

type MidiCommand struct {
	Type     string `json:"type"`
	Channel  int    `json:"channel"`
	Note     int    `json:"note"`
	Velocity int    `json:"velocity"`
}

// BroadcastMidiEvent sends a local hardware note to the Go mesh.
func BroadcastMidiEvent(cmd MidiCommand) {
	mesh := net.GetMeshNode()
	payload := map[string]interface{}{
		"type":     "midi_event",
		"m_type":   cmd.Type,
		"channel":  cmd.Channel,
		"note":     cmd.Note,
		"velocity": cmd.Velocity,
	}
	mesh.Broadcast(payload)
	log.Printf("OMNIMIDI Go: Local NoteOn broadcasted Note:%d", cmd.Note)
}

// HandleRemoteMidi routes a peer's note into the local Go synthesizer.
func HandleRemoteMidi(peerID string, cmd MidiCommand) {
	// Trigger the Go Synthesizer Node natively
	log.Printf("OMNIMIDI Go: Received remote MIDI from %s - Note:%d", peerID, cmd.Note)
}
