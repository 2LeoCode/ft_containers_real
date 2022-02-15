/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enable_if.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsuardi <lsuardi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 18:36:48 by crochu            #+#    #+#             */
/*   Updated: 2022/02/15 14:50:36 by lsuardi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft {
	template <
		bool B,
		class T = void
	> struct enable_if { };

	template <
		class T
	> struct enable_if< true, T > {
		typedef T	type;
	};
}
