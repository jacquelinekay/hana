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

    hana::test::foreach2(xs, [](auto a, auto b) {

        // reflexivity
        BOOST_HANA_CHECK(
            hana::equal(a, a)
        );

        // symmetry
        BOOST_HANA_CHECK(
            hana::equal(a, b) ^implies^ hana::equal(b, a)
        );

        // `not_equal` is the negation of `equal`
        BOOST_HANA_CHECK(
            hana::not_equal(a, b) ^iff^ hana::not_(hana::equal(a, b))
        );

        // equal.to and not_equal.to
        BOOST_HANA_CHECK(
            hana::equal.to(a)(b) ^iff^ hana::equal(a, b)
        );

        BOOST_HANA_CHECK(
            hana::not_equal.to(a)(b) ^iff^ hana::not_equal(a, b)
        );

        // comparing
        hana::test::_injection<0> f{};
        BOOST_HANA_CHECK(
            hana::comparing(f)(a, b) ^iff^ hana::equal(f(a), f(b))
        );
    });

    // transitivity
    hana::test::foreach3(xs, [](auto a, auto b, auto c) {
        BOOST_HANA_CHECK(
            hana::and_(hana::equal(a, b), hana::equal(b, c))
                ^implies^ hana::equal(a, c)
        );
    });



    constexpr auto list = hana::make<S>;
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        list(),
        list()
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        list(eq<0>{}),
        list()
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        list(),
        list(eq<0>{})
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        list(eq<0>{}),
        list(eq<0>{})
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        list(eq<0>{}, eq<1>{}),
        list(eq<0>{})
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        list(eq<0>{}, eq<1>{}),
        list(eq<0>{}, eq<1>{})
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::equal(
        list(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}),
        list(eq<0>{}, eq<1>{}, eq<2>{}, eq<4>{})
    )));
}





struct invalid { };
template <typename S, typename Xs>
void Orderable_tests(Xs xs) {
    using hana::test::implies;
    using hana::test::iff;

    hana::test::foreach2(xs, [](auto a, auto b) {
        // antisymmetry
        BOOST_HANA_CHECK(
            hana::and_(hana::less_equal(a, b), hana::less_equal(b, a))
                ^implies^ hana::equal(a, b)
        );

        // totality
        BOOST_HANA_CHECK(
            hana::or_(hana::less_equal(a, b), hana::less_equal(b, a))
        );

        // other methods in terms of `less_equal`
        BOOST_HANA_CHECK(
            hana::less(a, b) ^iff^ hana::not_(hana::less_equal(b, a))
        );

        BOOST_HANA_CHECK(
            hana::greater(a, b) ^iff^ hana::less(b, a)
        );

        BOOST_HANA_CHECK(
            hana::greater_equal(a, b) ^iff^ hana::not_(hana::less(a, b))
        );

        // less.than & al.
        BOOST_HANA_CHECK(hana::less.than(a)(b) ^iff^ hana::less(b, a));
        BOOST_HANA_CHECK(hana::greater.than(a)(b) ^iff^ hana::greater(b, a));
        BOOST_HANA_CHECK(hana::less_equal.than(a)(b) ^iff^ hana::less_equal(b, a));
        BOOST_HANA_CHECK(hana::greater_equal.than(a)(b) ^iff^ hana::greater_equal(b, a));

        // ordering
        hana::test::_injection<0> f{}; // test::_injection is also monotonic
        BOOST_HANA_CHECK(
            hana::ordering(f)(a, b) ^iff^ hana::less(f(a), f(b))
        );
    });

    // transitivity
    hana::test::foreach3(xs, [](auto a, auto b, auto c) {
        BOOST_HANA_CHECK(
            hana::and_(hana::less_equal(a, b), hana::less_equal(b, c))
                ^implies^ hana::less_equal(a, c)
        );
    });



    constexpr auto list = hana::make<S>;

    //////////////////////////////////////////////////////////////////
    // less
    //////////////////////////////////////////////////////////////////
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        list(),
        list()
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        list(),
        list(invalid{})
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        list(invalid{}),
        list()
    )));
    BOOST_HANA_CONSTANT_CHECK(hana::less(
        list(ord<0>{}),
        list(ord<7>{})
    ));
    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        list(ord<1>{}),
        list(ord<0>{})
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::less(
        list(ord<0>{}, ord<1>{}, ord<8>{}),
        list(ord<0>{}, ord<1>{})
    )));

    BOOST_HANA_CONSTANT_CHECK(hana::less(
        list(ord<0>{}, ord<0>{}, ord<8>{}),
        list(ord<0>{}, ord<1>{})
    ));
}



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

    Comparable_tests<hana::ext::boost::tuple_tag>(eq_tuples);
    Orderable_tests<hana::ext::boost::tuple_tag>(ord_tuples);
}
