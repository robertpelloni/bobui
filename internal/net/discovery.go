package net

import (
	"fmt"
	"log"
	"net"
	"time"
)

type DiscoveryBeacon struct {
	Active bool
}

func GetDiscoveryBeacon() *DiscoveryBeacon {
	return &DiscoveryBeacon{Active: true}
}

// StartBroadcasting pings the network so other BQt nodes can find us.
func (db *DiscoveryBeacon) StartBroadcasting(port int) {
	addr, _ := net.ResolveUDPAddr("udp", "255.255.255.255:9999")
	conn, _ := net.DialUDP("udp", nil, addr)

	go func() {
		for db.Active {
			msg := fmt.Sprintf("OMNI_NODE:%d", port)
			conn.Write([]byte(msg))
			time.Sleep(5 * time.Second)
		}
	}()
	log.Println("OMNICLOUD Go: Zero-Config Discovery Beacon started.")
}

// ListenForPeers captures remote beacons and adds them to our Mesh.
func (db *DiscoveryBeacon) ListenForPeers() {
	addr, _ := net.ResolveUDPAddr("udp", ":9999")
	conn, _ := net.ListenUDP("udp", addr)

	go func() {
		buf := make([]byte, 1024)
		for db.Active {
			n, remoteAddr, _ := conn.ReadFromUDP(buf)
			log.Printf("OMNICLOUD Go: Discovered remote peer at %s -> %s", remoteAddr, string(buf[:n]))
			// Add to MeshNode automatically...
		}
	}()
}
