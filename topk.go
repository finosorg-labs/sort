package sort

/*
#include <topk.h>
#include <stdlib.h>
*/
import "C"
import (
	"runtime"
	"unsafe"
)

// TopKLargestFloat64 selects the K largest elements from a float64 slice in-place
//
// The K largest elements will be placed at the beginning of the slice in descending order.
// The remaining elements are unordered.
//
// Algorithm selection:
//   - K < 100: Heap-based selection O(n log K)
//   - 100 <= K < n/10: QuickSelect O(n) average
//   - K >= n/10: Full sort O(n log n)
//
// Time complexity: O(n) average for small/medium K, O(n log n) for large K
// Space complexity: O(1) in-place operation
//
// Example:
//
//	data := []float64{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0}
//	err := sort.TopKLargestFloat64(data, 3)
//	// First 3 elements are now the 3 largest values in descending order: [9.0, 6.0, 5.0]
func TopKLargestFloat64(data []float64, k int) error {
	if len(data) == 0 || k == 0 {
		return nil
	}
	if k > len(data) {
		return ErrInvalidInput
	}

	err := C.fc_topk_largest_f64(
		(*C.double)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.size_t(k),
	)

	return cErrorToGo(err)
}

// TopKSmallestFloat64 selects the K smallest elements from a float64 slice in-place
//
// The K smallest elements will be placed at the beginning of the slice in ascending order.
// The remaining elements are unordered.
//
// Example:
//
//	data := []float64{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0}
//	err := sort.TopKSmallestFloat64(data, 3)
//	// First 3 elements are now the 3 smallest values in ascending order
func TopKSmallestFloat64(data []float64, k int) error {
	if len(data) == 0 || k == 0 {
		return nil
	}
	if k > len(data) {
		return ErrInvalidInput
	}

	err := C.fc_topk_smallest_f64(
		(*C.double)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.size_t(k),
	)

	return cErrorToGo(err)
}

// TopKLargestFloat32 selects the K largest elements from a float32 slice in-place
//
// Same algorithm selection as TopKLargestFloat64 but optimized for 32-bit floats.
//
// Example:
//
//	data := []float32{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0}
//	err := sort.TopKLargestFloat32(data, 3)
//	// Last 3 elements are now the 3 largest values in descending order
func TopKLargestFloat32(data []float32, k int) error {
	if len(data) == 0 || k == 0 {
		return nil
	}
	if k > len(data) {
		return ErrInvalidInput
	}

	err := C.fc_topk_largest_f32(
		(*C.float)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.size_t(k),
	)

	return cErrorToGo(err)
}

// TopKSmallestFloat32 selects the K smallest elements from a float32 slice in-place
//
// Example:
//
//	data := []float32{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0}
//	err := sort.TopKSmallestFloat32(data, 3)
//	// First 3 elements are now the 3 smallest values in ascending order
func TopKSmallestFloat32(data []float32, k int) error {
	if len(data) == 0 || k == 0 {
		return nil
	}
	if k > len(data) {
		return ErrInvalidInput
	}

	err := C.fc_topk_smallest_f32(
		(*C.float)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.size_t(k),
	)

	return cErrorToGo(err)
}

// TopKLargestFloat64Indexed returns the K largest values and their original indices
//
// Unlike the in-place versions, this function does not modify the input slice.
// Returns two slices: values (K largest in descending order) and indices (original positions).
//
// Example:
//
//	data := []float64{3.0, 1.0, 4.0, 1.0, 5.0}
//	values, indices, err := sort.TopKLargestFloat64Indexed(data, 2)
//	// values: [5.0, 4.0], indices: [4, 2]
func TopKLargestFloat64Indexed(data []float64, k int) ([]float64, []int, error) {
	if len(data) == 0 || k == 0 {
		return []float64{}, []int{}, nil
	}
	if k > len(data) {
		return nil, nil, ErrInvalidInput
	}

	values := make([]float64, k)
	indices := make([]uint64, k)

	err := C.fc_topk_largest_f64_indexed(
		(*C.double)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.size_t(k),
		(*C.double)(unsafe.Pointer(&values[0])),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
	)

	if err != C.FC_OK {
		return nil, nil, cErrorToGo(err)
	}

	// Convert uint64 indices to int
	intIndices := make([]int, k)
	for i := 0; i < k; i++ {
		intIndices[i] = int(indices[i])
	}

	return values, intIndices, nil
}

// TopKSmallestFloat64Indexed returns the K smallest values and their original indices
//
// Example:
//
//	data := []float64{3.0, 1.0, 4.0, 1.0, 5.0}
//	values, indices, err := sort.TopKSmallestFloat64Indexed(data, 2)
//	// values: [1.0, 1.0], indices: [1, 3]
func TopKSmallestFloat64Indexed(data []float64, k int) ([]float64, []int, error) {
	if len(data) == 0 || k == 0 {
		return []float64{}, []int{}, nil
	}
	if k > len(data) {
		return nil, nil, ErrInvalidInput
	}

	values := make([]float64, k)
	indices := make([]uint64, k)

	err := C.fc_topk_smallest_f64_indexed(
		(*C.double)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.size_t(k),
		(*C.double)(unsafe.Pointer(&values[0])),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
	)

	if err != C.FC_OK {
		return nil, nil, cErrorToGo(err)
	}

	// Convert uint64 indices to int
	intIndices := make([]int, k)
	for i := 0; i < k; i++ {
		intIndices[i] = int(indices[i])
	}

	return values, intIndices, nil
}

// TopKLargestFloat32Indexed returns the K largest values and their original indices
//
// Example:
//
//	data := []float32{3.0, 1.0, 4.0, 1.0, 5.0}
//	values, indices, err := sort.TopKLargestFloat32Indexed(data, 2)
//	// values: [5.0, 4.0], indices: [4, 2]
func TopKLargestFloat32Indexed(data []float32, k int) ([]float32, []int, error) {
	if len(data) == 0 || k == 0 {
		return []float32{}, []int{}, nil
	}
	if k > len(data) {
		return nil, nil, ErrInvalidInput
	}

	values := make([]float32, k)
	indices := make([]uint64, k)

	err := C.fc_topk_largest_f32_indexed(
		(*C.float)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.size_t(k),
		(*C.float)(unsafe.Pointer(&values[0])),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
	)

	if err != C.FC_OK {
		return nil, nil, cErrorToGo(err)
	}

	// Convert uint64 indices to int
	intIndices := make([]int, k)
	for i := 0; i < k; i++ {
		intIndices[i] = int(indices[i])
	}

	return values, intIndices, nil
}

// TopKSmallestFloat32Indexed returns the K smallest values and their original indices
//
// Example:
//
//	data := []float32{3.0, 1.0, 4.0, 1.0, 5.0}
//	values, indices, err := sort.TopKSmallestFloat32Indexed(data, 2)
//	// values: [1.0, 1.0], indices: [1, 3]
func TopKSmallestFloat32Indexed(data []float32, k int) ([]float32, []int, error) {
	if len(data) == 0 || k == 0 {
		return []float32{}, []int{}, nil
	}
	if k > len(data) {
		return nil, nil, ErrInvalidInput
	}

	values := make([]float32, k)
	indices := make([]uint64, k)

	err := C.fc_topk_smallest_f32_indexed(
		(*C.float)(unsafe.Pointer(&data[0])),
		C.size_t(len(data)),
		C.size_t(k),
		(*C.float)(unsafe.Pointer(&values[0])),
		(*C.size_t)(unsafe.Pointer(&indices[0])),
	)

	if err != C.FC_OK {
		return nil, nil, cErrorToGo(err)
	}

	// Convert uint64 indices to int
	intIndices := make([]int, k)
	for i := 0; i < k; i++ {
		intIndices[i] = int(indices[i])
	}

	return values, intIndices, nil
}

// TopKLargestFloat64Batch performs Top-K selection on multiple float64 slices in a single C call
//
// This function reduces cgo overhead by processing multiple arrays in one call,
// making it ideal for batch operations like market-wide aggregation.
// Each slice is modified in-place with its K largest elements at the front.
//
// All input slices must have the same K value. If different K values are needed,
// call this function multiple times with grouped slices.
//
// Example:
//
//	arrays := [][]float64{
//	    {3.0, 1.0, 4.0, 5.0},
//	    {9.0, 2.0, 6.0, 5.0},
//	    {8.0, 7.0, 1.0, 3.0},
//	}
//	err := sort.TopKLargestFloat64Batch(arrays, 2)
//	// Each array now has its 2 largest elements at the front
func TopKLargestFloat64Batch(arrays [][]float64, k int) error {
	if len(arrays) == 0 {
		return nil
	}

	// Prepare C arrays
	numBatches := len(arrays)
	dataPtrs := make([]*C.double, numBatches)
	sizes := make([]C.size_t, numBatches)
	ks := make([]C.size_t, numBatches)

	// Pin all Go pointers to prevent them from moving
	var pinner runtime.Pinner
	defer pinner.Unpin()

	for i, arr := range arrays {
		if len(arr) == 0 {
			continue
		}
		if k > len(arr) {
			return ErrInvalidInput
		}
		dataPtrs[i] = (*C.double)(unsafe.Pointer(&arr[0]))
		pinner.Pin(&arr[0])
		sizes[i] = C.size_t(len(arr))
		ks[i] = C.size_t(k)
	}

	// Pin the pointer arrays themselves
	pinner.Pin(&dataPtrs[0])
	pinner.Pin(&sizes[0])
	pinner.Pin(&ks[0])

	err := C.fc_topk_largest_f64_batch(
		(**C.double)(unsafe.Pointer(&dataPtrs[0])),
		(*C.size_t)(unsafe.Pointer(&sizes[0])),
		(*C.size_t)(unsafe.Pointer(&ks[0])),
		C.size_t(numBatches),
	)

	return cErrorToGo(err)
}

// TopKSmallestFloat64Batch performs Top-K smallest selection on multiple float64 slices
//
// Example:
//
//	arrays := [][]float64{
//	    {3.0, 1.0, 4.0, 5.0},
//	    {9.0, 2.0, 6.0, 5.0},
//	}
//	err := sort.TopKSmallestFloat64Batch(arrays, 2)
//	// Each array now has its 2 smallest elements at the front
func TopKSmallestFloat64Batch(arrays [][]float64, k int) error {
	if len(arrays) == 0 {
		return nil
	}

	// Prepare C arrays
	numBatches := len(arrays)
	dataPtrs := make([]*C.double, numBatches)
	sizes := make([]C.size_t, numBatches)
	ks := make([]C.size_t, numBatches)

	// Pin all Go pointers to prevent them from moving
	var pinner runtime.Pinner
	defer pinner.Unpin()

	for i, arr := range arrays {
		if len(arr) == 0 {
			continue
		}
		if k > len(arr) {
			return ErrInvalidInput
		}
		dataPtrs[i] = (*C.double)(unsafe.Pointer(&arr[0]))
		pinner.Pin(&arr[0])
		sizes[i] = C.size_t(len(arr))
		ks[i] = C.size_t(k)
	}

	// Pin the pointer arrays themselves
	pinner.Pin(&dataPtrs[0])
	pinner.Pin(&sizes[0])
	pinner.Pin(&ks[0])

	err := C.fc_topk_smallest_f64_batch(
		(**C.double)(unsafe.Pointer(&dataPtrs[0])),
		(*C.size_t)(unsafe.Pointer(&sizes[0])),
		(*C.size_t)(unsafe.Pointer(&ks[0])),
		C.size_t(numBatches),
	)

	return cErrorToGo(err)
}

// TopKLargestFloat32Batch performs Top-K selection on multiple float32 slices in a single C call
//
// Example:
//
//	arrays := [][]float32{
//	    {3.0, 1.0, 4.0, 5.0},
//	    {9.0, 2.0, 6.0, 5.0},
//	}
//	err := sort.TopKLargestFloat32Batch(arrays, 2)
func TopKLargestFloat32Batch(arrays [][]float32, k int) error {
	if len(arrays) == 0 {
		return nil
	}

	// Prepare C arrays
	numBatches := len(arrays)
	dataPtrs := make([]*C.float, numBatches)
	sizes := make([]C.size_t, numBatches)
	ks := make([]C.size_t, numBatches)

	// Pin all Go pointers to prevent them from moving
	var pinner runtime.Pinner
	defer pinner.Unpin()

	for i, arr := range arrays {
		if len(arr) == 0 {
			continue
		}
		if k > len(arr) {
			return ErrInvalidInput
		}
		dataPtrs[i] = (*C.float)(unsafe.Pointer(&arr[0]))
		pinner.Pin(&arr[0])
		sizes[i] = C.size_t(len(arr))
		ks[i] = C.size_t(k)
	}

	// Pin the pointer arrays themselves
	pinner.Pin(&dataPtrs[0])
	pinner.Pin(&sizes[0])
	pinner.Pin(&ks[0])

	err := C.fc_topk_largest_f32_batch(
		(**C.float)(unsafe.Pointer(&dataPtrs[0])),
		(*C.size_t)(unsafe.Pointer(&sizes[0])),
		(*C.size_t)(unsafe.Pointer(&ks[0])),
		C.size_t(numBatches),
	)

	return cErrorToGo(err)
}

// TopKSmallestFloat32Batch performs Top-K smallest selection on multiple float32 slices
//
// Example:
//
//	arrays := [][]float32{
//	    {3.0, 1.0, 4.0, 5.0},
//	    {9.0, 2.0, 6.0, 5.0},
//	}
//	err := sort.TopKSmallestFloat32Batch(arrays, 2)
func TopKSmallestFloat32Batch(arrays [][]float32, k int) error {
	if len(arrays) == 0 {
		return nil
	}

	// Prepare C arrays
	numBatches := len(arrays)
	dataPtrs := make([]*C.float, numBatches)
	sizes := make([]C.size_t, numBatches)
	ks := make([]C.size_t, numBatches)

	// Pin all Go pointers to prevent them from moving
	var pinner runtime.Pinner
	defer pinner.Unpin()

	for i, arr := range arrays {
		if len(arr) == 0 {
			continue
		}
		if k > len(arr) {
			return ErrInvalidInput
		}
		dataPtrs[i] = (*C.float)(unsafe.Pointer(&arr[0]))
		pinner.Pin(&arr[0])
		sizes[i] = C.size_t(len(arr))
		ks[i] = C.size_t(k)
	}

	// Pin the pointer arrays themselves
	pinner.Pin(&dataPtrs[0])
	pinner.Pin(&sizes[0])
	pinner.Pin(&ks[0])

	err := C.fc_topk_smallest_f32_batch(
		(**C.float)(unsafe.Pointer(&dataPtrs[0])),
		(*C.size_t)(unsafe.Pointer(&sizes[0])),
		(*C.size_t)(unsafe.Pointer(&ks[0])),
		C.size_t(numBatches),
	)

	return cErrorToGo(err)
}

// TopKLargestFloat64IndexedBatch performs Top-K selection on multiple float64 slices
// and returns their values and indices without modifying the original data
//
// This function is ideal for batch operations where you need to preserve the original
// data and track the positions of top-K elements. It reduces cgo overhead by processing
// multiple arrays in one call.
//
// Unlike the in-place batch functions, this does not modify the input arrays.
//
// Example:
//
//	arrays := [][]float64{
//	    {3.0, 1.0, 4.0, 5.0},
//	    {9.0, 2.0, 6.0, 5.0},
//	}
//	values, indices, err := sort.TopKLargestFloat64IndexedBatch(arrays, 2)
//	// values[0] = [5.0, 4.0], indices[0] = [3, 2]
//	// values[1] = [9.0, 6.0], indices[1] = [0, 2]
//	// Original arrays remain unchanged
func TopKLargestFloat64IndexedBatch(arrays [][]float64, k int) ([][]float64, [][]int, error) {
	if len(arrays) == 0 {
		return [][]float64{}, [][]int{}, nil
	}

	// Validate all arrays upfront to avoid partial processing
	for _, arr := range arrays {
		if len(arr) == 0 {
			return nil, nil, ErrInvalidInput
		}
		if k > len(arr) {
			return nil, nil, ErrInvalidInput
		}
	}

	numBatches := len(arrays)
	dataPtrs := make([]*C.double, numBatches)
	sizes := make([]C.size_t, numBatches)
	ks := make([]C.size_t, numBatches)
	valuePtrs := make([]*C.double, numBatches)
	indexPtrs := make([]*C.size_t, numBatches)

	// Allocate output arrays
	values := make([][]float64, numBatches)
	indices := make([][]uint64, numBatches)

	var pinner runtime.Pinner
	defer pinner.Unpin()

	for i, arr := range arrays {
		// Pin input array
		dataPtrs[i] = (*C.double)(unsafe.Pointer(&arr[0]))
		pinner.Pin(&arr[0])
		sizes[i] = C.size_t(len(arr))
		ks[i] = C.size_t(k)

		// Allocate and pin output arrays
		values[i] = make([]float64, k)
		indices[i] = make([]uint64, k)
		valuePtrs[i] = (*C.double)(unsafe.Pointer(&values[i][0]))
		indexPtrs[i] = (*C.size_t)(unsafe.Pointer(&indices[i][0]))
		pinner.Pin(&values[i][0])
		pinner.Pin(&indices[i][0])
	}

	// Pin the pointer arrays
	pinner.Pin(&dataPtrs[0])
	pinner.Pin(&sizes[0])
	pinner.Pin(&ks[0])
	pinner.Pin(&valuePtrs[0])
	pinner.Pin(&indexPtrs[0])

	err := C.fc_topk_largest_f64_indexed_batch(
		(**C.double)(unsafe.Pointer(&dataPtrs[0])),
		(*C.size_t)(unsafe.Pointer(&sizes[0])),
		(*C.size_t)(unsafe.Pointer(&ks[0])),
		C.size_t(numBatches),
		(**C.double)(unsafe.Pointer(&valuePtrs[0])),
		(**C.size_t)(unsafe.Pointer(&indexPtrs[0])),
	)

	if err != C.FC_OK {
		return nil, nil, cErrorToGo(err)
	}

	// Convert uint64 indices to int
	intIndices := make([][]int, numBatches)
	for i := 0; i < numBatches; i++ {
		intIndices[i] = make([]int, len(indices[i]))
		for j := 0; j < len(indices[i]); j++ {
			intIndices[i][j] = int(indices[i][j])
		}
	}

	return values, intIndices, nil
}

// TopKSmallestFloat64IndexedBatch performs Top-K smallest selection on multiple float64 slices
// and returns their values and indices without modifying the original data
//
// Example:
//
//	arrays := [][]float64{
//	    {3.0, 1.0, 4.0, 5.0},
//	    {9.0, 2.0, 6.0, 5.0},
//	}
//	values, indices, err := sort.TopKSmallestFloat64IndexedBatch(arrays, 2)
//	// values[0] = [1.0, 3.0], indices[0] = [1, 0]
//	// values[1] = [2.0, 5.0], indices[1] = [1, 3]
func TopKSmallestFloat64IndexedBatch(arrays [][]float64, k int) ([][]float64, [][]int, error) {
	if len(arrays) == 0 {
		return [][]float64{}, [][]int{}, nil
	}

	// Validate all arrays upfront to avoid partial processing
	for _, arr := range arrays {
		if len(arr) == 0 {
			return nil, nil, ErrInvalidInput
		}
		if k > len(arr) {
			return nil, nil, ErrInvalidInput
		}
	}

	numBatches := len(arrays)
	dataPtrs := make([]*C.double, numBatches)
	sizes := make([]C.size_t, numBatches)
	ks := make([]C.size_t, numBatches)
	valuePtrs := make([]*C.double, numBatches)
	indexPtrs := make([]*C.size_t, numBatches)

	values := make([][]float64, numBatches)
	indices := make([][]uint64, numBatches)

	var pinner runtime.Pinner
	defer pinner.Unpin()

	for i, arr := range arrays {
		dataPtrs[i] = (*C.double)(unsafe.Pointer(&arr[0]))
		pinner.Pin(&arr[0])
		sizes[i] = C.size_t(len(arr))
		ks[i] = C.size_t(k)

		values[i] = make([]float64, k)
		indices[i] = make([]uint64, k)
		valuePtrs[i] = (*C.double)(unsafe.Pointer(&values[i][0]))
		indexPtrs[i] = (*C.size_t)(unsafe.Pointer(&indices[i][0]))
		pinner.Pin(&values[i][0])
		pinner.Pin(&indices[i][0])
	}

	pinner.Pin(&dataPtrs[0])
	pinner.Pin(&sizes[0])
	pinner.Pin(&ks[0])
	pinner.Pin(&valuePtrs[0])
	pinner.Pin(&indexPtrs[0])

	err := C.fc_topk_smallest_f64_indexed_batch(
		(**C.double)(unsafe.Pointer(&dataPtrs[0])),
		(*C.size_t)(unsafe.Pointer(&sizes[0])),
		(*C.size_t)(unsafe.Pointer(&ks[0])),
		C.size_t(numBatches),
		(**C.double)(unsafe.Pointer(&valuePtrs[0])),
		(**C.size_t)(unsafe.Pointer(&indexPtrs[0])),
	)

	if err != C.FC_OK {
		return nil, nil, cErrorToGo(err)
	}

	intIndices := make([][]int, numBatches)
	for i := 0; i < numBatches; i++ {
		intIndices[i] = make([]int, len(indices[i]))
		for j := 0; j < len(indices[i]); j++ {
			intIndices[i][j] = int(indices[i][j])
		}
	}

	return values, intIndices, nil
}

// TopKLargestFloat32IndexedBatch performs Top-K selection on multiple float32 slices
// and returns their values and indices without modifying the original data
//
// Example:
//
//	arrays := [][]float32{
//	    {3.0, 1.0, 4.0, 5.0},
//	    {9.0, 2.0, 6.0, 5.0},
//	}
//	values, indices, err := sort.TopKLargestFloat32IndexedBatch(arrays, 2)
func TopKLargestFloat32IndexedBatch(arrays [][]float32, k int) ([][]float32, [][]int, error) {
	if len(arrays) == 0 {
		return [][]float32{}, [][]int{}, nil
	}

	// Validate all arrays upfront to avoid partial processing
	for _, arr := range arrays {
		if len(arr) == 0 {
			return nil, nil, ErrInvalidInput
		}
		if k > len(arr) {
			return nil, nil, ErrInvalidInput
		}
	}

	numBatches := len(arrays)
	dataPtrs := make([]*C.float, numBatches)
	sizes := make([]C.size_t, numBatches)
	ks := make([]C.size_t, numBatches)
	valuePtrs := make([]*C.float, numBatches)
	indexPtrs := make([]*C.size_t, numBatches)

	values := make([][]float32, numBatches)
	indices := make([][]uint64, numBatches)

	var pinner runtime.Pinner
	defer pinner.Unpin()

	for i, arr := range arrays {
		dataPtrs[i] = (*C.float)(unsafe.Pointer(&arr[0]))
		pinner.Pin(&arr[0])
		sizes[i] = C.size_t(len(arr))
		ks[i] = C.size_t(k)

		values[i] = make([]float32, k)
		indices[i] = make([]uint64, k)
		valuePtrs[i] = (*C.float)(unsafe.Pointer(&values[i][0]))
		indexPtrs[i] = (*C.size_t)(unsafe.Pointer(&indices[i][0]))
		pinner.Pin(&values[i][0])
		pinner.Pin(&indices[i][0])
	}

	pinner.Pin(&dataPtrs[0])
	pinner.Pin(&sizes[0])
	pinner.Pin(&ks[0])
	pinner.Pin(&valuePtrs[0])
	pinner.Pin(&indexPtrs[0])

	err := C.fc_topk_largest_f32_indexed_batch(
		(**C.float)(unsafe.Pointer(&dataPtrs[0])),
		(*C.size_t)(unsafe.Pointer(&sizes[0])),
		(*C.size_t)(unsafe.Pointer(&ks[0])),
		C.size_t(numBatches),
		(**C.float)(unsafe.Pointer(&valuePtrs[0])),
		(**C.size_t)(unsafe.Pointer(&indexPtrs[0])),
	)

	if err != C.FC_OK {
		return nil, nil, cErrorToGo(err)
	}

	intIndices := make([][]int, numBatches)
	for i := 0; i < numBatches; i++ {
		intIndices[i] = make([]int, len(indices[i]))
		for j := 0; j < len(indices[i]); j++ {
			intIndices[i][j] = int(indices[i][j])
		}
	}

	return values, intIndices, nil
}

// TopKSmallestFloat32IndexedBatch performs Top-K smallest selection on multiple float32 slices
// and returns their values and indices without modifying the original data
//
// Example:
//
//	arrays := [][]float32{
//	    {3.0, 1.0, 4.0, 5.0},
//	    {9.0, 2.0, 6.0, 5.0},
//	}
//	values, indices, err := sort.TopKSmallestFloat32IndexedBatch(arrays, 2)
func TopKSmallestFloat32IndexedBatch(arrays [][]float32, k int) ([][]float32, [][]int, error) {
	if len(arrays) == 0 {
		return [][]float32{}, [][]int{}, nil
	}

	// Validate all arrays upfront to avoid partial processing
	for _, arr := range arrays {
		if len(arr) == 0 {
			return nil, nil, ErrInvalidInput
		}
		if k > len(arr) {
			return nil, nil, ErrInvalidInput
		}
	}

	numBatches := len(arrays)
	dataPtrs := make([]*C.float, numBatches)
	sizes := make([]C.size_t, numBatches)
	ks := make([]C.size_t, numBatches)
	valuePtrs := make([]*C.float, numBatches)
	indexPtrs := make([]*C.size_t, numBatches)

	values := make([][]float32, numBatches)
	indices := make([][]uint64, numBatches)

	var pinner runtime.Pinner
	defer pinner.Unpin()

	for i, arr := range arrays {
		dataPtrs[i] = (*C.float)(unsafe.Pointer(&arr[0]))
		pinner.Pin(&arr[0])
		sizes[i] = C.size_t(len(arr))
		ks[i] = C.size_t(k)

		values[i] = make([]float32, k)
		indices[i] = make([]uint64, k)
		valuePtrs[i] = (*C.float)(unsafe.Pointer(&values[i][0]))
		indexPtrs[i] = (*C.size_t)(unsafe.Pointer(&indices[i][0]))
		pinner.Pin(&values[i][0])
		pinner.Pin(&indices[i][0])
	}

	pinner.Pin(&dataPtrs[0])
	pinner.Pin(&sizes[0])
	pinner.Pin(&ks[0])
	pinner.Pin(&valuePtrs[0])
	pinner.Pin(&indexPtrs[0])

	err := C.fc_topk_smallest_f32_indexed_batch(
		(**C.float)(unsafe.Pointer(&dataPtrs[0])),
		(*C.size_t)(unsafe.Pointer(&sizes[0])),
		(*C.size_t)(unsafe.Pointer(&ks[0])),
		C.size_t(numBatches),
		(**C.float)(unsafe.Pointer(&valuePtrs[0])),
		(**C.size_t)(unsafe.Pointer(&indexPtrs[0])),
	)

	if err != C.FC_OK {
		return nil, nil, cErrorToGo(err)
	}

	intIndices := make([][]int, numBatches)
	for i := 0; i < numBatches; i++ {
		intIndices[i] = make([]int, len(indices[i]))
		for j := 0; j < len(indices[i]); j++ {
			intIndices[i][j] = int(indices[i][j])
		}
	}

	return values, intIndices, nil
}

