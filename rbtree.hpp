/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Leo Suardi <lsuardi@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 23:43:13 by crochu            #+#    #+#             */
/*   Updated: 2022/02/13 19:59:37 by Leo Suardi       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdlib>
#include <limits>
#include <memory>
#include <iostream>
#include <iterator>
#include <fstream>
#include <functional>
#include <algorithm>
#include <string>
#include "reverse_iterator.hpp"
#include "pair.hpp"

template <
	class value_type,
	class value_compare,
	class Allocator = std::allocator< value_type >,
	typename size_type = std::size_t,
	typename difference_type = std::ptrdiff_t
> class rbtree {
	public:
		enum color {
			RED,
			BLACK
		};

		class rbnode {
			public:

				rbnode(
					enum color color,
					const value_type &data,
					rbnode *parent = NULL,
					void *hint = NULL
				) :	m_alloc(Allocator()),
					m_color(color),
					m_parent(parent),
					m_left(NULL),
					m_right(NULL),
					m_data(m_alloc.allocate(1, hint)) {					
					m_alloc.construct(m_data, data);
				}
				~rbnode() {
					if (m_parent) {
						if (m_parent->m_right == this) m_parent->m_right = NULL;
						else if (m_parent->m_left == this) m_parent->m_left = NULL;
					}
					m_alloc.destroy(m_data);
					m_alloc.deallocate(m_data, 1);
				}

				rbnode	&operator =(const rbnode &other) {
					m_data = other.m_data;
					return *this;
				}
				bool	operator ==(const rbnode &other) const {
					return *m_data == *other.m_data;
				}
				bool	operator !=(const rbnode &other) const {
					return !operator ==(other);
				}

				value_type			&data(void) { return *m_data; }
				const value_type	&data(void) const { return *m_data; }
				value_type			*addr(void) { return m_data; }
				const value_type	*addr(void) const { return m_data; }
				value_type			*&raddr(void) { return m_data; }
				const value_type	*&raddr(void) const { return m_data; }
				enum color			color(void) const { return m_color; }
				rbnode				*parent(void) { return m_parent; }
				rbnode				*left(void) { return m_left; }
				const rbnode		*left(void) const { return m_left; }
				rbnode				*&rleft(void) { return m_left; }
				rbnode				*right(void) { return m_right; }
				const rbnode		*right(void) const { return m_right; }
				rbnode				*&rright(void) { return m_right; }
				rbnode				*sibling(void) {
					if (!m_parent) return NULL;
					return m_parent->m_left == this ?
						m_parent->m_right : m_parent->m_left;
				}

				rbnode		*rotate_left(void) {
					rbnode	*parent = m_parent;
					m_parent = m_right;
					if (parent) {
						if (parent->m_left == this)
							parent->m_left = m_right;
						else parent->m_right =m_right;
					}
					rbnode	*y = m_right->m_left;
					m_right->m_left = this;
					m_right->m_parent = parent;
					m_right = y;
					if (m_right)
						m_right->m_parent = this;
					return m_parent;
				}
				rbnode		*rotate_right(void) {
					rbnode	*parent = m_parent;
					m_parent = m_left;
					if (parent) {
						if (parent->m_left == this)
							parent->m_left = m_left;
						else parent->m_right = m_left;
					}
					rbnode	*y = m_left->m_right;
					m_left->m_right = this;
					m_left->m_parent = parent;
					m_left = y;
					if (m_left)
						m_left->m_parent = this;
					return m_parent;
				}
				rbnode		*insert_rotation(rbnode *child) {
					if (m_left != child && m_right != child)
						throw std::invalid_argument("rbnode::insert_rotation");
					if (!m_parent)
						throw std::invalid_argument("rbnode::insert_rotation");

					if (m_parent->m_left == this) {
						if (m_left == child)
							return m_parent->rotate_right();// LL case
						else
							return rotate_left()->m_parent->rotate_right();
					} else if (m_right == child)
						return m_parent->rotate_left();// RR case
					return rotate_right()->m_parent->rotate_left(); // RL case
				}

				rbnode		*recolor(void) {
					m_color = BLACK;
					if (m_left) m_left->m_color = RED;
					if (m_right) m_right->m_color = RED;
					return this;
				}

				rbnode		*inorder_successor(void) {
					rbnode	*s = m_right;
					if (!s) return NULL;
					while (s->m_left) s = s->m_left;
					return s;
				}
				rbnode		*inorder_predecessor(void) {
					rbnode	*s = m_left;
					if (!s) return NULL;
					while (s->m_right) s = s->m_right;
					return s;
				}
				void		swap_color(rbnode *other) {
					std::swap(other->m_color, m_color);
				}
				void		set_color(enum color color) { m_color = color; }
				rbnode		*add_child(
					const value_type &data,
					const value_compare &cmp,
					void *hint = NULL
				) {
					bool	is_less = cmp(data, *m_data);

					if (is_less) {
						if (m_left) throw std::invalid_argument(
								"rbnode::add_child"
							);
					} else if (cmp(*m_data, data)) {
						if (m_right) 
							throw std::invalid_argument(
								"rbnode::add_child"
							);
					} else throw EDuplicate();

					rbnode	*child = new rbnode(RED, data, this, hint);
					if (is_less) m_left = child;
					else m_right = child;
					return (child);
				}
				void		swap(rbnode *other) { std::swap(*m_data, *other->m_data); }

				struct EDuplicate : public std::exception {
					const char	*what(void) const throw () {
						return "rbnode::EDuplicate";
					}
				};
			private:
				Allocator		m_alloc;
				enum color		m_color;
				rbnode			*m_parent;
				rbnode			*m_left;
				rbnode			*m_right;
				value_type		*m_data;
		};

		class const_iterator : public std::iterator <
			std::random_access_iterator_tag,
			value_type,
			difference_type,
			const value_type*,
			const value_type&
		> {
			public:
				const_iterator(rbnode *from = NULL, rbnode *root = NULL) : m_def(), m_root(root), m_node(from) { }
				const_iterator(const const_iterator &other) : m_def(), m_root(other.m_root), m_node(other.m_node) { }

				const_iterator		&operator =(const const_iterator &other) {
					m_root = other.m_root;
					m_node = other.m_node;
					return *this;
				}
				bool				operator ==(const const_iterator &other) const
				{ return m_node == other.m_node; }
				bool				operator !=(const const_iterator &other) const
				{ return m_node != other.m_node; }

				virtual const value_type	&operator *(void) const {
					if (m_node)
						return m_node->data();
					return m_def;
				}
				virtual const value_type	*operator ->(void) const {
					if (m_node)
						return m_node->addr();
					return &m_def;
				}
				const_iterator		&operator ++(void) {
					if (m_node) {
						if (m_node->right()) {
							m_node = m_node->right();
							while (m_node->left()) m_node = m_node->left();
						} else {
							rbnode	*y = m_node->parent();
							while (y && m_node == y->right()) {
								m_node = y;
								y = y->parent();
							}
							if (!y) {
								m_root = m_node;
								m_node = NULL;
							} else if (m_node->right() != y)
								m_node = y;
						}
					} else if ((m_node = m_root))
						while (m_node->left()) m_node = m_node->left();
					return *this;
				}
				const_iterator		operator ++(int) {
					const_iterator	backup(*this);
					operator ++();
					return backup;
				}
				const_iterator		&operator --(void) {
					if (m_node) {
						if (m_node->left()) {
							m_node = m_node->left();
							while (m_node->right()) m_node = m_node->right();
						} else {
							rbnode	*y = m_node->parent();
							while (y && m_node == y->left()) {
								m_node = y;
								y = y->parent();
							}
							if (!y) {
								m_root = m_node;
								m_node = NULL;
							}
							else if (m_node->left() != y)
								m_node = y;
						}
					} else if ((m_node = m_root))
						while (m_node->right()) m_node = m_node->right();
					return *this;
				}
				const_iterator		operator --(int) {
					const_iterator	backup(*this);
					operator--();
					return backup;
				}

			protected:
				value_type	m_def;
				rbnode		*m_root;
				rbnode		*m_node;
		};
		class iterator : public const_iterator {
			public:
				typedef value_type*	pointer;
				typedef value_type& reference;

				iterator(rbnode *from = NULL, rbnode *root = NULL) : const_iterator(from, root) { }
				iterator(const iterator &other) : const_iterator(other) { }
				//iterator(const const_iterator &other) : const_iterator(other) { }
				iterator	&operator =(const iterator &other) {
					const_iterator::m_root = other.const_iterator::m_root;
					const_iterator::m_node = other.const_iterator::m_node;
					return *this;
				}
				value_type			&operator *(void) { return const_iterator::m_node->data(); }
				value_type			*operator ->(void) { return const_iterator::m_node->addr(); }
				const value_type	&operator *(void) const { return const_iterator::m_node->data(); }
				const value_type	*operator ->(void) const { return const_iterator::m_node->addr(); }
				iterator	&operator ++(void) {
					const_iterator::operator ++();
					return *this;
				}
				iterator	operator ++(int) {
					iterator	backup(*this);
					const_iterator::operator ++();
					return backup;
				}
				iterator	&operator --(void) {
					const_iterator::operator --();
					return *this;
				}
				iterator	operator --(int) {
					iterator	backup(*this);
					const_iterator::operator --();
					return backup;
				}
		};
		typedef ft::reverse_iterator< iterator >		reverse_iterator;
		typedef ft::reverse_iterator< const_iterator >	const_reverse_iterator;

		rbtree(
			const value_compare &comp,
			const Allocator &alloc = Allocator()
		) :	m_alloc(alloc), m_comp(comp),
			m_size(0), m_root(NULL),
			m_begin(NULL), m_rbegin(NULL) { }
		rbtree(const rbtree &other)
		:	m_alloc(other.m_alloc), m_comp(other.m_comp),
			m_size(other.m_size), m_root(NULL),
			m_begin(NULL), m_rbegin(NULL) {
			m_preorder_copy(m_root, other.m_root);
			if ((m_begin = m_rbegin = m_root)) {
				while (m_begin->left()) m_begin = m_begin->left();
				while (m_rbegin->right()) m_rbegin = m_rbegin->right();
			}
		}
		template <
			class InputIt
		> rbtree(
			InputIt first, InputIt last,
			const value_compare &comp,
			const Allocator &alloc = Allocator()
		) : m_alloc(alloc), m_comp(comp), m_size(0), m_root(NULL)
		{ while (first != last) insert(*first++); }
		~rbtree() { clear(); }

		rbtree	&operator =(const rbtree &other) {
			clear();
			m_size = other.m_size;
			m_preorder_copy(m_root, other.m_root);
			m_begin = m_rbegin = m_root;
			while (m_begin->left()) m_begin = m_begin->left();
			while (m_rbegin->right()) m_rbegin = m_rbegin->right();
			return *this;
		}
		const rbnode	*root(void) const { return m_root; }

		size_type		size(void) const { return m_size; }
		size_type		max_size(void) const {
			return (
				std::numeric_limits< difference_type >::max() /
				(sizeof(rbnode) + sizeof(value_type))
				- sizeof(rbtree)
			);
		}

		void			clear(void) {
			m_clear_subtree(m_root);
			m_size = 0;
			m_root = m_begin = m_rbegin = NULL;
		}

		value_type		&find(const value_type &value) {
			rbnode	*node = m_root;

			if (!node) throw std::out_of_range("rbtree::find");
			while (node) {
				if (m_comp(value, node->data())) {
					node = node->left();
				} else if (m_comp(node->data(), value)) {
					node = node->right();
				} else break ;
			}
			if (!node) throw std::out_of_range("rbtree::find");
			return node->data();
		}
		const value_type	&find(const value_type &value) const {
			const rbnode	*node = m_root;

			if (!node) throw std::out_of_range("rbtree::find");
			while (true) {
				if (m_comp(value, node->data())) {
					if (!node->left()) throw std::out_of_range("rbtree::find");
					node = node->left();
				} else if (m_comp(node->data(), value)) {
					if (!node->right()) throw std::out_of_range("rbtree::find");
					node = node->right();
				} else break ;
			}
			return node->data();
		}
		iterator		find_iterator(const value_type &value) {
			rbnode	*node = m_root;

			if (!node) throw std::out_of_range("rbtree::find_iterator");
			while (true) {
				if (m_comp(value, node->data())) {
					if (!node->left())
						throw std::out_of_range("rbtree::find_iterator");
					node = node->left();
				} else if (m_comp(node->data(), value)) {
					if (!node->right())
						throw std::out_of_range("rbtree::find_iterator");
					node = node->right();
				} else break ;
			}
			return iterator(const_cast< rbnode* >(node));
		}
		const_iterator		find_iterator(const value_type &value) const {
			const rbnode	*node = m_root;

			if (!node) throw std::out_of_range("rbtree::find_iterator");
			while (true) {
				if (m_comp(value, node->data())) {
					if (!node->left())
						throw std::out_of_range("rbtree::find_iterator");
					node = node->left();
				} else if (m_comp(node->data(), value)) {
					if (!node->right())
						throw std::out_of_range("rbtree::find_iterator");
					node = node->right();
				} else break ;
			}
			return const_iterator(const_cast< rbnode* >(node));
		}

		ft::pair<
			iterator,
			bool
		>				insert(const value_type &value, void *hint = NULL) {
			rbnode	*new_node;
	
			if (!m_root)
				new_node	= m_begin = m_rbegin
							= m_root = new rbnode(BLACK, value, NULL, hint);
			else {
				rbnode	*parent = m_root;
				rbnode	*child;

				while (true) {
					if (!m_comp(value, parent->data()) && !m_comp(parent->data(), value)) {
						return ft::make_pair(const_cast< rbnode* >(parent), false);
					}
					if (m_comp(value, parent->data())) {
						if (parent->left()) parent = parent->left();
						else {
							new_node = parent->add_child(value, m_comp);
							break ;
						}
					} else if (parent->right()) parent = parent->right();
					else {
						new_node = parent->add_child(value, m_comp);
						break ;
					}
				}
				child = new_node;
				if (!m_comp(m_begin->data(), child->data())) m_begin = child;
				else if (m_comp(m_rbegin->data(), child->data())) m_rbegin = child;

color_check:	if (child->color() == RED && parent->color() == RED) {
					if (
						parent->sibling()
						&& parent->sibling()->color() == RED
					) {
						parent->sibling()->set_color(BLACK);
						parent->set_color(BLACK);
						if (parent->parent() && parent->parent()->parent()) {
							parent->parent()->set_color(RED);
							child = parent->parent();
							parent = child->parent();
							goto color_check;
						}
					} else {
						parent->insert_rotation(child)->recolor();
					}
				}
			}
			while (m_root->parent()) m_root = m_root->parent();
			++m_size;
			return ft::make_pair(iterator(const_cast< rbnode* >(new_node)), true);
		}

#define END_NODE reinterpret_cast< rbnode* >(-1)
		void			remove(const value_type &value) {
			if (!m_root) throw std::invalid_argument("rbtree::remove");
			rbnode	*node = m_root;

			while (true) {
				if (m_comp(value, node->data())) {
					if (node->left()) node = node->left();
					else throw std::invalid_argument("rbtree::remove");
				} else if (m_comp(node->data(), value)) {
					if (node->right()) node = node->right();
					else throw std::invalid_argument("rbtree::remove");
				} else {
					rbnode	*pre, *suc;
					pre = node->inorder_predecessor();
					suc = node->inorder_successor();

					while (pre || suc) {
						if (!pre || (suc && (suc->left() || suc->right()))) {
							std::swap(node->raddr(), suc->raddr());
							node = suc;
						} else {
							std::swap(node->raddr(), pre->raddr());
							node = pre;
						}
						pre = node->inorder_predecessor();
						suc = node->inorder_successor();
					}
					rbnode		*sib = node->sibling();
					rbnode		*par = node->parent();
					enum color	col = node->color();
					delete node;

					if (!--m_size) {
						m_root = m_begin = m_rbegin = NULL;
						return ;
					}

					if (col == BLACK)
						node = NULL;
					else node = END_NODE;
					while (node != END_NODE) {
						if (node == m_root) node = END_NODE;
						else {
							if (sib->color() == BLACK) {
								if ((
										!sib->left()
										|| sib->left()->color() == BLACK
									) && (
										!sib->right()
										|| sib->right()->color() == BLACK
								)) {
									sib->set_color(RED);
									if (par->color() == RED) {
										par->set_color(BLACK);
										node = END_NODE;
									} else {
										node = par;
										par = par->parent();
										sib = node->sibling();
									}
								} else {
									if (par->right() == node) {
										if ((
												!sib->left()
												|| sib->left()->color() == BLACK
											) && sib->right()
											&& sib->right()->color() == RED
										) {
											sib->right()->set_color(BLACK);
											sib->set_color(RED);
											sib->rotate_left();
											sib = sib->parent();
										} else {
											sib->swap_color(par);
											par->rotate_right();
											sib->left()->set_color(BLACK);
											node = END_NODE;
										}
											
									} else if ((
											!sib->right()
											|| sib->right()->color() == BLACK
										) && sib->left()
										&& sib->left()->color() == RED
									) {
										sib->left()->set_color(BLACK);
										sib->set_color(RED);
										sib->rotate_right();
										sib = sib->parent();
									} else {
										sib->swap_color(par);
										par->rotate_left();
										sib->right()->set_color(BLACK);
										node = END_NODE;
									}
								}
							} else {
								sib->swap_color(par);
								if (par->left() == node) {
									par->rotate_left();
									sib = par->right();
								} else {
									par->rotate_right();
									sib = par->left();
								}
							}
						}
					}
					break ;
				}
			}
			while (m_root->parent()) m_root = m_root->parent();
			m_begin = m_rbegin = m_root;
			while (m_begin->left()) m_begin = m_begin->left();
			while (m_rbegin->right()) m_rbegin = m_rbegin->right();
		}
#undef END_NODE

		void					remove(const_iterator first, const_iterator last) {
			rbnode				*pos = m_root;
			rbnode				*last_node = NULL;
			const value_type	*last_value = (last != end() ? &*last : NULL);

			if (last_value) {
				pos = m_root;
				while (pos)
					if (m_comp(pos->data(), *last_value))
						pos = pos->right();
					else if (m_comp(*last_value, pos->data()))
						pos = pos->left();
					else break ;
				if (!pos) throw std::invalid_argument("rbtree::remove");
				last_node = pos;
			}
			while (first != last) {
				const value_type	*next_value;
				bool				is_end = false;

				if (++first == end())
					is_end = true;
				else next_value = &*first;
				remove(*--first);
				if (is_end)
					first = last = end();
				else {
					pos = m_root;
					while (pos)
						if (m_comp(pos->data(), *next_value))
							pos = pos->right();
						else if (m_comp(*next_value, pos->data()))
							pos = pos->left();
						else break ;
					if (!pos) throw std::invalid_argument("rbtree::remove");
					first = iterator(const_cast< rbnode* >(pos));
				}
			}
		}
		iterator				lower_bound(const value_type &value) {
			rbnode	*prev = NULL;
			rbnode	*ptr = m_root;

			while (ptr) {
				if (m_comp(value, ptr->data())) {
					prev = ptr;
					ptr = ptr->left();
				} else if (m_comp(ptr->data(), value)) {
					ptr = ptr->right();
				} else
					return iterator(ptr);
			}
			if (prev)
				return iterator(prev);
			return end();
		}
		const_iterator			lower_bound(const value_type &value) const {
			rbnode	*prev = NULL;
			rbnode	*ptr = const_cast< rbnode* >(m_root);

			while (ptr) {
				if (m_comp(value, ptr->data())) {
					prev = ptr;
					ptr = ptr->left();
				} else if (m_comp(ptr->data(), value)) {
					ptr = ptr->right();
				} else
					return const_iterator(ptr);
			}
			if (prev)
				return const_iterator(prev);
			return end();
		}
		iterator				upper_bound(const value_type &value) {
			rbnode	*prev = NULL;
			rbnode	*ptr = m_root;

			while (ptr) {
				if (m_comp(value, ptr->data())) {
					prev = ptr;
					ptr = ptr->left();
				} else ptr = ptr->right();
			}
			if (prev)
				return iterator(prev);
			return end();
		}
		const_iterator			upper_bound(const value_type &value) const {
			rbnode	*prev = NULL;
			rbnode	*ptr = m_root;

			while (ptr) {
				if (m_comp(value, ptr->data())) {
					prev = ptr;
					ptr = ptr->left();
				} else ptr = ptr->right();
			}
			if (prev)
				return const_iterator(prev);
			return end();
		}

		iterator				begin(void) {
			return iterator(const_cast< rbnode* >(m_begin));
		}
		const_iterator			begin(void) const {
			return const_iterator(const_cast< rbnode* >(m_begin));
		}
		iterator				end(void) { return iterator(NULL, const_cast< rbnode* >(m_root)); }
		const_iterator			end(void) const { return const_iterator(NULL, const_cast< rbnode* >(m_root)); }
		reverse_iterator		rbegin(void) { return reverse_iterator(end()); }
		const_reverse_iterator	rbegin(void) const {
			return const_reverse_iterator(end());
		}
		reverse_iterator		rend(void) { return reverse_iterator(begin()); }
		const_reverse_iterator	rend(void) const { return const_reverse_iterator(begin()); }

		void					generate_dot(const char *name) const {
			std::ofstream	out(name, std::ios::trunc);

			out << "digraph rbtree {" << std::endl;
			m_inorder_dot(m_root, out);
			out << "}" << std::endl;
		}

		void					swap(rbtree &other) {
			std::swap(m_alloc, other.m_alloc);
			std::swap(m_comp, other.m_comp);
			std::swap(m_size, other.m_size);
			std::swap(m_root, other.m_root);
			std::swap(m_begin, other.m_begin);
			std::swap(m_rbegin, other.m_rbegin);
		}
	private:

		rbnode	*m_preorder_copy(rbnode *&dst, rbnode *src, rbnode *parent = NULL) {
			if (src) {
				dst = new rbnode(src->color(), src->data(), parent);
				if (parent) {
					if (src->parent()->left() == src) parent->rleft() = dst;
					else parent->rright() = dst;
				}
				m_preorder_copy(dst->rleft(), src->left(), dst);
				m_preorder_copy(dst->rright(), src->right(), dst);
			}
			return dst;
		}
#define NODE_COLOR(node) ((node)->color() == RED ? "red" : "black")
		void		m_inorder_dot(rbnode *node, std::ofstream &out) const {
			if (node->left()) m_inorder_dot(node->left(), out);

			if (node->left())
				out << '\t' <<
					node->left()->data().first << " [ color = " <<
					NODE_COLOR(node->left()) << " ]" <<
				std::endl << '\t' <<
					node->data().first << " -> " <<
					node->left()->data().first << " [ color = blue ]" <<
				std::endl;
			if (node->right())
				out << '\t' <<
					node->right()->data().first << " [ color = " <<
					NODE_COLOR(node->right()) << " ]" <<
				std::endl << '\t' <<
					node->data().first << " -> " <<
					node->right()->data().first << " [ color = red ]" <<
				std::endl;
			if (node->right()) m_inorder_dot(node->right(), out);
		}
#undef NODE_COLOR

		void		m_clear_subtree(rbnode *start) {
			if (start) {
				m_clear_subtree(start->left()), m_clear_subtree(start->right());
				delete start;
			}
		}

		Allocator			m_alloc;
		value_compare		m_comp;
		size_type			m_size;
		rbnode				*m_root;
		rbnode				*m_begin;
		rbnode				*m_rbegin;
};
