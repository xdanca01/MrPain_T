setwd("/home/pdancak/school/MrPain_T/MUNI/MV013/HW2")

library(corrplot)
library(ggfortify)

data <- read.csv("happiness.csv",sep=';')
data$Continent.Name <- as.factor(data$Continent.Name)

#Problem 1
corr_mat <- cor(data[3:8])
corrplot(corr_mat, order = "hclust", addrect = 3)


#Problem 2
my_pca <- prcomp(data[3:8], center=T, scale=T)
s <- summary(my_pca)
plot(s$importance[3,],main="Cumulative Proportion of variance",ylab="proportion",type='o')
#By the first component is explained around 50% variance and by second + first 75%

#Problem 3
autoplot(my_pca, data = data,loadings = TRUE,loadings.label = TRUE, col.ind = data$Continent.Name, colour = "Continent.Name")

#Problem 4
#my_eigen <- eigen(cov(data[3:8]))
#c1<-my_eigen$vectors[,1]
#c2<-my_eigen$vectors[,2]
#data$c1<-as.matrix(data[3:8]) %*% c1
#data$c2<-as.matrix(data[3:8]) %*% c2
#plot(kmeans(data$c1, 3),kmeans(data$c2, 3),xlab="First component",ylab="Second component", col=data$Continent.Name, pch = 19)
k <- kmeans(my_pca$x, 3)
autoplot(k, data = my_pca$x, frame = TRUE)

#Problem 5
#Rows are continents, cols are clusters
freq_table <- matrix(0, nrow = 5, ncol = 3, dimnames = list(levels(data$Continent.Name), c(1,2,3)))
states_clusters <- matrix(1:42, nrow = 42, ncol = 1, dimnames = list(data[data$Continent.Name == "Europe",]$Country.or.region, "Cluster"))
for(i in 1:length(k$cluster)){
  my_row <- as.integer(data[i,]$Continent.Name)
  my_col <- k$cluster[i]
  freq_table[my_row, my_col] <- freq_table[my_row, my_col] + 1
  if(data[i,]$Continent.Name == "Europe"){
    states_clusters[data[i,]$Country.or.region,1] <- k$cluster[i]
  }
}
freq_table
states_clusters

#Problem 6
par(mfrow = c(2, 3))
all_sd <- sd(data$Score)
all_mean <- mean(data$Score)
breaks <- 10
xx <- seq(3, 8,by=0.01)
for(i in levels(data$Continent.Name)){
  hist(data[data$Continent.Name == i,]$Score, main=i, xlab = "Score", xlim = c(3,8), breaks = breaks, freq=F)
  cont_mean <- mean(data[data$Continent.Name == i,]$Score)
  stan_deviation <- sd(data[data$Continent.Name == i,]$Score)
  y <- dnorm(xx, cont_mean, stan_deviation)
  lines(xx, y, col='red')
}
y <- dnorm(xx, mean = all_mean, sd = all_sd)
hist(data$Score, xlab = "Score", main="All", xlim = c(3,8), breaks = breaks, freq=F)
lines(xx, y, col='red')

