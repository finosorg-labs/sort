package sort

import (
	"math"
	"math/rand"
	"testing"
)

func TestFloat64Empty(t *testing.T) {
	data := []float64{}
	err := Float64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error for empty slice, got %v", err)
	}
}

func TestFloat64Single(t *testing.T) {
	data := []float64{42.0}
	err := Float64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}
	if data[0] != 42.0 {
		t.Errorf("Expected 42.0, got %f", data[0])
	}
}

func TestFloat64Ascending(t *testing.T) {
	data := []float64{9.0, 3.0, 7.0, 1.0, 5.0, 8.0, 2.0, 6.0, 4.0, 0.0}
	err := Float64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	expected := []float64{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0}
	for i := range data {
		if data[i] != expected[i] {
			t.Errorf("At index %d: expected %f, got %f", i, expected[i], data[i])
		}
	}
}

func TestFloat64Descending(t *testing.T) {
	data := []float64{0.0, 4.0, 6.0, 2.0, 8.0, 5.0, 1.0, 7.0, 3.0, 9.0}
	err := Float64(data, Descending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	expected := []float64{9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0}
	for i := range data {
		if data[i] != expected[i] {
			t.Errorf("At index %d: expected %f, got %f", i, expected[i], data[i])
		}
	}
}

func TestFloat64WithDuplicates(t *testing.T) {
	data := []float64{5.0, 2.0, 8.0, 2.0, 9.0, 1.0, 5.0, 3.0, 8.0, 1.0}
	err := Float64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	expected := []float64{1.0, 1.0, 2.0, 2.0, 3.0, 5.0, 5.0, 8.0, 8.0, 9.0}
	for i := range data {
		if data[i] != expected[i] {
			t.Errorf("At index %d: expected %f, got %f", i, expected[i], data[i])
		}
	}
}

func TestFloat64WithNegatives(t *testing.T) {
	data := []float64{-5.0, 3.0, -2.0, 8.0, -9.0, 1.0, -1.0, 6.0, 0.0, -7.0}
	err := Float64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	expected := []float64{-9.0, -7.0, -5.0, -2.0, -1.0, 0.0, 1.0, 3.0, 6.0, 8.0}
	for i := range data {
		if data[i] != expected[i] {
			t.Errorf("At index %d: expected %f, got %f", i, expected[i], data[i])
		}
	}
}

func TestFloat64WithNaNLast(t *testing.T) {
	data := []float64{3.0, math.NaN(), 1.0, 2.0, math.NaN()}
	err := Float64Ex(data, Ascending, NaNLast)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	if data[0] != 1.0 || data[1] != 2.0 || data[2] != 3.0 {
		t.Errorf("Expected sorted values [1.0, 2.0, 3.0], got [%f, %f, %f]", data[0], data[1], data[2])
	}
	if !math.IsNaN(data[3]) || !math.IsNaN(data[4]) {
		t.Errorf("Expected NaN values at end")
	}
}

func TestFloat64WithNaNFirst(t *testing.T) {
	data := []float64{3.0, math.NaN(), 1.0, 2.0, math.NaN()}
	err := Float64Ex(data, Ascending, NaNFirst)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	if !math.IsNaN(data[0]) || !math.IsNaN(data[1]) {
		t.Errorf("Expected NaN values at beginning")
	}
	if data[2] != 1.0 || data[3] != 2.0 || data[4] != 3.0 {
		t.Errorf("Expected sorted values [1.0, 2.0, 3.0], got [%f, %f, %f]", data[2], data[3], data[4])
	}
}

func TestFloat64WithNaNReject(t *testing.T) {
	data := []float64{3.0, math.NaN(), 1.0, 2.0, 4.0}
	err := Float64Ex(data, Ascending, NaNReject)
	if err != ErrInvalidInput {
		t.Errorf("Expected ErrInvalidInput, got %v", err)
	}
}

func TestFloat64MediumArray(t *testing.T) {
	n := 1000
	data := make([]float64, n)
	for i := 0; i < n; i++ {
		data[i] = float64(n - i - 1)
	}

	err := Float64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	for i := 0; i < n; i++ {
		if data[i] != float64(i) {
			t.Errorf("At index %d: expected %f, got %f", i, float64(i), data[i])
			break
		}
	}
}

func TestFloat64LargeArray(t *testing.T) {
	n := 100000
	data := make([]float64, n)
	rand.Seed(12345)
	for i := 0; i < n; i++ {
		data[i] = rand.Float64() * 1000.0
	}

	err := Float64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	if !IsSortedFloat64(data, Ascending) {
		t.Errorf("Array is not sorted")
	}
}

func TestFloat32Basic(t *testing.T) {
	data := []float32{9.0, 3.0, 7.0, 1.0, 5.0, 8.0, 2.0, 6.0, 4.0, 0.0}
	err := Float32(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	expected := []float32{0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0}
	for i := range data {
		if data[i] != expected[i] {
			t.Errorf("At index %d: expected %f, got %f", i, expected[i], data[i])
		}
	}
}

func TestFloat32Descending(t *testing.T) {
	data := []float32{0.0, 4.0, 6.0, 2.0, 8.0, 5.0, 1.0, 7.0, 3.0, 9.0}
	err := Float32(data, Descending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	expected := []float32{9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0}
	for i := range data {
		if data[i] != expected[i] {
			t.Errorf("At index %d: expected %f, got %f", i, expected[i], data[i])
		}
	}
}

func TestFloat32WithNaN(t *testing.T) {
	data := []float32{3.0, float32(math.NaN()), 1.0, 2.0, float32(math.NaN())}
	err := Float32(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	if data[0] != 1.0 || data[1] != 2.0 || data[2] != 3.0 {
		t.Errorf("Expected sorted values [1.0, 2.0, 3.0], got [%f, %f, %f]", data[0], data[1], data[2])
	}
	if !math.IsNaN(float64(data[3])) || !math.IsNaN(float64(data[4])) {
		t.Errorf("Expected NaN values at end")
	}
}

func TestIsSortedFloat64True(t *testing.T) {
	data := []float64{1.0, 2.0, 3.0, 4.0, 5.0}
	if !IsSortedFloat64(data, Ascending) {
		t.Errorf("Expected array to be sorted")
	}
}

func TestIsSortedFloat64False(t *testing.T) {
	data := []float64{1.0, 3.0, 2.0, 4.0, 5.0}
	if IsSortedFloat64(data, Ascending) {
		t.Errorf("Expected array to not be sorted")
	}
}

func TestIsSortedFloat64Descending(t *testing.T) {
	data := []float64{5.0, 4.0, 3.0, 2.0, 1.0}
	if !IsSortedFloat64(data, Descending) {
		t.Errorf("Expected array to be sorted in descending order")
	}
}

func TestIsSortedFloat32(t *testing.T) {
	data := []float32{1.0, 2.0, 3.0, 4.0, 5.0}
	if !IsSortedFloat32(data, Ascending) {
		t.Errorf("Expected array to be sorted")
	}
}

func BenchmarkFloat64Small(b *testing.B) {
	data := make([]float64, 100)
	backup := make([]float64, 100)
	rand.Seed(12345)
	for i := 0; i < 100; i++ {
		backup[i] = rand.Float64() * 1000.0
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		Float64(data, Ascending)
	}
}

func BenchmarkFloat64Medium(b *testing.B) {
	data := make([]float64, 10000)
	backup := make([]float64, 10000)
	rand.Seed(12345)
	for i := 0; i < 10000; i++ {
		backup[i] = rand.Float64() * 1000.0
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		Float64(data, Ascending)
	}
}

func BenchmarkFloat64Large(b *testing.B) {
	data := make([]float64, 1000000)
	backup := make([]float64, 1000000)
	rand.Seed(12345)
	for i := 0; i < 1000000; i++ {
		backup[i] = rand.Float64() * 1000.0
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		Float64(data, Ascending)
	}
}

func BenchmarkFloat32Medium(b *testing.B) {
	data := make([]float32, 10000)
	backup := make([]float32, 10000)
	rand.Seed(12345)
	for i := 0; i < 10000; i++ {
		backup[i] = float32(rand.Float64() * 1000.0)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		Float32(data, Ascending)
	}
}
