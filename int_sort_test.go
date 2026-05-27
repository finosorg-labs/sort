package sort

import (
	"math"
	"math/rand"
	"testing"
)

func TestSortInt64Empty(t *testing.T) {
	data := []int64{}
	err := SortInt64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}
}

func TestSortInt64Single(t *testing.T) {
	data := []int64{42}
	err := SortInt64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}
	if data[0] != 42 {
		t.Errorf("Expected 42, got %d", data[0])
	}
}

func TestSortInt64Ascending(t *testing.T) {
	data := []int64{9, 3, 7, 1, 5, 8, 2, 6, 4, 0}
	err := SortInt64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	for i := 0; i < len(data); i++ {
		if data[i] != int64(i) {
			t.Errorf("Expected %d at index %d, got %d", i, i, data[i])
		}
	}
}

func TestSortInt64Descending(t *testing.T) {
	data := []int64{0, 4, 6, 2, 8, 5, 1, 7, 3, 9}
	err := SortInt64(data, Descending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	for i := 0; i < len(data); i++ {
		if data[i] != int64(9-i) {
			t.Errorf("Expected %d at index %d, got %d", 9-i, i, data[i])
		}
	}
}

func TestSortInt64Negative(t *testing.T) {
	data := []int64{-5, 3, -2, 8, -9, 1, -7, 4, -1, 0}
	expected := []int64{-9, -7, -5, -2, -1, 0, 1, 3, 4, 8}

	err := SortInt64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	for i := 0; i < len(data); i++ {
		if data[i] != expected[i] {
			t.Errorf("Expected %d at index %d, got %d", expected[i], i, data[i])
		}
	}
}

func TestSortInt64Extremes(t *testing.T) {
	data := []int64{math.MaxInt64, math.MinInt64, 0, math.MaxInt64 / 2, math.MinInt64 / 2}
	err := SortInt64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	if data[0] != math.MinInt64 {
		t.Errorf("Expected MinInt64 at index 0, got %d", data[0])
	}
	if data[4] != math.MaxInt64 {
		t.Errorf("Expected MaxInt64 at index 4, got %d", data[4])
	}
}

func TestSortInt64Large(t *testing.T) {
	n := 100000
	data := make([]int64, n)
	rand.Seed(12345)
	for i := 0; i < n; i++ {
		data[i] = rand.Int63n(10000) - 5000
	}

	err := SortInt64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	if !IsSortedInt64(data, Ascending) {
		t.Errorf("Array is not sorted")
	}
}

func TestSortUint64Ascending(t *testing.T) {
	data := []uint64{9, 3, 7, 1, 5, 8, 2, 6, 4, 0}
	err := SortUint64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	for i := 0; i < len(data); i++ {
		if data[i] != uint64(i) {
			t.Errorf("Expected %d at index %d, got %d", i, i, data[i])
		}
	}
}

func TestSortUint64Descending(t *testing.T) {
	data := []uint64{0, 4, 6, 2, 8, 5, 1, 7, 3, 9}
	err := SortUint64(data, Descending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	for i := 0; i < len(data); i++ {
		if data[i] != uint64(9-i) {
			t.Errorf("Expected %d at index %d, got %d", 9-i, i, data[i])
		}
	}
}

func TestSortUint64Extremes(t *testing.T) {
	data := []uint64{math.MaxUint64, 0, math.MaxUint64 / 2, 1, math.MaxUint64 - 1}
	err := SortUint64(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	if data[0] != 0 {
		t.Errorf("Expected 0 at index 0, got %d", data[0])
	}
	if data[4] != math.MaxUint64 {
		t.Errorf("Expected MaxUint64 at index 4, got %d", data[4])
	}
}

func TestSortInt32Ascending(t *testing.T) {
	data := []int32{9, 3, 7, 1, 5, 8, 2, 6, 4, 0}
	err := SortInt32(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	for i := 0; i < len(data); i++ {
		if data[i] != int32(i) {
			t.Errorf("Expected %d at index %d, got %d", i, i, data[i])
		}
	}
}

func TestSortInt32Negative(t *testing.T) {
	data := []int32{-5, 3, -2, 8, -9, 1, -7, 4, -1, 0}
	expected := []int32{-9, -7, -5, -2, -1, 0, 1, 3, 4, 8}

	err := SortInt32(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	for i := 0; i < len(data); i++ {
		if data[i] != expected[i] {
			t.Errorf("Expected %d at index %d, got %d", expected[i], i, data[i])
		}
	}
}

func TestSortInt32Extremes(t *testing.T) {
	data := []int32{math.MaxInt32, math.MinInt32, 0, math.MaxInt32 / 2, math.MinInt32 / 2}
	err := SortInt32(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	if data[0] != math.MinInt32 {
		t.Errorf("Expected MinInt32 at index 0, got %d", data[0])
	}
	if data[4] != math.MaxInt32 {
		t.Errorf("Expected MaxInt32 at index 4, got %d", data[4])
	}
}

func TestSortUint32Ascending(t *testing.T) {
	data := []uint32{9, 3, 7, 1, 5, 8, 2, 6, 4, 0}
	err := SortUint32(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	for i := 0; i < len(data); i++ {
		if data[i] != uint32(i) {
			t.Errorf("Expected %d at index %d, got %d", i, i, data[i])
		}
	}
}

func TestSortUint32Extremes(t *testing.T) {
	data := []uint32{math.MaxUint32, 0, math.MaxUint32 / 2, 1, math.MaxUint32 - 1}
	err := SortUint32(data, Ascending)
	if err != nil {
		t.Errorf("Expected no error, got %v", err)
	}

	if data[0] != 0 {
		t.Errorf("Expected 0 at index 0, got %d", data[0])
	}
	if data[4] != math.MaxUint32 {
		t.Errorf("Expected MaxUint32 at index 4, got %d", data[4])
	}
}

func TestIsSortedInt64(t *testing.T) {
	data := []int64{1, 2, 3, 4, 5}
	if !IsSortedInt64(data, Ascending) {
		t.Errorf("Expected array to be sorted ascending")
	}

	data = []int64{5, 4, 3, 2, 1}
	if !IsSortedInt64(data, Descending) {
		t.Errorf("Expected array to be sorted descending")
	}

	data = []int64{1, 3, 2, 4, 5}
	if IsSortedInt64(data, Ascending) {
		t.Errorf("Expected array to not be sorted")
	}
}

func TestIsSortedUint64(t *testing.T) {
	data := []uint64{1, 2, 3, 4, 5}
	if !IsSortedUint64(data, Ascending) {
		t.Errorf("Expected array to be sorted ascending")
	}

	data = []uint64{1, 3, 2, 4, 5}
	if IsSortedUint64(data, Ascending) {
		t.Errorf("Expected array to not be sorted")
	}
}

func TestIsSortedInt32(t *testing.T) {
	data := []int32{1, 2, 3, 4, 5}
	if !IsSortedInt32(data, Ascending) {
		t.Errorf("Expected array to be sorted ascending")
	}
}

func TestIsSortedUint32(t *testing.T) {
	data := []uint32{1, 2, 3, 4, 5}
	if !IsSortedUint32(data, Ascending) {
		t.Errorf("Expected array to be sorted ascending")
	}
}

func BenchmarkSortInt64_1K(b *testing.B) {
	data := make([]int64, 1000)
	backup := make([]int64, 1000)
	rand.Seed(12345)
	for i := 0; i < 1000; i++ {
		backup[i] = rand.Int63n(1000000) - 500000
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		SortInt64(data, Ascending)
	}
}

func BenchmarkSortInt64_10K(b *testing.B) {
	data := make([]int64, 10000)
	backup := make([]int64, 10000)
	rand.Seed(12345)
	for i := 0; i < 10000; i++ {
		backup[i] = rand.Int63n(1000000) - 500000
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		SortInt64(data, Ascending)
	}
}

func BenchmarkSortInt64_100K(b *testing.B) {
	data := make([]int64, 100000)
	backup := make([]int64, 100000)
	rand.Seed(12345)
	for i := 0; i < 100000; i++ {
		backup[i] = rand.Int63n(1000000) - 500000
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		SortInt64(data, Ascending)
	}
}

func BenchmarkSortUint64_10K(b *testing.B) {
	data := make([]uint64, 10000)
	backup := make([]uint64, 10000)
	rand.Seed(12345)
	for i := 0; i < 10000; i++ {
		backup[i] = uint64(rand.Int63())
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		SortUint64(data, Ascending)
	}
}

func BenchmarkSortInt32_10K(b *testing.B) {
	data := make([]int32, 10000)
	backup := make([]int32, 10000)
	rand.Seed(12345)
	for i := 0; i < 10000; i++ {
		backup[i] = int32(rand.Int31n(1000000) - 500000)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		SortInt32(data, Ascending)
	}
}

func BenchmarkSortUint32_10K(b *testing.B) {
	data := make([]uint32, 10000)
	backup := make([]uint32, 10000)
	rand.Seed(12345)
	for i := 0; i < 10000; i++ {
		backup[i] = uint32(rand.Int31())
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		copy(data, backup)
		SortUint32(data, Ascending)
	}
}
