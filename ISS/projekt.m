#nacteni
[y,fs] = audioread('xbilyd01.wav');
#vzorkovaci frekvence
fs;
#delka ve vzorcich
samples = numel(y);

#delka v sekundach
time = samples/fs; 

#pocet symbolu
symbols = samples/16;
#prevod wav do binarky
values1(1) = y(8);
values2 = y(24:16:32000);
values = [values1; values2];
binary = [];
values = values';

for i = values
  if (i <=0)
    binary = [binary; 0];
  else
    binary = [binary; 1];
  endif
 endfor

 #srovnani txt a wav
 f = load('xbilyd01.txt');
 xor(binary, f);
 
 #casovy prubeh
 #0.02s = 320 vzorku / 16000(fs)
counter = 1:321;
t = counter./fs;
y_part = y(1:321);

t_binary = t(8);
t_binary = [t_binary t(24:16:321)];
#2000 vzorku na 2s, 20 vzorku na 0.02 s
binary_part = binary(1:20);

#hold on
#plot(t,y_part);
#stem(t_binary, binary_part);
#hold off

#filtr


b = [0.0192 -0.0185 -0.0185 0.0192];
a = [1.0000 -2.8870 2.7997 -0.9113];



function ukazmito(b,a,Fs)
% function ukazmito(b,a)
%
% - ukaze impulni odezvu 
% - ukaze kmit charku pro zadanou vzorkovaci frekvenci
% - ukaze poly a nuly a zjisti stabilitu 

N = 32; n = 0:N-1; imp = [1 zeros(1,N-1)]; 
h = filter(b,a,imp); 
#subplot(222); stem(n,h); grid; xlabel('n'); ylabel('h[n]')
H = freqz(b,a,256); f=(0:255) / 256 * Fs / 2; 
#plot(f,abs(H)); grid; xlabel('f'); ylabel('|H(f)|')
#subplot(212); plot (f,angle(H)); grid; xlabel('f'); ylabel('arg H(f)')
#plot(zplane (b,a)); 
p = roots(a); 
if (isempty(p) | abs(p) < 1) 
  disp('stabilni...')
else
  disp('NESTABILNI !!!')
endif
end

#ukazmito(b,a,fs);


#filtrovani
counter = 1:32000;
t = counter./fs;
ss = filter(b,a,y);
#hold on
#plot(t,y);
#plot(t,ss);
#hold off


ss_shifted = ss(16:numel(ss));
ss_shifted = [ss_shifted ; zeros(15,1)];
#hold off;
#hold on;
#plot(t,ss_shifted);
#plot(t,y);
#hold off;

#dekodovani ss_shifted
values1(1) = ss_shifted(8);
values2 = ss_shifted(24:16:32000);
values = [values1; values2];
binary_ss = [];
values = values';

for i = values
  if (i <=0)
    binary_ss = [binary_ss; 0];
  else
    binary_ss = [binary_ss; 1];
  endif
 endfor
 
  #0.02s = 320 vzorku / 16000(fs)
counter = 1:321;
t = counter./fs;
y_part = y(1:321);
ss_part = ss(1:321);
ss_shifted_part = ss_shifted(1:321);

t_binary = t(8);
t_binary = [t_binary t(24:16:321)];
#2000 vzorku na 2s, 20 vzorku na 0.02 s
binary_part = binary_ss(1:20);


#hold on
#stem(t_binary,binary_part);
#plot(t,y_part);
#plot(t,ss_part);
#plot(t,ss_shifted_part);
#hold off

#chybovost
err = xor(binary, binary_ss);
num_err = sum(err);
percent = num_err / numel(binary) *100;

#spektralni analyza

freq = (0 : samples / 2 - 1) / samples * fs;
fft_signal_ss = abs(fft(y));
fft_signal_ss = fft_signal_ss(1 : samples / 2);

fft_signal_ss_shifted = abs(fft(ss_shifted));
fft_signal_ss_shifted = fft_signal_ss_shifted(1 : samples / 2);

#hold on
#plot(freq, fft_signal_ss);
#plot(freq, fft_signal_ss_shifted);
#hold off


#rozdeleni pravdepodobnosti



#korelacni koeficienty
k = (-50 : 50);
R =xcorr(y,'biased') / samples;
R = R(k + samples);

