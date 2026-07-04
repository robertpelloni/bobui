package data

import "testing"

func TestVFSMountAndGetAsset(t *testing.T) {
	vfs := &VFS{assets: make(map[string][]byte)}
	blob := []byte("hello bqt")
	vfs.Mount("docs/readme.txt", blob)

	got := vfs.GetAsset("docs/readme.txt")
	if string(got) != "hello bqt" {
		t.Fatalf("expected mounted asset contents, got %q", string(got))
	}
	if missing := vfs.GetAsset("missing"); missing != nil {
		t.Fatalf("expected nil for missing asset, got %v", missing)
	}
}

func TestDatabaseConnectAndExecuteQueryInMemory(t *testing.T) {
	db := &Database{}
	if err := db.Connect(":memory:"); err != nil {
		t.Fatalf("expected in-memory database connection to succeed: %v", err)
	}
	defer db.conn.Close()

	if _, err := db.conn.Exec(`CREATE TABLE items (id INTEGER, name TEXT);`); err != nil {
		t.Fatalf("expected table creation to succeed: %v", err)
	}
	if _, err := db.conn.Exec(`INSERT INTO items (id, name) VALUES (1, 'alpha'), (2, 'beta');`); err != nil {
		t.Fatalf("expected insert to succeed: %v", err)
	}

	rows, err := db.ExecuteQuery(`SELECT id, name FROM items ORDER BY id;`)
	if err != nil {
		t.Fatalf("expected query to succeed: %v", err)
	}
	if len(rows) != 2 {
		t.Fatalf("expected 2 result rows, got %d", len(rows))
	}
	if rows[0]["name"] != "alpha" {
		t.Fatalf("expected first row name alpha, got %#v", rows[0]["name"])
	}
	if rows[1]["name"] != "beta" {
		t.Fatalf("expected second row name beta, got %#v", rows[1]["name"])
	}
}

func TestHistoryReconcilerAndPackageRegistryMeshCallsDoNotPanicWithoutPeers(t *testing.T) {
	hr := &HistoryReconciler{Active: true}
	hr.RequestMeshSync("/tmp/project")
	hr.MergeLedgers("peer-1", "/tmp/project", []string{"a", "b"})

	pr := &PackageRegistry{}
	pr.FetchRemotePackage("peer-1", "demo/module")
	pr.HandleRemotePackage("peer-1", "demo/module", []byte("payload"))
}
