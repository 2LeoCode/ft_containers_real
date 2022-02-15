/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 22:27:28 by crochu            #+#    #+#             */
/*   Updated: 2022/02/15 20:38:01 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft {
	template <
		class T1,
		class T2
	> struct pair {
		pair(void) : first(), second() { }
		pair(const T1 &x, const T2 &y) : first(x), second(y) { }
		template <
			class U1,
			class U2
		> pair(const pair< U1, U2 > &other)
		:	first(other.first),
			second(other.second) { }

		pair	&operator =(const pair< T1, T2 > &other) {
			first = other.first, second = other.second;
			return *this;
		}

		T1	first;
		T2	second;
	};

	template <
		class T1, class T2
	> pair< T1, T2 >	make_pair(T1 t, T2 u) {
		return pair< T1, T2 >(t, u);
	}

	template <
		class T1,
		class T2
	> bool	operator ==(
		const ft::pair< T1, T2 > &lhs,
		const ft::pair< T1, T2 > &rhs
	) { return lhs.first == rhs.first && lhs.second == rhs.second; }
	
	template <
		class T1,
		class T2
	> bool	operator !=(
		const ft::pair< T1, T2 > &lhs,
		const ft::pair< T1, T2 > &rhs
	) { return lhs.first != rhs.first || lhs.second != rhs.second; }
	
	template <
		class T1,
		class T2
	> bool	operator <(
		const ft::pair< T1, T2 > &lhs,
		const ft::pair< T1, T2 > &rhs
	) {
		return (
			lhs.first < rhs.first
			|| (lhs.first == rhs.first && lhs.second < rhs.second)
		);
	}
	
	template <
		class T1,
		class T2
	> bool	operator <=(
		const ft::pair< T1, T2 > &lhs,
		const ft::pair< T1, T2 > &rhs
	) { return !(rhs < lhs); }
	
	template <
		class T1,
		class T2
	> bool	operator >(
		const ft::pair< T1, T2 > &lhs,
		const ft::pair< T1, T2 > &rhs
	) {
		return (
			lhs.first > rhs.first
			|| (lhs.first == rhs.first && lhs.second > rhs.second)
		);
	}
	
	template <
		class T1,
		class T2
	> bool	operator >=(
		const ft::pair< T1, T2 > &lhs,
		const ft::pair< T1, T2 > &rhs
	) { return !(rhs > lhs); }

}
