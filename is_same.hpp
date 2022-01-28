/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_same.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crochu <crochu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 16:22:54 by crochu            #+#    #+#             */
/*   Updated: 2021/12/04 16:46:14 by crochu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "integral_constant.hpp"

namespace ft {
	template <
		class T,
		class U
	> struct is_same : ft::false_type { };

	template <
		class T
	> struct is_same< T, T > : ft::true_type { };
}
