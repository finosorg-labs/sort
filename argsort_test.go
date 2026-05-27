package sort

import (
	"math"
	"math/rand"
	"sort"
	"testing"
)

func TestArgsortFloat64Basic(t *testing.T) {
	data := []float64{3.0, 1.0, 2.0, 5.0, 4.0}
	expected := []int{1, 2, 0, 4, 3}

	indices, err := ArgsortFloat64(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortFloat64 failed: %v", err)
	}

	if len(indices) != len(expected) {
		t.Fatalf("Expected length %d, got %d", len(expected), len(indices))
	}

	for i := range indices {
		if indices[i] != expected[i] {
			t.Errorf("At index %d: expected %d, got %d", i, expected[i], indices[i])
		}
	}
}

func TestArgsortFloat64Descending(t *testing.T) {
	data := []float64{3.0, 1.0, 2.0, 5.0, 4.0}
	expected := []int{3, 4, 0, 2, 1}

	indices, err := ArgsortFloat64(data, ArgsortDescending)
	if err != nil {
		t.Fatalf("ArgsortFloat64 failed: %v", err)
	}

	for i := range indices {
		if indices[i] != expected[i] {
			t.Errorf("At index %d: expected %d, got %d", i, expected[i], indices[i])
		}
	}
}

func TestArgsortFloat64Empty(t *testing.T) {
	data := []float64{}
	indices, err := ArgsortFloat64(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortFloat64 failed: %v", err)
	}
	if len(indices) != 0 {
		t.Errorf("Expected empty result, got length %d", len(indices))
	}
}

func TestArgsortFloat64Single(t *testing.T) {
	data := []float64{42.0}
	indices, err := ArgsortFloat64(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortFloat64 failed: %v", err)
	}
	if len(indices) != 1 || indices[0] != 0 {
		t.Errorf("Expected [0], got %v", indices)
	}
}

func TestArgsortFloat64WithNaN(t *testing.T) {
	data := []float64{3.0, math.NaN(), 1.0, 2.0, math.NaN()}
	indices, err := ArgsortFloat64(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortFloat64 failed: %v", err)
	}

	if indices[0] != 2 {
		t.Errorf("Expected smallest at index 2, got %d", indices[0])
	}
	if indices[1] != 3 {
		t.Errorf("Expected second smallest at index 3, got %d", indices[1])
	}
	if indices[2] != 0 {
		t.Errorf("Expected third smallest at index 0, got %d", indices[2])
	}

	if !math.IsNaN(data[indices[3]]) && !math.IsNaN(data[indices[4]]) {
		t.Errorf("Expected NaN values at end")
	}
}

func TestArgsortFloat64WithInf(t *testing.T) {
	data := []float64{3.0, math.Inf(1), 1.0, math.Inf(-1), 2.0}
	expected := []int{3, 2, 4, 0, 1}

	indices, err := ArgsortFloat64(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortFloat64 failed: %v", err)
	}

	for i := range indices {
		if indices[i] != expected[i] {
			t.Errorf("At index %d: expected %d, got %d", i, expected[i], indices[i])
		}
	}
}

func TestArgsortFloat64Stable(t *testing.T) {
	data := []float64{3.0, 1.0, 2.0, 1.0, 3.0}
	expected := []int{1, 3, 2, 0, 4}

	indices, err := ArgsortFloat64Stable(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortFloat64Stable failed: %v", err)
	}

	for i := range indices {
		if indices[i] != expected[i] {
			t.Errorf("At index %d: expected %d, got %d", i, expected[i], indices[i])
		}
	}
}

func TestArgsortFloat64Large(t *testing.T) {
	n := 10000
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64(n - i)
	}

	indices, err := ArgsortFloat64(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortFloat64 failed: %v", err)
	}

	for i := 0; i < n-1; i++ {
		if data[indices[i]] > data[indices[i+1]] {
			t.Errorf("Array not sorted at index %d", i)
			break
		}
	}
}

func TestArgsortFloat32(t *testing.T) {
	data := []float32{3.0, 1.0, 2.0, 5.0, 4.0}
	expected := []int{1, 2, 0, 4, 3}

	indices, err := ArgsortFloat32(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortFloat32 failed: %v", err)
	}

	for i := range indices {
		if indices[i] != expected[i] {
			t.Errorf("At index %d: expected %d, got %d", i, expected[i], indices[i])
		}
	}
}

func TestArgsortInt64(t *testing.T) {
	data := []int64{3, 1, 2, 5, 4}
	expected := []int{1, 2, 0, 4, 3}

	indices, err := ArgsortInt64(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortInt64 failed: %v", err)
	}

	for i := range indices {
		if indices[i] != expected[i] {
			t.Errorf("At index %d: expected %d, got %d", i, expected[i], indices[i])
		}
	}
}

func TestArgsortInt64Descending(t *testing.T) {
	data := []int64{3, 1, 2, 5, 4}
	expected := []int{3, 4, 0, 2, 1}

	indices, err := ArgsortInt64(data, ArgsortDescending)
	if err != nil {
		t.Fatalf("ArgsortInt64 failed: %v", err)
	}

	for i := range indices {
		if indices[i] != expected[i] {
			t.Errorf("At index %d: expected %d, got %d", i, expected[i], indices[i])
		}
	}
}

func TestArgsortInt64Stable(t *testing.T) {
	data := []int64{3, 1, 2, 1, 3}
	expected := []int{1, 3, 2, 0, 4}

	indices, err := ArgsortInt64Stable(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortInt64Stable failed: %v", err)
	}

	for i := range indices {
		if indices[i] != expected[i] {
			t.Errorf("At index %d: expected %d, got %d", i, expected[i], indices[i])
		}
	}
}

func TestArgsortInt32(t *testing.T) {
	data := []int32{3, 1, 2, 5, 4}
	expected := []int{1, 2, 0, 4, 3}

	indices, err := ArgsortInt32(data, ArgsortAscending)
	if err != nil {
		t.Fatalf("ArgsortInt32 failed: %v", err)
	}

	for i := range indices {
		if indices[i] != expected[i] {
			t.Errorf("At index %d: expected %d, got %d", i, expected[i], indices[i])
		}
	}
}

func goArgsortFloat64(data []float64) []int {
	type indexValue struct {
		index int
		value float64
	}
	pairs := make([]indexValue, len(data))
	for i, v := range data {
		pairs[i] = indexValue{i, v}
	}
	sort.Slice(pairs, func(i, j int) bool {
		return pairs[i].value < pairs[j].value
	})
	indices := make([]int, len(data))
	for i, p := range pairs {
		indices[i] = p.index
	}
	return indices
}

func BenchmarkArgsortFloat64_1K(b *testing.B) {
	data := make([]float64, 1000)
	rand.Seed(42)
	for i := range data {
		data[i] = rand.Float64()
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_, _ = ArgsortFloat64(data, ArgsortAscending)
	}
}

func BenchmarkArgsortFloat64_10K(b *testing.B) {
	data := make([]float64, 10000)
	rand.Seed(42)
	for i := range data {
		data[i] = rand.Float64()
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_, _ = ArgsortFloat64(data, ArgsortAscending)
	}
}

func BenchmarkArgsortFloat64_100K(b *testing.B) {
	data := make([]float64, 100000)
	rand.Seed(42)
	for i := range data {
		data[i] = rand.Float64()
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_, _ = ArgsortFloat64(data, ArgsortAscending)
	}
}

func BenchmarkArgsortFloat64Stable_10K(b *testing.B) {
	data := make([]float64, 10000)
	rand.Seed(42)
	for i := range data {
		data[i] = rand.Float64()
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_, _ = ArgsortFloat64Stable(data, ArgsortAscending)
	}
}

func BenchmarkGoArgsortFloat64_1K(b *testing.B) {
	data := make([]float64, 1000)
	rand.Seed(42)
	for i := range data {
		data[i] = rand.Float64()
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = goArgsortFloat64(data)
	}
}

func BenchmarkGoArgsortFloat64_10K(b *testing.B) {
	data := make([]float64, 10000)
	rand.Seed(42)
	for i := range data {
		data[i] = rand.Float64()
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = goArgsortFloat64(data)
	}
}

func BenchmarkGoArgsortFloat64_100K(b *testing.B) {
	data := make([]float64, 100000)
	rand.Seed(42)
	for i := range data {
		data[i] = rand.Float64()
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = goArgsortFloat64(data)
	}
}

func BenchmarkArgsortInt64_10K(b *testing.B) {
	data := make([]int64, 10000)
	rand.Seed(42)
	for i := range data {
		data[i] = rand.Int63()
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_, _ = ArgsortInt64(data, ArgsortAscending)
	}
}
