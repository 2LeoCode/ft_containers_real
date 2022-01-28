/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rbtree.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crochu <crochu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 23:43:13 by crochu            #+#    #+#             */
/*   Updated: 2021/12/30 01:01:23 by crochu           ###   ########.fr       */
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
	class Key,
	class T,
	class value_compare,
	class Allocator = std::allocator< ft::pair< const Key, T > >,
	typename size_type = std::size_t,
	typename difference_type = std::ptrdiff_t
> class rbtree {
	public:
		typedef ft::pair< const Key, T >	value_type;
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
				) :	_allocator(Allocator()),
					_color(color),
					_parent(parent),
					_left(NULL),
					_right(NULL),
					_data(_allocator.allocate(1, hint)) {					
					_allocator.construct(_data, data);
				}
				~rbnode() {
					if (_parent) {
						if (_parent->_right == this) _parent->_right = NULL;
						else if (_parent->_left == this) _parent->_left = NULL;
					}
					_allocator.destroy(_data);
					_allocator.deallocate(_data, 1);
				}

				//rbnode	&operator =(const rbnode &other) {
				//	_data = other._data;
				//	return *this;
				//}
				bool	operator ==(const rbnode &other) const {
					return *_data == *other._data;
				}
				bool	operator !=(const rbnode &other) const {
					return !operator ==(other);
				}

				value_type			&data(void) { return *_data; }
				const value_type	&data(void) const { return *_data; }
				value_type			*addr(void) { return _data; }
				const value_type	*addr(void) const { return _data; }
				value_type			*&raddr(void) { return _data; }
				const value_type	*&raddr(void) const { return _data; }
				enum color			color(void) const { return _color; }
				rbnode				*parent(void) { return _parent; }
				rbnode				*left(void) { return _left; }
				const rbnode		*left(void) const { return _left; }
				rbnode				*&rleft(void) { return _left; }
				rbnode				*right(void) { return _right; }
				const rbnode		*right(void) const { return _right; }
				rbnode				*&rright(void) { return _right; }
				rbnode				*sibling(void) {
					if (_parent->_left == this)
						return _parent->_right;
					return _parent->_left;
				}

				rbnode		*rotate_left(void) {
					rbnode	*parent = _parent;
					_parent = _right;
					if (parent) {
						if (parent->_left == this)
							parent->_left = _right;
						else parent->_right =_right;
					}
					rbnode	*y = _right->_left;
					_right->_left = this;
					_right->_parent = parent;
					_right = y;
					if (_right)
						_right->_parent = this;
					return _parent;
				}
				rbnode		*rotate_right(void) {
					rbnode	*parent = _parent;
					_parent = _left;
					if (parent) {
						if (parent->_left == this)
							parent->_left = _left;
						else parent->_right = _left;
					}
					rbnode	*y = _left->_right;
					_left->_right = this;
					_left->_parent = parent;
					_left = y;
					if (_left)
						_left->_parent = this;
					return _parent;
				}
				rbnode		*insert_rotation(rbnode *child) {
					if (_left != child && _right != child)
						throw std::invalid_argument("invalid child node");
					if (!_parent)
						throw std::invalid_argument("can't rotate root node");

					if (_parent->_left == this) {
						if (_left == child)
							return _parent->rotate_right();// LL case
						else
							return rotate_left()->_parent->rotate_right();
					} else if (_right == child)
						return _parent->rotate_left();// RR case
					return rotate_right()->_parent->rotate_left(); // RL case
				}

				rbnode		*recolor(void) {
					_color = BLACK;
					if (_left) _left->_color = RED;
					if (_right) _right->_color = RED;
					return this;
				}

				rbnode		*inorder_successor(void) {
					rbnode	*s = _right;
					if (!s) return NULL;
					while (s->_left) s = s->_left;
					return s;
				}
				rbnode		*inorder_predecessor(void) {
					rbnode	*s = _left;
					if (!s) return NULL;
					while (s->_right) s = s->_right;
					return s;
				}
				void		swap_color(rbnode *other) {
					std::swap(other->_color, _color);
				}
				void		set_color(enum color color) { _color = color; }
				rbnode		*add_child(
					const value_type &data,
					const value_compare &cmp,
					void *hint = NULL
				) {
					bool	is_less = cmp(data, *_data);

					if (data.first == _data->first) throw EDuplicate();
					else if (is_less) {
						if (_left) throw std::invalid_argument(
								"node already has a left child"
							);
					} else if (_right) throw std::invalid_argument(
								"node already has a right child"
							);

					rbnode	*child = new rbnode(RED, data, this, hint);
					if (is_less) _left = child;
					else _right = child;
					return (child);
				}
				void		swap(rbnode *other) { std::swap(*_data, *other->_data); }

				struct EDuplicate {
					const char	*what(void) const throw () {
						return "duplicate node";
					}
				};
			private:
				Allocator		_allocator;
				enum color		_color;
				rbnode			*_parent;
				rbnode			*_left;
				rbnode			*_right;
				value_type		*_data;
		};

		class const_iterator : public std::iterator <
			std::random_access_iterator_tag,
			value_type
		> {
			public:
				const_iterator(rbnode **r = NULL) : root(*r), node(NULL) { }
				const_iterator(const rbnode *from, rbnode *&r) : root(r), node(const_cast< rbnode * >(from)) { }
				const_iterator(const const_iterator &other) : root(other.root), node(other.node) { }

				const_iterator		&operator =(const const_iterator &other) {
					root = other.root;
					node = other.node;
					return *this;
				}
				bool				operator ==(const const_iterator &other) const {
					return node == other.node;
				}
				bool				operator !=(const const_iterator &other) const {
					return node != other.node;
				}

				const value_type	&operator *(void) const { return node->data(); }
				const value_type	*operator ->(void) const { return node->addr(); }
				const_iterator		&operator++(void) {
					if (node) {
						if (node->right()) {
							node = node->right();
							while (node->left()) node = node->left();
						} else {
							rbnode	*y = node->parent();
							while (y && node == y->right()) {
								node = y;
								y = y->parent();
							}
							if (!y) node = NULL;
							else if (node->right() != y)
								node = y;
						}
					} else {
						if ((node = root))
							while (node->left()) node = node->left();
					}
					return *this;
				}
				const_iterator		operator++(int) {
					const_iterator	backup(node, root);
					operator ++();
					return backup;
				}
				const_iterator		&operator--(void) {
					if (node) {
						if (node->left()) {
							node = node->left();
							while (node->right()) node = node->right();
						} else {
							rbnode	*y = node->parent();
							while (y && node == y->left()) {
								node = y;
								y = y->parent();
							}
							if (!y) node = NULL;
							else if (node->left() != y)
								node = y;
						}
					} else {
						if ((node = root))
							while (node->right()) node = node->right();
					}
					return *this;
				}
				const_iterator		operator--(int) {
					const_iterator	backup(node, root);
					operator--();
					return backup;
				}

			protected:
				rbnode	*&root;
				rbnode	*node;
		};
		class iterator : public const_iterator {
			public:
				iterator(rbnode **root = NULL) : const_iterator(root) { }
				iterator(rbnode *from, rbnode *&root) : const_iterator(from, root) { }
				iterator(const iterator &other) : const_iterator(other) { }
				iterator	&operator =(const iterator &other) {
					const_iterator::root = other.const_iterator::root;
					const_iterator::node = other.const_iterator::node;
					return *this;
				}
				value_type	&operator *(void) { return const_iterator::node->data(); }
				value_type	*operator ->(void) { return const_iterator::node->addr(); }
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
		) :	_allocator(alloc), _comp(comp),
			_size(0), _root(NULL),
			_begin(NULL), _rbegin(NULL) { }
		rbtree(const rbtree &other)
		:	_allocator(other._allocator), _comp(other._comp),
			_size(other._size), _root(NULL),
			_begin(NULL), _rbegin(NULL) {
			_preorder_copy(_root, other._root);
			if ((_begin = _rbegin = _root)) {
				while (_begin->left()) _begin = _begin->left();
				while (_rbegin->right()) _rbegin = _rbegin->right();
			}
		}
		template <
			class InputIt
		> rbtree(
			InputIt first, InputIt last,
			const value_compare &comp,
			const Allocator &alloc = Allocator()
		) : _allocator(alloc), _comp(comp), _size(0), _root(NULL) {
			while (first != last) insert(*first++);
		}
		~rbtree() { clear(); }

		rbtree	&operator =(const rbtree &other) {
			clear();
			_size = other._size;
			_preorder_copy(_root, other._root);
			_begin = _rbegin = _root;
			while (_begin->left()) _begin = _begin->left();
			while (_rbegin->right()) _rbegin = _rbegin->right();
			return *this;
		}
		const rbnode	*root(void) const { return _root; }

		size_type		size(void) const { return _size; }
		size_type		max_size(void) const {
			return (
				std::numeric_limits< difference_type >::max() /
				(sizeof(rbnode) + sizeof(value_type))
			);
		}

		void			clear(void) {
			_clear_subtree(_root);
			_size = 0;
			_root = NULL;
		}

		template <
			class KeyCmp
		> T				&find(
			const Key &key,
			const KeyCmp &cmp = std::less< Key >()
		) {
			rbnode	*node = _root;

			if (!node) throw std::out_of_range("rbtree::find");
			while (node) {
				if (cmp(key, node->data().first)) {
					node = node->left();
				} else if (key != node->data().first) {
					node = node->right();
				} else break ;
			}
			if (!node) throw std::out_of_range("rbtree::find");
			return node->data().second;
		}
		template <
			class KeyCmp
		> const T			&find(
			const Key &key,
			const KeyCmp &cmp = std::less< Key >()
		) const {
			const rbnode	*node = _root;

			if (!node) throw std::out_of_range("tree is empty");
			while (true) {
				if (cmp(key, node->data().first)) {
					if (!node->left()) throw std::out_of_range("out of range");
					node = node->left();
				} else if (key != node->data().first) {
					if (!node->right()) throw std::out_of_range("out of range");
					node = node->right();
				} else break ;
			}
			return node->data().second;
		}
		template <
			class KeyCmp
		> iterator		find_iterator(
			const Key &key,
			const KeyCmp &cmp = std::less< Key >()
		) {
			rbnode	*node = _root;

			if (!node) throw std::out_of_range("tree is empty");
			while (true) {
				if (cmp(key, node->data().first)) {
					if (!node->left()) throw std::out_of_range("out of range");
					node = node->left();
				} else if (key != node->data().first) {
					if (!node->right()) throw std::out_of_range("out of range");
					node = node->right();
				} else break ;
			}
			return iterator(node, _root);
		}
		template <
			class KeyCmp
		> const_iterator		find_iterator(
			const Key &key,
			const KeyCmp &cmp = std::less< Key >()
		) const {
			const rbnode	*node = _root;

			if (!node) throw std::out_of_range("tree is empty");
			while (true) {
				if (cmp(key, node->data().first)) {
					if (!node->left()) throw std::out_of_range("out of range");
					node = node->left();
				} else if (key != node->data().first) {
					if (!node->right()) throw std::out_of_range("out of range");
					node = node->right();
				} else break ;
			}
			return const_iterator(node, _root);
		}

		ft::pair<
			iterator,
			bool
		>				insert(const value_type &value, void *hint = NULL) {
			rbnode	*new_node;
	
			if (!_root)
				new_node	= _begin = _rbegin
							= _root = new rbnode(BLACK, value, NULL, hint);
			else {
				rbnode	*parent = _root;
				rbnode	*child;

				while (true) {
					if (value.first == parent->data().first) {
						return ft::make_pair(iterator(parent, _root), false);
					}
					if (_comp(value, parent->data())) {
						if (parent->left()) parent = parent->left();
						else {
							new_node = parent->add_child(value, _comp);
							break ;
						}
					} else if (parent->right()) parent = parent->right();
					else {
						new_node = parent->add_child(value, _comp);
						break ;
					}
				}
				child = new_node;
				if (!_comp(_begin->data(), child->data())) _begin = child;
				else if (_comp(_rbegin->data(), child->data())) _rbegin = child;

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
			while (_root->parent()) _root = _root->parent();
			++_size;
			return ft::make_pair(iterator(new_node, _root), true);
		}

#define END_NODE reinterpret_cast< rbnode* >(-1)
		template <
			class KeyCmp
		> void			remove(
			const Key &key,
			const KeyCmp &cmp = std::less< Key >()
		) {
			if (!_root) throw std::invalid_argument("tree is empty");
			rbnode	*node = _root;

			while (true) {
				if (cmp(key, node->data().first)) {
					if (node->left()) node = node->left();
					else throw std::invalid_argument("value not found");
				} else if (key != node->data().first) {
					if (node->right()) node = node->right();
					else throw std::invalid_argument("value not found");
				} else {
					rbnode	*pre, *suc;
					pre = node->inorder_predecessor();
					suc = node->inorder_successor();

					if (key == _begin->data().first && !suc) _begin = _begin->parent();
					if (key == _rbegin->data().first && !pre) _rbegin = _rbegin->parent();
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

					if (!--_size) {
						_root = _begin = _rbegin = NULL;
						return ;
					}

					if (col == BLACK)
						node = NULL;
					else node = END_NODE;
					while (node != END_NODE) {
						if (node == _root) node = END_NODE;
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
			while (_root->parent()) _root = _root->parent();
		}
#undef END_NODE

		iterator				begin(void) { return iterator(_begin, _root); }
		const_iterator			begin(void) const {
			return const_iterator(_begin, _root);
		}
		iterator				end(void) { return iterator(&_root); }
		const_iterator			end(void) const { return const_iterator(&_root); }
		reverse_iterator		rbegin(void) { return reverse_iterator(end()); }
		const_reverse_iterator	rbegin(void) const {
			return const_reverse_iterator(end());
		}
		reverse_iterator		rend(void) { return reverse_iterator(begin()); }
		const_reverse_iterator	rend(void) const { return const_reverse_iterator(begin()); }

		void					generate_dot(const char *name) const {
			std::ofstream	out(name, std::ios::trunc);

			out << "digraph rbtree {" << std::endl;
			_inorder_dot(_root, out);
			out << "}" << std::endl;
		}

		void					swap(rbtree &other) {
			std::swap(_allocator, other._allocator);
			std::swap(_comp, other._comp);
			std::swap(_size, other._size);
			std::swap(_root, other._root);
			std::swap(_begin, other._begin);
			std::swap(_rbegin, other._rbegin);
		}
	private:

		rbnode	*_preorder_copy(rbnode *&dst, rbnode *src, rbnode *parent = NULL) {
			if (src) {
				dst = new rbnode(src->color(), src->data(), parent);
				if (parent) {
					if (src->parent()->left() == src) parent->rleft() = dst;
					else parent->rright() = dst;
				}
				_preorder_copy(dst->rleft(), src->left(), dst);
				_preorder_copy(dst->rright(), src->right(), dst);
			}
			return dst;
		}
#define NODE_COLOR(node) (node->color() == RED ? "red" : "black")
		void		_inorder_dot(rbnode *node, std::ofstream &out) const {
			if (node->left()) _inorder_dot(node->left(), out);

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

			if (node->right()) _inorder_dot(node->right(), out);
		}
#undef NODE_COLOR

		void		_clear_subtree(rbnode *start) {
			if (start) {
				_clear_subtree(start->left()), _clear_subtree(start->right());
				delete start;
			}
		}

		Allocator			_allocator;
		value_compare		_comp;
		size_type			_size;
		rbnode				*_root;
		rbnode				*_begin;
		rbnode				*_rbegin;
};
