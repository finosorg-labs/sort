package sort

import (
	"math/rand"
	"testing"
	"unsafe"
)

type Order struct {
	Price   float64
	Time    int64
	OrderID uint32
}

type Stock struct {
	Sector    int32
	MarketCap float64
	StockID   uint32
}

type Point struct {
	X  float32
	Y  float32
	ID int32
}

func TestMultiKeyBasicTwoKeys(t *testing.T) {
	orders := []Order{
		{100.5, 1000, 1},
		{100.5, 900, 2},
		{100.0, 1100, 3},
		{100.5, 1000, 4},
		{101.0, 800, 5},
		{100.0, 1000, 6},
		{100.5, 1100, 7},
		{100.0, 1000, 8},
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Descending},
		{Offset: unsafe.Offsetof(Order{}.Time), Type: KeyTypeInt64, Order: Ascending},
	}

	err := MultiKeySort(orders, keys)
	if err != nil {
		t.Fatalf("Sort failed: %v", err)
	}

	if orders[0].Price != 101.0 || orders[0].OrderID != 5 {
		t.Errorf("First should be highest price, got price=%f, id=%d", orders[0].Price, orders[0].OrderID)
	}

	if orders[1].Price != 100.5 || orders[1].Time != 900 {
		t.Errorf("Second should be 100.5 with earliest time, got price=%f, time=%d", orders[1].Price, orders[1].Time)
	}

	if !IsSortedMultiKey(orders, keys) {
		t.Error("Array should be sorted")
	}
}

func TestMultiKeyThreeKeys(t *testing.T) {
	orders := []Order{
		{100.5, 1000, 3},
		{100.5, 1000, 1},
		{100.5, 1000, 2},
		{100.5, 900, 5},
		{100.5, 900, 4},
		{101.0, 800, 6},
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Descending},
		{Offset: unsafe.Offsetof(Order{}.Time), Type: KeyTypeInt64, Order: Ascending},
		{Offset: unsafe.Offsetof(Order{}.OrderID), Type: KeyTypeUInt32, Order: Ascending},
	}

	err := MultiKeySort(orders, keys)
	if err != nil {
		t.Fatalf("Sort failed: %v", err)
	}

	if orders[0].Price != 101.0 {
		t.Errorf("First should be highest price")
	}

	if orders[1].Price != 100.5 || orders[1].Time != 900 || orders[1].OrderID != 4 {
		t.Errorf("Second should be 100.5, time 900, order_id 4")
	}

	if orders[2].Price != 100.5 || orders[2].Time != 900 || orders[2].OrderID != 5 {
		t.Errorf("Third should be 100.5, time 900, order_id 5")
	}

	if orders[3].OrderID != 1 {
		t.Errorf("Fourth should have order_id 1")
	}
}

func TestMultiKeyAscendingOrder(t *testing.T) {
	points := []Point{
		{3.0, 2.0, 5},
		{1.0, 4.0, 2},
		{3.0, 1.0, 4},
		{1.0, 3.0, 1},
		{2.0, 2.0, 3},
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Point{}.X), Type: KeyTypeFloat, Order: Ascending},
		{Offset: unsafe.Offsetof(Point{}.Y), Type: KeyTypeFloat, Order: Ascending},
	}

	err := MultiKeySort(points, keys)
	if err != nil {
		t.Fatalf("Sort failed: %v", err)
	}

	if points[0].X != 1.0 || points[0].Y != 3.0 {
		t.Errorf("First point should be (1,3), got (%f,%f)", points[0].X, points[0].Y)
	}

	if points[1].X != 1.0 || points[1].Y != 4.0 {
		t.Errorf("Second point should be (1,4), got (%f,%f)", points[1].X, points[1].Y)
	}

	if points[4].X != 3.0 || points[4].Y != 2.0 {
		t.Errorf("Fifth point should be (3,2), got (%f,%f)", points[4].X, points[4].Y)
	}
}

func TestMultiKeyIndexed(t *testing.T) {
	orders := []Order{
		{100.5, 1000, 1},
		{100.5, 900, 2},
		{100.0, 1100, 3},
		{101.0, 800, 4},
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Descending},
		{Offset: unsafe.Offsetof(Order{}.Time), Type: KeyTypeInt64, Order: Ascending},
	}

	indices, err := MultiKeySortIndexed(orders, keys)
	if err != nil {
		t.Fatalf("Indexed sort failed: %v", err)
	}

	if len(indices) != 4 {
		t.Fatalf("Expected 4 indices, got %d", len(indices))
	}

	if indices[0] != 3 {
		t.Errorf("First index should be 3 (101.0), got %d", indices[0])
	}

	if indices[1] != 1 {
		t.Errorf("Second index should be 1 (100.5, 900), got %d", indices[1])
	}

	if indices[2] != 0 {
		t.Errorf("Third index should be 0 (100.5, 1000), got %d", indices[2])
	}

	if indices[3] != 2 {
		t.Errorf("Fourth index should be 2 (100.0, 1100), got %d", indices[3])
	}

	if orders[0].OrderID != 1 {
		t.Error("Original data should not be modified")
	}
}

func TestMultiKeySingleKey(t *testing.T) {
	orders := []Order{
		{100.5, 1000, 1},
		{100.0, 900, 2},
		{101.0, 1100, 3},
		{99.5, 800, 4},
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Ascending},
	}

	err := MultiKeySort(orders, keys)
	if err != nil {
		t.Fatalf("Single key sort failed: %v", err)
	}

	if orders[0].Price != 99.5 {
		t.Errorf("First should be 99.5, got %f", orders[0].Price)
	}

	if orders[3].Price != 101.0 {
		t.Errorf("Last should be 101.0, got %f", orders[3].Price)
	}
}

func TestMultiKeyLargeArray(t *testing.T) {
	const count = 10000
	orders := make([]Order, count)

	rand.Seed(42)
	for i := 0; i < count; i++ {
		orders[i].Price = 100.0 + float64(rand.Intn(100))*0.01
		orders[i].Time = 1000000 + int64(rand.Intn(100000))
		orders[i].OrderID = uint32(i)
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Ascending},
		{Offset: unsafe.Offsetof(Order{}.Time), Type: KeyTypeInt64, Order: Ascending},
		{Offset: unsafe.Offsetof(Order{}.OrderID), Type: KeyTypeUInt32, Order: Ascending},
	}

	err := MultiKeySort(orders, keys)
	if err != nil {
		t.Fatalf("Large array sort failed: %v", err)
	}

	if !IsSortedMultiKey(orders, keys) {
		t.Error("Large array should be sorted")
	}

	for i := 1; i < count; i++ {
		if orders[i].Price < orders[i-1].Price {
			t.Errorf("Prices not ascending at index %d", i)
			break
		}
		if orders[i].Price == orders[i-1].Price && orders[i].Time < orders[i-1].Time {
			t.Errorf("Times not ascending at index %d", i)
			break
		}
		if orders[i].Price == orders[i-1].Price && orders[i].Time == orders[i-1].Time &&
			orders[i].OrderID < orders[i-1].OrderID {
			t.Errorf("Order IDs not ascending at index %d", i)
			break
		}
	}
}

func TestMultiKeyStability(t *testing.T) {
	type Record struct {
		Key           int32
		OriginalIndex int32
	}

	records := []Record{
		{1, 0}, {2, 1}, {1, 2}, {3, 3}, {2, 4}, {1, 5},
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Record{}.Key), Type: KeyTypeInt32, Order: Ascending},
	}

	err := MultiKeySort(records, keys)
	if err != nil {
		t.Fatalf("Sort failed: %v", err)
	}

	if records[0].Key != 1 || records[0].OriginalIndex != 0 {
		t.Error("First 1 should maintain order")
	}

	if records[1].Key != 1 || records[1].OriginalIndex != 2 {
		t.Error("Second 1 should maintain order")
	}

	if records[2].Key != 1 || records[2].OriginalIndex != 5 {
		t.Error("Third 1 should maintain order")
	}
}

func TestMultiKeyEmptyAndSingle(t *testing.T) {
	empty := []Order{}
	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Ascending},
	}

	err := MultiKeySort(empty, keys)
	if err != nil {
		t.Errorf("Empty array should succeed: %v", err)
	}

	single := []Order{{100.0, 1000, 1}}
	err = MultiKeySort(single, keys)
	if err != nil {
		t.Errorf("Single element should succeed: %v", err)
	}

	if single[0].OrderID != 1 {
		t.Error("Single element should be unchanged")
	}
}

func TestMultiKeyInvalidArgs(t *testing.T) {
	orders := []Order{
		{100.0, 1000, 1},
		{101.0, 900, 2},
	}

	err := MultiKeySort(orders, []KeySpec{})
	if err == nil {
		t.Error("Empty keys should fail")
	}

	tooManyKeys := make([]KeySpec, 10)
	err = MultiKeySort(orders, tooManyKeys)
	if err == nil {
		t.Error("Too many keys should fail")
	}

	notSlice := 42
	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Ascending},
	}
	err = MultiKeySort(notSlice, keys)
	if err == nil {
		t.Error("Non-slice should fail")
	}
}

func TestMultiKeyMixedTypes(t *testing.T) {
	type MixedRecord struct {
		ID       uint64
		Priority int32
		Score    float32
		Value    float64
	}

	records := []MixedRecord{
		{100, 1, 95.5, 1000.0},
		{200, 1, 98.0, 900.0},
		{300, 2, 95.5, 1100.0},
		{400, 1, 95.5, 950.0},
		{500, 2, 98.0, 1050.0},
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(MixedRecord{}.Priority), Type: KeyTypeInt32, Order: Ascending},
		{Offset: unsafe.Offsetof(MixedRecord{}.Score), Type: KeyTypeFloat, Order: Descending},
		{Offset: unsafe.Offsetof(MixedRecord{}.Value), Type: KeyTypeDouble, Order: Ascending},
	}

	err := MultiKeySort(records, keys)
	if err != nil {
		t.Fatalf("Mixed types sort failed: %v", err)
	}

	if records[0].Priority != 1 || records[0].Score != 98.0 {
		t.Error("First should be priority 1, score 98")
	}

	if records[1].Priority != 1 || records[1].Score != 95.5 || records[1].Value != 950.0 {
		t.Error("Second should be priority 1, score 95.5, value 950")
	}
}

func BenchmarkMultiKeyTwoKeys1K(b *testing.B) {
	orders := make([]Order, 1000)
	rand.Seed(42)
	for i := 0; i < 1000; i++ {
		orders[i].Price = 100.0 + float64(rand.Intn(100))*0.01
		orders[i].Time = 1000000 + int64(rand.Intn(100000))
		orders[i].OrderID = uint32(i)
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Descending},
		{Offset: unsafe.Offsetof(Order{}.Time), Type: KeyTypeInt64, Order: Ascending},
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		temp := make([]Order, len(orders))
		copy(temp, orders)
		MultiKeySort(temp, keys)
	}
}

func BenchmarkMultiKeyThreeKeys1K(b *testing.B) {
	orders := make([]Order, 1000)
	rand.Seed(42)
	for i := 0; i < 1000; i++ {
		orders[i].Price = 100.0 + float64(rand.Intn(100))*0.01
		orders[i].Time = 1000000 + int64(rand.Intn(100000))
		orders[i].OrderID = uint32(rand.Intn(10000))
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Descending},
		{Offset: unsafe.Offsetof(Order{}.Time), Type: KeyTypeInt64, Order: Ascending},
		{Offset: unsafe.Offsetof(Order{}.OrderID), Type: KeyTypeUInt32, Order: Ascending},
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		temp := make([]Order, len(orders))
		copy(temp, orders)
		MultiKeySort(temp, keys)
	}
}

func BenchmarkMultiKeyIndexed1K(b *testing.B) {
	orders := make([]Order, 1000)
	rand.Seed(42)
	for i := 0; i < 1000; i++ {
		orders[i].Price = 100.0 + float64(rand.Intn(100))*0.01
		orders[i].Time = 1000000 + int64(rand.Intn(100000))
		orders[i].OrderID = uint32(i)
	}

	keys := []KeySpec{
		{Offset: unsafe.Offsetof(Order{}.Price), Type: KeyTypeDouble, Order: Descending},
		{Offset: unsafe.Offsetof(Order{}.Time), Type: KeyTypeInt64, Order: Ascending},
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		MultiKeySortIndexed(orders, keys)
	}
}
