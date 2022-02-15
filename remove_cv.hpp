/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_cv.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/15 15:31:10 by Leo Suardi        #+#    #+#             */
/*   Updated: 2022/02/15 15:43:30 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

namespace ft {
	template < class T > struct remove_cv { typedef T type; };
	template < class T > struct remove_cv< const T > : remove_cv< T > { };
	template < class T > struct remove_cv< volatile T > : remove_cv< T > { };
	template < class T > struct remove_cv< const volatile T > : remove_cv< T > { };
}
