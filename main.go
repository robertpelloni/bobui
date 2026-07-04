package main

import (
	"log"
	"gioui.org/app"
	"github.com/robertpelloni/bqt/internal/kernel"
	"github.com/robertpelloni/bqt/internal/net"
	"github.com/robertpelloni/bqt/internal/ui"
	"github.com/robertpelloni/bqt/internal/vm"
)

func main() {
	log.Println("OmniUI Go: Orchestrating Cognitive Singularity...")

	// 1. Setup AI Assistant & Healer
	assistant := vm.GetAIAssistant()
	healer := kernel.GetSelfHealer()

	// 2. Wrap main logic in AI Self-Healing Kernel
	defer healer.Intercept("KernelMain", func(report string) {
		assistant.RequestCompletion(report)
	})

	// 3. Setup Distributed Mesh
	net.GetMeshNode().StartNode("8081", nil, nil)

	// 4. Launch GPU Engine
	go func() {
		engine := ui.NewEngine()
		if err := engine.Run(); err != nil { log.Fatal(err) }
	}()

	app.Main()
}
