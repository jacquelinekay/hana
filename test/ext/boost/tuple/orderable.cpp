// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/ext/boost/tuple.hpp>
#include <boost/hana/tuple.hpp>
#include <laws/comparable.hpp>
#include <laws/orderable.hpp>
#include <boost/tuple/tuple.hpp>
namespace hana = boost::hana;


template <int i>
using eq = hana::test::ct_eq<i>;

template <int i>
using ord = hana::test::ct_ord<i>;

template <typename S, typename Xs>
void Comparable_tests(Xs xs) {
    using hana::test::iff;
    using hana::test::implies;

    // transitivity
    hana::test::foreach3(xs, [](auto a, auto b, auto c) {
        BOOST_HANA_CHECK(
            hana::and_(hana::equal(a, b), hana::equal(b, c))
                ^implies^ hana::equal(a, c)
        );
    });
}





struct invalid { };
template <typename S, typename Xs>
void Orderable_tests(Xs xs) {
    using hana::test::implies;
    using hana::test::iff;

    // transitivity
    hana::test::foreach3(xs, [](auto a, auto b, auto c) {
        BOOST_HANA_CHECK(
            hana::and_(hana::less_equal(a, b), hana::less_equal(b, c))
                ^implies^ hana::less_equal(a, c)
        );
    });
}



int main() {
    auto eq_tuples = hana::make_tuple(
          ::boost::make_tuple()
        , ::boost::make_tuple(eq<0>{})
        , ::boost::make_tuple(eq<0>{}, eq<1>{})
    );

    auto ord_tuples = hana::make_tuple(
          ::boost::make_tuple()
        , ::boost::make_tuple(ord<0>{})
        , ::boost::make_tuple(ord<0>{}, ord<1>{})
    );

    Comparable_tests<hana::ext::boost::tuple_tag>(eq_tuples);
    Orderable_tests<hana::ext::boost::tuple_tag>(ord_tuples);
}
