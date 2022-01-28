/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enable_if.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crochu <crochu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 18:36:48 by crochu            #+#    #+#             */
/*   Updated: 2021/11/25 22:24:57 by crochu           ###   ########.fr       */
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