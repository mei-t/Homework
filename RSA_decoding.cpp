// RSA暗号を解読するプログラム
// 公開鍵から素因数分解と拡張ユークリッドの互除法を用いて解読する。
#include <iostream>
#include <float.h>
using namespace std;

long long N; // 公開鍵N

// 素因数分解をする関数
// 素因数分解できる数であるが前提なので、inputは最小の合成数である
// 4以上として一般性を失わない。
// 「合成数Nはp<=√Nを満たす素因数pを持つ」という性質を用いて最適化した。
// ref https://excelmath.atelierkobato.com/composite/
//
// Input: なし(Nのみを用いる)
// Output: Nの素因数p, q (ただしp<q)
long long Factoring(){

  // この方法でも可能であるが、時間計算量が大きくなるので他の方法を採用する。
  // if(N%2==0){return 2;}
  // for(long long i=3; i*i<=N; i+=2){
  //   if(N%i == 0){return i;}
  // }

  // 最適化するためにEratosthenesの篩の考え方を用いた。
  // 2,3,5を素因子にもつ30の周期について考えると、素数になりうるのは
  //   30*n + a (a = 1, 7, 11, 13, 17, 23, 29)
  // となる。したがって、2,3,5の倍数でないかをはじめに調べ、その後は上記のaについて調べる。
  if(N%2 == 0){ return 2; }
  else if(N%3 == 0){ return 3; }
  else if(N%5 == 0){ return 5; }
  for(long long i = 0; i*i*30 <= N; i++){
    if(N%(30*i+1) == 0 && i != 0){ return 30*i+1; } // i=0の時、1となるので不適
    else if(N%(30*i+7) == 0){ return 30*i+7; }
    else if(N%(30*i+11) == 0){ return 30*i+11; }
    else if(N%(30*i+13) == 0){ return 30*i+13; }
    else if(N%(30*i+17) == 0){ return 30*i+17; }
    else if(N%(30*i+19) == 0){ return 30*i+19; }
    else if(N%(30*i+23) == 0){ return 30*i+23; }
    else if(N%(30*i+29) == 0){ return 30*i+29; }
  }

  // 素因数分解できなかった場合
  cout << "ERROR: Factoring is failed." << endl;
  return 0;
}

// φ(n)(phi_n)と暗号鍵eから以下の式を満たす復号鍵dを求める。
//    d*e (modφ(n)) = 1
// 拡張ユークリッドの互除法を用いて求める
//
// この関数はeとphi_nの最大公約数を求める。RSA暗号では常に1となる。
// dを求める際に必要なのはこの過程の戻り値が返された時のyの値である。
// Input(最初):
//   a = e(暗号鍵)
//   b = phi_n
// Output(最後):
//   1(eとphi_nの最大公約数)
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

// cをd乗してNで割った余りを求める
long long mypow(long long c, long long d){
  if(d == 0){ return 1; }
  long long rest = mypow(c * c % N, d/2);
  if(d%2 == 1){
    rest = rest * c % N;
  }
  return rest;
}

// 復号をする関数
long long Decoding(long long c, long long d){
  long long m  = mypow(c, d);
  return m;
}

int main(void){
  long long e, c; // 暗号鍵e, 暗号文c.
  long long p, q, d; // 秘密鍵: Nの素因数p,q, 復号鍵d
  long long m; // 平文m

  // 公開鍵の入力
  cout << "Input public keys, N" << endl;
  cin >> N;
  cout << "Input public keys, e" << endl;
  cin >> e;
  cout << "Input a cryptogram, c" << endl;
  cin >> c;

  // Nの素因数分解
  p = Factoring();
  q = N/p;
  cout << "p: " << p << " q: " << q << endl;

  // 復号鍵dを求める
  long long phi_n = (p-1)*(q-1);
  long long x, y;
  ExtGCD(e, phi_n, x, y);
  d = x;
  if(d<0){ d += phi_n; }
  cout << d << endl;

  // 平文の復号
  m = Decoding(c, d);
  cout << m << endl;
  return 0;
}
