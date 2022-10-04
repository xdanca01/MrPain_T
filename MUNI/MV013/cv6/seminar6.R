#Normality tests
library(fBasics)

normality_tests<-function(x){
  #fBasics package 
  #https://www.rdocumentation.org/packages/fBasics/versions/240.10068.1/topics/OneSampleTests
  #https://rdrr.io/cran/fBasics/src/R/test-normalityTest.R (implementation)
  if (length(x)>20){ #at least 20 observation
    print(dagoTest(x))     
    message('#####################')
  }
  print(jarqueberaTest(x))  
  message('#####################')
  print(shapiroTest(x))   
  message('#####################')
  if (length(unique(x))==length(x)){#ties should not be present for the Kolmogorov-Smirnov test
    print(ks.test(data.length,"pnorm",mean(x),sd(x))) 
  }
  message('#####################')
  print(lillieTest(x))
  message('#####################')
  print(cvmTest(x))
  message('#####################')
  print(adTest(x))
  message('#####################')
  print(pchiTest(x)) #adhusted p-value
  
}


#TASK 1
data.length<-c(19.92, 21.17, 24.87, 22.90, 20.88, 19.43, 24.39, 23.14, 16.99, 24.85, 20.80, 19.93, 
               22.28, 27.61, 27.50, 19.63, 20.35, 22.96, 17.57, 22.91)


#subtask a
normality_tests(data.length)

#Q-Q plot (step by step)
sample<-sort(data.length)
n<-length(data.length)
i<-1:n
beta<-0.5
alpha<-(i-beta)/(n+1-2*beta)
theoretical.quantiles<-qnorm(alpha,mean(data.length),sd(data.length))
plot(theoretical.quantiles,sample,main="Q-Q plot")
#Q-Q line
least_sqaure<-lm(sample~theoretical.quantiles)
xx<-seq(16,28,by=0.001)
lines(xx,least_sqaure$coefficients[1]+least_sqaure$coefficients[2]*xx)

#Q-Q plot ->function
qqnorm(data.length) 
qqline(data.length) 

#ECDF
plot(ecdf(data.length),main="Cumulative distribution function")
lines(xx,pnorm(xx,mean(data.length),sd(data.length)),col='red',lwd=3)

#P-P plot
cdf<-ecdf(data.length)
adj.empirical.cdf<-n/(n+1)*cdf(data.length)
theoretical.cdf<-pnorm(data.length,mean(data.length),sd(data.length))
plot(adj.empirical.cdf,theoretical.cdf,main="P-P plot")

#subtasks b-d
#H0: mu=20, H1: mu<>20
mu<-20
alpha<-0.05
x<-mean(data.length)
n<-length(data.length)
S<-sqrt(1/(n-1)*sum((data.length-x)^2))
T<-sqrt(n)*(x-mu)/S

#critical region
CR_1<-qt(alpha/2,n-1)
CR_2<-qt(1-alpha/2,n-1)

xx<-seq(-5,5,0.01)
plot(xx,dt(xx,n-1),type='l',main="Critical region",xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
rect(-5,0,CR_1,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
rect(CR_2,0,5,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
abline(v=T,col='blue',lwd=2)

#p-value
p.value<-2*(1-pt(abs(T),n-1))
xx<-seq(-5,5,0.01)
plot(xx,dt(xx,n-1),type='l',main=paste("P-value",round(p.value,3)),xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
rect(-5,0,-abs(T),1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
rect(abs(T),0,5,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
abline(v=T,col='blue',lwd=2)

#TASK 2
data<-read.csv("Computers.csv")
price<-data[data$ram==2,"price"]

#subtask a
normality_tests(price)
qqnorm(price)
qqline(price)

plot(ecdf(price),main="Cumulative distribution function")
xx<-seq(900,2500,by=10)
lines(xx,pnorm(xx,mean(price),sd(price)),col='red',lwd=3)

#subtask b

par(mfrow=c(2,2))


h<-hist(price,xlim=c(700,2500),freq = F,ylim=c(0,0.0015))

x<-mean(price)
n<-length(price)
S<-sqrt(1/(n-1)*sum((price-x)^2))

alpha<-0.05
u.q<-qnorm(1-alpha/2)
u.q.2<-qnorm(1-alpha)

CI_lower<-x-u.q*S/sqrt(n)
CI_upper<-x+u.q*S/sqrt(n)

plot(h,xlim=c(700,2500),freq = F,ylim=c(0,0.0015),main="Two sided CI for mean")
abline(v=c(CI_upper,CI_lower),col='red',lwd=2)
abline(v=x,col='blue',lwd=2)

rect(CI_lower,0,CI_upper,0.0015, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)

####
CI_left<-x+u.q.2*S/sqrt(n)

plot(h,xlim=c(700,2500),freq = F,ylim=c(0,0.0015),main="Left sided CI for mean")
abline(v=CI_left,col='red',lwd=2)
abline(v=x,col='blue',lwd=2)

rect(700,0,CI_left,0.0015, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)

####
CI_right<-x-u.q.2*S/sqrt(n)

plot(h,xlim=c(700,2500),freq = F,ylim=c(0,0.0015),main="Right sided CI for mean")
abline(v=CI_right,col='red',lwd=2)
abline(v=x,col='blue',lwd=2)

rect(CI_right,0,2500,0.0015, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)

#subtask c
my.t<-function(mu){
  x<-mean(price)
  n<-length(price)
  S<-sqrt(1/(n-1)*sum((price-x)^2))
  T<-sqrt(n)*(x-mu)/S
  
  #critical region
  CR_1<-qnorm(alpha/2)
  CR_2<-qnorm(1-alpha/2)
  
  xx<-seq(-5,5,0.01)
  plot(xx,dnorm(xx),type='l',main="Critical region",xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
  rect(-5,0,CR_1,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
  rect(CR_2,0,5,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
  abline(v=T,col='blue',lwd=2)
  
  #p-value
  p.value<-2*(1-pnorm(abs(T)))
  xx<-seq(-5,5,0.01)
  plot(xx,dnorm(xx),type='l',main=paste("P-value",round(p.value,3)),xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
  rect(-5,0,-abs(T),1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
  rect(abs(T),0,5,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
  abline(v=T,col='blue',lwd=2)
}

par(mfrow=c(3,2))
#H0: mu=1650, H1: mu<>1650
my.t(1650) #REJECT

#H0: mu=1700, H1: mu<>1700
my.t(1700) #NOT REJECT

#H0: mu=1750, H1: mu<>1750
my.t(1750) #REJECT

#subtask d

my.t.right<-function(mu){
  x<-mean(price)
  n<-length(price)
  S<-sqrt(1/(n-1)*sum((price-x)^2))
  T<-sqrt(n)*(x-mu)/S
  
  #critical region
  CR_1<-qnorm(1-alpha)
  
  xx<-seq(-5,5,0.01)
  plot(xx,dnorm(xx),type='l',main="Critical region",xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
  rect(CR_1,0,5,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
  abline(v=T,col='blue',lwd=2)
  
  #p-value
  p.value<-pnorm(T,lower=F)
  xx<-seq(-5,5,0.01)
  
  plot(xx,dnorm(xx),type='l',main=paste("P-value",round(p.value,3)),xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
  rect(T,0,5,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
  abline(v=T,col='blue',lwd=2)
}

par(mfrow=c(3,2))
#H0: mu=1650, H1: mu>1650
my.t.right(1650) #REJECT

#H0: mu=1700, H1: mu>1700
my.t.right(1700) #NOT REJECT

#H0: mu=1750, H1: mu>1750
my.t.right(1750) #NOT REJECT

#subtask e
my.t.left<-function(mu){
  x<-mean(price)
  n<-length(price)
  S<-sqrt(1/(n-1)*sum((price-x)^2))
  T<-sqrt(n)*(x-mu)/S
  
  #critical region
  CR_1<-qnorm(alpha)
  
  xx<-seq(-5,5,0.01)
  plot(xx,dnorm(xx),type='l',main="Critical region",xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
  rect(-5,0,CR_1,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
  abline(v=T,col='blue',lwd=2)
  
  #p-value
  p.value<-pnorm(T)
  xx<-seq(-5,5,0.01)
  
  plot(xx,dnorm(xx),type='l',main=paste("P-value",round(p.value,3)),xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
  rect(-5,0,T,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
  abline(v=T,col='blue',lwd=2)
}

par(mfrow=c(3,2))
#H0: mu=1650, H1: mu<1650
my.t.left(1650) #NOT REJECT

#H0: mu=1700, H1: mu<1700
my.t.left(1700) #NOT REJECT

#H0: mu=1750, H1: mu<1750
my.t.left(1750) #REJECT

par(mfrow=c(1,1))


###############TASK 3
ones_in_file<-function(file){
  data<-readChar(fileName, file.info(fileName)$size)
  data.int<-as.numeric(as.vector(strsplit(data, "")[[1]]))
  
  len<-100
  len_total<-length(data.int)
  nones<-c()
  for (i in seq(1,(len_total-len+1),len)){
    nones<-c(sum(data.int[i:(len+i-1)]),nones)
  }
  return(nones)}

fileName <- 'file.txt'
#fileName <- 'file2.txt'
nones1<-ones_in_file(fileName)
n<-length(nones1)

#subtask a
xx<-0:100
h<-hist(nones1, xlim=c(0,100),freq = F)
lines(xx,dnorm(xx,mean(nones1),sd(nones1)),col='red',lwd=3)

t<-table(nones1)
ct<-as.table(cumsum(t))
plot(ct,type='s',main="Cumulative sum")

plot(ecdf(nones1),main="Cumulative distribution function")
lines(xx,pnorm(xx,mean(nones1),sd(nones1)),col='red',lwd=3)

#Q-Q plot
qqnorm(nones1) 
qqline(nones1) 

#P-P plot
cdf<-ecdf(nones1)
adj.empirical.cdf<-n/(n+1)*cdf(nones1)
theoretical.cdf<-pnorm(nones1,mean(nones1),sd(nones1))
plot(adj.empirical.cdf,theoretical.cdf,main="P-P plot")

normality_tests(nones1)
#We do not reject hypothesis about normality of our data


#subtask b
xx<-0:100
h<-hist(nones1, xlim=c(0,100),freq = F)
lines(xx,dbinom(xx,100,mean(nones1)/100),col='red',lwd=3)

sample<-sort(nones1)
n<-length(nones1)
i<-1:n
beta<-0.5
alpha<-(i-beta)/(n+1-2*beta)
theoretical.quantiles<-qbinom(alpha,100,mean(nones1)/100)
plot(theoretical.quantiles,sample,main="Q-Q plot")

library(car)
qqPlot(nones1,distribution="binom",size=100,prob=mean(nones1)/100,envelope=F)



