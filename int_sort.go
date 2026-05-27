package sort

/*
#include <int_sort.h>
#include <stdlib.h>
*/
import "C"
import (
	"unsafe"
)

// SortInt64 sorts a slice of int64 values in-place using radix sort
func SortInt64(data []int64, order SortOrder) error {
	if len(data) == 0 {
		return nil
	}

	status := C.fc_sort_int64(
		(*C.int64_t)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return cErrorToGo(status)
}

// SortUint64 sorts a slice of uint64 values in-place using radix sort
func SortUint64(data []uint64, order SortOrder) error {
	if len(data) == 0 {
		return nil
	}

	status := C.fc_sort_uint64(
		(*C.uint64_t)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return cErrorToGo(status)
}

// SortInt32 sorts a slice of int32 values in-place using radix sort
func SortInt32(data []int32, order SortOrder) error {
	if len(data) == 0 {
		return nil
	}

	status := C.fc_sort_int32(
		(*C.int32_t)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return cErrorToGo(status)
}

// SortUint32 sorts a slice of uint32 values in-place using radix sort
func SortUint32(data []uint32, order SortOrder) error {
	if len(data) == 0 {
		return nil
	}

	status := C.fc_sort_uint32(
		(*C.uint32_t)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return cErrorToGo(status)
}

// IsSortedInt64 checks if a slice of int64 values is sorted
func IsSortedInt64(data []int64, order SortOrder) bool {
	if len(data) == 0 {
		return true
	}

	result := C.fc_sort_is_sorted_int64(
		(*C.int64_t)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return result != 0
}

// IsSortedUint64 checks if a slice of uint64 values is sorted
func IsSortedUint64(data []uint64, order SortOrder) bool {
	if len(data) == 0 {
		return true
	}

	result := C.fc_sort_is_sorted_uint64(
		(*C.uint64_t)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return result != 0
}

// IsSortedInt32 checks if a slice of int32 values is sorted
func IsSortedInt32(data []int32, order SortOrder) bool {
	if len(data) == 0 {
		return true
	}

	result := C.fc_sort_is_sorted_int32(
		(*C.int32_t)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return result != 0
}

// IsSortedUint32 checks if a slice of uint32 values is sorted
func IsSortedUint32(data []uint32, order SortOrder) bool {
	if len(data) == 0 {
		return true
	}

	result := C.fc_sort_is_sorted_uint32(
		(*C.uint32_t)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return result != 0
}

// SortInt sorts a slice of int values in-place using radix sort
// This is a convenience wrapper that handles platform-specific int sizes
func SortInt(data []int, order SortOrder) error {
	if len(data) == 0 {
		return nil
	}

	// Convert to int64 for sorting (works on both 32-bit and 64-bit platforms)
	data64 := make([]int64, len(data))
	for i, v := range data {
		data64[i] = int64(v)
	}

	if err := SortInt64(data64, order); err != nil {
		return err
	}

	for i, v := range data64 {
		data[i] = int(v)
	}

	return nil
}

// SortUint sorts a slice of uint values in-place using radix sort
// This is a convenience wrapper that handles platform-specific uint sizes
func SortUint(data []uint, order SortOrder) error {
	if len(data) == 0 {
		return nil
	}

	// Convert to uint64 for sorting (works on both 32-bit and 64-bit platforms)
	data64 := make([]uint64, len(data))
	for i, v := range data {
		data64[i] = uint64(v)
	}

	if err := SortUint64(data64, order); err != nil {
		return err
	}

	for i, v := range data64 {
		data[i] = uint(v)
	}

	return nil
}
