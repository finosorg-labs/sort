package sort

import (
	"math"
	"testing"
)

func TestTopKLargestFloat64Basic(t *testing.T) {
	data := []float64{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0}
	err := TopKLargestFloat64(data, 3)
	if err != nil {
		t.Fatalf("TopKLargestFloat64 failed: %v", err)
	}

	// Top 3 should be 9.0, 6.0, 5.0 in descending order
	expected := []float64{9.0, 6.0, 5.0}
	for i := 0; i < 3; i++ {
		if math.Abs(data[i]-expected[i]) > 1e-10 {
			t.Errorf("data[%d] = %f, expected %f", i, data[i], expected[i])
		}
	}
}

func TestTopKSmallestFloat64Basic(t *testing.T) {
	data := []float64{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0}
	err := TopKSmallestFloat64(data, 3)
	if err != nil {
		t.Fatalf("TopKSmallestFloat64 failed: %v", err)
	}

	// Top 3 smallest should be 1.0, 1.0, 2.0 in ascending order
	expected := []float64{1.0, 1.0, 2.0}
	for i := 0; i < 3; i++ {
		if math.Abs(data[i]-expected[i]) > 1e-10 {
			t.Errorf("data[%d] = %f, expected %f", i, data[i], expected[i])
		}
	}
}

func TestTopKLargestFloat64Empty(t *testing.T) {
	data := []float64{}
	err := TopKLargestFloat64(data, 0)
	if err != nil {
		t.Fatalf("TopKLargestFloat64 with empty array failed: %v", err)
	}
}

func TestTopKLargestFloat64KGreaterThanN(t *testing.T) {
	data := []float64{1.0, 2.0, 3.0}
	err := TopKLargestFloat64(data, 5)
	if err != ErrInvalidInput {
		t.Fatalf("Expected ErrInvalidInput, got %v", err)
	}
}

func TestTopKLargestFloat64KEqualsN(t *testing.T) {
	data := []float64{3.0, 1.0, 4.0, 5.0, 2.0}
	err := TopKLargestFloat64(data, 5)
	if err != nil {
		t.Fatalf("TopKLargestFloat64 failed: %v", err)
	}

	// Should be sorted in descending order
	expected := []float64{5.0, 4.0, 3.0, 2.0, 1.0}
	for i := 0; i < 5; i++ {
		if math.Abs(data[i]-expected[i]) > 1e-10 {
			t.Errorf("data[%d] = %f, expected %f", i, data[i], expected[i])
		}
	}
}

func TestTopKLargestFloat64SmallK(t *testing.T) {
	// Test heap-based algorithm (k < 100)
	n := 1000
	k := 50
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64(i)
	}

	err := TopKLargestFloat64(data, k)
	if err != nil {
		t.Fatalf("TopKLargestFloat64 failed: %v", err)
	}

	// Top k should be 999, 998, ..., 950 in descending order
	for i := 0; i < k; i++ {
		expected := float64(n - 1 - i)
		if math.Abs(data[i]-expected) > 1e-10 {
			t.Errorf("data[%d] = %f, expected %f", i, data[i], expected)
		}
	}
}

func TestTopKLargestFloat64MediumK(t *testing.T) {
	// Test QuickSelect algorithm (100 <= k < n/10)
	n := 1000
	k := 150
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64(i)
	}

	err := TopKLargestFloat64(data, k)
	if err != nil {
		t.Fatalf("TopKLargestFloat64 failed: %v", err)
	}

	// Top k should be in descending order
	for i := 0; i < k-1; i++ {
		if data[i] < data[i+1] {
			t.Errorf("data[%d] = %f < data[%d] = %f, expected descending order", i, data[i], i+1, data[i+1])
		}
	}
}

func TestTopKLargestFloat64LargeK(t *testing.T) {
	// Test full sort algorithm (k >= n/10)
	n := 1000
	k := 900
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64((i * 7919) % n)
	}

	err := TopKLargestFloat64(data, k)
	if err != nil {
		t.Fatalf("TopKLargestFloat64 failed: %v", err)
	}

	// Top k should be in descending order
	for i := 0; i < k-1; i++ {
		if data[i] < data[i+1] {
			t.Errorf("data[%d] = %f < data[%d] = %f, expected descending order", i, data[i], i+1, data[i+1])
		}
	}
}

func TestTopKSmallestFloat64SmallK(t *testing.T) {
	n := 1000
	k := 50
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64(n - 1 - i)
	}

	err := TopKSmallestFloat64(data, k)
	if err != nil {
		t.Fatalf("TopKSmallestFloat64 failed: %v", err)
	}

	// Top k smallest should be 0, 1, ..., 49 in ascending order
	for i := 0; i < k; i++ {
		expected := float64(i)
		if math.Abs(data[i]-expected) > 1e-10 {
			t.Errorf("data[%d] = %f, expected %f", i, data[i], expected)
		}
	}
}

func TestTopKLargestFloat32Basic(t *testing.T) {
	data := []float32{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0}
	err := TopKLargestFloat32(data, 3)
	if err != nil {
		t.Fatalf("TopKLargestFloat32 failed: %v", err)
	}

	// Top 3 should be 9.0, 6.0, 5.0 in descending order
	expected := []float32{9.0, 6.0, 5.0}
	for i := 0; i < 3; i++ {
		if math.Abs(float64(data[i]-expected[i])) > 1e-6 {
			t.Errorf("data[%d] = %f, expected %f", i, data[i], expected[i])
		}
	}
}

func TestTopKSmallestFloat32Basic(t *testing.T) {
	data := []float32{3.0, 1.0, 4.0, 1.0, 5.0, 9.0, 2.0, 6.0}
	err := TopKSmallestFloat32(data, 3)
	if err != nil {
		t.Fatalf("TopKSmallestFloat32 failed: %v", err)
	}

	// Top 3 smallest should be 1.0, 1.0, 2.0 in ascending order
	expected := []float32{1.0, 1.0, 2.0}
	for i := 0; i < 3; i++ {
		if math.Abs(float64(data[i]-expected[i])) > 1e-6 {
			t.Errorf("data[%d] = %f, expected %f", i, data[i], expected[i])
		}
	}
}

func TestTopKLargestFloat64IndexedBasic(t *testing.T) {
	data := []float64{3.0, 1.0, 4.0, 1.0, 5.0}
	values, indices, err := TopKLargestFloat64Indexed(data, 2)
	if err != nil {
		t.Fatalf("TopKLargestFloat64Indexed failed: %v", err)
	}

	// Top 2 largest: 5.0 (index 4), 4.0 (index 2)
	if math.Abs(values[0]-5.0) > 1e-10 {
		t.Errorf("values[0] = %f, expected 5.0", values[0])
	}
	if math.Abs(values[1]-4.0) > 1e-10 {
		t.Errorf("values[1] = %f, expected 4.0", values[1])
	}
	if indices[0] != 4 {
		t.Errorf("indices[0] = %d, expected 4", indices[0])
	}
	if indices[1] != 2 {
		t.Errorf("indices[1] = %d, expected 2", indices[1])
	}
}

func TestTopKSmallestFloat64IndexedBasic(t *testing.T) {
	data := []float64{3.0, 1.0, 4.0, 1.0, 5.0}
	values, indices, err := TopKSmallestFloat64Indexed(data, 2)
	if err != nil {
		t.Fatalf("TopKSmallestFloat64Indexed failed: %v", err)
	}

	// Top 2 smallest: 1.0 (index 1), 1.0 (index 3)
	if math.Abs(values[0]-1.0) > 1e-10 {
		t.Errorf("values[0] = %f, expected 1.0", values[0])
	}
	if math.Abs(values[1]-1.0) > 1e-10 {
		t.Errorf("values[1] = %f, expected 1.0", values[1])
	}

	// Indices should be 1 and 3 (in some order)
	if !((indices[0] == 1 && indices[1] == 3) || (indices[0] == 3 && indices[1] == 1)) {
		t.Errorf("indices = %v, expected [1, 3] or [3, 1]", indices)
	}
}

func TestTopKLargestFloat64IndexedEmpty(t *testing.T) {
	data := []float64{}
	values, indices, err := TopKLargestFloat64Indexed(data, 0)
	if err != nil {
		t.Fatalf("TopKLargestFloat64Indexed with empty array failed: %v", err)
	}
	if len(values) != 0 || len(indices) != 0 {
		t.Errorf("Expected empty slices, got values=%v, indices=%v", values, indices)
	}
}

func TestTopKLargestFloat64IndexedKGreaterThanN(t *testing.T) {
	data := []float64{1.0, 2.0, 3.0}
	_, _, err := TopKLargestFloat64Indexed(data, 5)
	if err != ErrInvalidInput {
		t.Fatalf("Expected ErrInvalidInput, got %v", err)
	}
}

func TestTopKLargestFloat32IndexedBasic(t *testing.T) {
	data := []float32{3.0, 1.0, 4.0, 1.0, 5.0}
	values, indices, err := TopKLargestFloat32Indexed(data, 2)
	if err != nil {
		t.Fatalf("TopKLargestFloat32Indexed failed: %v", err)
	}

	// Top 2 largest: 5.0 (index 4), 4.0 (index 2)
	if math.Abs(float64(values[0]-5.0)) > 1e-6 {
		t.Errorf("values[0] = %f, expected 5.0", values[0])
	}
	if math.Abs(float64(values[1]-4.0)) > 1e-6 {
		t.Errorf("values[1] = %f, expected 4.0", values[1])
	}
	if indices[0] != 4 {
		t.Errorf("indices[0] = %d, expected 4", indices[0])
	}
	if indices[1] != 2 {
		t.Errorf("indices[1] = %d, expected 2", indices[1])
	}
}

func TestTopKSmallestFloat32IndexedBasic(t *testing.T) {
	data := []float32{3.0, 1.0, 4.0, 1.0, 5.0}
	values, indices, err := TopKSmallestFloat32Indexed(data, 2)
	if err != nil {
		t.Fatalf("TopKSmallestFloat32Indexed failed: %v", err)
	}

	// Top 2 smallest: 1.0 (index 1), 1.0 (index 3)
	if math.Abs(float64(values[0]-1.0)) > 1e-6 {
		t.Errorf("values[0] = %f, expected 1.0", values[0])
	}
	if math.Abs(float64(values[1]-1.0)) > 1e-6 {
		t.Errorf("values[1] = %f, expected 1.0", values[1])
	}

	// Indices should be 1 and 3 (in some order)
	if !((indices[0] == 1 && indices[1] == 3) || (indices[0] == 3 && indices[1] == 1)) {
		t.Errorf("indices = %v, expected [1, 3] or [3, 1]", indices)
	}
}

func TestTopKLargestFloat64WithDuplicates(t *testing.T) {
	data := []float64{5.0, 5.0, 3.0, 3.0, 1.0, 1.0, 4.0, 4.0, 2.0, 2.0}
	err := TopKLargestFloat64(data, 4)
	if err != nil {
		t.Fatalf("TopKLargestFloat64 failed: %v", err)
	}

	// Top 4 should be 5.0, 5.0, 4.0, 4.0
	expected := []float64{5.0, 5.0, 4.0, 4.0}
	for i := 0; i < 4; i++ {
		if math.Abs(data[i]-expected[i]) > 1e-10 {
			t.Errorf("data[%d] = %f, expected %f", i, data[i], expected[i])
		}
	}
}

func TestTopKSmallestFloat64WithDuplicates(t *testing.T) {
	data := []float64{5.0, 5.0, 3.0, 3.0, 1.0, 1.0, 4.0, 4.0, 2.0, 2.0}
	err := TopKSmallestFloat64(data, 4)
	if err != nil {
		t.Fatalf("TopKSmallestFloat64 failed: %v", err)
	}

	// Top 4 smallest should be 1.0, 1.0, 2.0, 2.0
	expected := []float64{1.0, 1.0, 2.0, 2.0}
	for i := 0; i < 4; i++ {
		if math.Abs(data[i]-expected[i]) > 1e-10 {
			t.Errorf("data[%d] = %f, expected %f", i, data[i], expected[i])
		}
	}
}

func BenchmarkTopKLargestFloat64SmallK(b *testing.B) {
	n := 100000
	k := 10
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64(i)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64(data, k)
	}
}

func BenchmarkTopKLargestFloat64MediumK(b *testing.B) {
	n := 100000
	k := 500
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64(i)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64(data, k)
	}
}

func BenchmarkTopKLargestFloat64LargeK(b *testing.B) {
	n := 100000
	k := 10000
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64(i)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64(data, k)
	}
}

func BenchmarkTopKLargestFloat64Indexed(b *testing.B) {
	n := 10000
	k := 100
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64(i)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64Indexed(data, k)
	}
}

func TestTopKLargestFloat64BatchBasic(t *testing.T) {
	arrays := [][]float64{
		{3.0, 1.0, 4.0, 5.0},
		{9.0, 2.0, 6.0, 5.0},
		{8.0, 7.0, 1.0, 3.0},
	}

	err := TopKLargestFloat64Batch(arrays, 2)
	if err != nil {
		t.Fatalf("TopKLargestFloat64Batch failed: %v", err)
	}

	// Check array 1: top 2 should be [5.0, 4.0]
	expected1 := []float64{5.0, 4.0}
	for i := 0; i < 2; i++ {
		if math.Abs(arrays[0][i]-expected1[i]) > 1e-10 {
			t.Errorf("arrays[0][%d] = %f, expected %f", i, arrays[0][i], expected1[i])
		}
	}

	// Check array 2: top 2 should be [9.0, 6.0]
	expected2 := []float64{9.0, 6.0}
	for i := 0; i < 2; i++ {
		if math.Abs(arrays[1][i]-expected2[i]) > 1e-10 {
			t.Errorf("arrays[1][%d] = %f, expected %f", i, arrays[1][i], expected2[i])
		}
	}

	// Check array 3: top 2 should be [8.0, 7.0]
	expected3 := []float64{8.0, 7.0}
	for i := 0; i < 2; i++ {
		if math.Abs(arrays[2][i]-expected3[i]) > 1e-10 {
			t.Errorf("arrays[2][%d] = %f, expected %f", i, arrays[2][i], expected3[i])
		}
	}
}

func TestTopKSmallestFloat64BatchBasic(t *testing.T) {
	arrays := [][]float64{
		{3.0, 1.0, 4.0, 5.0},
		{9.0, 2.0, 6.0, 5.0},
	}

	err := TopKSmallestFloat64Batch(arrays, 2)
	if err != nil {
		t.Fatalf("TopKSmallestFloat64Batch failed: %v", err)
	}

	// Check array 1: top 2 smallest should be [1.0, 3.0]
	expected1 := []float64{1.0, 3.0}
	for i := 0; i < 2; i++ {
		if math.Abs(arrays[0][i]-expected1[i]) > 1e-10 {
			t.Errorf("arrays[0][%d] = %f, expected %f", i, arrays[0][i], expected1[i])
		}
	}

	// Check array 2: top 2 smallest should be [2.0, 5.0]
	expected2 := []float64{2.0, 5.0}
	for i := 0; i < 2; i++ {
		if math.Abs(arrays[1][i]-expected2[i]) > 1e-10 {
			t.Errorf("arrays[1][%d] = %f, expected %f", i, arrays[1][i], expected2[i])
		}
	}
}

func TestTopKLargestFloat64BatchEmpty(t *testing.T) {
	arrays := [][]float64{}
	err := TopKLargestFloat64Batch(arrays, 2)
	if err != nil {
		t.Fatalf("TopKLargestFloat64Batch with empty arrays failed: %v", err)
	}
}

func TestTopKLargestFloat32BatchBasic(t *testing.T) {
	arrays := [][]float32{
		{3.0, 1.0, 4.0, 5.0},
		{9.0, 2.0, 6.0, 5.0},
	}

	err := TopKLargestFloat32Batch(arrays, 2)
	if err != nil {
		t.Fatalf("TopKLargestFloat32Batch failed: %v", err)
	}

	// Check array 1: top 2 should be [5.0, 4.0]
	expected1 := []float32{5.0, 4.0}
	for i := 0; i < 2; i++ {
		if math.Abs(float64(arrays[0][i]-expected1[i])) > 1e-6 {
			t.Errorf("arrays[0][%d] = %f, expected %f", i, arrays[0][i], expected1[i])
		}
	}

	// Check array 2: top 2 should be [9.0, 6.0]
	expected2 := []float32{9.0, 6.0}
	for i := 0; i < 2; i++ {
		if math.Abs(float64(arrays[1][i]-expected2[i])) > 1e-6 {
			t.Errorf("arrays[1][%d] = %f, expected %f", i, arrays[1][i], expected2[i])
		}
	}
}

func TestTopKSmallestFloat32BatchBasic(t *testing.T) {
	arrays := [][]float32{
		{3.0, 1.0, 4.0, 5.0},
		{9.0, 2.0, 6.0, 5.0},
	}

	err := TopKSmallestFloat32Batch(arrays, 2)
	if err != nil {
		t.Fatalf("TopKSmallestFloat32Batch failed: %v", err)
	}

	// Check array 1: top 2 smallest should be [1.0, 3.0]
	expected1 := []float32{1.0, 3.0}
	for i := 0; i < 2; i++ {
		if math.Abs(float64(arrays[0][i]-expected1[i])) > 1e-6 {
			t.Errorf("arrays[0][%d] = %f, expected %f", i, arrays[0][i], expected1[i])
		}
	}

	// Check array 2: top 2 smallest should be [2.0, 5.0]
	expected2 := []float32{2.0, 5.0}
	for i := 0; i < 2; i++ {
		if math.Abs(float64(arrays[1][i]-expected2[i])) > 1e-6 {
			t.Errorf("arrays[1][%d] = %f, expected %f", i, arrays[1][i], expected2[i])
		}
	}
}

func BenchmarkTopKLargestFloat64Batch10Arrays(b *testing.B) {
	arrays := make([][]float64, 10)
	for i := 0; i < 10; i++ {
		arrays[i] = make([]float64, 100)
		for j := 0; j < 100; j++ {
			arrays[i][j] = float64(j)
		}
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64Batch(arrays, 10)
	}
}

func BenchmarkTopKLargestFloat64Batch100Arrays(b *testing.B) {
	arrays := make([][]float64, 100)
	for i := 0; i < 100; i++ {
		arrays[i] = make([]float64, 100)
		for j := 0; j < 100; j++ {
			arrays[i][j] = float64(j)
		}
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64Batch(arrays, 10)
	}
}

func BenchmarkTopKLargestFloat64Batch1000Arrays(b *testing.B) {
	arrays := make([][]float64, 1000)
	for i := 0; i < 1000; i++ {
		arrays[i] = make([]float64, 100)
		for j := 0; j < 100; j++ {
			arrays[i][j] = float64(j)
		}
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64Batch(arrays, 10)
	}
}

// Tests for Indexed Batch functions

func TestTopKLargestFloat64IndexedBatchBasic(t *testing.T) {
	arrays := [][]float64{
		{3.0, 1.0, 4.0, 5.0, 9.0},
		{2.0, 7.0, 1.0, 8.0, 3.0},
		{6.0, 2.0, 5.0, 3.0, 8.0},
	}

	// Make copies to verify original arrays are not modified
	originalArrays := make([][]float64, len(arrays))
	for i, arr := range arrays {
		originalArrays[i] = make([]float64, len(arr))
		copy(originalArrays[i], arr)
	}

	k := 3
	values, indices, err := TopKLargestFloat64IndexedBatch(arrays, k)
	if err != nil {
		t.Fatalf("TopKLargestFloat64IndexedBatch failed: %v", err)
	}

	// Verify array 0: top 3 should be [9.0, 5.0, 4.0] from indices [4, 3, 2]
	expectedValues0 := []float64{9.0, 5.0, 4.0}
	expectedIndices0 := []int{4, 3, 2}
	for i := 0; i < k; i++ {
		if math.Abs(values[0][i]-expectedValues0[i]) > 1e-10 {
			t.Errorf("values[0][%d] = %f, expected %f", i, values[0][i], expectedValues0[i])
		}
		if indices[0][i] != expectedIndices0[i] {
			t.Errorf("indices[0][%d] = %d, expected %d", i, indices[0][i], expectedIndices0[i])
		}
	}

	// Verify array 1: top 3 should be [8.0, 7.0, 3.0] from indices [3, 1, 4]
	expectedValues1 := []float64{8.0, 7.0, 3.0}
	expectedIndices1 := []int{3, 1, 4}
	for i := 0; i < k; i++ {
		if math.Abs(values[1][i]-expectedValues1[i]) > 1e-10 {
			t.Errorf("values[1][%d] = %f, expected %f", i, values[1][i], expectedValues1[i])
		}
		if indices[1][i] != expectedIndices1[i] {
			t.Errorf("indices[1][%d] = %d, expected %d", i, indices[1][i], expectedIndices1[i])
		}
	}

	// Verify array 2: top 3 should be [8.0, 6.0, 5.0] from indices [4, 0, 2]
	expectedValues2 := []float64{8.0, 6.0, 5.0}
	expectedIndices2 := []int{4, 0, 2}
	for i := 0; i < k; i++ {
		if math.Abs(values[2][i]-expectedValues2[i]) > 1e-10 {
			t.Errorf("values[2][%d] = %f, expected %f", i, values[2][i], expectedValues2[i])
		}
		if indices[2][i] != expectedIndices2[i] {
			t.Errorf("indices[2][%d] = %d, expected %d", i, indices[2][i], expectedIndices2[i])
		}
	}

	// Verify original arrays are not modified
	for i, arr := range arrays {
		for j, val := range arr {
			if math.Abs(val-originalArrays[i][j]) > 1e-10 {
				t.Errorf("Original array[%d][%d] was modified: got %f, expected %f", i, j, val, originalArrays[i][j])
			}
		}
	}
}

func TestTopKSmallestFloat64IndexedBatchBasic(t *testing.T) {
	arrays := [][]float64{
		{3.0, 1.0, 4.0, 5.0},
		{9.0, 2.0, 6.0, 5.0},
	}

	// Make copies to verify original arrays are not modified
	originalArrays := make([][]float64, len(arrays))
	for i, arr := range arrays {
		originalArrays[i] = make([]float64, len(arr))
		copy(originalArrays[i], arr)
	}

	k := 2
	values, indices, err := TopKSmallestFloat64IndexedBatch(arrays, k)
	if err != nil {
		t.Fatalf("TopKSmallestFloat64IndexedBatch failed: %v", err)
	}

	// Verify array 0: top 2 smallest should be [1.0, 3.0] from indices [1, 0]
	expectedValues0 := []float64{1.0, 3.0}
	expectedIndices0 := []int{1, 0}
	for i := 0; i < k; i++ {
		if math.Abs(values[0][i]-expectedValues0[i]) > 1e-10 {
			t.Errorf("values[0][%d] = %f, expected %f", i, values[0][i], expectedValues0[i])
		}
		if indices[0][i] != expectedIndices0[i] {
			t.Errorf("indices[0][%d] = %d, expected %d", i, indices[0][i], expectedIndices0[i])
		}
	}

	// Verify array 1: top 2 smallest should be [2.0, 5.0] from indices [1, 3]
	expectedValues1 := []float64{2.0, 5.0}
	expectedIndices1 := []int{1, 3}
	for i := 0; i < k; i++ {
		if math.Abs(values[1][i]-expectedValues1[i]) > 1e-10 {
			t.Errorf("values[1][%d] = %f, expected %f", i, values[1][i], expectedValues1[i])
		}
		if indices[1][i] != expectedIndices1[i] {
			t.Errorf("indices[1][%d] = %d, expected %d", i, indices[1][i], expectedIndices1[i])
		}
	}

	// Verify original arrays are not modified
	for i, arr := range arrays {
		for j, val := range arr {
			if math.Abs(val-originalArrays[i][j]) > 1e-10 {
				t.Errorf("Original array[%d][%d] was modified: got %f, expected %f", i, j, val, originalArrays[i][j])
			}
		}
	}
}

func TestTopKLargestFloat64IndexedBatchEmpty(t *testing.T) {
	arrays := [][]float64{}
	values, indices, err := TopKLargestFloat64IndexedBatch(arrays, 2)
	if err != nil {
		t.Fatalf("TopKLargestFloat64IndexedBatch with empty arrays failed: %v", err)
	}
	if len(values) != 0 || len(indices) != 0 {
		t.Errorf("Expected empty results, got values=%d, indices=%d", len(values), len(indices))
	}
}

func TestTopKLargestFloat64IndexedBatchKGreaterThanN(t *testing.T) {
	arrays := [][]float64{
		{1.0, 2.0, 3.0},
		{4.0, 5.0, 6.0},
	}
	_, _, err := TopKLargestFloat64IndexedBatch(arrays, 5)
	if err != ErrInvalidInput {
		t.Fatalf("Expected ErrInvalidInput, got %v", err)
	}
}

func TestTopKLargestFloat32IndexedBatchBasic(t *testing.T) {
	arrays := [][]float32{
		{3.0, 1.0, 4.0, 5.0},
		{9.0, 2.0, 6.0, 5.0},
	}

	// Make copies to verify original arrays are not modified
	originalArrays := make([][]float32, len(arrays))
	for i, arr := range arrays {
		originalArrays[i] = make([]float32, len(arr))
		copy(originalArrays[i], arr)
	}

	k := 2
	values, indices, err := TopKLargestFloat32IndexedBatch(arrays, k)
	if err != nil {
		t.Fatalf("TopKLargestFloat32IndexedBatch failed: %v", err)
	}

	// Verify array 0: top 2 should be [5.0, 4.0] from indices [3, 2]
	expectedValues0 := []float32{5.0, 4.0}
	expectedIndices0 := []int{3, 2}
	for i := 0; i < k; i++ {
		if math.Abs(float64(values[0][i]-expectedValues0[i])) > 1e-6 {
			t.Errorf("values[0][%d] = %f, expected %f", i, values[0][i], expectedValues0[i])
		}
		if indices[0][i] != expectedIndices0[i] {
			t.Errorf("indices[0][%d] = %d, expected %d", i, indices[0][i], expectedIndices0[i])
		}
	}

	// Verify array 1: top 2 should be [9.0, 6.0] from indices [0, 2]
	expectedValues1 := []float32{9.0, 6.0}
	expectedIndices1 := []int{0, 2}
	for i := 0; i < k; i++ {
		if math.Abs(float64(values[1][i]-expectedValues1[i])) > 1e-6 {
			t.Errorf("values[1][%d] = %f, expected %f", i, values[1][i], expectedValues1[i])
		}
		if indices[1][i] != expectedIndices1[i] {
			t.Errorf("indices[1][%d] = %d, expected %d", i, indices[1][i], expectedIndices1[i])
		}
	}

	// Verify original arrays are not modified
	for i, arr := range arrays {
		for j, val := range arr {
			if math.Abs(float64(val-originalArrays[i][j])) > 1e-6 {
				t.Errorf("Original array[%d][%d] was modified: got %f, expected %f", i, j, val, originalArrays[i][j])
			}
		}
	}
}

func TestTopKSmallestFloat32IndexedBatchBasic(t *testing.T) {
	arrays := [][]float32{
		{3.0, 1.0, 4.0, 5.0},
		{9.0, 2.0, 6.0, 5.0},
	}

	// Make copies to verify original arrays are not modified
	originalArrays := make([][]float32, len(arrays))
	for i, arr := range arrays {
		originalArrays[i] = make([]float32, len(arr))
		copy(originalArrays[i], arr)
	}

	k := 2
	values, indices, err := TopKSmallestFloat32IndexedBatch(arrays, k)
	if err != nil {
		t.Fatalf("TopKSmallestFloat32IndexedBatch failed: %v", err)
	}

	// Verify array 0: top 2 smallest should be [1.0, 3.0] from indices [1, 0]
	expectedValues0 := []float32{1.0, 3.0}
	expectedIndices0 := []int{1, 0}
	for i := 0; i < k; i++ {
		if math.Abs(float64(values[0][i]-expectedValues0[i])) > 1e-6 {
			t.Errorf("values[0][%d] = %f, expected %f", i, values[0][i], expectedValues0[i])
		}
		if indices[0][i] != expectedIndices0[i] {
			t.Errorf("indices[0][%d] = %d, expected %d", i, indices[0][i], expectedIndices0[i])
		}
	}

	// Verify array 1: top 2 smallest should be [2.0, 5.0] from indices [1, 3]
	expectedValues1 := []float32{2.0, 5.0}
	expectedIndices1 := []int{1, 3}
	for i := 0; i < k; i++ {
		if math.Abs(float64(values[1][i]-expectedValues1[i])) > 1e-6 {
			t.Errorf("values[1][%d] = %f, expected %f", i, values[1][i], expectedValues1[i])
		}
		if indices[1][i] != expectedIndices1[i] {
			t.Errorf("indices[1][%d] = %d, expected %d", i, indices[1][i], expectedIndices1[i])
		}
	}

	// Verify original arrays are not modified
	for i, arr := range arrays {
		for j, val := range arr {
			if math.Abs(float64(val-originalArrays[i][j])) > 1e-6 {
				t.Errorf("Original array[%d][%d] was modified: got %f, expected %f", i, j, val, originalArrays[i][j])
			}
		}
	}
}

func BenchmarkTopKLargestFloat64IndexedBatch10Arrays(b *testing.B) {
	arrays := make([][]float64, 10)
	for i := 0; i < 10; i++ {
		arrays[i] = make([]float64, 100)
		for j := 0; j < 100; j++ {
			arrays[i][j] = float64(j)
		}
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64IndexedBatch(arrays, 10)
	}
}

func BenchmarkTopKLargestFloat64IndexedBatch100Arrays(b *testing.B) {
	arrays := make([][]float64, 100)
	for i := 0; i < 100; i++ {
		arrays[i] = make([]float64, 100)
		for j := 0; j < 100; j++ {
			arrays[i][j] = float64(j)
		}
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64IndexedBatch(arrays, 10)
	}
}

func BenchmarkTopKLargestFloat64IndexedBatch1000Arrays(b *testing.B) {
	arrays := make([][]float64, 1000)
	for i := 0; i < 1000; i++ {
		arrays[i] = make([]float64, 100)
		for j := 0; j < 100; j++ {
			arrays[i][j] = float64(j)
		}
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		TopKLargestFloat64IndexedBatch(arrays, 10)
	}
}
