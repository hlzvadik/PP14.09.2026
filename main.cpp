#include <iostream>
#include <string>
#include <vector>
#include <future>

#include "clicker.hpp"

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

value_t sumInData(const data_t& data, size_t start_index, size_t end_index) {
  value_t sum{0};
  for (size_t i = start_index; i < end_index; ++i) {
    sum += data[i];
  }
  return sum;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    throw std::runtime_error("Not correct count of arguments");
  }
  size_t count_of_threads = std::stoull(std::string(argv[1]));

  constexpr size_t size{200'000'000};
  double init{0}, total{0};
  value_t sum{0};

  size_t size_for_big_threads = size / count_of_threads + 1;
  size_t size_for_small_threads = size / count_of_threads;
  size_t count_of_big_threads = size % count_of_threads;
  {
    Clicker cl;
    data_t values(size, 1);
    init = cl.millisec();
    auto futures = std::vector< std::future< value_t > >(count_of_threads);
    for (size_t i = 0; i < count_of_big_threads; ++i) {
      size_t start = i * size_for_big_threads;
      futures[i] = std::async(std::launch::async,
        sumInData, std::ref(values), start, start + size_for_big_threads);
    }
    for (size_t i = count_of_big_threads; i < count_of_threads; ++i) {
      size_t start = count_of_big_threads * size_for_big_threads + (i - count_of_big_threads) * size_for_small_threads;
      futures[i] = std::async(std::launch::async,
        sumInData, std::ref(values), start, start + size_for_small_threads);
    }
    for (size_t i = 0; i < count_of_threads; ++i) {
      sum += futures[i].get();
    }
    total = cl.millisec();
  }
  std::cout << "Sum: " << sum << "\n";
  std::cout << "Total time: " << total - init << "\n";
}
