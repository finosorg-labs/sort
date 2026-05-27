package sort

/*
#include <multikey.h>
#include <stdlib.h>
*/
import "C"
import (
	"fmt"
	"reflect"
)

// KeyType represents the data type of a sort key
type KeyType int

const (
	KeyTypeInt32  KeyType = C.FC_SORT_KEY_INT32
	KeyTypeInt64  KeyType = C.FC_SORT_KEY_INT64
	KeyTypeUInt32 KeyType = C.FC_SORT_KEY_UINT32
	KeyTypeUInt64 KeyType = C.FC_SORT_KEY_UINT64
	KeyTypeFloat  KeyType = C.FC_SORT_KEY_FLOAT
	KeyTypeDouble KeyType = C.FC_SORT_KEY_DOUBLE
)


// KeySpec specifies a single sort key
type KeySpec struct {
	Offset uintptr  // Byte offset of the field in the struct
	Type   KeyType  // Data type of the field
	Order  SortOrder // Sort order (ascending or descending)
}

// MultiKeySort sorts a slice of structs by multiple keys in-place.
//
// The data parameter must be a slice of structs. Keys are evaluated in order:
// if two records are equal on keys[0], they are compared on keys[1], and so on.
//
// Example:
//
//	type Order struct {
//	    Price   float64
//	    Time    int64
//	    OrderID uint32
//	}
//
//	orders := []Order{...}
//	keys := []KeySpec{
//	    {Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Descending},
//	    {Offset: unsafe.Offsetof(Order{}.Time), Type: KeyTypeInt64, Order: Ascending},
//	    {Offset: unsafe.Offsetof(Order{}.OrderID), Type: KeyTypeUInt32, Order: Ascending},
//	}
//	err := MultiKeySort(orders, keys)
func MultiKeySort(data interface{}, keys []KeySpec) error {
	if len(keys) == 0 {
		return fmt.Errorf("at least one key is required")
	}
	if len(keys) > C.FC_SORT_MAX_KEYS {
		return fmt.Errorf("too many keys (max %d)", C.FC_SORT_MAX_KEYS)
	}

	val := reflect.ValueOf(data)
	if val.Kind() != reflect.Slice {
		return fmt.Errorf("data must be a slice")
	}

	count := val.Len()
	if count == 0 {
		return nil
	}

	elemType := val.Type().Elem()
	if elemType.Kind() != reflect.Struct {
		return fmt.Errorf("slice elements must be structs")
	}

	recordSize := elemType.Size()
	dataPtr := val.Index(0).Addr().UnsafePointer()

	cKeys := make([]C.fc_sort_key_spec_t, len(keys))
	for i, key := range keys {
		cKeys[i].offset = C.size_t(key.Offset)
		cKeys[i]._type = C.fc_sort_key_type_t(key.Type)
		cKeys[i].order = C.fc_sort_order_t(key.Order)
	}

	status := C.fc_sort_multikey(
		dataPtr,
		C.size_t(count),
		C.size_t(recordSize),
		&cKeys[0],
		C.size_t(len(keys)),
	)

	if status != C.FC_OK {
		return fmt.Errorf("sort failed with status %d", status)
	}

	return nil
}

// MultiKeySortIndexed sorts a slice of structs by multiple keys and returns
// the sorted indices without modifying the original data.
//
// Returns a slice of indices representing the sorted order.
//
// Example:
//
//	orders := []Order{...}
//	indices, err := MultiKeySortIndexed(orders, keys)
//	// Access sorted order: orders[indices[0]], orders[indices[1]], ...
func MultiKeySortIndexed(data interface{}, keys []KeySpec) ([]int, error) {
	if len(keys) == 0 {
		return nil, fmt.Errorf("at least one key is required")
	}
	if len(keys) > C.FC_SORT_MAX_KEYS {
		return nil, fmt.Errorf("too many keys (max %d)", C.FC_SORT_MAX_KEYS)
	}

	val := reflect.ValueOf(data)
	if val.Kind() != reflect.Slice {
		return nil, fmt.Errorf("data must be a slice")
	}

	count := val.Len()
	if count == 0 {
		return []int{}, nil
	}

	elemType := val.Type().Elem()
	if elemType.Kind() != reflect.Struct {
		return nil, fmt.Errorf("slice elements must be structs")
	}

	recordSize := elemType.Size()
	dataPtr := val.Index(0).Addr().UnsafePointer()

	cKeys := make([]C.fc_sort_key_spec_t, len(keys))
	for i, key := range keys {
		cKeys[i].offset = C.size_t(key.Offset)
		cKeys[i]._type = C.fc_sort_key_type_t(key.Type)
		cKeys[i].order = C.fc_sort_order_t(key.Order)
	}

	indices := make([]C.size_t, count)

	status := C.fc_sort_multikey_indexed(
		dataPtr,
		C.size_t(count),
		C.size_t(recordSize),
		&cKeys[0],
		C.size_t(len(keys)),
		&indices[0],
	)

	if status != C.FC_OK {
		return nil, fmt.Errorf("indexed sort failed with status %d", status)
	}

	result := make([]int, count)
	for i := 0; i < count; i++ {
		result[i] = int(indices[i])
	}

	return result, nil
}

// IsSortedMultiKey checks if a slice is sorted according to the key specifications.
func IsSortedMultiKey(data interface{}, keys []KeySpec) bool {
	if len(keys) == 0 {
		return false
	}

	val := reflect.ValueOf(data)
	if val.Kind() != reflect.Slice {
		return false
	}

	count := val.Len()
	if count == 0 {
		return true
	}

	elemType := val.Type().Elem()
	if elemType.Kind() != reflect.Struct {
		return false
	}

	recordSize := elemType.Size()
	dataPtr := val.Index(0).Addr().UnsafePointer()

	cKeys := make([]C.fc_sort_key_spec_t, len(keys))
	for i, key := range keys {
		cKeys[i].offset = C.size_t(key.Offset)
		cKeys[i]._type = C.fc_sort_key_type_t(key.Type)
		cKeys[i].order = C.fc_sort_order_t(key.Order)
	}

	result := C.fc_sort_is_sorted_multikey(
		dataPtr,
		C.size_t(count),
		C.size_t(recordSize),
		&cKeys[0],
		C.size_t(len(keys)),
	)

	return result != 0
}
