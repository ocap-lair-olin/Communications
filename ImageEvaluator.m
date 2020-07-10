% Last revized 7/10/20
% Samuel Cabrera Valencia
% LAIR Lab OCAP project Summer 2020
% MATLAB script that for the purpose of evaluating the quality of images
% transmitted through RF when underwater, this script takes in the original
% image and a datalog file saved by the RX module, and reproduces both
% images, finds and plots the pixel-by-pixel difference, and calculates the
% Mean-Square-Error and Peak Signal-to-Noise Ratio (PSNR)
% TODO: ??? 
clear all 
imagev1 = importdata("ogvector.txt"); %Original Vec
imagev2 = importdata("datalog.txt");%transmitted Vec
%reshapes vectors into their appropriate grey-scale matrices 
image1 = reshape(imagev1,480,720);
image2 = reshape(imagev2,480,720);
imageSize = 720 .* 480;
pixeldiff = imagev1 - imagev2; %pixel-by-pixel difference
meanSqError = sum(reshape(pixeldiff, 720, 480),'all')/imageSize; %formula for mean square
PSNR = 10 .* log10((255.^2)/meanSqError); %formula for PSNR
subplot(2,3,1);
imagesc(image1);
title("original image");
subplot(2,3,2);
imagesc(image2);
title("Image transmitted through RF (900MHz)");
colormap gray;
subplot(2,3,3);
plot(pixeldiff);
title("difference of pixels");

fprintf("Mean Square Error: %d \nPeak Signal-to-Noise Ratio: %d", meanSqError, PSNR);