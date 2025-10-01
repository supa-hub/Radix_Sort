#pragma once

#include <concepts>
#include <iterator>
#include <vector>
#include <array>
#include <type_traits>



namespace sup
{

template <typename T>
concept integral_iterator = 
std::random_access_iterator<T> &&
requires(T a)
{
    *a;
    requires std::integral< typename std::remove_reference<decltype(*a)>::type >;
};



constexpr void sort(integral_iterator auto first, integral_iterator auto last)
requires std::same_as<decltype(first), decltype(last)>
{
    // store the type of the values, so dont have to always write the long expression
    using value_type = typename std::remove_reference<decltype(*first)>::type;
    using iterator_type = decltype(first);

    // sort only if the start is below the ending
    if ( first >= last ) return;


    // create the buckets
    std::array<size_t, 256> iters;

    // didn't use end - first, because it returns ptrdiff_t, which is signed, compared to size_t
    std::vector<value_type> aux_swap(first, last);

    // use the underlying iterators to swap between the given array and the auxiliary array,
    // since every byte iteration the container that is being sorted changes
    iterator_type aux_first = aux_swap.begin();
    iterator_type aux_last = aux_swap.end();

    iterator_type given_first = first;
    iterator_type given_last = last;


    const int mask = 0xff;
    size_t bucket = 0;
    const int decr = std::is_signed_v<value_type> ? 1 : 0;

    /*
     * if the type can have negative values, decrease the amount of bytes by 1 since in the last iteration we have to take the
     * signed bit into account. Stop sorting if already sorted.
    */
    bool sorted = std::is_sorted(first, last);
    for ( size_t idx = 0; (idx < sizeof( value_type ) - decr) && !sorted; idx++ ) {
        iters.fill(0); // need to reset the iterations array


        // increment the index of the byte value
        for ( iterator_type jdx = given_first; jdx < given_last; jdx++ ) {
            bucket = ((*jdx) >> (idx * 8)) & mask;
            iters[bucket]++;
        }


        // accumulate the counts
        size_t increment = iters[0];
        iters[0] = 0;

        for ( size_t i = 1; i < iters.size(); i++ ) {
            size_t next = iters[i];
            iters[i] = iters[i - 1] + increment;
            increment = next;
        }
        
        // insert the values by their current byte into a sorted position
        for ( iterator_type jdx = given_first; jdx < given_last; jdx++ ) {
            bucket = ((*jdx) >> (idx * 8)) & mask;

            size_t location = iters[bucket];
            iters[bucket] += 1;

            *(aux_first + location) = *jdx;
        }

        // swap the array iterators for the next iteration
        std::swap(aux_first, given_first);
        std::swap(aux_last, given_last);

        // check if already sorted
        sorted = std::is_sorted(first, last);
    }


    /*
     * have to add the almost identical code to take the signed bit into account,
     * only the bucket indexing changes a little bit.
    */
    if (std::is_signed_v<value_type> && !sorted) {
        iters.fill(0); // need to reset the iterations array


        // increment the index of the byte value
        for ( iterator_type jdx = given_first; jdx < given_last; jdx++ ) {
            // now we only have to look at the most significant byte
            bucket = ((*jdx) >> ((sizeof( value_type ) - 1) * 8)) & mask;
            bucket ^= 0x80;

            iters[bucket]++;
        }


        // accumulate the counts
        size_t increment = iters[0];
        iters[0] = 0;

        for ( size_t i = 1; i < iters.size(); i++ ) {
            size_t next = iters[i];
            iters[i] = iters[i - 1] + increment;
            increment = next;
        }
        
        // insert the values by their current byte into a sorted position
        for ( iterator_type jdx = given_first; jdx < given_last; jdx++ ) {
            bucket = ((*jdx) >> ((sizeof( value_type ) - 1) * 8)) & mask;
            bucket ^= 0x80;

            size_t location = iters[bucket];
            iters[bucket] += 1;

            *(aux_first + location) = *jdx;
        }

        // swap the array iterators for the next iteration
        std::swap(aux_first, given_first);
        std::swap(aux_last, given_last);
    }

    
    return;
}

}