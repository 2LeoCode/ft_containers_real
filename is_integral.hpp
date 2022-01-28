/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_integral.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crochu <crochu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/06 00:37:48 by crochu            #+#    #+#             */
/*   Updated: 2021/12/06 01:50:38 by crochu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "integral_constant.hpp"

namespace ft {
	template <typename> struct is_integral : ft::false_type { };
	template <> struct is_integral< bool > : ft::true_type { };
	template <> struct is_integral< char > : ft::true_type { };
	template <> struct is_integral< short > : ft::true_type { };
	template <> struct is_integral< int > : ft::true_type { };
	template <> struct is_integral< long > : ft::true_type { };
	template <> struct is_integral< long long > : ft::true_type { };
	template <> struct is_integral< unsigned char > : ft::true_type { };
	template <> struct is_integral< unsigned > : ft::true_type { };
	template <> struct is_integral< unsigned long > : ft::true_type { };
	template <> struct is_integral< unsigned long long > : ft::true_type { };
};
