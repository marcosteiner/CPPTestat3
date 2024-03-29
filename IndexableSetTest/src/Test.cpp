#include "IndexableSet.h"
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

std::array<int const, 5> default_range{1, 2, 3, 4, 5};

void test_default_constructor_creates_empty_indexable_set() {
	indexable_set::indexable_set<int> const set{};
	ASSERT_EQUAL(0, set.size());
}

void test_range_constructor() {
	/*
	 * Constructs a container with as many elements as the range [first,last),
	 * with each element emplace-constructed from its corresponding element in that range.
	 * Source: www.cplusplus.com/reference/set/set
	 */
	indexable_set::indexable_set<int> const set{begin(default_range), end(default_range)};

	ASSERT_EQUAL(5, set.size());
}

void test_copy_constructor() {
	/*
	 * Constructs a container with a copy of each of the elements in x.
	 * Source: www.cplusplus.com/reference/set/set
	 */
	indexable_set::indexable_set<int> const first_set{begin(default_range), end(default_range)};

	indexable_set::indexable_set<int> const second_set{first_set};
	ASSERT_EQUAL(5, second_set.size());
}


void test_copy_constructor_copies() {

	indexable_set::indexable_set<int> const first_set{begin(default_range), end(default_range)};
	indexable_set::indexable_set<int> const second_set{first_set};

	ASSERT_EQUAL(1, second_set[0]);
	ASSERT_EQUAL(5, second_set.size());
}

// endregion

void test_move_constructor() {
	/*
	 * Constructs a container that acquires the elements of x.
	 * If alloc is specified and is different from x's allocator, the elements are moved.
	 * Otherwise, no elements are constructed (their ownership is directly transferred).
	 * x is left in an unspecified but valid state.
	 * Source: www.cplusplus.com/reference/set/set
	 */
	indexable_set::indexable_set<int> const first_set{begin(default_range), end(default_range)};
	indexable_set::indexable_set<int> const new_set = std::move(first_set);

	ASSERT_EQUAL(5, new_set.size());
}

void test_initializer_list_constructor() {
	/*
	 * Constructs a container with a copy of each of the elements in il.
	 * Source: www.cplusplus.com/reference/set/set
	 */
	indexable_set::indexable_set<int> const my_set{1,2,3,4,5};
	ASSERT_EQUAL(5, my_set.size());
}

void test_index_access() {
	indexable_set::indexable_set<int> const my_set{begin(default_range), end(default_range)};

	ASSERT_EQUAL(2, my_set[1]);
}

void test_index_access_sorted_order() {
	std::array<const int, 5> my_array{3, 4, 1, 2, 5};
	indexable_set::indexable_set<int> const my_set{begin(my_array), end(my_array)};

	ASSERT_EQUAL(2, my_set[1]);
}

void test_index_access_reverse() {
	indexable_set::indexable_set<int> const my_set{begin(default_range), end(default_range)};

	ASSERT_EQUAL(5, my_set[-1]);
}

void test_front() {
	indexable_set::indexable_set<int> const my_set{begin(default_range), end(default_range)};

	ASSERT_EQUAL(1, my_set.front());
}

void test_back() {
	indexable_set::indexable_set<int> const my_set{begin(default_range), end(default_range)};

	ASSERT_EQUAL(5, my_set.back());
}

void test_out_of_bounds() {
	indexable_set::indexable_set<int> const my_set{begin(default_range), end(default_range)};

	ASSERT_THROWS(my_set[5], std::out_of_range);
}

void test_out_of_bounds_min() {
	indexable_set::indexable_set<int> const my_set{begin(default_range), end(default_range)};

	ASSERT_THROWS(my_set[-6], std::out_of_range);
}

void test_front_empty_set_throws_out_of_range() {
	indexable_set::indexable_set<int> const empty_set{};

	ASSERT_THROWS(empty_set.front(), std::out_of_range);
}

void test_back_empty_set_throws_out_of_range(){
	indexable_set::indexable_set<int> const empty_set{};

	ASSERT_THROWS(empty_set.back(), std::out_of_range);
}


void test_positive_index_empty_set_throws_out_of_range(){

	indexable_set::indexable_set<int> const empty_set{};

	ASSERT_THROWS(empty_set[1], std::out_of_range);
}

void test_negative_index_empty_set_throws_out_of_range(){

	indexable_set::indexable_set<int> const empty_set{};

	ASSERT_THROWS(empty_set[-1], std::out_of_range);
}

void test_compare_functor() {
	indexable_set::indexable_set<int, std::greater<int>> const my_set{1, 2, 3, 4, 5};

	ASSERT_EQUAL(5, my_set[0]);
}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	s.push_back(CUTE(test_default_constructor_creates_empty_indexable_set));
	s.push_back(CUTE(test_range_constructor));
	s.push_back(CUTE(test_copy_constructor));
	s.push_back(CUTE(test_copy_constructor_copies));
	s.push_back(CUTE(test_move_constructor));
	s.push_back(CUTE(test_initializer_list_constructor));
	s.push_back(CUTE(test_index_access));
	s.push_back(CUTE(test_index_access_reverse));
	s.push_back(CUTE(test_index_access_sorted_order));
	s.push_back(CUTE(test_front));
	s.push_back(CUTE(test_back));
	s.push_back(CUTE(test_out_of_bounds));
	s.push_back(CUTE(test_out_of_bounds_min));
	s.push_back(CUTE(test_front_empty_set_throws_out_of_range));
	s.push_back(CUTE(test_compare_functor));
	s.push_back(CUTE(test_back_empty_set_throws_out_of_range));
	s.push_back(CUTE(test_positive_index_empty_set_throws_out_of_range));
	s.push_back(CUTE(test_negative_index_empty_set_throws_out_of_range));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner { cute::makeRunner(lis, argc, argv) };
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
