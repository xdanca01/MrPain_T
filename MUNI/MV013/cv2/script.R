data <- read.csv("Computers.csv",header = TRUE ,sep=';',fileEncoding = "UTF-8-BOM")
#attach(data) 


price <- data$price
price <- gsub(',','',price)
price <- na.omit(price)
price <- as.numeric(price)
mean(price)
median(price)

my_quantile <- function(x, k){
  sorted_x = sort(x)
  n <- length(sorted_x) 
  middle <- k*n
  if(middle == round(middle))
    return((sorted_x[k*n]+sorted_x[k*n+1])/2)
  else
    return(sorted_x[ceiling(middle)])
}

my_quantile(price, 0.3)
quantile(price, 0.3)

my_trimmed <- function(x){
  top_bound <- my_quantile(x, 0.95)
  low_bound <- my_quantile(x, 0.05)
  n = length(x)
  n2 = length(x[x >= top_bound]) + length(x[x <= low_bound]) 
  return(sum(x[x > low_bound & x < top_bound])/(n-n2))
}

my_winsored <- function(x){
  top_bound <- my_quantile(x, 0.95)
  low_bound <- my_quantile(x, 0.05)
  n <- length(x)
  x[x > top_bound] <- top_bound
  x[x <= low_bound] <- low_bound
  return(sum(x)/n)
}

variance <- function(x){
  n <- length(x)
  my_sum <- sum(x)-n*mean(x)
  return(1/n * my_sum**2)
}

mean(price, 0.05)
my_trimmed(price)
my_winsored(price)
variance(x)
