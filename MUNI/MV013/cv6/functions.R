normality_tests<-function(x){
  #if (length(x)>20){ #at least 20 observation
  #  print(dagoTest(x)) #test for skewness curtoisis and combination
  #  message('#####################')
  #}
  #print(jarqueberaTest(x)) #test for skewness-curtoisis combination
  message('#####################')
  print(shapiro.test(x))
  message('#####################')
  print(lillie.test(x))
  message('#####################')
  print(cvm.test(x))
  message('#####################')
  print(ad.test(x))
  message('#####################')
  print(pearson.test(x))
  
}

normal_fit <-  function(par,x){
  
  mu <- par[1]
  sigma <- par[2]
  #just raw formula for log-likelihood function - Needs to be optimized
  # - log(prod(dnorm(x, mean = mu, sd = sigma )))
  
  #formula for log likelihood function - Needs to be optimized
  - sum(dnorm(x, mean = mu, sd = sigma, log = TRUE) )
  
}

binom_fit <- function(x, par){
  
  #just raw formula for likelihood function - Needs to be optimized
  # - prod(dbinom(x,100,par))
  
  #formula for log likelihood function - Needs to be optimized
  # - sum(dbinom(x,100,par, log = TRUE))
  
  #Derived formula for sample from binomial distribution - easy to compute, without optimization
  sum(x) / (100 * length(x))
  
}

zeros_in_file<-function(fileName){
  data<-readChar(fileName, file.info(fileName)$size)
  data.int<-as.numeric(strsplit(data, "")[[1]])
  
  len<-100
  len_total<-length(data.int)
  nzeros<-c()
  for (i in seq(1,(len_total),len)){
    nzeros<-c(sum(na.omit(data.int[i:(len+i)])),nzeros)
  }
  return(nzeros)}