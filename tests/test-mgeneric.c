/*
 * Copyright (c) 2017-2023, Patrick Pelissier
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * + Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * + Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdbool.h>
#include <assert.h>
#include "coverage.h"

#include "m-string.h"
#include "m-generic.h"

// Generic is not supported if not C11
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

static void h(string_t x)
{
	string_set_str(x, "TEST STRING");
}

const string_t gx;


/*
	init(x)
	init_set(x,y)
	set(x,y)
	clear(x)
	init_move(x,y)
	init_with(x, ...)
	swap(x,y)
	reset(x)
	bool empty_p(x)
	size_t get_size(x)
	size_t hash(x)
	bool equal(x, y)
	int cmp(x,y)
	add(x, y)
	sub(x, y)
	mul(x, y)
	div(x, y)
	T* get(x, key)
	set(x, key, value)
	T *safe_get(x, key)
	erase(x, key)
	push(x, y)
	pop(x, y)
	...
	pas de pb

	pb for iterator through "each":
	FIXME: uses of C23 typeof + extensions ?
 */

#define STR1 M_OPEXTEND(STRING_OPLIST, GENTYPE(struct m_string_s *), PUSH(m_string_push_u))
#define FLT1 (GENTYPE(float), TYPE(float), INIT(M_INIT_BASIC), INIT_SET(M_SET_BASIC), SET(M_SET_BASIC),               \
   CLEAR(M_NOTHING_DEFAULT) )

//#define M_GENERIC_ORG_MLIB_COMP_1() (CORE)
//#define M_GENERIC_ORG_MLIB_COMP_CORE_OPLIST_1() STR1
//#define M_GENERIC_ORG_MLIB_COMP_CORE_OPLIST_5() FLT1

#define M_GENERIC_ORG_2() (USER)
#define M_GENERIC_ORG_USER_COMP_1() (CORE)
#define M_GENERIC_ORG_USER_COMP_CORE_OPLIST_6() FLT1
#define M_GENERIC_ORG_USER_COMP_CORE_OPLIST_7() STR1

// typeof(_Generic(...) ) ?
// GCC/CLANG/TCC: __extension__ __typeof__
// MSVC++: decltype
// C23; typeof

static bool test_empty(const string_t p)
{
  return empty_p(p);
}

static void test_string(string_t p)
{
  string_t s , d;

  init(s);
  h(s);
  init_set(d, s);
  h(d);
  init_set(p, s);
  bool b = test_empty(p);
  assert(!b);
  push(p, 'c');
  out_str(stdout, p);
  clear(s);
  clear(d);

  float f;
  init(f);
  clear(f);

  clear(p);
}

int main(void)
{
	string_t p;
	test_string(p);

	exit(0);
}

#else
int main(void)
{
  exit(0);
}
#endif
