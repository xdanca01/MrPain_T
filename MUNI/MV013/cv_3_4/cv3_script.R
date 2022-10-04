#install.packages("jpeg")
library(jpeg)
my_image <- readJPEG("./picture.jpg")
rasterImage(my_image, 0, 0, 467, 700)

par(mfrow, 3)
for(i in 1:3){
  hist(my_image[,,i], probability = TRUE)
}
