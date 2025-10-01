#include "radix_sort.hpp"

#include <chrono>
#include <random>
#include <functional>
#include <iostream>


/*
 * compile time tests of the concept
*/
int test(sup::integral_iterator auto a)
{
    return 2;
}

int test(std::vector<float>::iterator a)
{
    return 3;
}



int main()
{
    std::vector<int> a = { 3, 2, 1 };
    std::vector<int> a1 = { 1, 2, -10, 10000000, 5000, 222, -3434, 100, 2, 3, 4 };
    std::vector<float> b;

    std::vector<int>::iterator c = a.begin();
    typename std::remove_reference<decltype(*c)>::type p; // evaluates to int


    std::cout << test(a.begin()) << std::endl;
    std::cout << test(b.begin()) << std::endl;

    auto ok = a.begin() > a1.begin();
    auto ok1 = a.begin() - a1.begin();

    std::cout << ok << std::endl;

    static_assert( sup::integral_iterator<decltype(a.begin())> );

    static_assert( !sup::integral_iterator<decltype(b.begin())> );


    sup::sort(a.begin(), a.end());
    sup::sort(a1.begin(), a1.end());

    for ( const auto& a_value : a ) { std::cout << a_value << " "; }
    std::cout << std::endl;

    for ( const auto& a_value : a1 ) { std::cout << a_value << " "; }
    std::cout << std::endl;


    /*
     * create an array with random values between LLONG_MIN - LLONG_MAX 
     * and copy the values into 2 arrays to compare sort performance between
     * std::sort and our own radix sort
    */
    const size_t amount_of_values = 1e8;
    using value_type = long long;


    std::default_random_engine generator;
    generator.seed(std::chrono::system_clock::now().time_since_epoch().count()); // give our generator a random seed
    std::uniform_int_distribution<value_type> distribution(std::numeric_limits<value_type>::min(), std::numeric_limits<value_type>::max());
    
    auto random_num = std::bind( distribution, generator );  // bind the generator to the distribution object
 
    std::vector<value_type> test_values(amount_of_values);
    std::generate(test_values.begin(), test_values.end(), random_num);
    std::cout << "generated random numbers" << std::endl;


    std::vector<value_type> test_radix(amount_of_values);
    std::copy(test_values.begin(), test_values.end(), test_radix.begin());

    std::vector<value_type> test_standard(amount_of_values);
    std::copy(test_values.begin(), test_values.end(), test_standard.begin());

    std::cout << "copied the values to the test arrays" << std::endl;

    std::chrono::high_resolution_clock::time_point time0 = std::chrono::high_resolution_clock::now();
    sup::sort(test_radix.begin(), test_radix.end());
    std::chrono::high_resolution_clock::time_point time1 = std::chrono::high_resolution_clock::now();

    std::cout << "duration of radix sort: " << (std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0)).count() << " milliseconds. \n";


    std::chrono::high_resolution_clock::time_point time2 = std::chrono::high_resolution_clock::now();
    std::sort(test_standard.begin(), test_standard.end());
    std::chrono::high_resolution_clock::time_point time3 = std::chrono::high_resolution_clock::now();

    std::cout << "duration of std::sort: " << (std::chrono::duration_cast<std::chrono::milliseconds>(time3 - time2)).count() << " milliseconds. \n";

    std::cout << "is the radix sort solution sorted: " << (std::is_sorted(test_radix.begin(), test_radix.end()) ? "true" : "false") << std::endl;

    return 0;
}

