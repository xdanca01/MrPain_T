setwd("/home/pdancak/school/MrPain_T/MUNI/MV013/cv5")

#################################################################################################################
#EXERCISE 1

#task a

ones_in_file <- function(fileName){
  
  data<-readChar(fileName, file.info(fileName)$size)
  data.int<-as.numeric(strsplit(data, "")[[1]])
  
  len<-100
  len.total<-length(data.int)
  nones<-c()
  for (i in seq(1,(len.total),len)){
    nones <- c(sum(na.omit(data.int[i:(len+i-1)])), nones)
  }
  return(nones)}

zeros_in_file<-function(fileName){
  data<-readChar(fileName, file.info(fileName)$size)
  data.int<-as.numeric(strsplit(data, "")[[1]])
  
  len<-100
  len_total<-length(data.int)
  nzeros<-c()
  for (i in seq(1,(len_total),len)){
    nzeros <- c(sum(data.int[i:(len+i)]),nzeros)
  }
  return(nzeros)}


#Create your function and then use optim function (see ?optim) to find coefficients of binomial distr.
#first file
fileName <- 'file.txt'
nzeros <- ones_in_file(fileName)

N = 100

fit_binom <- function(x, par){
  return(sum(x)/(100*length(x)))
}

#par <- optim() #set up coefficients for optimization
hist(nzeros, xlim = c(0,100), freq = FALSE)
x <- 0:100
p_hat <- fit_binom(nzeros, 0)
lines(x, dbinom(x, 100, p_hat), col = 'red')

fit_binom_optim <- function(x, par){
  return(-prod(dbinom(x,100,par)))  
}


#second file 
fileName <- 'file2.txt'
nzeros1 <- ones_in_file(fileName)

hist(nzeros1, xlim = c(0,100), freq = FALSE)
par1 <- optim(0.5, fit_binom, x=nzeros1, lower=0, upper=1, method="Brent") #set up coefficients for optimization
par2 <- optim(0.5, fit_binom_optim, x=nzeros1, lower=0, upper=1, method="Brent")
x <- 0:100
lines(x, dbinom(x, 100, par1$par[1]), col = 'red')

#task b --> in the template for solution a

#task c
#Create your function and then use optim function (see ?optim) to find coefficients of normal distr


fit_normal <- function(x, par){
  
  mu <- par[1]
  sigma <- par[2]
  -sum(dnorm(x, mean=mu, sd=sigma, log=TRUE))
}

#first file
par <- optim(c(50, 3), fit_normal, x=nzeros) #set up coefficients for optimization
hist(nzeros, xlim = c(0,100), freq = FALSE)
lines(0:100, dnorm(0:100, mean = par$par[1], sd = par$par[2]), col = 'red')


#second file
par <- optim(c(30, 5), fit_normal, x=nzeros1) #set up coefficients for optimization
hist(nzeros1, xlim = c(0,100), freq = FALSE)
lines(0:100, dnorm(0:100, mean = par$par[1], sd = par$par[2]), col = 'red')


#task d --> in the template for solution a



#################################################################################################################
#EXERCISE 2

#task a

data <- read.csv('Computers.csv', header = TRUE, sep = ',')
head(data)
ram_2 <- as.numeric(data[data$ram == 2,]$price) #take only prices for computers with 4GB RAM
  
hist(ram_2, freq=FALSE, ylim = c(0, 0.002)) #create a histogram, set up proper values for x-axis and y-axis
par <- optim(c(1700, 70), fit_normal, x=ram_2) #set up coefficients for optimization
x <- seq(800, 2500, 1)
lines(x , dnorm(x, mean =par$par[1], sd = par$par[2] ), col = 'blue' )


#task b
#Does the data see that they are normally distributed? See histogram, qqplot (use qqnorm with qqline)
#Try to use logarithmic transformation and look at the graphs again.

ram_4 <- as.numeric(data[data$ram == 4,]$price)#take only prices for computers with 4GB RAM 
hist(ram_4, freq=FALSE, breaks=15, xlim=c(800, 3500)) #create a histogram, set up proper values for x-axis and y-axis
#qqnorm(ram_4)
par <- optim(c(2000, 5), fit_normal, x=ram_4) #set up coefficients for optimization
x <- seq(800,4200, 1)
lines(x , dnorm(x, mean =par$par[1], sd = par$par[2] ), col = 'blue' )


qqnorm(ram_4)
qqline(ram_4)

hist(ram_4, freq = FALSE)
lines(density(ram_4), col = 'blue')


#task c


ram_4_log <- #take the logarithm of prices for computers with 4GB RAM
  
  #qqplot
  
  qqnorm(ram_4_log)
qqline(ram_4_log)


#histogram 
hist(ram_4_log, freq = FALSE)
lines(density(ram_4_log), col = 'red')


#maximizing function

par <- optim() #set up coefficients for optimization
x <- seq(7,8.5, 0.01)
hist(ram_4_log, freq = FALSE)
lines(x , dnorm(x, mean =par$par[1], sd = par$par[2] ), col = 'blue' )





























