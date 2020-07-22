% Last revized 7/22/20
% Samuel Cabrera Valencia
% LAIR Lab OCAP project Summer 2020
% MATLAB script that for the purpose of evaluating the quality of images
% sent through an XBee hooked up to an arduino for use evaluation of
% underwater attenuation. The Script takes two input vectorized images
% that comprise each comprising of 3 image vectors that are 480p x 640p
% in greyscale ranging from 0-255 for per pixel, concatenated and broken
% up with a 256 digit placed between the first and second vector and the
% second and third vector. The first vector is expected to be a refference 
% vector and the second one is the one to be evaluated. This script will 
% then break each long vector into 3 seperate image vectors and compare the
% input to the refferece, both reproducing all input images and calcualting
% Mean-Square-Error and Peak Signal-to-Noise Ratio (PSNR)
% TODO: ??? 
clear all
%import ref vector and Rx vector as one long file 
imagev1 = importdata("256modo.txt"); %Original Vec
imagev2 = importdata("datalog.txt");

%Initialize all the seperate image files as empty vectors
v1_1 = zeros(307200,1);
v1_2 = zeros(307200,1);
v1_3 = zeros(307200,1);
v2_1 = zeros(307200,1);
v2_2 = zeros(307200,1);
v2_3 = zeros(307200,1);

%Run function to 'chop' the long vectors into 3 seperate
%vectors, start and stop of each vector is demarked by a 256 digit
%Function documentation at the end of the document
[v1_1, v1_2, v1_3] = vec_chop(imagev1,v1_1, v1_2, v1_3);
[v2_1, v2_2, v2_3] = vec_chop(imagev1,v2_1, v2_2, v2_3);

%Runs the function that fills the vectors if any bytes are missing with 0's
%Function Documentation at the end of the document
v1_1f = vec_prep(v1_1);
v1_2f = vec_prep(v1_2);
v1_3f = vec_prep(v1_3);
v2_1f = vec_prep(v2_1);
v2_2f = vec_prep(v2_2);
v2_3f = vec_prep(v2_3);

%Reshape vectors into 480x640 matrices
image1_1 = reshape(v1_1f,480,640);
image1_2 = reshape(v1_2f,480,640);
image1_3 = reshape(v1_3f,480,640);
image2_1 = reshape(v2_1f,480,640);
image2_2 = reshape(v2_2f,480,640);
image2_3 = reshape(v2_3f,480,640);

%Calculates each image's PSNR and MSE and prints it 
meanSqError1 = immse(image2_1, image1_1); 
[peaksnr1, snr1] = psnr(image2_1, image1_1);
meanSqError2 = immse(image2_2, image1_2); 
[peaksnr2, snr2] = psnr(image2_2, image1_2);
meanSqError3 = immse(image2_3, image1_3); 
[peaksnr3, snr3] = psnr(image2_3, image1_3);
fprintf("Mean Square Error 1: %d \nPeak Signal-to-Noise Ratio 1: %d \nMean Square Error 2: %d \nPeak Signal-to-Noise Ratio 2: %d \nMean Square Error 3: %d \nPeak Signal-to-Noise Ratio 3: %d", meanSqError1, peaksnr1,meanSqError2, peaksnr2,meanSqError3, peaksnr3);

%Recontructs all the images and displays them
subplot(2,3,1)
imagesc(image1_1)
title('Ref Image 1')
subplot(2,3,2)
imagesc(image1_2)
title('Ref Image 2')
subplot(2,3,3)
imagesc(image1_3)
title('Ref Image 3')
subplot(2,3,4)
imagesc(image2_1)
title('Tx Image 1')
subplot(2,3,5)
imagesc(image2_2)
title('Tx Image 2')
subplot(2,3,6)
imagesc(image2_3)
title('Tx Image 3')
colormap gray

%This function scrubs through the cut vectors and both replaces any digits
%over 255 or under 0 that may have been caused by errors in transmisission
%or hardware 
function fix_vec = vec_prep(vec)
for i = 1:size(vec)
    if vec(i,1) > 255 || vec(i,1) < 0
        vec(i,1) = 0;
    end
    fix_vec = vec;
end
end

%This Function scrubs through the long vector and chops it into 3 seperate
%vectors. This is demarked by a 256 at the end of the first and second
%vector. 
function [vec1, vec2, vec3] = vec_chop(vec,vec1,vec2,vec3)
i = 1;
for counter = 1:3
switch counter 
    case 1
    while vec(i,1) ~= 256
     vec1(i,1) = vec(i,1);
     i = i + 1;
    end
    case 2 
      while vec(i+1,1) ~= 256
      vec2(i-307200,1) = vec(i+1,1);
      i = i + 1;
      end
    case 3
        vec3(1:size(vec3),1) = vec(i+2:size(vec),1);
end
end
end