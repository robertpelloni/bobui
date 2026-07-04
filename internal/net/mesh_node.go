package net

import (
	"net/http"
	"sync"
	"gioui.org/f32"
	"github.com/gorilla/websocket"
	"github.com/robertpelloni/bqt/internal/kernel"
)

var upgrader = websocket.Upgrader{CheckOrigin: func(r *http.Request) bool { return true }}

type MeshNode struct {
	mu    sync.Mutex
	peers map[*websocket.Conn]bool
}

var (
	nodeInstance *MeshNode
	nodeOnce     sync.Once
)

func GetMeshNode() *MeshNode {
	nodeOnce.Do(func() { nodeInstance = &MeshNode{peers: make(map[*websocket.Conn]bool)} })
	return nodeInstance
}

func (mn *MeshNode) StartNode(port string, spawnCallback func(string, string, f32.Point, f32.Point, bool), ledgerSync func(string, string)) {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		conn, err := upgrader.Upgrade(w, r, nil)
		if err != nil { return }
		mn.mu.Lock(); mn.peers[conn] = true; mn.mu.Unlock()
		defer func() { mn.mu.Lock(); delete(mn.peers, conn); mn.mu.Unlock(); conn.Close() }()

		for {
			var msg map[string]interface{}
			if err := conn.ReadJSON(&msg); err != nil { break }
			
			mType := msg["type"].(string)
			if mType == "cursor" {
				kernel.GetInputManager().UpdateCursor("net-"+msg["deviceId"].(string), msg["x"].(float64), msg["y"].(float64))
			} else if mType == "ledger_update" {
				// --- DISTRIBUTED TEMPORAL SYNC ---
				ledgerSync(msg["path"].(string), msg["content"].(string))
			}
		}
	})
	go http.ListenAndServe(":"+port, nil)
}

func (mn *MeshNode) Broadcast(payload interface{}) {
	mn.mu.Lock(); defer mn.mu.Unlock()
	for peer := range mn.peers { peer.WriteJSON(payload) }
}
