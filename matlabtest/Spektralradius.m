% Spektralradius von der Matrix A
% HSR -- Mathematisches Seminar / green's funtion
% Datum: 05.05.2014
% Andreas Linggi

clear;clc;
n = 5;        %dimension von f
T = [];
for i=1:n-1
    T(i,i+1)=1;
end;
for i=2:n
    T(i,i-1)=1;
end;
T = T+ eye(n)*-4;
E = eye(n);
Z = zeros(n);

A = [T E Z Z Z;
     E T E Z Z;
     Z E T E Z;
     Z Z E T E;
     Z Z Z E T];

M = tril(A);
N = A-M;
C = pinv(M)*N;
Spektralradius1 = max(eig(C));


fprintf('Spektralradius von A(5^2 x 5^2): %4.5f\n',Spektralradius1);

n = 10;        %dimension von f
T = [];
for i=1:n-1
    T(i,i+1)=1;
end;
for i=2:n
    T(i,i-1)=1;
end;
T = T+ eye(n)*-4;
E = eye(n);
Z = zeros(n);

A2 =[T E Z Z Z Z Z Z Z Z;
     E T E Z Z Z Z Z Z Z;
     Z E T E Z Z Z Z Z Z;
     Z Z E T E Z Z Z Z Z;
     Z Z Z E T E Z Z Z Z;
     Z Z Z Z E T E Z Z Z;
     Z Z Z Z Z E T E Z Z;
     Z Z Z Z Z Z E T E Z;
     Z Z Z Z Z Z Z E T E;
     Z Z Z Z Z Z Z Z E T];

M2 = tril(A2);
N2 = A2-M2;
C2 = pinv(M2)*N2;
Spektralradius2 = max(eig(C2));

fprintf('Spektralradius von A(10^2 x 10^2): %4.5f\n',Spektralradius2);