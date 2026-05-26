package sort

/*
#include <float_sort.h>
#include <stdlib.h>
*/
import "C"
import (
	"errors"
	"unsafe"
)

// SortOrder represents the sort order
type SortOrder int

const (
	// Ascending sort order
	Ascending SortOrder = 0
	// Descending sort order
	Descending SortOrder = 1
)

// NaNPolicy represents how to handle NaN values during sorting
type NaNPolicy int

const (
	// NaNFirst places NaN values at the beginning
	NaNFirst NaNPolicy = 0
	// NaNLast places NaN values at the end
	NaNLast NaNPolicy = 1
	// NaNReject returns an error if NaN is encountered
	NaNReject NaNPolicy = 2
)

var (
	// ErrNullPointer is returned when a null pointer is passed
	ErrNullPointer = errors.New("null pointer")
	// ErrInvalidInput is returned for invalid input (e.g., NaN with NaNReject policy)
	ErrInvalidInput = errors.New("invalid input")
	// ErrUnknown is returned for unknown errors
	ErrUnknown = errors.New("unknown error")
)

func cErrorToGo(err C.fc_status_t) error {
	switch err {
	case C.FC_OK:
		return nil
	case C.FC_ERR_INVALID_ARG:
		return ErrInvalidInput
	default:
		return ErrUnknown
	}
}

// Float64 sorts a float64 slice in-place using the default NaN handling (NaNLast)
//
// Automatically selects optimal algorithm based on slice size:
//   - Small (< 32): Sorting network
//   - Medium (32-10000): Introsort
//   - Large (> 10000): Radix sort
//
// Time complexity: O(n log n) average, O(n) for nearly sorted data
// Space complexity: O(log n) stack space
//
// Example:
//
//	data := []float64{3.0, 1.0, 4.0, 1.0, 5.0}
//	err := sort.Float64(data, sort.Ascending)
//	// data is now [1.0, 1.0, 3.0, 4.0, 5.0]
func Float64(data []float64, order SortOrder) error {
	if len(data) == 0 {
		return nil
	}

	err := C.fc_sort_float64(
		(*C.double)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return cErrorToGo(err)
}

// Float64Ex sorts a float64 slice in-place with custom NaN handling
//
// Example:
//
//	data := []float64{3.0, math.NaN(), 1.0, 2.0}
//	err := sort.Float64Ex(data, sort.Ascending, sort.NaNFirst)
//	// data is now [NaN, 1.0, 2.0, 3.0]
func Float64Ex(data []float64, order SortOrder, nanPolicy NaNPolicy) error {
	if len(data) == 0 {
		return nil
	}

	err := C.fc_sort_float64_ex(
		(*C.double)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
		C.fc_sort_nan_policy_t(nanPolicy),
	)

	return cErrorToGo(err)
}

// Float32 sorts a float32 slice in-place using the default NaN handling (NaNLast)
//
// Same algorithm selection as Float64 but optimized for 32-bit floats.
//
// Example:
//
//	data := []float32{3.0, 1.0, 4.0, 1.0, 5.0}
//	err := sort.Float32(data, sort.Ascending)
//	// data is now [1.0, 1.0, 3.0, 4.0, 5.0]
func Float32(data []float32, order SortOrder) error {
	if len(data) == 0 {
		return nil
	}

	err := C.fc_sort_float32(
		(*C.float)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return cErrorToGo(err)
}

// Float32Ex sorts a float32 slice in-place with custom NaN handling
//
// Example:
//
//	data := []float32{3.0, float32(math.NaN()), 1.0, 2.0}
//	err := sort.Float32Ex(data, sort.Ascending, sort.NaNFirst)
//	// data is now [NaN, 1.0, 2.0, 3.0]
func Float32Ex(data []float32, order SortOrder, nanPolicy NaNPolicy) error {
	if len(data) == 0 {
		return nil
	}

	err := C.fc_sort_float32_ex(
		(*C.float)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
		C.fc_sort_nan_policy_t(nanPolicy),
	)

	return cErrorToGo(err)
}

// IsSortedFloat64 checks if a float64 slice is sorted in the specified order
//
// Example:
//
//	data := []float64{1.0, 2.0, 3.0, 4.0, 5.0}
//	sorted := sort.IsSortedFloat64(data, sort.Ascending) // true
func IsSortedFloat64(data []float64, order SortOrder) bool {
	if len(data) == 0 {
		return true
	}

	result := C.fc_sort_is_sorted_float64(
		(*C.double)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return result != 0
}

// IsSortedFloat32 checks if a float32 slice is sorted in the specified order
//
// Example:
//
//	data := []float32{1.0, 2.0, 3.0, 4.0, 5.0}
//	sorted := sort.IsSortedFloat32(data, sort.Ascending) // true
func IsSortedFloat32(data []float32, order SortOrder) bool {
	if len(data) == 0 {
		return true
	}

	result := C.fc_sort_is_sorted_float32(
		(*C.float)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.fc_sort_order_t(order),
	)

	return result != 0
}
