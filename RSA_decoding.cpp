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
#include <iostream>
#include <float.h>
using namespace std;

// 与えられた合成数Nに対し、Nが持つ最小の素因数を返す。
// 今回はRSA暗号の解読に用いる目的なので、素因数分解できる数であるが前提であり、
// inputは最小の合成数である4以上として一般性を失わない。
//
// Input: 
//     N (long long) : 素因数分解をしたい自然数
// Output:
//     p (long long) : Nの最小の素因数
long long Factoring(long long N){

  // 最適化するためにEratosthenesの篩の考え方を用いた。
  // 2,3,5を素因子にもつ30の周期について考えると、素数になりうるのは
  //   30*n + a (a = 1, 7, 11, 13, 17, 23, 29)
  // となる。したがって、2,3,5の倍数でないかをはじめに調べ、その後は上記のaについて調べる。
  if(N%2 == 0){ return 2; }
  else if(N%3 == 0){ return 3; }
  else if(N%5 == 0){ return 5; }

  // 30*i+res[] = 1 を除くため1ではなく、31を用いる。
  long long res[]={7,11,13,17,19,23,29,31};

  // 「合成数Nはp<=√Nを満たす素因数pを持つ」という性質を用いた。
  // ref https://excelmath.atelierkobato.com/composite/
  for(long long i = 0; i*i*900 <= N; i++){
    for(long long j=0; j<8;j++){
      if(N%(30*i+res[j])==0){ return 30*i+res[j]; }
    }
  }

  // 素因数分解できなかった場合RSA暗号の解読ができないのでエラー
  cout << "ERROR: Factoring is failed." << endl;
  return 0;
}

// 与えられた正整数 a, b に対し，a と b の最大公約数 gcd を求めて返す。
// それと同時に，ax + by = gcd をみたす整数の組 (x,y) を1つ求め，x, y に代入する。
// （拡張ユークリッドの互除法）
// Input:
//     a, b (long long) : 最大公約数を求めたい2つの自然数
//     x, y (long long)：ax + by = gcd の解 (x,y) を代入するための変数
// Output:
//     gcd (long long)：a と b の最大公約数(RSA暗号では常に1となる。)
long long ExtGCD(long long a, long long b, long long& x, long long& y){
  long long gcd = a;
  if(b == 0){
    x = 1;
    y = 0;
  }else{
    gcd = ExtGCD(b, a%b, y, x);
    y -= (a/b) * x;
  }
  return gcd;
}

// 復号鍵を用いて暗号文を復号する関数
// cをd乗してNで割った余りを求める。
long long Decoding(long long c, long long d, long long N){
  if(d == 0){ return 1; }
   long long rest = Decoding(c * c % N, d/2, N);
   if(d%2 == 1){
     rest = rest * c % N;
  }
  return rest;
}

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
