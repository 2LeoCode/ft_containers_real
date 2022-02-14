/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 23:27:32 by crochu            #+#    #+#             */
/*   Updated: 2022/02/14 17:28:07 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "vector.hpp"

namespace ft { template < class T, class Container > class stack; }

template <
	class T,
	class Container
> bool	operator ==(
	const ft::stack< T, Container > &lhs,
	const ft::stack< T, Container > &rhs
) { return lhs.c == rhs.c; }

template <
	class T,
	class Container
> bool	operator !=(
	const ft::stack< T, Container > &lhs,
	const ft::stack< T, Container > &rhs
) { return lhs.c != rhs.c; }

template <
	class T,
	class Container
> bool	operator <(
	const ft::stack< T, Container > &lhs,
	const ft::stack< T, Container > &rhs
) { return lhs.c < rhs.c; }

template <
	class T,
	class Container
> bool	operator <=(
	const ft::stack< T, Container > &lhs,
	const ft::stack< T, Container > &rhs
) { return lhs.c <= rhs.c; }

template <
	class T,
	class Container
> bool	operator >(
	const ft::stack< T, Container > &lhs,
	const ft::stack< T, Container > &rhs
) { return lhs.c > rhs.c; }

template <
	class T,
	class Container
> bool	operator >=(
	const ft::stack< T, Container > &lhs,
	const ft::stack< T, Container > &rhs
) { return lhs.c >= rhs.c; }

namespace ft {
	template <
		class T,
		class Container = ft::vector< T >
	> class stack {
		public:
			typedef Container							container_type;
			typedef T									value_type;
			typedef typename Container::size_type		size_type;
			typedef typename Container::reference		reference;
			typedef typename Container::const_reference	const_reference;

			friend bool	operator == < T, Container >(const stack&, const stack&);
			friend bool	operator != < T, Container >(const stack&, const stack&);
			friend bool	operator < < T, Container >(const stack&, const stack&);
			friend bool	operator <= < T, Container >(const stack&, const stack&);
			friend bool	operator > < T, Container >(const stack&, const stack&);
			friend bool	operator >= < T, Container >(const stack&, const stack&);

			explicit stack(const Container &cont = Container()) : c(cont) { }
			stack(const stack &other) :	c(other.c) { }

			~stack() { };

			stack	&operator =(const stack &other) {
				c = other.c;
				return *this;
			}

			reference		top(void) { return c.back(); }
			const_reference	top(void) const { return c.back(); }
			bool			empty(void) const { return c.empty(); }
			size_type		size(void) const { return c.size(); }
			void			push(const value_type &value) { c.push_back(value); }
			void			pop(void) { c.pop_back(); }

		protected:
			Container	c;
	};
}
