# RSA暗号の解読
RSA暗号を素因数分解を用いることで、公開鍵のみから解読する。

## 使い方
* Input  
公開鍵 N, e  
暗号文 c  

* Output  
復号鍵 d  
平文 m  

* cipher.txt  
ターミナルで実行するときに**./a.out < cipher.txt**　と実行すると
```
The decoding key is 755432125.
The plain text is 23057.
```
と出力される。
