% Spektralradius von der Matrix A
% HSR -- Mathematisches Seminar / green's funtion
% Parametrisiert nur mit n
% Datum: 05.05.2014
% Andreas Linggi

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
