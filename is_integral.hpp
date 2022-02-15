/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_integral.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 00:37:48 by crochu            #+#    #+#             */
/*   Updated: 2022/02/15 15:49:14 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "integral_constant.hpp"
#include "remove_cv.hpp"

namespace ft {
	template <typename> struct is_integral_b : false_type { };
	template <> struct is_integral_b< bool > : true_type { };
	template <> struct is_integral_b< char > : true_type { };
	template <> struct is_integral_b< short > : true_type { };
	template <> struct is_integral_b< int > : true_type { };
	template <> struct is_integral_b< long > : true_type { };
	template <> struct is_integral_b< long long > : true_type { };
	template <> struct is_integral_b< unsigned char > : true_type { };
	template <> struct is_integral_b< unsigned > : true_type { };
	template <> struct is_integral_b< unsigned long > : true_type { };
	template <> struct is_integral_b< unsigned long long > : true_type { };
	template <typename T> struct is_integral
	: is_integral_b< typename remove_cv< T >::type > { };
};
