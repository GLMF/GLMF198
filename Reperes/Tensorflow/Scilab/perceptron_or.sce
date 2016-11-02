//perceptron OR
clear all;
funcprot(0);
exec('droite.sci'); //load the function in memory


x1min = -0.5;
x1max = 1.5;
x2min = -0.5;
x2max = 1.5;

X = [0 0 1 1;0 1 0 1];
T = [0 1 1 1];

i = 1

Err = 1;
k=10
fout = 0;

key=rand("normal");
key=rand("info");
disp(key);
W = rand(1,2); //initialisation aléatoire des poids
B = ones(1,4)*rand(1,1); //biais identique pour chaque entrée

while (sum(abs(Err)) ~= 0) //tant que l'erreur n'est pas nulle
    //activation
    WX = W*X;
    //ajout du biais
    WXb = WX + B;
    //calcul sortie par seuillage
    output = WXb > fout;
    //calcul erreur
    Err = T - output;
    //affichage
    droite(X,T,W,B(1),x1min ,x1max ,x2min ,x2max ,i)
    i = i+1;
    //mise à jour pour apprentissage
    dW = k * Err * X';
    dB = sum(Err)*ones(1,4);
    W = W + dW;
    B = B + dB;
end
disp('B=',B);
disp('W=',W);
