setwd("~/school/MrPain_T/MUNI/MV013/HW3")

library(fitdistrplus)
library(fBasics)

data <- read.csv("race.txt",sep=';')
data$time <- sort(data$time)
#time <- data$time[data$time < 60]

## Problem 1

print("Random variable is finish time of competitors")
n <- length(data$time)
xx <- 0:100
#Break sturges rule
num_breaks <- log2(n)+1
hist(data$time, breaks = num_breaks, freq=FALSE)
#Normal distr
lines(xx, dnorm(xx, mean(data$time), sd(data$time)), col='red')
#lines(xx, dgamma(xx, 11, rate=0.27), col="blue")

qqnorm(data$time)
qqline(data$time, col='red')
plot(ecdf(data$time), main="Cumulative df")
lines(xx, pnorm(xx, mean(data$time), sd(data$time)), col='red')

normality_tests(data$time)

print("Based on the graphical visualisation in CDF, QQPlot and histogram we can't say much so we use statistical tests.")
print(pchiTest(data$time))
print("Based on statistical test we can see that our p-value is higher than 0.05 so we can assume that our data come from normal distribution.")

mi <- mean(data$time)



##H0 - Only 20% of all competitors qualified.
print("Null hyphothesis H0: Only 20% of all competitors qualified")
##H1 - Percentage of qualified competitors is not 20%.
print("Alternative hyphothesis H1: More than 20% of all competitors qualified")



###Problem 2
year2020 <- data$time[data$year == 2020]
year2021 <- data$time[data$year == 2021]
