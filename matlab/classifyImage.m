function [] = classifyImage(img,i)

t = imread('redtriangle.jpg');
t2 = preprocessImage(t);
t3 = t2(:,:,2);
tri = bwdist(t3);

t = imread('redsquare.jpg');
t2 = preprocessImage(t);
t3 = t2(:,:,2);
squ = bwdist(t3);

t = imread('redcircle.jpg');
t2 = preprocessImage(t);
t3 = t2(:,:,2);
cir = bwdist(t3);

t = imread('blueperson.jpg');
t2 = preprocessImage(t);
t3 = t2(:,:,1);
per = bwdist(t3);

t = imread('greenface.jpg');
t2 = preprocessImage(t);
t3 = t2(:,:,1);
%t3 = edge(t3, 'canny');
fac = bwdist(t3);

i2 = preprocessImage(img);
i3 = i2(:,:,i);
%i3 = edge(i3, 'canny');
D = bwdist(i3);

sum(sum(abs(minus(D,tri))))
sum(sum(abs(minus(D,squ))))
sum(sum(abs(minus(D,cir))))
sum(sum(abs(minus(D,per))))
sum(sum(abs(minus(D,fac))))

%figure;
%imshow(mat2gray(fac));
%figure;
imshow(mat2gray(D));
