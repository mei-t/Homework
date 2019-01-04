#include "RSA_decoding.h"
#include <iostream>
#include <float.h>
using namespace std;

// RSA暗号を解読するプログラム
// 公開鍵から素因数分解と拡張ユークリッドの互除法を用いて解読する。
// 公開鍵と暗号文を入力すると復号鍵と平文を出力する。
//
// >> Input public keys, N
// 2870558567
// >> Input public keys, e
// 78157
// >> Input a cryptogram, c
// 1102754603
// >> The decoding key is 755432125.
// >> The plain text is 23057.
int main(void){
  long long N, e; // 公開鍵: 2つの素数からなる合成数N, 暗号鍵e
  long long c; // 暗号文c
  long long p, q, d; // 秘密鍵: Nの素因数p,q, 復号鍵d
  long long m; // 平文m

  // 公開鍵の入力
  cout << "Input public keys, N" << endl;
  cin >> N;
  cout << "Input public keys, e" << endl;
  cin >> e;

  // 暗号文の入力
  cout << "Input a cryptogram, c" << endl;
  cin >> c;

  // Nの素因数分解
  //
  // Input: 
  //     N (long long) : 公開鍵
  // Output:
  //     p (long long) : Nの最小の素因数
  p = Factoring(N);
  q = N/p;

  // 復号鍵dを求める。
  // dはφ(n)(phi_n)と暗号鍵eとの間に以下の関係式が成り立つ。
  //    d*e (modφ(n)) = 1
  // 拡張ユークリッドの互除法を用いて、最大公約数を計算する過程で求めることができる。
  long long phi_n = (p-1)*(q-1);
  long long x, y;

  // 最大公約数の計算
  // dを求める際に必要なのは、最大公約数が求まった時のxの値である。
  //
  // Input:
  //   a (long long) : 暗号鍵e
  //   b (long long) : phi_n
  // Output:
  //   gcd (long long) : 1 (eとphi_nの最大公約数)
  long long gcd = ExtGCD(e, phi_n, x, y);

  // eとphi_n(φ(n))が互いに素でなければエラー
  if(gcd != 1){
    cout << "ERROR: e and φ(n) are not relatively prime.";
    return 0;
  }

  d = x;
  if(d<0){ d += phi_n; }

  // 復号鍵dの出力
  cout << "The decoding key is " << d << "." <<endl;

  // 平文の復号
  // 
  // Input:
  //     c (long long) : 暗号文
  //     d (long long) : 復号鍵
  //     N (long long) : 公開鍵
  // Output:
  //     rest (long long) : 平文m
  m = Decoding(c, d, N);

  // 平文mの出力
  cout << "The plain text is " << m << "." << endl;
  return 0;
}
