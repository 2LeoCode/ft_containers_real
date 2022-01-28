/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   to_string.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crochu <crochu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 22:28:34 by crochu            #+#    #+#             */
/*   Updated: 2021/12/04 15:12:44 by crochu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <climits>
#include <iostream>

static size_t	number_size(int value) {
	size_t	size = 0;

	if (value == INT_MIN) ++value;
	if (value < 0) value = -value, ++size;
	while (value) value /= 10, ++size;
	return (size);
}

static size_t	number_size(long value) {
	size_t	size = 0;

	if (value == LONG_MIN) ++value;
	if (value < 0) value = -value, ++size;
	while (value) value /= 10, ++size;
	return (size);
}

static size_t	number_size(long long value) {
	size_t	size = 0;

	if (value == LLONG_MIN) ++value;
	if (value < 0) value = -value, ++size;
	while (value) value /= 10, ++size;
	return (size);
}

static size_t	number_size(unsigned value) {
	size_t	size = 0;

	while (value) value /= 10, ++size;
	return (size);
}

static size_t	number_size(unsigned long value) {
	size_t	size = 0;

	while (value) value /= 10, ++size;
	return (size);
}

static size_t	number_size(unsigned long long value) {
	size_t	size = 0;

	while (value) value /= 10, ++size;
	return (size);
}

namespace ft {
	std::string	to_string(int value) {
		size_t		count = number_size(value);
		char		a[count + 1];

		a[count] = 0;
		if (value < 0) *a = '-';
		while (value) a[--count] = value % 10 + '0', value /= 10;
		return a;
	}
	std::string	to_string(long value) {
		size_t		count = number_size(value);
		char		a[count + 1];

		a[count] = 0;
		if (value < 0) *a = '-';
		while (value) a[--count] = value % 10 + '0', value /= 10;
		return a;
	}
	std::string	to_string(long long value) {
		size_t		count = number_size(value);
		char		a[count + 1];

		a[count] = 0;
		if (value < 0) *a = '-';
		while (value) a[--count] = value % 10 + '0', value /= 10;
		return a;
	}
	std::string	to_string(unsigned value) {
		size_t		count = number_size(value);
		char		a[count + 1];

		a[count] = 0;
		while (value) a[--count] = value % 10 + '0', value /= 10;
		return a;
	}
	std::string	to_string(unsigned long value) {
		size_t		count = number_size(value);
		char		a[count + 1];

		a[count] = 0;
		while (value) a[--count] = value % 10 + '0', value /= 10;
		return a;
	}
	std::string	to_string(unsigned long long value) {
		size_t		count = number_size(value);
		char		a[count + 1];

		a[count] = 0;
		while (value) a[--count] = value % 10 + '0', value /= 10;
		return a;
	}
}
