#TASK 1 PAIRED T-TEST
#https://www.chmi.cz/historicka-data/pocasi/uzemni-teploty# rok 2019
T2019<-c(-1.7,1.7,5.6,9.4,10.7,20.7,18.8,18.9,13.3,9.5,5.6,1.9)
T2018<-c(1.8,-3.5,0.8,12.7,16.2,17.5,19.7,20.6,14.5,10.0,4.3,1.2)
N<-c(-2.8,-1.1,2.5,7.3,12.3,15.5,16.9,16.4,12.8,8.0,2.7,-1.0)

#H0: T2018=T2019, H1: T2018<>T2019
#H0: T2018-T2019=0, H1: T2018-T2019<>0
Temp<-T2018-T2019
mu<-0
alpha<-0.05

x<-mean(Temp)
n<-length(Temp)
S<-sd(Temp)
T<-sqrt(n)*(x-mu)/S

#critical region
CR_1<-qt(alpha/2,n-1)
CR_2<-qt(1-alpha/2,n-1)

xx<-seq(-5,5,0.01)
plot(xx,dt(xx,n-1),type='l',main="Critical region",xlab="x",ylab = "f(x)")
rect(-5,0,CR_1,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
rect(CR_2,0,5,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
abline(v=T,col='blue',lwd=2)

#p-value
p.value<-2*(1-pt(abs(T),df=n-1))
xx<-seq(-5,5,0.01)
plot(xx,dt(xx,n-1),type='l',main=paste("P-value",round(p.value,3)),xlab="x",ylab = "f(x)")
rect(-5,0,-abs(T),1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
rect(abs(T),0,5,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
abline(v=T,col='blue',lwd=2)

#R function
t<-t.test(T2018,T2019,paired = T)
#NOT REJECT H0

#H0: T2019=N, H1: T2019>N
#H0: T2019-N=0, H1: T2019-N>0
Temp<-T2019-N
mu<-0
alpha<-0.05

x<-mean(Temp)
n<-length(Temp)
S<-sd(Temp)
T<-sqrt(n)*(x-mu)/S


#critical region
CR_1<-qt(1-alpha,n-1)

xx<-seq(-5,5,0.01)
plot(xx,dt(xx,n-1),type='l',main="Critical region",xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
rect(CR_1,0,5,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
abline(v=T,col='blue',lwd=2)

#p-value
p.value<-pt(T,df=n-1,lower=F)
xx<-seq(-5,5,0.01)

plot(xx,dt(xx,n-1),type='l',main=paste("P-value",round(p.value,3)),xlab="x",ylab = "f(x)",sub = paste("mu=",mu))
rect(T,0,5,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
abline(v=T,col='blue',lwd=2)

#R function
t<-t.test(T2019,N,paired = T,alternative="greater")
#REJECT H0

#TASK 2 TWO SAMPLED T-TEST
mach.1<-c(29, 27, 29, 35, 29, 32, 28, 34, 32, 33)
mach.2<-c(31, 28, 30, 28, 37, 29, 27, 27, 39, 33, 31, 32, 31, 29, 32, 28, 27, 28, 24, 34)

alpha<-0.05
X1<-mean(mach.1)
n1<-length(mach.1)
S1<-sd(mach.1)
X2<-mean(mach.2)
n2<-length(mach.2)
S2<-sd(mach.2)

#F-test for equal variances
F<-S1^2/S2^2

#critical region
CR_1<-qf(alpha/2,n1-1,n2-1)
CR_2<-qf(1-alpha/2,n1-1,n2-1)

xx<-seq(0,5,0.01)
plot(xx,df(xx,n1-1,n2-1),type='l',main="Critical region",xlab="x",ylab = "f(x)")
rect(0,0,CR_1,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
rect(CR_2,0,5,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
abline(v=T,col='blue',lwd=2)

#p-value
p.value<-2*(min(pf(F,n1-1,n2-1),1-pf(F,n1-1,n2-1)))
xx<-seq(0,5,0.01)
plot(xx,df(xx,n1-1,n2-1),type='l',main=paste("P-value",round(p.value,3)),xlab="x",ylab = "f(x)")

r1<-min(qf(1-pf(F,n1-1,n2-1),n1-1,n2-1),qf(pf(F,n1-1,n2-1),n1-1,n2-1))
r2<-max(qf(1-pf(F,n1-1,n2-1),n1-1,n2-1),qf(pf(F,n1-1,n2-1),n1-1,n2-1))
rect(0,0,r1,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
rect(r2,0,5,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
abline(v=F,col='blue',lwd=2)

#R function
f<-var.test(mach.1,mach.2)
#NOT REJECT H0

#T-test
S<-sqrt(((n1-1)*S1^2+(n2-1)*S2^2)/(n1+n2-2))
T<-(X1-X2)/(S*sqrt((n1+n2)/(n1*n2)))

#critical region
CR_1<-qt(alpha/2,n1+n2-2)
CR_2<-qt(1-alpha/2,n1+n2-2)

xx<-seq(-5,5,0.01)
plot(xx,dt(xx,n1+n2-2),type='l',main="Critical region",xlab="x",ylab = "f(x)")
rect(-5,0,CR_1,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
rect(CR_2,0,5,1, col=rgb(red=1, green=0, blue=0, alpha=0.2), lty=0)
abline(v=F,col='blue',lwd=2)

#p-value
p.value<-2*(1-pt(abs(T),df=n1+n2-2))
xx<-seq(-5,5,0.01)
plot(xx,dt(xx,n1+n2-2),type='l',main=paste("P-value",round(p.value,3)),xlab="x",ylab = "f(x)")
rect(-5,0,-abs(T),1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
rect(abs(T),0,5,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
abline(v=T,col='blue',lwd=2)

#R function
t<-t.test(mach.1,mach.2,var.equal = T)
#NOT REJECT H0

#TASK 3 
n_bombs<-c(0,1,2,3,4,7)
n_sectors<-c(229,211,93,35,7,1)

S<-sum(n_bombs*n_sectors)
n<-sum(n_sectors)
lambda<-1

p.value<-2*(min(ppois(S,lambda*n),1-ppois(S,lambda*n)))
xx<-seq(450,700,1)

r1<-min(qpois(ppois(S,lambda*n),lambda*n),qpois(1-ppois(S,lambda*n),lambda*n))
r2<-max(qpois(ppois(S,lambda*n),lambda*n),qpois(1-ppois(S,lambda*n),lambda*n))

plot(xx,dpois(xx,lambda*n),type='l',main=paste("P-value",round(p.value,3)),xlab="x",ylab = "f(x)")
rect(0,0,r1,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
rect(r2,0,900,1, col=rgb(red=0, green=0, blue=1, alpha=0.2), lty=0)
abline(v=S,col='blue',lwd=2)

library(exactci) #https://www.rdocumentation.org/packages/exactci/versions/1.3-3/topics/poisson.exact
poisson.exact(S,n,alternative = "two.sided") #poison test from stats is different
