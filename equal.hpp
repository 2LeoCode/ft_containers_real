/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   equal.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 21:53:29 by crochu            #+#    #+#             */
/*   Updated: 2022/01/31 16:42:04 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft {
	template <
		class InputIt1,
		class InputIt2
	> bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
		while (first1 != last1 && *first1 == *first2)
			++first1, ++first2;
		return first1 == last1;
	}
	
	template <
		class InputIt1,
		class InputIt2,
		class BinaryPredicate
	> bool	equal(
		InputIt1 first1, InputIt1 last1,
		InputIt2 first2,
		BinaryPredicate p
	) {
		while (first1 != last1 && p(*first1, *first2))
			++first1, ++first2;
		return first1 == last1;
	}
}