setwd("/home/pdancak/school/MrPain_T/MUNI/MV013/cv10")

prices<-read.csv("GoldSilver.csv", sep=",")
prices$X <- as.Date(prices$X)
plot(prices$X, prices$gold, type="l")
lm()



###### 2
data<-read.csv("Computers.csv", sep=",")
head(data)
#check datatypes
str(data)

#need to convert char to factor
data$cd <- as.factor(data$cd)
data$multi <- as.factor(data$multi)
data$premium <- as.factor(data$multi)
data$ram <- as.factor(data$ram)
data$screen <- as.factor(data$screen)

#b)
data$price <- as.numeric(data$price)
data$speed <- as.numeric(data$speed)
data$trend <- as.numeric(data$trend)

#c)
lm()
