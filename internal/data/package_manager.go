package data

import (
	"log"
	"github.com/robertpelloni/bqt/internal/net"
)

type PackageRegistry struct {
	AvailableModules []string
}

var (
	pkgInstance *PackageRegistry
)

func GetPackageRegistry() *PackageRegistry {
	if pkgInstance == nil {
		pkgInstance = &PackageRegistry{AvailableModules: make([]string, 0)}
	}
	return pkgInstance
}

// FetchRemotePackage requests a binary Go module from a mesh peer.
func (pr *PackageRegistry) FetchRemotePackage(peerID, moduleName string) {
	payload := map[string]interface{}{
		"type":    "package_request",
		"module":  moduleName,
	}
	net.GetMeshNode().Broadcast(payload)
	log.Printf("OMNIPACKAGE Go: Requested remote module [%s] from %s", moduleName, peerID)
}

// HandleRemotePackage streams the binary module into the local PluginHost.
func (pr *PackageRegistry) HandleRemotePackage(peerID, moduleName string, data []byte) {
	log.Printf("OMNIPACKAGE Go: Received binary package [%s] from %s", moduleName, peerID)
	// Native hot-loading via kernel/plugin_host.go...
}
