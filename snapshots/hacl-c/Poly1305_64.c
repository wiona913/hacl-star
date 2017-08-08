#include "Poly1305_64.h"

inline static void Hacl_Bignum_Modulo_reduce(uint64_t *b)
{
  uint64_t b0 = b[0];
  b[0] = (b0 << (uint32_t )4) + (b0 << (uint32_t )2);
}

inline static void Hacl_Bignum_Modulo_carry_top(uint64_t *b)
{
  uint64_t b2 = b[2];
  uint64_t b0 = b[0];
  uint64_t b2_42 = b2 >> (uint32_t )42;
  b[2] = b2 & (uint64_t )0x3ffffffffff;
  b[0] = (b2_42 << (uint32_t )2) + b2_42 + b0;
}

inline static void Hacl_Bignum_Modulo_carry_top_wide(FStar_UInt128_t *b)
{
  FStar_UInt128_t b2 = b[2];
  FStar_UInt128_t b0 = b[0];
  FStar_UInt128_t
  b2_ = FStar_UInt128_logand(b2, FStar_Int_Cast_Full_uint64_to_uint128((uint64_t )0x3ffffffffff));
  uint64_t
  b2_42 = FStar_Int_Cast_Full_uint128_to_uint64(FStar_UInt128_shift_right(b2, (uint32_t )42));
  FStar_UInt128_t
  b0_ =
    FStar_UInt128_add(b0,
      FStar_Int_Cast_Full_uint64_to_uint128((b2_42 << (uint32_t )2) + b2_42));
  b[2] = b2_;
  b[0] = b0_;
}

inline static void
Hacl_Bignum_Fproduct_copy_from_wide_(uint64_t *output, FStar_UInt128_t *input)
{
  for (uint32_t i = (uint32_t )0; i < (uint32_t )3; i = i + (uint32_t )1)
  {
    FStar_UInt128_t uu____429 = input[i];
    uint64_t uu____428 = FStar_Int_Cast_Full_uint128_to_uint64(uu____429);
    output[i] = uu____428;
  }
}

inline static void Hacl_Bignum_Fproduct_shift(uint64_t *output)
{
  uint64_t tmp = output[2];
  for (uint32_t i = (uint32_t )0; i < (uint32_t )2; i = i + (uint32_t )1)
  {
    uint32_t ctr = (uint32_t )3 - i - (uint32_t )1;
    uint64_t z = output[ctr - (uint32_t )1];
    output[ctr] = z;
  }
  output[0] = tmp;
}

inline static void
Hacl_Bignum_Fproduct_sum_scalar_multiplication_(
  FStar_UInt128_t *output,
  uint64_t *input,
  uint64_t s
)
{
  for (uint32_t i = (uint32_t )0; i < (uint32_t )3; i = i + (uint32_t )1)
  {
    FStar_UInt128_t uu____871 = output[i];
    uint64_t uu____874 = input[i];
    FStar_UInt128_t
    uu____870 = FStar_UInt128_add_mod(uu____871, FStar_UInt128_mul_wide(uu____874, s));
    output[i] = uu____870;
  }
}

inline static void Hacl_Bignum_Fproduct_carry_wide_(FStar_UInt128_t *tmp)
{
  for (uint32_t i = (uint32_t )0; i < (uint32_t )2; i = i + (uint32_t )1)
  {
    uint32_t ctr = i;
    FStar_UInt128_t tctr = tmp[ctr];
    FStar_UInt128_t tctrp1 = tmp[ctr + (uint32_t )1];
    uint64_t
    r0 =
      FStar_Int_Cast_Full_uint128_to_uint64(tctr)
      & (((uint64_t )1 << (uint32_t )44) - (uint64_t )1);
    FStar_UInt128_t c = FStar_UInt128_shift_right(tctr, (uint32_t )44);
    tmp[ctr] = FStar_Int_Cast_Full_uint64_to_uint128(r0);
    tmp[ctr + (uint32_t )1] = FStar_UInt128_add(tctrp1, c);
  }
}

inline static void Hacl_Bignum_Fproduct_carry_limb_(uint64_t *tmp)
{
  for (uint32_t i = (uint32_t )0; i < (uint32_t )2; i = i + (uint32_t )1)
  {
    uint32_t ctr = i;
    uint64_t tctr = tmp[ctr];
    uint64_t tctrp1 = tmp[ctr + (uint32_t )1];
    uint64_t r0 = tctr & (((uint64_t )1 << (uint32_t )44) - (uint64_t )1);
    uint64_t c = tctr >> (uint32_t )44;
    tmp[ctr] = r0;
    tmp[ctr + (uint32_t )1] = tctrp1 + c;
  }
}

static uint64_t
*Hacl_Impl_Poly1305_64_State___proj__MkState__item__r(
  Hacl_Impl_Poly1305_64_State_poly1305_state projectee
)
{
  Hacl_Impl_Poly1305_64_State_poly1305_state scrut = projectee;
  uint64_t *r = scrut.r;
  return r;
}

static uint64_t
*Hacl_Impl_Poly1305_64_State___proj__MkState__item__h(
  Hacl_Impl_Poly1305_64_State_poly1305_state projectee
)
{
  Hacl_Impl_Poly1305_64_State_poly1305_state scrut = projectee;
  uint64_t *h = scrut.h;
  return h;
}

inline static void Hacl_Bignum_Fmul_shift_reduce(uint64_t *output)
{
  Hacl_Bignum_Fproduct_shift(output);
  Hacl_Bignum_Modulo_reduce(output);
}

static void
Hacl_Bignum_Fmul_mul_shift_reduce_(FStar_UInt128_t *output, uint64_t *input, uint64_t *input2)
{
  for (uint32_t i = (uint32_t )0; i < (uint32_t )3; i = i + (uint32_t )1)
  {
    uint32_t ctr = (uint32_t )3 - i - (uint32_t )1;
    uint32_t i1 = ctr;
    uint32_t j = (uint32_t )2 - i1;
    uint64_t input2i = input2[j];
    Hacl_Bignum_Fproduct_sum_scalar_multiplication_(output, input, input2i);
    if (ctr > (uint32_t )0)
      Hacl_Bignum_Fmul_shift_reduce(input);
  }
}

inline static void Hacl_Bignum_Fmul_fmul_(uint64_t *output, uint64_t *input, uint64_t *input2)
{
  KRML_CHECK_SIZE(FStar_Int_Cast_Full_uint64_to_uint128((uint64_t )0), (uint32_t )3);
  FStar_UInt128_t t[3];
  for (uintmax_t _i = 0; _i < (uint32_t )3; ++_i)
    t[_i] = FStar_Int_Cast_Full_uint64_to_uint128((uint64_t )0);
  Hacl_Bignum_Fmul_mul_shift_reduce_(t, input, input2);
  Hacl_Bignum_Fproduct_carry_wide_(t);
  Hacl_Bignum_Modulo_carry_top_wide(t);
  Hacl_Bignum_Fproduct_copy_from_wide_(output, t);
  uint64_t i0 = output[0];
  uint64_t i1 = output[1];
  uint64_t i0_ = i0 & (((uint64_t )1 << (uint32_t )44) - (uint64_t )1);
  uint64_t i1_ = i1 + (i0 >> (uint32_t )44);
  output[0] = i0_;
  output[1] = i1_;
}

inline static void Hacl_Bignum_Fmul_fmul(uint64_t *output, uint64_t *input, uint64_t *input2)
{
  uint64_t tmp[3] = { 0 };
  memcpy(tmp, input, (uint32_t )3 * sizeof input[0]);
  Hacl_Bignum_Fmul_fmul_(output, tmp, input2);
}

inline static void
Hacl_Bignum_AddAndMultiply_add_and_multiply(uint64_t *acc, uint64_t *block, uint64_t *r)
{
  for (uint32_t i = (uint32_t )0; i < (uint32_t )3; i = i + (uint32_t )1)
  {
    uint64_t uu____871 = acc[i];
    uint64_t uu____874 = block[i];
    uint64_t uu____870 = uu____871 + uu____874;
    acc[i] = uu____870;
  }
  Hacl_Bignum_Fmul_fmul(acc, acc, r);
}

inline static void
Hacl_Impl_Poly1305_64_poly1305_update(
  Hacl_Impl_Poly1305_64_State_poly1305_state st,
  uint8_t *m
)
{
  uint64_t *acc = Hacl_Impl_Poly1305_64_State___proj__MkState__item__h(st);
  uint64_t *r = Hacl_Impl_Poly1305_64_State___proj__MkState__item__r(st);
  uint64_t tmp[3] = { 0 };
  FStar_UInt128_t m0 = load128_le(m);
  uint64_t r0 = FStar_Int_Cast_Full_uint128_to_uint64(m0) & (uint64_t )0xfffffffffff;
  uint64_t
  r1 =
    FStar_Int_Cast_Full_uint128_to_uint64(FStar_UInt128_shift_right(m0, (uint32_t )44))
    & (uint64_t )0xfffffffffff;
  uint64_t
  r2 = FStar_Int_Cast_Full_uint128_to_uint64(FStar_UInt128_shift_right(m0, (uint32_t )88));
  tmp[0] = r0;
  tmp[1] = r1;
  tmp[2] = r2;
  uint64_t b2 = tmp[2];
  uint64_t b2_ = (uint64_t )0x10000000000 | b2;
  tmp[2] = b2_;
  Hacl_Bignum_AddAndMultiply_add_and_multiply(acc, tmp, r);
}

inline static void
Hacl_Impl_Poly1305_64_poly1305_process_last_block_(
  uint8_t *block,
  Hacl_Impl_Poly1305_64_State_poly1305_state st,
  uint8_t *m,
  uint64_t rem_
)
{
  uint64_t tmp[3] = { 0 };
  FStar_UInt128_t m0 = load128_le(block);
  uint64_t r0 = FStar_Int_Cast_Full_uint128_to_uint64(m0) & (uint64_t )0xfffffffffff;
  uint64_t
  r1 =
    FStar_Int_Cast_Full_uint128_to_uint64(FStar_UInt128_shift_right(m0, (uint32_t )44))
    & (uint64_t )0xfffffffffff;
  uint64_t
  r2 = FStar_Int_Cast_Full_uint128_to_uint64(FStar_UInt128_shift_right(m0, (uint32_t )88));
  tmp[0] = r0;
  tmp[1] = r1;
  tmp[2] = r2;
  Hacl_Bignum_AddAndMultiply_add_and_multiply(Hacl_Impl_Poly1305_64_State___proj__MkState__item__h(st),
    tmp,
    Hacl_Impl_Poly1305_64_State___proj__MkState__item__r(st));
}

inline static void
Hacl_Impl_Poly1305_64_poly1305_process_last_block(
  Hacl_Impl_Poly1305_64_State_poly1305_state st,
  uint8_t *m,
  uint64_t rem_
)
{
  uint8_t zero1 = (uint8_t )0;
  KRML_CHECK_SIZE(zero1, (uint32_t )16);
  uint8_t block[16];
  for (uintmax_t _i = 0; _i < (uint32_t )16; ++_i)
    block[_i] = zero1;
  uint32_t i0 = (uint32_t )rem_;
  uint32_t i = (uint32_t )rem_;
  memcpy(block, m, i * sizeof m[0]);
  block[i0] = (uint8_t )1;
  Hacl_Impl_Poly1305_64_poly1305_process_last_block_(block, st, m, rem_);
}

static void Hacl_Impl_Poly1305_64_poly1305_last_pass(uint64_t *acc)
{
  Hacl_Bignum_Fproduct_carry_limb_(acc);
  Hacl_Bignum_Modulo_carry_top(acc);
  uint64_t a0 = acc[0];
  uint64_t a10 = acc[1];
  uint64_t a20 = acc[2];
  uint64_t a0_ = a0 & (uint64_t )0xfffffffffff;
  uint64_t r0 = a0 >> (uint32_t )44;
  uint64_t a1_ = (a10 + r0) & (uint64_t )0xfffffffffff;
  uint64_t r1 = (a10 + r0) >> (uint32_t )44;
  uint64_t a2_ = a20 + r1;
  acc[0] = a0_;
  acc[1] = a1_;
  acc[2] = a2_;
  Hacl_Bignum_Modulo_carry_top(acc);
  uint64_t i0 = acc[0];
  uint64_t i1 = acc[1];
  uint64_t i0_ = i0 & (((uint64_t )1 << (uint32_t )44) - (uint64_t )1);
  uint64_t i1_ = i1 + (i0 >> (uint32_t )44);
  acc[0] = i0_;
  acc[1] = i1_;
  uint64_t a00 = acc[0];
  uint64_t a1 = acc[1];
  uint64_t a2 = acc[2];
  uint64_t mask0 = FStar_UInt64_gte_mask(a00, (uint64_t )0xffffffffffb);
  uint64_t mask1 = FStar_UInt64_eq_mask(a1, (uint64_t )0xfffffffffff);
  uint64_t mask2 = FStar_UInt64_eq_mask(a2, (uint64_t )0x3ffffffffff);
  uint64_t mask = mask0 & mask1 & mask2;
  uint64_t a0_0 = a00 - ((uint64_t )0xffffffffffb & mask);
  uint64_t a1_0 = a1 - ((uint64_t )0xfffffffffff & mask);
  uint64_t a2_0 = a2 - ((uint64_t )0x3ffffffffff & mask);
  acc[0] = a0_0;
  acc[1] = a1_0;
  acc[2] = a2_0;
}

static Hacl_Impl_Poly1305_64_State_poly1305_state
Hacl_Impl_Poly1305_64_mk_state(uint64_t *r, uint64_t *h)
{
  return ((Hacl_Impl_Poly1305_64_State_poly1305_state ){ .r = r, .h = h });
}

static void
Hacl_Standalone_Poly1305_64_poly1305_blocks(
  Hacl_Impl_Poly1305_64_State_poly1305_state st,
  uint8_t *m,
  uint64_t len1
)
{
  if (len1 == (uint64_t )0)
  {
    
  }
  else
  {
    uint8_t *block = m;
    uint8_t *tail1 = m + (uint32_t )16;
    Hacl_Impl_Poly1305_64_poly1305_update(st, block);
    uint64_t len2 = len1 - (uint64_t )1;
    Hacl_Standalone_Poly1305_64_poly1305_blocks(st, tail1, len2);
  }
}

static void
Hacl_Standalone_Poly1305_64_poly1305_partial(
  Hacl_Impl_Poly1305_64_State_poly1305_state st,
  uint8_t *input,
  uint64_t len1,
  uint8_t *kr
)
{
  uint64_t *x0 = Hacl_Impl_Poly1305_64_State___proj__MkState__item__r(st);
  FStar_UInt128_t k1 = load128_le(kr);
  FStar_UInt128_t
  k_clamped =
    FStar_UInt128_logand(k1,
      FStar_UInt128_logor(FStar_UInt128_shift_left(FStar_Int_Cast_Full_uint64_to_uint128((uint64_t )0x0ffffffc0ffffffc),
          (uint32_t )64),
        FStar_Int_Cast_Full_uint64_to_uint128((uint64_t )0x0ffffffc0fffffff)));
  uint64_t r0 = FStar_Int_Cast_Full_uint128_to_uint64(k_clamped) & (uint64_t )0xfffffffffff;
  uint64_t
  r1 =
    FStar_Int_Cast_Full_uint128_to_uint64(FStar_UInt128_shift_right(k_clamped, (uint32_t )44))
    & (uint64_t )0xfffffffffff;
  uint64_t
  r2 = FStar_Int_Cast_Full_uint128_to_uint64(FStar_UInt128_shift_right(k_clamped, (uint32_t )88));
  x0[0] = r0;
  x0[1] = r1;
  x0[2] = r2;
  uint64_t *x00 = Hacl_Impl_Poly1305_64_State___proj__MkState__item__h(st);
  x00[0] = (uint64_t )0;
  x00[1] = (uint64_t )0;
  x00[2] = (uint64_t )0;
  Hacl_Standalone_Poly1305_64_poly1305_blocks(st, input, len1);
}

static void
Hacl_Standalone_Poly1305_64_poly1305_complete(
  Hacl_Impl_Poly1305_64_State_poly1305_state st,
  uint8_t *m,
  uint64_t len1,
  uint8_t *k1
)
{
  uint8_t *kr = k1;
  uint64_t len16 = len1 >> (uint32_t )4;
  uint64_t rem16 = len1 & (uint64_t )0xf;
  uint8_t *part_input = m;
  uint8_t *last_block = m + (uint32_t )((uint64_t )16 * len16);
  Hacl_Standalone_Poly1305_64_poly1305_partial(st, part_input, len16, kr);
  if (rem16 == (uint64_t )0)
  {
    
  }
  else
    Hacl_Impl_Poly1305_64_poly1305_process_last_block(st, last_block, rem16);
  uint64_t *acc = Hacl_Impl_Poly1305_64_State___proj__MkState__item__h(st);
  Hacl_Impl_Poly1305_64_poly1305_last_pass(acc);
}

static void
Hacl_Standalone_Poly1305_64_crypto_onetimeauth_(
  uint8_t *output,
  uint8_t *input,
  uint64_t len1,
  uint8_t *k1
)
{
  uint64_t buf[6] = { 0 };
  uint64_t *r = buf;
  uint64_t *h = buf + (uint32_t )3;
  Hacl_Impl_Poly1305_64_State_poly1305_state st = Hacl_Impl_Poly1305_64_mk_state(r, h);
  uint8_t *key_s = k1 + (uint32_t )16;
  Hacl_Standalone_Poly1305_64_poly1305_complete(st, input, len1, k1);
  uint64_t *acc = Hacl_Impl_Poly1305_64_State___proj__MkState__item__h(st);
  FStar_UInt128_t k_ = load128_le(key_s);
  uint64_t h0 = acc[0];
  uint64_t h1 = acc[1];
  uint64_t h2 = acc[2];
  FStar_UInt128_t
  acc_ =
    FStar_UInt128_logor(FStar_UInt128_shift_left(FStar_Int_Cast_Full_uint64_to_uint128(h2
          << (uint32_t )24
          | h1 >> (uint32_t )20),
        (uint32_t )64),
      FStar_Int_Cast_Full_uint64_to_uint128(h1 << (uint32_t )44 | h0));
  FStar_UInt128_t mac_ = FStar_UInt128_add_mod(acc_, k_);
  store128_le(output, mac_);
}

static void
Hacl_Standalone_Poly1305_64_crypto_onetimeauth(
  uint8_t *output,
  uint8_t *input,
  uint64_t len1,
  uint8_t *k1
)
{
  Hacl_Standalone_Poly1305_64_crypto_onetimeauth_(output, input, len1, k1);
}

void *Poly1305_64_op_String_Access(FStar_HyperStack_mem h, uint8_t *b)
{
  return (void *)(uint8_t )0;
}

Hacl_Impl_Poly1305_64_State_poly1305_state Poly1305_64_mk_state(uint64_t *r, uint64_t *acc)
{
  return Hacl_Impl_Poly1305_64_mk_state(r, acc);
}

void Poly1305_64_init(Hacl_Impl_Poly1305_64_State_poly1305_state st, uint8_t *k1)
{
  uint64_t *x0 = Hacl_Impl_Poly1305_64_State___proj__MkState__item__r(st);
  FStar_UInt128_t k10 = load128_le(k1);
  FStar_UInt128_t
  k_clamped =
    FStar_UInt128_logand(k10,
      FStar_UInt128_logor(FStar_UInt128_shift_left(FStar_Int_Cast_Full_uint64_to_uint128((uint64_t )0x0ffffffc0ffffffc),
          (uint32_t )64),
        FStar_Int_Cast_Full_uint64_to_uint128((uint64_t )0x0ffffffc0fffffff)));
  uint64_t r0 = FStar_Int_Cast_Full_uint128_to_uint64(k_clamped) & (uint64_t )0xfffffffffff;
  uint64_t
  r1 =
    FStar_Int_Cast_Full_uint128_to_uint64(FStar_UInt128_shift_right(k_clamped, (uint32_t )44))
    & (uint64_t )0xfffffffffff;
  uint64_t
  r2 = FStar_Int_Cast_Full_uint128_to_uint64(FStar_UInt128_shift_right(k_clamped, (uint32_t )88));
  x0[0] = r0;
  x0[1] = r1;
  x0[2] = r2;
  uint64_t *x00 = Hacl_Impl_Poly1305_64_State___proj__MkState__item__h(st);
  x00[0] = (uint64_t )0;
  x00[1] = (uint64_t )0;
  x00[2] = (uint64_t )0;
}

void *Poly1305_64_empty_log = (void *)(uint8_t )0;

void Poly1305_64_update_block(Hacl_Impl_Poly1305_64_State_poly1305_state st, uint8_t *m)
{
  Hacl_Impl_Poly1305_64_poly1305_update(st, m);
}

void
Poly1305_64_update(Hacl_Impl_Poly1305_64_State_poly1305_state st, uint8_t *m, uint32_t len1)
{
  if (len1 == (uint32_t )0)
  {
    
  }
  else
  {
    uint8_t *block = m;
    uint8_t *m_ = m + (uint32_t )16;
    uint32_t len2 = len1 - (uint32_t )1;
    Poly1305_64_update_block(st, block);
    Poly1305_64_update(st, m_, len2);
  }
}

void
Poly1305_64_update_last(
  Hacl_Impl_Poly1305_64_State_poly1305_state st,
  uint8_t *m,
  uint32_t len1
)
{
  if ((uint64_t )len1 == (uint64_t )0)
  {
    
  }
  else
    Hacl_Impl_Poly1305_64_poly1305_process_last_block(st, m, (uint64_t )len1);
  uint64_t *acc = Hacl_Impl_Poly1305_64_State___proj__MkState__item__h(st);
  Hacl_Impl_Poly1305_64_poly1305_last_pass(acc);
}

void
Poly1305_64_finish(Hacl_Impl_Poly1305_64_State_poly1305_state st, uint8_t *mac, uint8_t *k1)
{
  uint64_t *acc = Hacl_Impl_Poly1305_64_State___proj__MkState__item__h(st);
  FStar_UInt128_t k_ = load128_le(k1);
  uint64_t h0 = acc[0];
  uint64_t h1 = acc[1];
  uint64_t h2 = acc[2];
  FStar_UInt128_t
  acc_ =
    FStar_UInt128_logor(FStar_UInt128_shift_left(FStar_Int_Cast_Full_uint64_to_uint128(h2
          << (uint32_t )24
          | h1 >> (uint32_t )20),
        (uint32_t )64),
      FStar_Int_Cast_Full_uint64_to_uint128(h1 << (uint32_t )44 | h0));
  FStar_UInt128_t mac_ = FStar_UInt128_add_mod(acc_, k_);
  store128_le(mac, mac_);
}

void
Poly1305_64_crypto_onetimeauth(uint8_t *output, uint8_t *input, uint64_t len1, uint8_t *k1)
{
  Hacl_Standalone_Poly1305_64_crypto_onetimeauth(output, input, len1, k1);
}

