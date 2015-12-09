# nali-c
A fast tool handles text stream and add ip geolocation info on it

### Requirements


1. First download http://update.cz88.net/soft/setup.zip, extract the zip and exe (use innoextract), and put `QQWry.Dat` to `/usr/local/share/QQWry.Dat`
2. Then `make && make install`

nali-update need unzip and innoextract to extract zip and exe files

Mac user can install theme with
```
brew install unzip innoextract
```

### Usage



``` shell
➜  ~  nali 8.8.8.8
8.8.8.8[美国 加利福尼亚州圣克拉拉县山景市谷歌公司DNS服务器]
➜  ~  nali
8.8.8.8
8.8.8.8[美国 加利福尼亚州圣克拉拉县山景市谷歌公司DNS服务器]
➜  ~
```



### Advanced

add these lines to `.bashrc`

``` shell
alias ping='function _ping(){ ping $@ | nali; }; _ping'
alias mtr='function _mtr(){ mtr $@ | nali; }; _mtr'
alias dig='function _dig(){ dig $@ | nali; }; _dig'
alias nslookup='function _nslookup(){ nslookup $@ | nali; }; _nslookup'
```

then you can use dig like these

``` shell
➜  ~  dig twitter.com

; <<>> DiG 9.8.3-P1 <<>> twitter.com
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 14905
;; flags: qr rd ra; QUERY: 1, ANSWER: 2, AUTHORITY: 4, ADDITIONAL: 6

;; QUESTION SECTION:
;twitter.com.			IN	A

;; ANSWER SECTION:
twitter.com.		79852	IN	A	104.244.42.193[美国 Twiiter公司]
twitter.com.		79852	IN	A	104.244.42.129[美国 Twiiter公司]

...more
```

And any out put can be piped into nail-ng (super fast!)

``` 
cat xxx.txt | nali
sudo pdnsd-ctl dump | nali
```
