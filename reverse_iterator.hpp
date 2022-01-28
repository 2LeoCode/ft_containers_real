/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse_iterator.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsuardi <lsuardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:57:23 by crochu            #+#    #+#             */
/*   Updated: 2022/01/28 16:38:35 by lsuardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <memory>
#include "iterator_traits.hpp"

namespace ft {
	template <
		class Iter
	> class reverse_iterator {
		public:
			typedef Iter
					iterator_type;
			typedef typename iterator_traits< Iter >::iterator_category
					iterator_category;
			typedef typename iterator_traits< Iter >::value_type
					value_type;
			typedef typename iterator_traits< Iter >::difference_type
					difference_type;
			typedef typename iterator_traits< Iter >::pointer
					pointer;
			typedef typename iterator_traits< Iter >::reference
					reference;
	
			reverse_iterator() : current() {  }
			explicit reverse_iterator(iterator_type x) : current(x) { }
			template <
				class U
			> reverse_iterator(const reverse_iterator< U > &other) : current(other.base()) { }

			template <
				class U
			> reverse_iterator	&operator =(const reverse_iterator< U > &other) {
				current = other.base();
				return *this;
			}

			iterator_type	base(void) const { return current; }
		
			reference	operator *(void) const {
				Iter	tmp = current;
				return *--tmp;
			}
			pointer		operator ->(void) const {
				Iter	tmp = current;
				return &*--tmp;
			}
			reference	operator [](difference_type n) {
				return current[-n - 1];
			}
			reverse_iterator		&operator +=(difference_type n) {
				current -= n;
				return *this;
			}
			reverse_iterator		&operator -=(difference_type n) {
				current += n;
				return *this;
			}
			reverse_iterator		&operator ++(void) {
				--current;
				return *this;
			}
			reverse_iterator		&operator --(void) {
				++current;
				return *this;
			}
			reverse_iterator		operator ++(int) { return reverse_iterator(current--); }
			reverse_iterator		operator --(int) { return reverse_iterator(current++); }
			reverse_iterator		operator +(difference_type n) const {
				return reverse_iterator(current - n);
			}
			reverse_iterator		operator -(difference_type n) const {
				return reverse_iterator(current + n);
			}

		protected:
			Iter	current;
	};
}

template <
	class Iterator1,
	class Iterator2
> bool	operator ==(
	const ft::reverse_iterator< Iterator1 > &lhs,
	const ft::reverse_iterator< Iterator2 > &rhs
) {

	return lhs.base() == rhs.base();
}

template <
	class Iterator1,
	class Iterator2
> bool	operator !=(
	const ft::reverse_iterator< Iterator1 > &lhs,
	const ft::reverse_iterator< Iterator2 > &rhs
) { return lhs.base() != rhs.base(); }

template <
	class Iterator1,
	class Iterator2
> bool operator <(
	const ft::reverse_iterator< Iterator1 > &lhs,
	const ft::reverse_iterator< Iterator2 > &rhs
) { return lhs.base() > rhs.base(); }

template <
	class Iterator1,
	class Iterator2
> bool operator <=(
	const ft::reverse_iterator< Iterator1 > &lhs,
	const ft::reverse_iterator< Iterator2 > &rhs
) { return lhs.base() >= rhs.base(); }

template <
	class Iterator1,
	class Iterator2
> bool operator >(
	const ft::reverse_iterator< Iterator1 > &lhs,
	const ft::reverse_iterator< Iterator2 > &rhs
) { return lhs.base() < rhs.base(); }

template <
	class Iterator1,
	class Iterator2
> bool operator >=(
	const ft::reverse_iterator< Iterator1 > &lhs,
	const ft::reverse_iterator< Iterator2 > &rhs
) { return lhs.base() <= rhs.base(); }

template <
	class Iter
> ft::reverse_iterator< Iter >	operator +(
	typename ft::reverse_iterator< Iter >::difference_type n,
	const ft::reverse_iterator< Iter > &it
) { return it + n; }

template <
	class Iter1,
	class Iter2
> typename ft::reverse_iterator< Iter1 >::difference_type	operator -(
	const ft::reverse_iterator< Iter1 >& lhs,
	const ft::reverse_iterator< Iter2 >& rhs
) { return rhs.base() - lhs.base(); }
