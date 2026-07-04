package data

import (
	"log"
	"sync"
	"github.com/robertpelloni/bqt/internal/net"
)

type TimeMachine struct {
	mu sync.Mutex
}

var (
	tmInstance *TimeMachine
	tmOnce     sync.Once
)

func GetTimeMachine() *TimeMachine {
	tmOnce.Do(func() { tmInstance = &TimeMachine{} })
	return tmInstance
}

// CommitState saves a file snapshot and broadcasts to the Go mesh.
func (tm *TimeMachine) CommitState(path string, content string) error {
	db := GetDatabase()
	_, err := db.conn.Exec("INSERT INTO _omni_timemachine (filepath, content) VALUES (?, ?)", path, content)
	if err != nil { return err }

	// --- DISTRIBUTED TEMPORAL SYNC ---
	payload := map[string]interface{}{
		"type":    "ledger_update",
		"path":    path,
		"content": content,
	}
	net.GetMeshNode().Broadcast(payload)
	
	log.Printf("OmniTimeMachine Go: Local state committed and broadcasted for %s", path)
	return nil
}

func (tm *TimeMachine) ReceiveRemoteCommit(path, content string) {
	db := GetDatabase()
	db.conn.Exec("INSERT INTO _omni_timemachine (filepath, content) VALUES (?, ?)", path, content)
	log.Printf("OmniTimeMachine Go: Remote Peer history synced for %s", path)
}
