package sort

/*
#include <argsort.h>
#include <stdlib.h>
*/
import "C"
import (
	"unsafe"
)

// ArgsortOrder represents the sort order for argsort
type ArgsortOrder int

const (
	// ArgsortAscending sorts in ascending order
	ArgsortAscending ArgsortOrder = 0
	// ArgsortDescending sorts in descending order
	ArgsortDescending ArgsortOrder = 1
)

// ArgsortFloat64 computes indices that would sort a float64 array
//
// Returns the permutation indices that would sort the input array.
// The original array is not modified.
//
// Example:
//
//	data := []float64{3.0, 1.0, 2.0}
//	indices, err := ArgsortFloat64(data, ArgsortAscending)
//	// indices = []int{1, 2, 0}
//
// NaN handling:
//   - NaN values are placed at the end for both ascending and descending order
//   - All NaN values compare as equal
//
// Time complexity: O(n log n)
func ArgsortFloat64(data []float64, order ArgsortOrder) ([]int, error) {
	n := len(data)
	if n == 0 {
		return []int{}, nil
	}

	indices := make([]C.size_t, n)
	status := C.fc_sort_argsort_f64(
		(*C.double)(unsafe.Pointer(&data[0])),
		C.size_t(n),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
		C.fc_argsort_order_t(order),
	)

	if err := cErrorToGo(status); err != nil {
		return nil, err
	}

	result := make([]int, n)
	for i := 0; i < n; i++ {
		result[i] = int(indices[i])
	}

	return result, nil
}

// ArgsortFloat64Stable computes indices that would sort a float64 array (stable version)
//
// Same as ArgsortFloat64 but preserves the relative order of equal elements.
// Slightly slower than the unstable version.
func ArgsortFloat64Stable(data []float64, order ArgsortOrder) ([]int, error) {
	n := len(data)
	if n == 0 {
		return []int{}, nil
	}

	indices := make([]C.size_t, n)
	status := C.fc_sort_argsort_f64_stable(
		(*C.double)(unsafe.Pointer(&data[0])),
		C.size_t(n),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
		C.fc_argsort_order_t(order),
	)

	if err := cErrorToGo(status); err != nil {
		return nil, err
	}

	result := make([]int, n)
	for i := 0; i < n; i++ {
		result[i] = int(indices[i])
	}

	return result, nil
}

// ArgsortFloat32 computes indices that would sort a float32 array
func ArgsortFloat32(data []float32, order ArgsortOrder) ([]int, error) {
	n := len(data)
	if n == 0 {
		return []int{}, nil
	}

	indices := make([]C.size_t, n)
	status := C.fc_sort_argsort_f32(
		(*C.float)(unsafe.Pointer(&data[0])),
		C.size_t(n),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
		C.fc_argsort_order_t(order),
	)

	if err := cErrorToGo(status); err != nil {
		return nil, err
	}

	result := make([]int, n)
	for i := 0; i < n; i++ {
		result[i] = int(indices[i])
	}

	return result, nil
}

// ArgsortFloat32Stable computes indices that would sort a float32 array (stable version)
func ArgsortFloat32Stable(data []float32, order ArgsortOrder) ([]int, error) {
	n := len(data)
	if n == 0 {
		return []int{}, nil
	}

	indices := make([]C.size_t, n)
	status := C.fc_sort_argsort_f32_stable(
		(*C.float)(unsafe.Pointer(&data[0])),
		C.size_t(n),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
		C.fc_argsort_order_t(order),
	)

	if err := cErrorToGo(status); err != nil {
		return nil, err
	}

	result := make([]int, n)
	for i := 0; i < n; i++ {
		result[i] = int(indices[i])
	}

	return result, nil
}

// ArgsortInt64 computes indices that would sort an int64 array
func ArgsortInt64(data []int64, order ArgsortOrder) ([]int, error) {
	n := len(data)
	if n == 0 {
		return []int{}, nil
	}

	indices := make([]C.size_t, n)
	status := C.fc_sort_argsort_i64(
		(*C.int64_t)(unsafe.Pointer(&data[0])),
		C.size_t(n),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
		C.fc_argsort_order_t(order),
	)

	if err := cErrorToGo(status); err != nil {
		return nil, err
	}

	result := make([]int, n)
	for i := 0; i < n; i++ {
		result[i] = int(indices[i])
	}

	return result, nil
}

// ArgsortInt64Stable computes indices that would sort an int64 array (stable version)
func ArgsortInt64Stable(data []int64, order ArgsortOrder) ([]int, error) {
	n := len(data)
	if n == 0 {
		return []int{}, nil
	}

	indices := make([]C.size_t, n)
	status := C.fc_sort_argsort_i64_stable(
		(*C.int64_t)(unsafe.Pointer(&data[0])),
		C.size_t(n),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
		C.fc_argsort_order_t(order),
	)

	if err := cErrorToGo(status); err != nil {
		return nil, err
	}

	result := make([]int, n)
	for i := 0; i < n; i++ {
		result[i] = int(indices[i])
	}

	return result, nil
}

// ArgsortInt32 computes indices that would sort an int32 array
func ArgsortInt32(data []int32, order ArgsortOrder) ([]int, error) {
	n := len(data)
	if n == 0 {
		return []int{}, nil
	}

	indices := make([]C.size_t, n)
	status := C.fc_sort_argsort_i32(
		(*C.int32_t)(unsafe.Pointer(&data[0])),
		C.size_t(n),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
		C.fc_argsort_order_t(order),
	)

	if err := cErrorToGo(status); err != nil {
		return nil, err
	}

	result := make([]int, n)
	for i := 0; i < n; i++ {
		result[i] = int(indices[i])
	}

	return result, nil
}

// ArgsortInt32Stable computes indices that would sort an int32 array (stable version)
func ArgsortInt32Stable(data []int32, order ArgsortOrder) ([]int, error) {
	n := len(data)
	if n == 0 {
		return []int{}, nil
	}

	indices := make([]C.size_t, n)
	status := C.fc_sort_argsort_i32_stable(
		(*C.int32_t)(unsafe.Pointer(&data[0])),
		C.size_t(n),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
		C.fc_argsort_order_t(order),
	)

	if err := cErrorToGo(status); err != nil {
		return nil, err
	}

	result := make([]int, n)
	for i := 0; i < n; i++ {
		result[i] = int(indices[i])
	}

	return result, nil
}
