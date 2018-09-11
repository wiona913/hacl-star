module Spec.SHA1

module H = Spec.Hash.Helpers
module U32 = FStar.UInt32
module Seq = FStar.Seq
module E = FStar.Kremlin.Endianness

(* Source: https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf *)

(* Section 5.3.1 *)

inline_for_extraction
let init_as_list = [
  0x67452301ul;
  0xefcdab89ul;
  0x98badcfeul;
  0x10325476ul;
  0xc3d2e1f0ul;
]

let h0 : hash_w SHA1 = Seq.seq_of_list init_as_list

let init = h0

(* Section 2.2.2: rotate left *)

inline_for_extraction
let rotl (n_:U32.t{0 < U32.v n_ /\ U32.v n_ < 32}) (x:U32.t): Tot U32.t =
  U32.((x <<^ n_) |^ (x >>^ (32ul -^ n_)))

(* Section 6.1.2 Step 1: message schedule *)

let rec w (mi: Seq.lseq (word SHA1) size_block_w) (t: U32.t {U32.v t <= 79}) : Tot (word SHA1) (decreases (U32.v t)) =
  if U32.lt t 16ul
  then Seq.index mi (U32.v t)
  else rotl 1ul (w mi (t `U32.sub` 3ul) `U32.logxor` w mi (t `U32.sub` 8ul) `U32.logxor` w mi (t `U32.sub` 14ul) `U32.logxor` w mi (t `U32.sub` 16ul))

(* Section 4.1.1: logical functions *)

inline_for_extraction
let f (t: U32.t {U32.v t <= 79}) (x y z: word SHA1) : Tot (word SHA1) =
  if U32.lt t 20ul
  then
    (x `U32.logand` y) `U32.logxor` (U32.lognot x `U32.logand` z)
  else if U32.lt 39ul t && U32.lt t 60ul
  then
    (x `U32.logand` y) `U32.logxor` (x `U32.logand` z) `U32.logxor` (y `U32.logand` z)
  else
    x `U32.logxor` y `U32.logxor` z

(* Section 6.1.2 Step 2 *)

type working_state = {
  a: word SHA1;
  b: word SHA1;
  c: word SHA1;
  d: word SHA1;
  e: word SHA1;
}

let step2 (h: hash_w SHA1) : Tot working_state = {
  a = Seq.index h 0;
  b = Seq.index h 1;
  c = Seq.index h 2;
  d = Seq.index h 3;
  e = Seq.index h 4;
}

(* Section 4.2.1 *)

inline_for_extraction
let k (t: U32.t { U32.v t <= 79 } ) : Tot (word SHA1) =
  if U32.lt t 20ul
  then 0x5a827999ul
  else if U32.lt t 40ul
  then 0x6ed9eba1ul
  else if U32.lt t  60ul
  then 0x8f1bbcdcul
  else 0xca62c1d6ul

(* Section 6.1.2 Step 3 *)

let word_block = Seq.lseq (word SHA1) size_block_w

let step3_body'
  (mi: word_block)
  (st: working_state)
  (t: U32.t {U32.v t < 80})
: Tot working_state
= let _T = rotl 5ul st.a `U32.add_mod` f t st.b st.c st.d `U32.add_mod` st.e `U32.add_mod` k t `U32.add_mod` w mi t in
  let e = st.d in
  let d = st.c in
  let c = rotl 30ul st.b in
  let b = st.a in
  let a = _T in
  {a = a; b = b; c = c; d = d; e = e; }

let step3_body
  (mi: word_block)
  (st: working_state)
  (t: nat {t < 80})
: Tot working_state
= step3_body' mi st (U32.uint_to_t t)

let step3
  (mi: word_block)
  (h: hash_w SHA1)
: Tot working_state
= Spec.Loops.repeat_range 0 80 (step3_body mi) (step2 h)

(* Section 6.1.2 Step 4 *)

let step4
  (mi: word_block)
  (h: hash_w SHA1)
: Tot (hash_w SHA1) =
  let st = step3 mi h in
  Seq.seq_of_list [
    st.a `U32.add_mod` Seq.index h 0;
    st.b `U32.add_mod` Seq.index h 1;
    st.c `U32.add_mod` Seq.index h 2;
    st.d `U32.add_mod` Seq.index h 3;
    st.e `U32.add_mod` Seq.index h 4;
  ]

(* Section 3.1 al. 2: words and bytes, big-endian *)

let words_of_bytes_block
  (l: bytes { Seq.length l == size_block SHA1 } )
: Tot word_block
= E.seq_uint32_of_be size_block_w l

(* Section 6.1.2: outer loop body *)

let update h l =
  let mi = words_of_bytes_block l in
  step4 mi h

(* Section 5.1.1: padding *)

let pad = Spec.Hash.Common.pad SHA1

(* Section 6.1.2: no truncation needed *)

let finish = Spec.Hash.Common.finish _