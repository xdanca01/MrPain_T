setwd("~/school/MrPain_T/MUNI/MV013/HW3")

library(fBasics)

data <- read.csv("race.txt", sep=";")
attach(data)
time <- sort(time)

print("Random variable is count of qualified competitors.")
print("We assume the data distribution follow binomial distribution.")

alpha <- 0.05
n <- length(time)
print("Null hypothesis H0: 20% of all competitors qualified (p = 0.2)")
print("Null hypothesis H0: number of competitors qualified is not 20% (p != 0.2)")
qualified <- length(time[time <= 30])

xx <- 0:100
my_binom <- dbinom(xx, n, 0.2)
hist(time[time<=30], main="Binomial distribution functions", freq=F, xlim=c(0,30), breaks=seq(20.5,30.5,by=1))
lines(xx, dbinom(xx, n, 0.2), col='red')
legend(0, 0.14, legend=c("DF with probability 20%"),
       col=c("red"), lty=1:2, cex=0.8)
k <- quantile(dbinom(xx, n, 0.2), 0.95)
my_binom[qualified+1]
fit_binom_optim <- function(x, par, n){
  return(sum(x)/(n*length(x)))
}

#P(X >= n)
X_largerEqual_n <- sum(my_binom[qualified:81])
print(paste("P(X >= n) =" , X_largerEqual_n, " which is higher than alpha", alpha,". We can't reject H0."))


finished <- hist(time[time <=30], breaks=seq(20.5,30.5,by=1), plot = FALSE)$counts
finished <- c(double(min(time)-1), finished, c(double(max(time)-30)))
my_mle <- function(par, x, n){
  -sum(dbinom(x, n, par))
}

prob_MLE <- optim(0.2, my_mle, x=sum(finished), n=n, method="Brent",lower=0,upper=1)$par
print(paste("MLE returned as probability value:", prob_MLE))
CI <- binom.test(qualified, n, 0.2)
print(paste("95 percent confidence interval: ", CI$conf.int[1], CI$conf.int[2]))

CLM <- function(par,x) {
  m<-par[1]
  sd<-par[2]
  -sum(dnorm(x,mean=m,sd=sd,log=T))
}

optim(c(3.2,2),CLM,x=sum(finished),method="BFGS")

## Problem 2
data <- read.csv("race.txt", sep=";")
attach(data)

year20 <- time[year == 2020]
year21 <- time[year == 2021]
print("We need to add some times to make the vectors same length.
We use mean value of year21.")
year21 <- as.integer(c(year21, double(6)+mean(year21)))

print("We define differences of 2020-2021 times as Z.")
print("H0: u = 0 (mean of Z is = 0. The times are same.)")
print("H1: u < 0 (mean of Z is < 0. the times improved)")
print("We use left sided one-sample t-test")

alpha <- 0.05
Z <- year20 - year21

xx <- -60:60
plot(ecdf(Z),main="Cumulative distribution function")
lines(xx,pnorm(xx,mean(Z),sd(Z)),col='red',lwd=3)
hist(Z, freq = F, main="Histogram of 2020 - 2021")
lines(xx, dnorm(xx, mean(Z), sd(Z)), col="red")
legend(23, 0.02, legend=c("Distribution function"), col=c("red"), lty=1, cex=0.8)
print("Based on ECDF we can deduce, that the times are following normal distribution.")



n <- length(time)
S <- sqrt(1/(n-1)*sum((Z-mean(Z))^2))
T <- sqrt(n)*mean(Z)/S
CR <- qt(alpha,n-1)
print(paste("test statistic T:", T, "Is not in critical region (-inf,", CR, "). Cant reject H0."))
p <- pt(abs(T), n-1)
print(paste("P value is:", p, "which is higher than alpha(0.05). Can't reject H0."))
CI <- mean(Z)+qt(alpha, n-1)*S/sqrt(n)
print(paste("Confidence interval is: (-Inf, ", CI, "). We cant reject H0"))
print("Final conclusion is, that we can't reject H0. The hyphothesis")
detach(data)

## Problem 3
data <- read.csv("race.txt", sep=";")
attach(data)
tab <- table(team, year, dnn=c("TEAM", "YEAR"))
tab2 <- addmargins(tab,c(1,2),sum)
tab2
chisq.test(tab)

tab_avgC <- colSums(tab)/nrow(tab)
tab_avgR <- rowSums(tab)/ncol(tab)
tab_avg <- mean(tab)
expected <- tab_avgR%*%t(tab_avgC)/tab_avg
residual <- tab-expected
r<-chisq.residuals(tab)
detach(data)
