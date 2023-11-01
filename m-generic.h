/*
 * M*LIB - Generic module
 *
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
#ifndef MSTARLIB_GENERIC_H
#define MSTARLIB_GENERIC_H

#include "m-core.h"

/* 3 levels of indirection are neeeded:
    * One level is the organisation.
    * One level is the component of the organisation.
    * One level is the oplist within the component of the organisation
  Return the list of unevaled oplist that are registered accorss all organisations, all components.
  MLIB Organisation is the default organisation and is always registered
  Usage Domain: Nb organizations * Nb components * Nb oplists < M_MAX_NB_ARGUMENT
*/
#define M_G3N_REGISTERED_ITEMS()                                              \
  M_CROSS_MAP( M_G3N_IS_PRESENT3, ( comp M_CROSS_MAP(M_G3N_IS_PRESENT2, (MLIB M_MAP(M_G3N_IS_PRESENT, M_SEQ(1, 50))), ( M_SEQ(1, 50) ) ) ), ( M_SEQ(1, 50) ) )

#define M_G3N_IS_PRESENT(num)                                                 \
  M_IF(M_PARENTHESIS_P(M_C(M_GENERIC_ORG_, num)() ))(M_DEFERRED_COMMA M_ID M_C(M_GENERIC_ORG_, num)(), )

#define M_G3N_IS_PRESENT2(el1, num)                                           \
  M_IF(M_PARENTHESIS_P(M_C4(M_GENERIC_ORG_, el1, _COMP_, num)()))(M_DEFERRED_COMMA M_APPLY(M_C3, el1, _COMP_, M_ID M_C4(M_GENERIC_ORG_, el1, _COMP_, num)()), )

#define M_G3N_IS_PRESENT3(el1, num)                                           \
  M_IF(M_OPLIST_P(M_C4(M_GENERIC_ORG_, el1, _OPLIST_, num)()))(M_DEFERRED_COMMA M_C4(M_GENERIC_ORG_, el1, _OPLIST_, num), )

// Dummy unused structure. Just to fill in default case of the _Generic
struct m_g3neric_dummys;

// Call the OPERATOR call of the oplist registered to the variable 'x'
// which takes one argument.
#define M_G3N_CALL_1(op, x)                                                   \
  _Generic( ((void)0, (x)),                                                   \
            M_MAP2(M_G3N_CALL_1_func, (op, x) M_G3N_REGISTERED_ITEMS() )      \
            struct m_g3neric_dummys *: /* cannot happen */ (void)0)
#define M_G3N_CALL_1_func(x, oplist)                                          \
  M_G3N_CALL_1_func_test(M_GET_GENTYPE oplist(), M_PAIR_1 x, M_PAIR_2 x, oplist)
#define M_G3N_CALL_1_func_test(gentype, op, x, oplist)                        \
  M_IF_METHOD(op, oplist())(M_G3N_CALL_1_func_expand, M_EAT)(gentype, op, x, oplist)
#define M_G3N_CALL_1_func_expand(gentype, op, x, oplist)                      \
  gentype: M_C(M_CALL_, op)(oplist(), M_AS_TYPE(gentype, x)),                 \
  const   gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(const gentype, x, gentype, x)),

// Call the OPERATOR call of the oplist registered to the variable 'x'
// which takes two argument (type, type)
#define M_G3N_CALL_2(op, x, y)                                                \
  _Generic( ((void)0, (x)),                                                   \
            M_MAP2(M_G3N_CALL_2_func, (op, x, y) M_G3N_REGISTERED_ITEMS() )   \
            struct m_g3neric_dummys *: /* cannot happen */ (void) 0)
#define M_G3N_CALL_2_func(x, oplist)                                          \
  M_G3N_CALL_2_func_test(M_GET_GENTYPE oplist(), M_TRIPLE_1 x, M_TRIPLE_2 x, M_TRIPLE_3 x, oplist)
#define M_G3N_CALL_2_func_test(gentype, op, x, y, oplist)                     \
  M_IF_METHOD(op, oplist())(M_G3N_CALL_2_func_expand, M_EAT)(gentype, op, x, y, oplist)
#define M_G3N_CALL_2_func_expand(gentype, op, x, y, oplist)                   \
  gentype: M_C(M_CALL_, op)(oplist(), M_AS_TYPE(gentype, x), M_AS_LINKED_TYPE(gentype, x, gentype, y)), \
  const gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(const gentype, x, gentype, x), M_AS_LINKED_TYPE(const gentype, x, gentype, y)),

// Call the OPERATOR call of the oplist registered to the variable 'x'
// which takes two argument (type, subtype)
#define M_G3N_CALL_2t(op, x, y)                                               \
  _Generic( ((void)0, (x)),                                                   \
            M_MAP2(M_G3N_CALL_2t_func, (op, x, y) M_G3N_REGISTERED_ITEMS() )  \
            struct m_g3neric_dummys *: /* cannot happen */ (void) 0)
#define M_G3N_CALL_2t_func(x, oplist)                                         \
  M_G3N_CALL_2t_func_test(M_GET_GENTYPE oplist(),                             \
                           M_IF_METHOD(GENTYPE, M_GET_OPLIST oplist())(M_GET_GENTYPE M_GET_OPLIST oplist(), M_GET_SUBTYPE oplist()), \
                           M_TRIPLE_1 x, M_TRIPLE_2 x, M_TRIPLE_3 x, oplist)
#define M_G3N_CALL_2t_func_test(gentype, basictype, op, x, y, oplist)         \
  M_IF_METHOD(op, oplist())(M_G3N_CALL_2t_func_expand, M_EAT)(gentype, basictype, op, x, y, oplist)
#define M_G3N_CALL_2t_func_expand(gentype, basictype, op, x, y, oplist)       \
  gentype: M_C(M_CALL_, op)(oplist(), M_AS_TYPE(gentype, x), M_AS_LINKED_TYPE(gentype, x, basictype, y)), \
  const gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(const gentype, x, gentype, x), M_AS_LINKED_TYPE(const gentype, x, basictype, y)),

// Call the OPERATOR call of the oplist registered to the variable 'x'
// which takes two argument (subtype *, type)
#define M_G3N_CALL_t2(op, x, y)                                               \
  _Generic( ((void)0, (y)),                                                   \
            M_MAP2(M_G3N_CALL_t2_func, (op, x, y) M_G3N_REGISTERED_ITEMS() )  \
            struct m_g3neric_dummys *: /* cannot happen */ (void) 0)
#define M_G3N_CALL_t2_func(x, oplist)                                         \
  M_G3N_CALL_t2_func_test(M_GET_GENTYPE oplist(),                             \
                           M_IF_METHOD(GENTYPE, M_GET_OPLIST oplist())(M_GET_GENTYPE M_GET_OPLIST oplist(), M_GET_SUBTYPE oplist()), \
                           M_TRIPLE_1 x, M_TRIPLE_2 x, M_TRIPLE_3 x, oplist)
#define M_G3N_CALL_t2_func_test(gentype, basictype, op, x, y, oplist)         \
  M_IF_METHOD(op, oplist())(M_G3N_CALL_t2_func_expand, M_EAT)(gentype, basictype, op, x, y, oplist)
#define M_G3N_CALL_t2_func_expand(gentype, basictype, op, x, y, oplist)       \
  gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(gentype, y, basictype *, x), M_AS_LINKED_TYPE(gentype, y, gentype, y)), \
  const gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(gentype, y, basictype *, x), M_AS_LINKED_TYPE(const gentype, y, gentype, y)),

// Call the OPERATOR call of the oplist registered to the variable 'x'
// which takes two argument (type, a) typeof(a) is always the same for all types
#define M_G3N_CALL_21(op, x, y)                                               \
  _Generic( ((void)0, (x)),                                                   \
            M_MAP2(M_G3N_CALL_21_func, (op, x, y) M_G3N_REGISTERED_ITEMS() )  \
            struct m_g3neric_dummys *: /* cannot happen */ (void) 0)
#define M_G3N_CALL_21_func(x, oplist)                                         \
  M_G3N_CALL_21_func_test(M_GET_GENTYPE oplist(),                             \
                           M_TRIPLE_1 x, M_TRIPLE_2 x, M_TRIPLE_3 x, oplist)
#define M_G3N_CALL_21_func_test(gentype, op, x, y, oplist)         \
  M_IF_METHOD(op, oplist())(M_G3N_CALL_21_func_expand, M_EAT)(gentype, op, x, y, oplist)
#define M_G3N_CALL_21_func_expand(gentype, op, x, y, oplist)       \
  gentype: M_C(M_CALL_, op)(oplist(), M_AS_TYPE(gentype, x), y), \
  const gentype: M_C(M_CALL_, op)(oplist(), M_AS_TYPE(const gentype, x), y),

// Call the OPERATOR call of the oplist registered to the variable 'x'
// which takes two argument (a, type) typeof(a) is always the same for all types
#define M_G3N_CALL_22(op, x, y)                                               \
  _Generic( ((void)0, (y)),                                                   \
            M_MAP2(M_G3N_CALL_22_func, (op, x, y) M_G3N_REGISTERED_ITEMS() )  \
            struct m_g3neric_dummys *: /* cannot happen */ (void) 0)
#define M_G3N_CALL_22_func(x, oplist)                                         \
  M_G3N_CALL_22_func_test(M_GET_GENTYPE oplist(),                             \
                           M_TRIPLE_1 x, M_TRIPLE_2 x, M_TRIPLE_3 x, oplist)
#define M_G3N_CALL_22_func_test(gentype, op, x, y, oplist)         \
  M_IF_METHOD(op, oplist())(M_G3N_CALL_22_func_expand, M_EAT)(gentype, op, x, y, oplist)
#define M_G3N_CALL_22_func_expand(gentype, op, x, y, oplist)       \
  gentype: M_C(M_CALL_, op)(oplist(), x, M_AS_TYPE(gentype, y)), \
  const gentype: M_C(M_CALL_, op)(oplist(), x, M_AS_TYPE(const gentype, y)),

#define M_AS_LINKED_TYPE(typex, x, typey, y) _Generic(((void)0,(x)), typex: (y), default: (typey) {0})

// Define generic functions
// TODO: m_ prefix?
#define init(x)              M_G3N_CALL_1(INIT, x)
#define init_set(x, y)       M_G3N_CALL_2(INIT_SET, x, y)
#define init_move(x, y)      M_G3N_CALL_2(INIT_MOVE, x, y)
#define move(x, y)           M_G3N_CALL_2(MOVE, x, y)
#define set(x, y)            M_G3N_CALL_2(SET, x, y)
#define clear(x)             M_G3N_CALL_1(CLEAR, x)
#define swap(x, y)           M_G3N_CALL_2(SWAP, x, y)
#define hash(x)              M_G3N_CALL_1(HASH, x)
#define equal(x, y)          M_G3N_CALL_2(EQUAL, x, y)
#define cmp(x, y)            M_G3N_CALL_2(CMP, x, y)
#define sort(x)              M_G3N_CALL_1(SORT, x)
// splice_back
// splice_at
// it*
#define empty_p(x)           M_G3N_CALL_1(EMPTY_P, x)
#define add(x, y)            M_G3N_CALL_2(ADD, x, y)
#define sub(x, y)            M_G3N_CALL_2(SUB, x, y)
#define mul(x, y)            M_G3N_CALL_2(MUL, x, y)
#define div(x, y)            M_G3N_CALL_2(DIV, x, y)
#define reset(x)             M_G3N_CALL_1(RESET, x)
// get
// set_at
// safe_get
// erase
#define get_size(x)          M_G3N_CALL_1(GET_SIZE, x)
#define push(x, y)           M_G3N_CALL_2t(PUSH, x, y)
#define pop(x, y)            M_G3N_CALL_t2(PUSH, x, y)
#define push_move(x, y)      M_G3N_CALL_2t(PUSH_MOVE, x, y)
#define pop_move(x, y)       M_G3N_CALL_t2(PUSH, x, y)
#define reverse(x)           M_G3N_CALL_1(REVERSE, x)
//#define get_str()             
//#define parse_str()             
#define out_str(x, y)        M_G3N_CALL_22(OUT_STR, x, y)
#define in_str(x, y)         M_G3N_CALL_21(IN_STR, x, y)
#define out_serial(x, y)     M_G3N_CALL_22(OUT_SERIAL, x, y)
#define in_serial(x, y)      M_G3N_CALL_21(IN_SERIAL, x, y)

// TODO: init_with ? How to handle the different type of parameters ?
// TODO: emplace ?
// TODO: for each(x, cont) { }
// TODO: Integrate in M_PRINT ?

/* User code has to register oplists :

#define M_GENERIC_ORG_2() (USER)
#define M_GENERIC_ORG_USER_COMP_1() (CORE)
#define M_GENERIC_ORG_USER_COMP_CORE_OPLIST_6() FLT1
#define M_GENERIC_ORG_USER_COMP_CORE_OPLIST_7() STR1
*/

// Don't need to register M*LIB organisation as it is the default one
//#define M_GENERIC_ORG_1() (MLIB)

// TODO: Register CORE component
// TODO: Register oplist to CORE component (m-string ? serial ?)
// TODO: Add GENTYPE to all types
//#define M_GENERIC_ORG_MLIB_COMP_1() (CORE)
//#define M_GENERIC_ORG_MLIB_COMP_CORE_OPLIST_1() STR1
//#define M_GENERIC_ORG_MLIB_COMP_CORE_OPLIST_5() FLT1



#endif
