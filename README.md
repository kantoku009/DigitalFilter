Digital Filter
====================
#はじめに
デジタルフィルタを作成した。  
作成したフィルタは以下。

- FIR: Butterworth
- IIR: Chebycheff

----

#Butterworthフィルタ
whitenoizeをButterworthフィルタに通した結果を示す。

##ローパスフィルタ
フィルタの設定は以下のようにした。

- カットオフ周波数： 400[Hz]
- -48[db/oct]

![Butterworth Low-pass -48dB/oct](./img/Butterworth_lowpass_48dB.png)

##ハイパスフィルタ
フィルタの設定は以下のようにした。

- カットオフ周波数： 2000[Hz]
- -48[db/oct]

![Butterworth High-pass -48dB/oct](./img/Butterworth_highpass_48dB.png)

----

#Chebycheffフィルタ
##ローパスフィルタ
フィルタの設定は以下のようにした。

- カットオフ周波数： 400[Hz]
- -48[db/oct]

![Chebycheff Low-pass -48dB/oct](./img/Chebycheff_lowpass_48dB.png)

##ハイパスフィルタ
フィルタの設定は以下のようにした。

- カットオフ周波数： 2000[Hz]
- -48[db/oct]

![Chebycheff High-pass -48dB/oct](./img/Chebycheff_highpass_48dB.png)
