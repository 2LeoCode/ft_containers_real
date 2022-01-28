/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator_traits.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crochu <crochu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:45:39 by crochu            #+#    #+#             */
/*   Updated: 2021/12/06 01:06:34 by crochu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstddef>
#include <iterator>

namespace ft {
	template <
		class Iter
	> struct iterator_traits {
		typedef typename Iter::difference_type		difference_type;
		typedef typename Iter::value_type			value_type;
		typedef typename Iter::pointer				pointer;
		typedef typename Iter::reference			reference;
		typedef typename Iter::iterator_category	iterator_category;
	};
	
	template <
		class T
	> struct iterator_traits< T* > {
		typedef std::ptrdiff_t					difference_type;
		typedef T								value_type;
		typedef T*								pointer;
		typedef T&								reference;
		typedef std::random_access_iterator_tag	iterator_category;
	};

	struct basic_iterator_traits {
		typedef void	difference_type;
		typedef void	value_type;
		typedef void	pointer;
		typedef void	reference;
		typedef void	iterator_category;
	};

	template <
	> struct iterator_traits< bool > : basic_iterator_traits { };

	template <
	> struct iterator_traits< char > : basic_iterator_traits { };

	template <
	> struct iterator_traits< short > : basic_iterator_traits { };

	template <
	> struct iterator_traits< int > : basic_iterator_traits { };

	template <
	> struct iterator_traits< long > : basic_iterator_traits { };

	template <
	> struct iterator_traits< long long > : basic_iterator_traits { };

	template <
	> struct iterator_traits< unsigned > : basic_iterator_traits { };

	template <
	> struct iterator_traits< unsigned long > : basic_iterator_traits { };

	template <
	> struct iterator_traits< unsigned long long > : basic_iterator_traits { };

	template <
	> struct iterator_traits< float > : basic_iterator_traits { };

	template <
	> struct iterator_traits< double > : basic_iterator_traits { };

	template <
	> struct iterator_traits< long double > : basic_iterator_traits { };
}
