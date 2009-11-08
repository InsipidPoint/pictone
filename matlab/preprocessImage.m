function im = preprocessImage(i)

im = arrayfun(@(x) thresh(x), i);


function y = thresh(x)
if x > 100
  y = 0;
else
  y = 255;
end
