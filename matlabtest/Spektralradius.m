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
Spektralradius1 = max(abs(eig(C)));


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
Spektralradius2 = max(abs(eig(C2)));

fprintf('Spektralradius von A(10^2 x 10^2): %4.5f\n',Spektralradius2);

%% Parametrisiert nur mit n
clear;
n = 100
n2 = n^2
A = zeros(n2);
for i=1:n2
    for j=1:n2
        if j==i
           A(i,j) = -4;
        end;
        if (i == j-1 || (i==j+1))
            if ~((mod(j,n)==0 && (mod(i-1,n)==0)) || (mod(j-1,n)==0 && (mod(i,n)==0)))
            A(i,j)=1;
            end;
        end;
        if (i == j-n || (i==j+n))
            A(i,j)=1;
        end;
    end;
end;
M = tril(A);
N = A-M;
C = pinv(M)*N;
Spektralradius1 = max(abs(eig(C)));


fprintf('Spektralradius von A(5^2 x 5^2): %4.5f\n',Spektralradius1);
