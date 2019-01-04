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