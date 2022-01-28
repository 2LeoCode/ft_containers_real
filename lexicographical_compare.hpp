/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexicographical_compare.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsuardi <lsuardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 22:13:00 by crochu            #+#    #+#             */
/*   Updated: 2022/01/28 15:33:43 by lsuardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft {
	template <
		class InputIt1,
		class InputIt2
	> bool	lexicographical_compare(
		InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2
	) {
		while (first1 != last1 && first2 != last2 && *first1 == *first2)
			++first1, ++first2;
		if (first1 == last1 && first2 == last2) return false;
		return first1 == last1 || *first1 < *first2;
	}

	template <
		class InputIt1,
		class InputIt2,
		class Compare
	> bool	lexicographical_compare(
		InputIt1 first1, InputIt1 last1,
		InputIt2 first2, InputIt2 last2,
		Compare comp
	) {
		while (first1 != last1 && first2 != last2 && *first1 == *first2)
			++first1, ++first2;
		if (first1 == last1 && first2 == last2) return false;
		return first1 == last1 || comp(*first1, *first2);
	}
}
