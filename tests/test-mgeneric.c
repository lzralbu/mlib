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
#include "m-array.h"
#include "m-generic.h"

// Generic is not supported if not C11
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

ARRAY_DEF(array_int, int)
#define M_OPL_array_int_t() ARRAY_OPLIST(array_int, M_BASIC_OPLIST)

static void h(string_t x)
{
	string_set_str(x, "TEST STRING");
}

const string_t gx;

#define STR1 M_OPEXTEND(STRING_OPLIST, GENTYPE(struct m_string_s *), PUSH(m_string_push_u))
#define FLT1 (GENTYPE(float), TYPE(float), INIT(M_INIT_BASIC), INIT_SET(M_SET_BASIC), SET(M_SET_BASIC),               \
   CLEAR(M_NOTHING_DEFAULT) )
#define ARRAY1 M_OPEXTEND(M_OPL_array_int_t(), GENTYPE(struct array_int_s *))

//#define M_GENERIC_ORG_MLIB_COMP_1() (CORE)
//#define M_GENERIC_ORG_MLIB_COMP_CORE_OPLIST_1() STR1
//#define M_GENERIC_ORG_MLIB_COMP_CORE_OPLIST_5() FLT1

#define M_GENERIC_ORG_2() (USER)
#define M_GENERIC_ORG_USER_COMP_1() (CORE)
#define M_GENERIC_ORG_USER_COMP_CORE_OPLIST_6() FLT1
#define M_GENERIC_ORG_USER_COMP_CORE_OPLIST_7() STR1

#define M_GENERIC_ORG_3() (ARRAY)
#define M_GENERIC_ORG_ARRAY_COMP_5() (INT)
#define M_GENERIC_ORG_ARRAY_COMP_INT_OPLIST_6() ARRAY1

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

static void test_array(void)
{
	M_LET(a, array_int_t) {
		push(a, 14);
		push(a, 15);
		out_str(stdout, a);
		for each(it, a) {
			printf("=%d, ", *it);
		}
 	}
}

int main(void)
{
	string_t p;
	test_string(p);
	test_array();
	exit(0);
}

#else
int main(void)
{
  exit(0);
}
#endif
