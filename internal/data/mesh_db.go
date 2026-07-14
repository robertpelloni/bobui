package data

import (
	"log"
	"github.com/robertpelloni/bqt/internal/net"
)

type DistributedDB struct {
	LocalDB *Database
}

var (
	meshDBInstance *DistributedDB
)

func GetMeshDB() *DistributedDB {
	if meshDBInstance == nil {
		meshDBInstance = &DistributedDB{LocalDB: GetDatabase()}
	}
	return meshDBInstance
}

// ExecuteReplicatedQuery runs SQL locally and broadcasts the instruction to the P2P Mesh.
func (m *DistributedDB) ExecuteReplicatedQuery(query string) error {
	// 1. Execute Locally
	_, err := m.LocalDB.ExecuteQuery(query)
	if err != nil { return err }

	// 2. Broadcast SQL Payload to Mesh
	payload := map[string]interface{}{
		"type":  "sql_transaction",
		"query": query,
	}
	net.GetMeshNode().Broadcast(payload)
	
	log.Printf("OMNIDATA Go: Replicated SQL Transaction broadcasted.")
	return nil
}

// HandleRemoteTransaction executes a SQL payload received from a P2P peer.
func (m *DistributedDB) HandleRemoteTransaction(peerID string, query string) {
	log.Printf("OMNIDATA Go: Syncing remote transaction from %s", peerID)
	m.LocalDB.ExecuteQuery(query)
}
