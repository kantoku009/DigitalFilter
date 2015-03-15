Digital Filter
====================
#はじめに
C++言語にてデジタルフィルタを作成した。  
作成したフィルタは以下。

- Butterworth
- Chebycheff

WAVEファイルを読み込んで、デジタルフィルタに通すようにした。  
全ソースコードのダウンロードは以下から行える。

- [DigitalFilter](https://github.com/kantoku009/DigitalFilter/archive/master.zip)

----

#ビルド
Unix系のOSでビルドするのを想定している。  
以下コマンドが必要。

- g++
- make

zipファイルを解凍し、コマンドラインから以下のディレクトリに移動。  

 - %cd ./source

makeを実行する。  

 - %make

以下の実行ファイルが出来上がる。

 - myfilter

使い方は以下。  

 - %./myfilter [WAVEファイル名] [フィルタ種類]  

フィルタ種類は以下数値を指定する。

 - 0 : Butterworth ローパスフィルタ
 - 1 : Butterworth バンドパスフィルタ
 - 2 : Butterworth ハイパスフィルタ
 - 3 : Chebycheff ローパスフィルタ
 - 4 : Chebycheff バンドパスフィルタ
 - 5 : Chebycheff ハイパスフィルタ

各フィルタの設定値は以下のファイルに記載されている。  

 - ./config/ButterworthLowPassFilter.conf
 - ./config/ButterworthBandPassFilter.conf
 - ./config/ButterworthHighPassFilter.conf
 - ./config/ChebycheffLowPassFilter.conf
 - ./config/ChebycheffBandPassFilter.conf
 - ./config/ChebycheffHighPassFilter.conf

----

#Butterworthフィルタ
ホワイトノイズをButterworthフィルタに通した結果を周波数解析した。

##ローパスフィルタ
###設定
Butterworthローパスフィルタの設定値は、以下のファイルに書かれている。  

 - ./config/ButterworthLowPassFilter.conf

フィルタの設定は、デフォルトでは以下のようにした。

- カットオフ周波数： 400[Hz]
- 1オクターブの減衰率： -48[db/oct]

![Butterworth Low-pass -48dB/oct](./img/Butterworth_lowpass_48dB.png)

##バンドパスフィルタ
###設定
Butterworthバンドパスフィルタの設定値は、以下のファイルに書かれている。  

 - ./config/ButterworthBandPassFilter.conf

フィルタの設定は、デフォルトでは以下のようにした。

- カットオフ周波数： 400[Hz]
- カットオフ周波数： 2000[Hz]
- 1オクターブの減衰率： -48[db/oct]

![Butterworth Band-pass -48dB/oct](./img/Butterworth_bandpass_48dB.png)

##ハイパスフィルタ
###設定
Butterworthハイパスフィルタの設定値は、以下のファイルに書かれている。  

 - ./config/ButterworthHighPassFilter.conf

フィルタの設定は、デフォルトでは以下のようにした。

- カットオフ周波数： 2000[Hz]
- 1オクターブの減衰率： -48[db/oct]

![Butterworth High-pass -48dB/oct](./img/Butterworth_highpass_48dB.png)

----

#Chebycheffフィルタ
ホワイトノイズをButterworthフィルタに通した結果を周波数解析した。

##ローパスフィルタ
###設定
Chebycheffローパスフィルタの設定値は、以下のファイルに書かれている。  

 - ./config/ChebycheffLowPassFilter.conf

フィルタの設定は、デフォルトでは以下のようにした。

- カットオフ周波数： 400[Hz]
- 1オクターブの減衰率： -48[db/oct]

![Chebycheff Low-pass -48dB/oct](./img/Chebycheff_lowpass_48dB.png)

##バンドパスフィルタ
###設定
Chebycheffバンドパスフィルタの設定値は、以下のファイルに書かれている。  

 - ./config/ChebycheffBandPassFilter.conf

フィルタの設定は、デフォルトでは以下のようにした。

- カットオフ周波数： 400[Hz]
- カットオフ周波数： 2000[Hz]
- 1オクターブの減衰率： -48[db/oct]

![Chebycheff Band-pass -48dB/oct](./img/Chebycheff_bandpass_48dB.png)

##ハイパスフィルタ
###設定
Chebycheffハイパスフィルタの設定値は、以下のファイルに書かれている。  

 - ./config/ChebycheffHighPassFilter.conf

フィルタの設定は、デフォルトでは以下のようにした。

- カットオフ周波数： 2000[Hz]
- 1オクターブの減衰率： -48[db/oct]

![Chebycheff High-pass -48dB/oct](./img/Chebycheff_highpass_48dB.png)

----

#参考情報
##ホワイトノイズ
ホワイトノイズを周波数解析した。  
ホワイトノイズは全ての周波数に対して同じ強度となる性質を持っている。  
その為、ホワイトノイズをフィルタに通すことによって、フィルタの性能を確認することができる。  
![whitenoise](./img/WhiteNoise.png)

ホワイトノイズのファイルは、以下のサイトから借用した。  
[High Quality White Noise | Play & Download .wav .mp3 Audio Files](http://www.audiocheck.net/testtones_whitenoise.php)

##周波数解析
周波数解析には音声編集ソフトWavePadというアプリケーションを利用した。  
以下のサイトからダウンローが可能。  
[WavePad 音声編集ソフト](http://www.nch.com.au/wavepad/jp/index.html?gclid=CLbS3J3y9MMCFRWSvQodjjYAqw)

##デジタルフィルタ
###サイト
デジタルフィルタに関しては以下サイトの[実用ディジタル信号処理講座 第3章 デジタルフィルタの基礎](http://www.sound.sie.dendai.ac.jp/dsp/Text/PDF/Chap3.pdf)を参考にした。  
[音響情報研究室](http://www.sound.sie.dendai.ac.jp/index-j.html)

###書籍
以下の書籍を参考にした。  
[「デジタルフィルタ (ビギナーズ)」  中村 尚五 (著)](http://www.amazon.co.jp/dp/4501313501/)

