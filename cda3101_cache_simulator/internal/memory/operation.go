package memory

import "fmt"

// OperationKind is a kind of operation.
type OperationKind byte

const (
	// OperationLoad is a memory load operation.
	OperationLoad OperationKind = 'l'

	// OperationStore is a memory store operation.
	OperationStore OperationKind = 's'
)

// Operation is a memory operation; kind and address.
type Operation struct {
	Kind    OperationKind
	Address uint
	Size    uint
}

func (o Operation) String() string {
	var kind string

	switch o.Kind {
	case OperationLoad:
		kind = "load"
	case OperationStore:
		kind = "store"
	}

	return fmt.Sprintf("%v/0x%08x", kind, o.Address)
}
