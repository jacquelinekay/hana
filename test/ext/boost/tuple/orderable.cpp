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


template <int i>
using eq = hana::test::ct_eq<i>;

template <int i>
using ord = hana::test::ct_ord<i>;

template <int i>
struct for_each_n {
    template <typename Xs, typename F>
    constexpr auto operator()(Xs xs, F f) const {
        hana::for_each(xs,
            hana::compose(
                hana::partial(for_each_n<i - 1>{}, xs),
                hana::partial(hana::partial, f)
            )
        );
    }
};

template <>
struct for_each_n<1> {
    template <typename Xs, typename F>
    constexpr auto operator()(Xs xs, F f) const {
        hana::for_each(xs, f);
    }
};

template <typename S, typename Eqs, typename Ords>
void tests(Eqs eqs, Ords ords) {
    for_each_n<3>{}(eqs, [](auto a, auto b, auto c) {
        std::cout << "Comparable 1 pass" << std::endl;
    });

    for_each_n<3>{}(ords, [](auto a, auto b, auto c) {
        std::cout << "Orderable 1 pass" << std::endl;
    });
}



int main() {
    auto eq_tuples = hana::make_tuple(
          ::boost::make_tuple(eq<0>{})
        , ::boost::make_tuple(eq<0>{}, eq<1>{})
        , ::boost::make_tuple(eq<0>{}, eq<1>{}, eq<2>{})
    );

    auto ord_tuples = hana::make_tuple(
          ::boost::make_tuple(ord<0>{})
        , ::boost::make_tuple(ord<0>{}, ord<1>{})
        , ::boost::make_tuple(ord<0>{}, ord<1>{}, ord<2>{})
    );

    tests<hana::ext::boost::tuple_tag>(eq_tuples, ord_tuples);
}
