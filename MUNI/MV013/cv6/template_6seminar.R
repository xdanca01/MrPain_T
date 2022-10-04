setwd("/home/pdancak/school/MrPain_T/MUNI/MV013/cv6")


library(fBasics)
library(nortest)
library(car)

source('functions.R')


#EXERCISE 1

data <- c(19.92, 21.17, 24.87, 22.90, 20.88, 19.43, 24.39, 23.14, 16.99, 24.85, 20.80, 19.93, 22.28, 27.61, 27.50, 19.63, 20.35, 22.96, 17.57, 22.91)
n <- length(data)


#task a 

#use graphical tools to check noramlity of our dataset
# Q-Q plot
#histogram + theoretical function with estimated par.
#kernel esimate + theoretical function with estimated par.
#plot of empirical estimate + cfd with esimated par.

#histogram plot
par <- optim(c(22.5,3),normal_fit,x=data)
x <- seq(15,30,by=0.01)
hist(data,xlim=c(15,30),freq = F,ylim=c(0,0.4), breaks = 6)
lines(x,dnorm(x,par$par[1],par$par[2]),col='red',lwd=3)

#Empirical distribution function plot - our
t<-table(data)
ct<-as.table(cumsum(t))
plot(ct / length(data)) 
lines(x,pnorm(x,par$par[1],par$par[2]), col = 'red')

#Empirical distribution function plot - built in function
plot(ecdf(data))
lines(x,pnorm(x,par$par[1],par$par[2]), col = 'red')

#Q-Q plot
qqnorm(data)
qqline(data)

#normality tests
normality_tests(data) 


#mean length is 20mm
#assume normality

#task b

#define a value of test statistic

val.test <- sqrt(n) * (mean(data) - 20) / sd(data)


#HA: mu <> 20
#define a critical region

val.test < -qt(1 - alpha/2,n-1) || val.test > qt(1-alpha/2,n-1)

p.val <- 2 * (1 -  pt(abs(val.test), n -1))

#compute a test statistic
# (you can use formulas from slides from lecture)

#task c
#plot of Student density function, and critical region

x <- seq(-6,6,0.01)
y <- dt(x, n - 1)
plot(x,y, type = 'l', col = 'red')

rect(-6,0,-qt(1-alpha/2, n- 1),1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
rect(qt(1-alpha/2, n - 1),0,6,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
abline(v = val.test, col = 'black')
text(val.test+2.5,0.1,"Value of test statistic")



#task d 

x <- seq(-6,6,0.01)
y <- dt(x, n - 1)
plot(x,y, type = 'l', col = 'red')

polygon(c(x[x>val.test],max(x),val.test), c(y[x>val.test],0,0), col = 'red' )
text(val.test+1,0.03,"p-value")



#EXERCISE 2


data <- read.csv('Computers.csv', sep = ',', header = TRUE)


#task a 
# -- SIMILARLY like in the previous example

#take b prices just for values for 2GB ram computers
ram_2 <- data$price[data$ram == 2]
par <- optim(c(1700,100),normal_fit,x=ram_2)
x <- seq(0,3000,by=1)
hist(ram_2,freq = F, breaks = 6)
lines(x,dnorm(x,par$par[1],par$par[2]),col='red',lwd=3)

n <- length(ram_2)
alpha <- 0.05

x_hat <- mean(ram_2)

t<-table(ram_2)
ct<-as.table(cumsum(t))
plot(ct / n) 
lines(x,pnorm(x,par$par[1],par$par[2]), col = 'red')

#Q-Q plot
qqnorm(ram_2)
qqline(ram_2)

#normality tests
normality_tests(ram_2) 

#task b
#val.test <- sqrt(n) * (mean(ram_2) - 394) / sd(ram_2)

#both-sided alternative 
#W <- val.test < -qt(1 - alpha/2,n-1) || val.test > qt(1-alpha/2,n-1)
#P <- 2 * min(c((1 -  pt(abs(val.test), n -1)), pt(abs(val.test), n -1)))
u1 <- qnorm(1-alpha/2)
etc <- alpha/2*sd(ram_2)/sqrt(n)
CI_L <- x_hat - u1 - etc
CI_R <- x_hat + u1 - etc
hist(ram_2,freq = F, breaks = 10, xlim=c(1300,2100), ylim=c(0,0.0016))
rect(CI_L,0,CI_R,0.0014,col=rgb(red=1, green=0, blue=0, alpha=0.8))
  
#left-sided alternative 


rect(-6,0,-qt(1-alpha/2, n- 1),1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)

#right-sided alternative
rect(qt(1-alpha/2, n - 1),0,6,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)

#task c
test_hyp1 <- function(x,mu.0, p.val){
}



#task d
test_hyp2 <- function(x,mu.0, p.val){
}


#task e
test_hyp3 <- function(x,mu.0, p.val){
}



#EXERCISE 3


data <- zeros_in_file('file.txt')

#use graphical tools to check noramlity of our dataset
# Q-Q plot
#histogram + theoretical function with estimated par.
#kernel esimate + theoretical function with estimated par.
#plot of empirical estimate + cfd with esimated par.

#task a

#use for file.txt
par <- optim(c(50,5),normal_fit,x=data)

hist(data, xlim = c(0,100), freq = FALSE)
lines(0:100, dnorm(0:100, par$par[1], par$par[2]), col = 'red', lwd = 2)

plot(density(data), col = 'red', ylim = c(0,0.09))
lines(0:100, dnorm(0:100, par$par[1], par$par[2]), col = 'blue', lwd = 2)

plot(ecdf(data))
lines(0:100, pnorm(0:100, par$par[1], par$par[2]), col = 'red', lwd = 2)

qqnorm(data)
qqline(data, col = 'red', lwd = 2)

#Then try your function normality test 

normality_tests(data) #could not reject Normality of our data


#task b 

#Our function for Q-Q plot
#QQ plot 
my_qqPlot <- function(x, beta, n , p ){
  x <- sort(x)
  alpha <- (seq(1, n)-beta)/ (n + 1 -2*beta)
  F_alpha <- qnorm(alpha)
  plot(F_alpha,x)
  
}

my_qqPlot(data,0.5, length(data))
my_qqPlot(data,0.375,length(data))
abline(a = mean(data), b = mad(data), col = 'red') 







