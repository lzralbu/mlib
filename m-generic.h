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

#ifdef _MSC_VER
#define m_typeof(x) decltype(x)
#else
// Definition valid for C23, GCC, CLANG, TCC, ...
#define m_typeof(x) typeof(x)
#endif

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

// Synonymous for some basic pointers doesn't work well with prepropressing code
typedef FILE *m_g3n_file;
typedef const char *m_g3n_cstring;
typedef const char **m_g3n_cstring_end;

#define M_AS_LINKED_TYPE(typex, x, typey, y) _Generic(((void)0,(x)), typex: (y), default: (typey) {0})

#define M_G3N_IT_TYPE(it_type)  typeof( ((void)0, (it_type){0}))

// Translate type' into the container type if type == TYPE, IT_TYPE into its iterator
// KEY_TYPE, VALUE_TYPE, SUBTYPE into the associated type in the oplist,
// or keep 'type' otherwise
#define M_G3N_TYPE(type, oplist) M_GET_METHOD(type, type, M_OPFLAT oplist ())

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
// which takes two argument according to the variable svar
#define M_G3N_CALL_2(op, svar, type1, var1, type2, var2) \
  _Generic( ((void)0, (svar)),                                                   \
            M_MAP2(M_G3N_CALL_2_func, (op, svar, type1, var1, type2, var2) M_G3N_REGISTERED_ITEMS() )   \
            struct m_g3neric_dummys *: /* cannot happen */ (void) 0)
#define M_G3N_CALL_2_func(x, oplist)                                          \
  M_APPLY(M_G3N_CALL_2_func_test, M_GET_GENTYPE oplist(), M_ID x, oplist)
#define M_G3N_CALL_2_func_test(gentype, op, svar, type1, var1, type2, var2, oplist)                     \
  M_IF_METHOD(op, oplist())(M_G3N_CALL_2_func_expand, M_EAT)(gentype, op, svar, M_G3N_TYPE(type1, oplist), var1, M_G3N_TYPE(type2, oplist), var2, oplist)
#define M_G3N_CALL_2_func_expand(gentype, op, svar, type1, var1, type2, var2, oplist)                   \
  gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(gentype, svar, type1, var1), M_AS_LINKED_TYPE(gentype, svar, type2, var2) ), \
  const gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(const gentype, svar, type1, var1), M_AS_LINKED_TYPE(const gentype, svar, type2, var2) ),

// Call the OPERATOR call of the oplist registered to the variable 'x'
// which takes three argument according to the variable svar
#define M_G3N_CALL_3(op, svar, type1, var1, type2, var2, type3, var3) \
  _Generic( ((void)0, (svar)),                                                   \
            M_MAP2(M_G3N_CALL_3_func, (op, svar, type1, var1, type2, var2, type3, var3) M_G3N_REGISTERED_ITEMS() )   \
            struct m_g3neric_dummys *: /* cannot happen */ (void) 0)
#define M_G3N_CALL_3_func(x, oplist)                                          \
  M_APPLY(M_G3N_CALL_3_func_test, M_GET_GENTYPE oplist(), M_ID x, oplist)
#define M_G3N_CALL_3_func_test(gentype, op, svar, type1, var1, type2, var2, type3, var3, oplist)                     \
  M_IF_METHOD(op, oplist())(M_G3N_CALL_3_func_expand, M_EAT)(gentype, op, svar, M_G3N_TYPE(type1, oplist), var1, M_G3N_TYPE(type2, oplist), var2, M_G3N_TYPE(type3, oplist), var3, oplist)
#define M_G3N_CALL_3_func_expand(gentype, op, svar, type1, var1, type2, var2, oplist)                   \
  gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(gentype, svar, type1, var1), M_AS_LINKED_TYPE(gentype, svar, type2, var2) ), \
  const gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(const gentype, svar, type1, var1), M_AS_LINKED_TYPE(const gentype, svar, type2, var2) ),

// Call the OPERATOR call of the oplist registered to the variable 'x'
// which takes four argument according to the variable svar
#define M_G3N_CALL_4(op, svar, type1, var1, type2, var2, type3, var3, type4, var4) \
  _Generic( ((void)0, (svar)),                                                   \
            M_MAP2(M_G3N_CALL_4_func, (op, svar, type1, var1, type2, var2, type3, var3, type4, var4) M_G3N_REGISTERED_ITEMS() )   \
            struct m_g3neric_dummys *: /* cannot happen */ (void) 0)
#define M_G3N_CALL_4_func(x, oplist)                                          \
  M_APPLY(M_G3N_CALL_4_func_test, M_GET_GENTYPE oplist(), M_ID x, oplist)
#define M_G3N_CALL_4_func_test(gentype, op, svar, type1, var1, type2, var2, type3, var3, type4, var4, oplist)                     \
  M_IF_METHOD(op, oplist())(M_G3N_CALL_4_func_expand, M_EAT)(gentype, op, svar, M_G3N_TYPE(type1, oplist), var1, M_G3N_TYPE(type2, oplist), var2, M_G3N_TYPE(type3, oplist), var3, M_G3N_TYPE(type4, oplist), var4, oplist)
#define M_G3N_CALL_4_func_expand(gentype, op, svar, type1, var1, type2, var2, oplist)                   \
  gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(gentype, svar, type1, var1), M_AS_LINKED_TYPE(gentype, svar, type2, var2) ), \
  const gentype: M_C(M_CALL_, op)(oplist(), M_AS_LINKED_TYPE(const gentype, svar, type1, var1), M_AS_LINKED_TYPE(const gentype, svar, type2, var2) ),

// Define generic functions
// TODO: m_ prefix?
#define init(x)              M_G3N_CALL_1(INIT, x)
#define init_set(x, y)       M_G3N_CALL_2(INIT_SET, x, TYPE, x, TYPE, y)
#define init_move(x, y)      M_G3N_CALL_2(INIT_MOVE, x, TYPE, x, TYPE, y)
#define move(x, y)           M_G3N_CALL_2(MOVE, x, TYPE, x, TYPE, y)
#define set(x, y)            M_G3N_CALL_2(SET, x, TYPE, x , TYPE, y)
#define clear(x)             M_G3N_CALL_1(CLEAR, x)
#define swap(x, y)           M_G3N_CALL_2(SWAP, x, TYPE, x , TYPE, y)
#define hash(x)              M_G3N_CALL_1(HASH, x)
#define equal(x, y)          M_G3N_CALL_2(EQUAL, x, TYPE, x , TYPE, y)
#define cmp(x, y)            M_G3N_CALL_2(CMP, x, TYPE, x , TYPE, y)
#define sort(x)              M_G3N_CALL_1(SORT, x)
#define splice_back(dst, src, it) M_G3N_CALL_3(SPLICE_BACK, dst, TYPE, dst, TYPE, src, IT_TYPE, it)
#define splice_at(dst, itdst, src, itsrc) M_G3N_CALL_4(SPLICE_BACK, dst, TYPE, dst, IT_TYPE, itdst, TYPE, src, IT_TYPE, itsrc)
#define it_type(x)           m_typeof(M_G3N_TYPE_1(IT_TYPE, x))
#define sub_type(x)          m_typeof(M_G3N_TYPE_1(SUBTYPE, x))
#define key_type(x)          m_typeof(M_G3N_TYPE_1(KEY_TYPE, x))
#define value_type(x)        m_typeof(M_G3N_TYPE_1(VALUE_TYPE, x))
#define it_first(x, y)       M_G3N_CALL_2(IT_FIST, y, IT_TYPE, x, TYPE, y)
#define it_last(x, y)        M_G3N_CALL_i2(IT_LAST, y, IT_TYPE, x, TYPE, y)
#define it_end(x, y)         M_G3N_CALL_i2(IT_END, y, IT_TYPE, x, TYPE, y)
// it_set(it_type, it_type)
// it_end_p(it) + it_last_p(it) + it_next(it) + it_previous(it) + it_ref(it) + it_cref(it)
// it_equal_p(it, it)
#define it_insert(cont, it, obj) M_G2N_CALL_3(IT_INSERT, cont, TYPE, cont, IT_TYPE, it, SUBTYPE, obj)
#define it_remove(cont, it)  M_G3N_CALL_2(IT_REMOVE, cont, TYPE, cont, IT_TYPE, it)
#define empty_p(x)           M_G3N_CALL_1(EMPTY_P, x)
#define add(x, y)            M_G3N_CALL_2(ADD, x, TYPE, x, TYPE, y)
#define sub(x, y)            M_G3N_CALL_2(SUB, x, TYPE, x, TYPE, y)
#define mul(x, y)            M_G3N_CALL_2(MUL, x, TYPE, x, TYPE, y)
#define div(x, y)            M_G3N_CALL_2(DIV, x, TYPE, x, TYPE, y)
#define reset(x)             M_G3N_CALL_1(RESET, x)
#define get(x, y)            M_G3N_CALL_2(GET_KEY, x, TYPE, x, KEY_TYPE, y)
#define set_at(x, y, z)      M_G3N_CALL_3(SET_KEY, x, TYPE, x, KEY_TYPE, y, VALUE_TYPE, z)
#define safe_get(x, y)       M_G3N_CALL_2(SAFE_GET_KEY, x, TYPE, x, KEY_TYPE, y)
#define erase(x, y)          M_G3N_CALL_2(ERASE, x, TYPE, x, KEY_TYPE, y)
#define get_size(x)          M_G3N_CALL_1(GET_SIZE, x)
#define push(x, y)           M_G3N_CALL_2(PUSH, x, TYPE, x, SUBTYPE, y)
#define pop(x, y)            M_G3N_CALL_2(PUSH, x, TYPE, x, SUBTYPE, y)
#define push_move(x, y)      M_G3N_CALL_2(PUSH_MOVE, x, TYPE, x, SUBTYPE *, y)
#define pop_move(x, y)       M_G3N_CALL_2(PUSH, x, TYPE, x, SUBTYPE*, y)
#define reverse(x)           M_G3N_CALL_1(REVERSE, x)
#define get_str(s, c, b)     M_G3N_CALL_3(GET_STR, c, string_t, s, TYPE, c, bool, b)
#define parse_str(c, s, e)   M_G3N_CALL_3(PARSE_STR, c, TYPE, c, m_g3n_cstring, s, m_g3n_cstring_end, e)
#define out_str(x, y)        M_G3N_CALL_2(OUT_STR, y, m_g3n_file, x, TYPE, y)
#define in_str(x, y)         M_G3N_CALL_2(IN_STR, x, TYPE, x, m_g3n_file, y)
#define out_serial(x, y)     M_G3N_CALL_2(OUT_SERIAL, y, m_serial_write_t, x, TYPE, y)
#define in_serial(x, y)      M_G3N_CALL_2(IN_SERIAL, x, TYPE, x, m_serial_read_t, y)

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
