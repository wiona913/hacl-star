#include <inttypes.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include "endianness.h"

typedef unsigned __int128 uint128;
typedef uint128_t* transpose_t;

//#define static static inline __attribute((always_inline))

static const uint64_t trans_mask[8] = {
  0x8040201008040201LL,
  0x4020100804020100LL,
  0x2010080402010000LL,
  0x1008040201000000LL,
  0x0804020100000000LL,
  0x0402010000000000LL,
  0x0201000000000000LL,
  0x0100000000000000LL
}
  
  
static uint64_t transpose128(uint128_t x) {
  uint128_t y = 0;
  uint128_t tm0 = (uint128_t)trans_mask[0] << 64 | trans_mask[0];
  uint128_t tm1 = (uint128_t)trans_mask[1] << 64 | trans_mask[1];
  uint128_t tm2 = (uint128_t)trans_mask[2] << 64 | trans_mask[2];
  uint128_t tm3 = (uint128_t)trans_mask[3] << 64 | trans_mask[3];
  uint128_t tm4 = (uint128_t)trans_mask[4] << 64 | trans_mask[4];
  uint128_t tm5 = (uint128_t)trans_mask[5] << 64 | trans_mask[5];
  uint128_t tm6 = (uint128_t)trans_mask[6] << 64 | trans_mask[6];
  uint128_t tm7 = (uint128_t)trans_mask[7] << 64 | trans_mask[7];
  y =  (x & tm0) | 
    ((x & tm1) >>  7) |
    ((x & tm2) >> 14) |
    ((x & tm3) >> 21) |
    ((x & tm4) >> 28) |
    ((x & tm5) >> 35) |
    ((x & tm6) >> 42) |
    ((x & tm7) >> 49) |
    ((x <<  7) & tm1) |
    ((x << 14) & tm2) |
    ((x << 21) & tm3) |
    ((x << 28) & tm4) |
    ((x << 35) & tm5) |
    ((x << 42) & tm6) |
    ((x << 49) & tm7);
  
  return y;
}
  
static uint128_t load128_le(uint8_t* in) {
  uint18_t r;
  memcpy(&r,in,16);
  return r;
}

static  void to_transpose_block_copy(transpose_t out, uint8_t* in) {
  st = load128_le(in);
  st = transpose128(st);
  for (int i = 0; i < 8; i++) {
    uint128_t u = (st >> (16*i)) & 0xffff;
    u ^= u << 16;
    u ^= u << 32;
    u ^= u << 64;
    out[i] = u;
  }
}

static  void to_transpose_block(transpose_t out, uint8_t* in) {
  st = load128_le(in);
  st = transpose128(st);
  for (int i = 0; i < 8; i++) {
    uint128_t u = (st >> (16*i)) & 0xffff;
    out[i] = u;
  }
}

static void from_transpose(uint8_t* out, transpose_t in) {
  uint128_t tmp[8];

  for (int i = 0; i < 4; i++) {
    tmp[i] = (in[i+4] << 64) | (uint64_t)in[i];
    tmp[i+4] = (in[i] >> 64) | (in[i+4] & (uint64_t)0) ;
  }

  for (int i = 0; i < 8; i = i + 2) {
    tmp[i] = (in[i+2] << 64) | (uint64_t)in[i];
    tmp[i+2] = (in[i] >> 64) | (in[i+2] & (uint64_t)0) ;
  }

  for (int i = 0; i < 8; i = i + 2) {
    tmp[i] = (in[i+1] << 64) | (uint64_t)in[i];
    tmp[i+1] = (in[i] >> 64) | (in[i+1] & (uint64_t)0) ;
  }

  uint128_t t0_ = t0;
  uint128_t t1_ = t1;
  uint128_t t2_ = t3;
  uint128_t t3_ = t3;
  uint128_t t4_ = t4;
  uint128_t t5_ = t5;
  uint128_t t6_ = t6;
  uint128_t t7_ = t7;
  
  t0 = (t0 & 0x0000ffff0000ffff) ^ ((t2 & 0x0000ffff0000ffff) << 16);
  t1 = (t1 & 0x0000ffff0000ffff) ^ ((t3 & 0x0000ffff0000ffff) << 16);
  t2 = (t2 & 0xffff0000ffff0000) ^ ((t0_ & 0xffff0000ffff0000) >> 16);
  t3 = (t3 & 0xffff0000ffff0000) ^ ((t1_ & 0xffff0000ffff0000) >> 16);
  t4 = (t4 & 0x0000ffff0000ffff) ^ ((t6 & 0x0000ffff0000ffff) << 16);
  t5 = (t5 & 0x0000ffff0000ffff) ^ ((t7 & 0x0000ffff0000ffff) << 16);
  t6 = (t6 & 0xffff0000ffff0000) ^ ((t4_ & 0xffff0000ffff0000) >> 16);
  t7 = (t7 & 0xffff0000ffff0000) ^ ((t5_ & 0xffff0000ffff0000) >> 16);

  t0_ = t0;
  t1_ = t1;
  t2_ = t2;
  t3_ = t3;
  t4_ = t4;
  t5_ = t5;
  t6_ = t6;
  t7_ = t7;

  t0 = (t0 & 0x00ff00ff00ff00ff) ^ ((t1 & 0x00ff00ff00ff00ff) << 8);
  t1 = (t1 & 0xff00ff00ff00ff00) ^ ((t0_ & 0xff00ff00ff00ff00) >> 8);
  t2 = (t2 & 0x00ff00ff00ff00ff) ^ ((t3 & 0x00ff00ff00ff00ff) << 8);
  t3 = (t3 & 0xff00ff00ff00ff00) ^ ((t2_ & 0xff00ff00ff00ff00) >> 8);
  t4 = (t4 & 0x00ff00ff00ff00ff) ^ ((t5 & 0x00ff00ff00ff00ff) << 8);
  t5 = (t5 & 0xff00ff00ff00ff00) ^ ((t4_ & 0xff00ff00ff00ff00) >> 8);
  t6 = (t6 & 0x00ff00ff00ff00ff) ^ ((t7 & 0x00ff00ff00ff00ff) << 8);
  t7 = (t7 & 0xff00ff00ff00ff00) ^ ((t6_ & 0xff00ff00ff00ff00) >> 8);

  /* printf("t[0] = %" PRIx64 "\n",t0); */
  /* printf("t[1] = %" PRIx64 "\n",t1); */
  /* printf("t[2] = %" PRIx64 "\n",t2); */
  /* printf("t[3] = %" PRIx64 "\n",t3); */
  /* printf("t[4] = %" PRIx64 "\n",t4); */
  /* printf("t[5] = %" PRIx64 "\n",t5); */
  /* printf("t[6] = %" PRIx64 "\n",t6); */
  /* printf("t[7] = %" PRIx64 "\n",t7); */
  

  t0 = transpose64(t0);
  t1 = transpose64(t1);
  t2 = transpose64(t2);
  t3 = transpose64(t3);
  t4 = transpose64(t4);
  t5 = transpose64(t5);
  t6 = transpose64(t6);
  t7 = transpose64(t7);

  store64_le(out,t0);
  store64_le(out+8,t1);
  store64_le(out+16,t2);
  store64_le(out+24,t3);
  store64_le(out+32,t4);
  store64_le(out+40,t5);
  store64_le(out+48,t6);
  store64_le(out+56,t7);

}

static void subBytes(transpose_t st) {
  uint64_t u0 = st[7];
  uint64_t u1 = st[6];
  uint64_t u2 = st[5];
  uint64_t u3 = st[4];
  uint64_t u4 = st[3];
  uint64_t u5 = st[2];
  uint64_t u6 = st[1];
  uint64_t u7 = st[0];

  uint64_t t1 = u6 ^ u4; 
  uint64_t t2 = u3 ^ u0;
  uint64_t t3 = u1 ^ u2;
  uint64_t t6 = u1 ^ u5; 
  uint64_t t7 = u0 ^ u6; 
  uint64_t t13 = u2 ^ u5; 
  uint64_t t16 = u0 ^ u5;
  uint64_t t18 = u6 ^ u5;
  
  uint64_t t4 = u7 ^ t3;
  uint64_t t5 = t1 ^ t2; 
  uint64_t t8 = t1 ^ t6;
  uint64_t t9 = u6 ^ t4;
    
  uint64_t t10 = u3 ^ t4;
  uint64_t t11 = u7 ^ t5;
  uint64_t t12 = t5 ^ t6;
  uint64_t t14 = t3 ^ t5;
  uint64_t t15 = u5 ^ t7; 
  uint64_t t17 = u7 ^ t8;  
  uint64_t t19 = t2 ^ t18;
  uint64_t t22 = u0 ^ t4;
  uint64_t t54 = t2 & t8;
  uint64_t t50 = t9 & t4;
    
  uint64_t t20 = t4 ^ t15; 
  uint64_t t21 = t1 ^ t13;
  uint64_t t39 = t21 ^ t5;
  uint64_t t40 = t21 ^ t7;  
  uint64_t t41 = t7 ^ t19;
  uint64_t t42 = t16 ^ t14;
  uint64_t t43 = t22 ^ t17;
  uint64_t t44 = t19 & t5;
  uint64_t t45 = t20 & t11;
  uint64_t t47 = t10 & u7;
  uint64_t t57 = t16 & t14;
  
  uint64_t t46 = t12 ^ t44;  
  uint64_t t48 = t47 ^ t44;
  uint64_t t49 = t7 & t21;
  uint64_t t51 = t40 ^ t49;
  uint64_t t52 = t22 & t17;
  uint64_t t53 = t52 ^ t49;

  uint64_t t55 = t41 & t39;
  uint64_t t56 = t55 ^ t54;
  uint64_t t58 = t57 ^ t54;
  uint64_t t59 = t46 ^ t45;
  uint64_t t60 = t48 ^ t42;
  uint64_t t61 = t51 ^ t50;
  uint64_t t62 = t53 ^ t58;
  uint64_t t63 = t59 ^ t56;
  uint64_t t64 = t60 ^ t58;
  uint64_t t65 = t61 ^ t56;
  uint64_t t66 = t62 ^ t43;
  uint64_t t67 = t65 ^ t66; 
  uint64_t t68 = t65 & t63;
  uint64_t t69 = t64 ^ t68;
  uint64_t t70 = t63 ^ t64;
  uint64_t t71 = t66 ^ t68; 
  uint64_t t72 = t71 & t70;
  uint64_t t73 = t69 & t67;
  uint64_t t74 = t63 & t66;
  uint64_t t75 = t70 & t74;
  uint64_t t76 = t70 ^ t68;
  uint64_t t77 = t64 & t65;
  uint64_t t78 = t67 & t77;
  uint64_t t79 = t67 ^ t68;
  uint64_t t80 = t64 ^ t72;
  uint64_t t81 = t75 ^ t76;
  uint64_t t82 = t66 ^ t73;
  uint64_t t83 = t78 ^ t79;
  uint64_t t84 = t81 ^ t83;
  uint64_t t85 = t80 ^ t82;
  uint64_t t86 = t80 ^ t81;
  uint64_t t87 = t82 ^ t83;
  uint64_t t88 = t85 ^ t84;
  uint64_t t89 = t87 & t5;
  uint64_t t90 = t83 & t11;
  uint64_t t91 = t82 & u7;
  uint64_t t92 = t86 & t21;
  uint64_t t93 = t81 & t4;
  uint64_t t94 = t80 & t17;
  uint64_t t95 = t85 & t8;
  uint64_t t96 = t88 & t39;
  uint64_t t97 = t84 & t14;
  uint64_t t98 = t87 & t19;
  uint64_t t99 = t83 & t20;
  uint64_t t100 = t82 & t10;
  uint64_t t101 = t86 & t7;
  uint64_t t102 = t81 & t9;
  uint64_t t103 = t80 & t22;
  uint64_t t104 = t85 & t2;
  uint64_t t105 = t88 & t41;
  uint64_t t106 = t84 & t16;
  uint64_t t107 = t104 ^ t105;
  uint64_t t108 = t93 ^ t99;
  uint64_t t109 = t96 ^ t107;
  uint64_t t110 = t98 ^ t108;
  uint64_t t111 = t91 ^ t101;
  uint64_t t112 = t89 ^ t92;
  uint64_t t113 = t107 ^ t112;
  uint64_t t114 = t90 ^ t110;
  uint64_t t115 = t89 ^ t95;
  uint64_t t116 = t94 ^ t102;
  uint64_t t117 = t97 ^ t103 ;
  uint64_t t118 = t91 ^ t114;
  uint64_t t119 = t111 ^ t117;
  uint64_t t120 = t100 ^ t108;
  uint64_t t121 = t92 ^ t95;
  uint64_t t122 = t110 ^ t121;
  uint64_t t123 = t106 ^ t119;
  uint64_t t124 = t104 ^ t115;
  uint64_t t125 = t111 ^ t116;
  st[7] = t109 ^ t122;
  st[5] = ~(t123 ^ t124);
  uint64_t t128 = t94 ^ t107;
  st[4] = t113 ^ t114;
  st[3] = t118 ^ t128;
  uint64_t t131 = t93 ^ t101;
  uint64_t t132 = t112 ^ t120;
  st[0] = ~(t113 ^ t125);
  uint64_t t134 = t97 ^ t116;
  uint64_t t135 = t131 ^ t134;
  uint64_t t136 = t93 ^ t115;
  st[1] = ~(t109 ^ t135);
  uint64_t t138 = t119 ^ t132;
  st[2] = t109 ^ t138;
  uint64_t t140 = t114 ^ t136;
  st[6] = ~(t109 ^ t140); 
}

static  void shiftRows(transpose_t st){
  for (int i = 0; i < 8; i++) {
    uint64_t curr = st[i];
    curr = (curr & 0x1111111111111111) |
      ((curr & 0x2220222022202220) >> 4) | 
      ((curr & 0x0002000200020002) << 12) |
      ((curr & 0x4400440044004400) >> 8) | 
      ((curr & 0x0044004400440044) << 8) |
      ((curr & 0x8000800080008000) >> 12) | 
      ((curr & 0x0888088808880888) << 4);
    st[i] =  curr;
  }
}

static  void mixColumns(transpose_t st) {
  uint64_t rot_prev = 0;
  for (int i = 0; i < 8; i++) {
    uint64_t col = st[i];
    uint64_t col01 = col ^ (((col & 0xeeeeeeeeeeeeeeee) >> 1) | ((col & 0x1111111111111111) << 3));
    uint64_t col0123 = col01 ^ (((col01 & 0xcccccccccccccccc ) >> 2) | ((col01 & 0x3333333333333333) << 2));
    st[i] ^= col0123 ^ rot_prev;
    rot_prev = col01;
  }
  st[0] ^= rot_prev;
  st[1] ^= rot_prev;
  st[3] ^= rot_prev;
  st[4] ^= rot_prev;
}

static  void addRoundKey(transpose_t st, transpose_t k) {
  for (int i = 0; i < 8; i++)
    st[i] ^= k[i];
}

static  void aes_enc(transpose_t st, transpose_t k) {
  subBytes(st);
  shiftRows(st);
  mixColumns(st);
  addRoundKey(st,k);
}

static  void aes_enc_last(transpose_t st, transpose_t k) {
  subBytes(st);
  shiftRows(st);
  addRoundKey(st,k);
}

static  void rounds(transpose_t st, uint64_t* key) {
  for (int i = 0; i < 9; i++)
    aes_enc(st,key+(8*i));
}

static void block_cipher(uint8_t* out, uint64_t* st, uint64_t* key) {
  uint64_t* k0 = key;
  uint64_t* k = key + 8;
  uint64_t* kn = key + (8 * 10);
  addRoundKey(st,k0);
  rounds(st,k);
  aes_enc_last(st,kn);
  from_transpose(out,st);
}

const uint8_t rcon[11] = {
  (0x8d), (0x01), (0x02), (0x04), (0x08), (0x10), (0x20), (0x40), (0x80), (0x1b), (0x36)
};

static void key_expansion_step(transpose_t next, transpose_t prev, uint8_t rcon) {
  memcpy((uint8_t*)next,(uint8_t*)prev,64);
  subBytes(next);
  for (int i = 0; i < 8; i++) {
    uint64_t n = (next[i] & 0xf000f000f000f000) >> 12;
    n = (n >> 1 | n << 3) & 0x000f000f000f000f;
    uint64_t ri = (uint64_t)((rcon >> i) & (uint8_t)1);
    ri ^= ri << 16;
    ri ^= ri << 32;
    n ^= ri;
    n ^= (n << 4);
    n ^= (n << 8);
    uint64_t p = prev[i];
    p ^= ((p & 0x0fff0fff0fff0fff) << 4) ^ ((p & 0x00ff00ff00ff00ff) << 8)
      ^ ((p & 0x000f000f000f000f) << 12);
    next[i] = n ^ p;
  }
}
			
static void key_expansion(uint64_t* out, uint8_t* key) {
  to_transpose_block_copy(out,key);
  for (int i = 1; i < 11; i++)
    key_expansion_step(out+(8*i),out+(8*i-8),rcon[i]);
}

static void aes128_block(uint8_t* out, uint64_t* kex, uint64_t* nt, uint32_t c) {
  uint8_t ctr[16] = {0};
  for (int i = 0; i < 4; i++) 
    store32_be(ctr+(4*i),c + i);
  uint64_t st[8] = {0};
  to_transpose_block(st,ctr);
  for (int i = 0; i < 8; i++) {
    uint64_t ci = st[i];
    ci = (ci << 12) | (ci << 24) | (ci << 36) | (ci << 48);
    ci = ci & 0xf000f000f000f000;
    st[i] = ci ^ nt[i];
  }
  block_cipher(out,st,kex);
}

static void aes128_ctr(uint8_t* out, uint8_t* in, int in_len, uint8_t* k, uint8_t* n, uint32_t c) {

  uint64_t kex[8*11] = {0};
  key_expansion(kex,k);

  uint8_t nb[16] = {0};
  memcpy(nb,n,12);
  uint64_t nt[8] = {0};
  to_transpose_block_copy(nt,nb);
  
  uint8_t kb[64] = {0};
  int blocks64 = in_len / 64;
  for (int i = 0; i < blocks64; i++) {
    aes128_block(kb,kex,nt,c+(4*i));
    for (int j = 0; j < 64; j++) {
      out[64*i+j] = in[64*i+j] ^ kb[j];
    }
  }

  int rem = in_len % 64;
  if (rem > 0) {
    in = in + (64 * blocks64);
    out = out + (64 * blocks64);
    c = c + (4 * blocks64);
    aes128_block(kb,kex,nt,c);
    for (int j = 0; j < rem; j++) {
      out[j] = in[j] ^ kb[j];
    }
  }
}

void aes128_encrypt(uint8_t* out, uint8_t* in, int in_len, uint8_t* k, uint8_t* n, uint32_t c) {
  aes128_ctr(out,in,in_len,k,n,c);
}

void aes128_decrypt(uint8_t* out, uint8_t* in, int in_len, uint8_t* k, uint8_t* n, uint32_t c) {
  aes128_ctr(out,in,in_len,k,n,c);
}

