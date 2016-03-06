// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/ext/boost/tuple.hpp>
#include <boost/hana/tuple.hpp>
#include <laws/comparable.hpp>
#include <laws/orderable.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>
namespace hana = boost::hana;
using hana::test::implies;
using hana::test::iff;

template <int i>
using eq = hana::test::ct_eq<i>;

template <int i>
using ord = hana::test::ct_ord<i>;


int main() {
    auto eq_tuples = hana::make_tuple(
          ::boost::make_tuple()
        , ::boost::make_tuple(eq<0>{})
        , ::boost::make_tuple(eq<0>{}, eq<1>{})
        , ::boost::make_tuple(eq<0>{}, eq<1>{}, eq<2>{})
    );

    auto ord_tuples = hana::make_tuple(
          ::boost::make_tuple()
        , ::boost::make_tuple(ord<0>{})
        , ::boost::make_tuple(ord<0>{}, ord<1>{})
        , ::boost::make_tuple(ord<0>{}, ord<1>{}, ord<2>{})
    );


    hana::test::foreach3(eq_tuples, [](auto a, auto b, auto c) {
        std::cout << "Comparable 1 pass" << std::endl;
        BOOST_HANA_CHECK(
            hana::and_(hana::equal(a, b), hana::equal(b, c))
                ^implies^ hana::equal(a, c)
        );
    });

    hana::test::foreach3(ord_tuples, [](auto a, auto b, auto c) {
        std::cout << "Orderable 1 pass" << std::endl;
        BOOST_HANA_CHECK(
            hana::and_(hana::less_equal(a, b), hana::less_equal(b, c))
                ^implies^ hana::less_equal(a, c)
        );
    });
}
